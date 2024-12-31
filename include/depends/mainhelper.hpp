#include <thread>
#include <string>

namespace helper {
  /**
   * @brief Function to return total Thread is available into machine
   * @returns value between 1 to 128
   */
  unsigned short getTotalThreads() {
    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount < 1) {
      return 1;
    } else if (threadCount > 128) {
      return 128;
    } else {
      return threadCount;
    }
  }

  /**
   * @brief Set the max UV Threads in Thread Pool
   * @param count Total Count of the threads in thread Pool
   */
  void setUVThreads(unsigned int count) {
    std::string threads = std::to_string(count);
    #ifdef _WIN32
      std::string new_env = "UV_THREADPOOL_SIZE" + threads;
      _putenv(new_env.c_str());
    #elif __linux__
      setenv("UV_THREADPOOL_SIZE", threads.c_str(), 1);
    #elif __APPLE__
      setenv("UV_THREADPOOL_SIZE", threads.c_str(), 1);
    #endif
  }

  /**
   * @brief Get total threads set by the user
   */
  unsigned short getallocatedThreads() {
    return std::atoi(std::getenv("UV_THREADPOOL_SIZE"));
  }
};