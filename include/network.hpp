#include <uv.h>
#include <iostream>
#include <cstring>
#include "parser.hpp"

namespace Network {
  // Allocating Buffer
  void alloc_buff(uv_handle_s *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
  }

  // Send response to the client
  void send_resp(uv_stream_t *client, char *message, size_t message_len) {
    auto *req = new uv_write_t();
    uv_buf_t buf = uv_buf_init(message, message_len);

    // Perform the write operation
    uv_write(req, client, &buf, 1, [](uv_write_t *req, int status) {
        if (status < 0) {
            std::cerr << "Write error: " << uv_strerror(status) << std::endl;
        }
        delete[] static_cast<char *>(req->data); // Free the message buffer
        delete req;         // Free the write request
    });

    // Attach the buffer to the write request for cleanup in the callback
    req->data = message;
  }

  // Reading the data
  void read(uv_stream_s *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
      // When Successfully received the data
      // Parse the information and process it
      // And send response to the client

      size_t resp_size;
      char *resp = Parser::parse(buf->base, nread, &resp_size);
      send_resp(client, resp, resp_size);
    }
    else if (nread < 0) {
      if (nread != UV_EOF) {
        std::cerr << "Query Read error: " << uv_strerror(nread) << "\n";
      }

      // Close the connection
      uv_close((uv_handle_t*)client, [](uv_handle_t* handle) {
          delete (uv_tcp_t*)handle;
      });
    }

    delete buf->base;
  }

  void query(uv_stream_t *server, int status) {
    // Connection Error
    if (status < 0) {
      return;
    }

    auto *client = new uv_tcp_t();
    uv_tcp_init(server->loop, client);

    // Accepting the connection
    if (uv_accept(server, (uv_stream_t *)client) == 0) {
      // New connection
      uv_read_start((uv_stream_t *)client, alloc_buff, read);
    } else {
      uv_close((uv_handle_t *)client, nullptr);
      delete client;
    }
  }
}