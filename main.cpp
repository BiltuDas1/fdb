#include <iostream>
#include <cstdlib>
#include <string>
#include <uv.h>
#include "include/depends/network.hpp"

using std::cout, std::cerr, std::endl, std::string;

uv_tcp_t server;
uv_loop_t *loop;


void on_signal(uv_signal_t *req, int signum) {
  cout << "\nTurning off the server... ";

  // Close the server handle
  uv_close((uv_handle_t *)&server, [](uv_handle_t *handle) {
      cout << "Done" << endl;
  });

  // Stop the event loop
  uv_stop(loop);
  uv_signal_stop(req);
  uv_close((uv_handle_t *)req, nullptr);
}


int main() {
  char *port = std::getenv("PORT");

  loop = uv_default_loop();
  uv_tcp_init(loop, &server); // Initialize the TCP Server

  sockaddr_in addr;
  // If port is NULL
  if (port == nullptr) {
    uv_ip4_addr("0.0.0.0", 11111, &addr);
  } else {
    uv_ip4_addr("0.0.0.0", std::stoi(port), &addr); // Binding the port to the addr
  }

  uv_tcp_bind(&server, (const sockaddr*)&addr, 0); // Bind server to the addr
  int r = uv_listen((uv_stream_t *)&server, 128, Network::query);

  if (r) {
    cerr << "Listen error: " << uv_strerror(r) << endl;
    return 1;
  }

  if (port == nullptr) {
    cout << "Database is running on port 11111" << endl;
  } else {
    cout << "Database is running on port " << port << endl;
  }

  // Signal handling for graceful shutdown
  uv_signal_t sig;
  uv_signal_init(loop, &sig);
  uv_signal_start(&sig, on_signal, SIGINT); // Handle Ctrl+C (SIGINT)

  // Start the server along with Infinite loop
  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}