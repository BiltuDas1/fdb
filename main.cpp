#include <iostream>
#include <cstdlib>
#include <string>
#include <memory>
#include <uv.h>
#include "include/depends/network.hpp"
#include "include/depends/mainhelper.hpp"

using Network::loop, Network::server;
using std::cout, std::cerr, std::endl, std::string;

/**
 * @brief Function to turn off the TCP connection, and clean all the objects
 */
void powerOff(uv_signal_t *req, int signum) {
  cout << "\nTurning off the server... ";

  // Close the server handle
  uv_close((uv_handle_t *)&server, [](uv_handle_t *handle) {
      cout << "Done" << endl;
  });

  // Stop the event loop
  uv_stop(loop);
  uv_signal_stop(req);
  uv_close((uv_handle_t *)req, nullptr);
  delete Network::clientPool;
}

int main() {
  // Initializing Threads/Memory Pools
  // If Thread is explicitly set by environment, then set it as libuv worker thread
  char *threadsBuff = std::getenv("THREADS");
  unsigned short threadCount;
  if (threadsBuff == nullptr)
    threadCount = helper::getTotalThreads();
  else
    threadCount = atoi(threadsBuff);
  helper::setUVThreads(threadCount);
  Network::clientPool = new Pool::FixedMemoryPool<uv_tcp_t>(threadCount * sizeof(uv_tcp_t) * 2);


  loop = uv_default_loop();
  sockaddr_in addr;
  uv_tcp_init(loop, &server); // Initialize the TCP Server

  // If port is NULL, then use default port 11111
  char *port = std::getenv("PORT");
  if (port == nullptr) {
    uv_ip4_addr("0.0.0.0", 11111, &addr);
  } else {
    uv_ip4_addr("0.0.0.0", std::stoi(port), &addr); // Binding the port to the addr
  }

  uv_tcp_bind(&server, (const sockaddr*)&addr, 0); // Bind server to the addr
  int err = uv_listen((uv_stream_t *)&server, 128, Network::handleClient);

  if (err) {
    cerr << "Listen error: " << uv_strerror(err) << endl;
    return 1;
  }

  cout << "Using " << helper::getallocatedThreads() << " threads" << endl;

  if (port == nullptr) {
    cout << "Database is running on port 11111" << endl;
  } else {
    cout << "Database is running on port " << port << endl;
  }

  // Signal handling for graceful shutdown
  uv_signal_t sig;
  uv_signal_init(loop, &sig);
  uv_signal_start(&sig, powerOff, SIGINT); // Handle Ctrl+C (SIGINT)

  // Start the server along with Infinite loop
  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}