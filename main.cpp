#include <iostream>
#include <cstdlib>
#include <string>
#include <uv.h>
#include "include/network.hpp"

using std::cout, std::cerr, std::endl, std::string;

int main() {
  char *port = std::getenv("PORT");

  uv_loop_t *loop = uv_default_loop();
  uv_tcp_t server;
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

  // Start the server along with Infinite loop
  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}