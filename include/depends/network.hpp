#include <uv.h>
#include <iostream>
#include <cstring>
#include <future>
#include "parser.hpp"
#include "../core/mempool.hpp"

using std::cout, std::endl;

namespace Network {
  Pool::FixedMemoryPool<uv_tcp_t> *clientPool; // Memory Pool for client Objects
  Pool::FixedMemoryPool<char> *dataPool;

  uv_tcp_t server;
  uv_loop_t *loop;

  struct WorkerData {
      uv_stream_t *client; // Client connection
      char *message;       // Message to process
      std::size_t message_len;  // Length of the message
      char *response;      // Processed response
      std::size_t response_len; // Length of the processed response
  };


  /**
   * @brief Function to close the client connecion
   * @param client Reference to the Client
   */
  void closeConn(uv_handle_t* client) {
    uv_close(client, [](uv_handle_t* handle) {
      auto delJob = std::async(std::launch::async, &Pool::FixedMemoryPool<uv_tcp_t>::deallocate, clientPool, handle);
    });
  }

  // Allocating Buffer
  void alloc_buff(uv_handle_s *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
  }

  /**
   * @brief Function to send the message to the client side
   */
  void send_resp(uv_stream_t *client, char *message, std::size_t message_len) {
    auto *req = new uv_write_t();
    uv_buf_t buf = uv_buf_init(message, message_len);

    // Attach the buffer to the write request for cleanup in the callback
    req->data = message;

    // Perform the write operation
    uv_write(req, client, &buf, 1, [](uv_write_t *req, int status) {
        if (status < 0) {
            std::cerr << "Write error: " << uv_strerror(status) << std::endl;
        }
        // delete[] static_cast<char *>(req->data); // Free the message buffer
        delete req;         // Free the write request
    });
  }

  /**
   * @brief The main Worker of Each Thread
   */
  void workerJob(uv_work_s *req) {
    auto *data = static_cast<WorkerData *>(req->data);
    data->response = Parser::parse(data->message, data->message_len, &data->response_len);
  }

  /**
   * @brief Helper function of the worker, Executes after worker completed it's job
   */
  void postWorkerJob(uv_work_s *req, int status) {
    auto *data = static_cast<WorkerData *>(req->data);

    if (status == 0) {
      send_resp(data->client, data->response, data->response_len);
    } else {
      std::cerr << "Worker thread failed for message processing" << std::endl;
    }

    // Cleaning Up the memory
    delete[] data->message;
    delete[] data->response;
    delete data;
    delete req;
  }


  // Reading the data
  void read(uv_stream_s *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
      // When Successfully received the data
      // Parse the information and process it
      // And send response to the client

      auto *data = new WorkerData;
      data->client = client;
      data->message = buf->base;
      data->message_len = nread;
      // std::memcpy(data->message, buf->base, nread);

      // Queue
      uv_work_t *work_req = new uv_work_t();
      work_req->data = data;

      uv_queue_work(loop, work_req, workerJob, postWorkerJob);
    }
    else if (nread < 0) {
      if (nread != UV_EOF) {
        std::cerr << "Query Read error: " << uv_strerror(nread) << "\n";
      }

      closeConn((uv_handle_t *)client);
    }

    // delete[] buf->base;
  }


  /**
   * @brief This function executes when new connection is established, or new data is received
   */
  void handleClient(uv_stream_t *server, int status) {
    // Connection Error
    if (status < 0) {
      std::cerr << "Connection error: " << uv_strerror(status) << std::endl;
      return;
    }

    auto *client = clientPool->allocate();
    uv_tcp_init(server->loop, client);

    // Accepting the connection
    if (uv_accept(server, (uv_stream_t *)client) == 0) {
      // New connection
      uv_read_start((uv_stream_t *)client, alloc_buff, read);
    } else {
      uv_close((uv_handle_t *)client, nullptr);
      auto delJob = std::async(std::launch::async, &Pool::FixedMemoryPool<uv_tcp_t>::deallocate, clientPool, client);
    }
  }
}