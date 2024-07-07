#ifndef UTILITIES_TCP_SERVER_H
#define UTILITIES_TCP_SERVER_H

#include <public_types/error_codes.h>

#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <atomic>
#include <thread>
#include <vector>

namespace qle {

/**
 * @brief TCP Server class
 */
class Server {
 public:
  /**
   * @brief Server's connection handling mode
   */
  enum class Mode {
    BLOCKING,      ///< Blocking mode
    NON_BLOCKING,  ///< Non blocking mode
  };

  /**
   * @brief Default constructor deleted
   */
  Server() = delete;

  /**
   * @brief Copy constructor deleted
   */
  Server(const Server &) = delete;

  /**
   * @brief Move constructor deleted
   */
  Server(Server &&) = delete;

  /**
   * @brief Copy assignment deleted
   */
  Server &operator=(const Server &) = delete;

  /**
   * @brief Move assignment deleted
   */
  Server &operator=(Server &&) = delete;

  /**
   * @brief Construct a new Server object
   *
   * @param port Binding port
   */
  explicit Server(int port, Mode mode = Mode::BLOCKING,
                  int timeout_sec = -1) noexcept;

  /**
   * @brief Destroy the Server object
   */
  ~Server() noexcept { stop(); };

  /**
   * @brief Initialize TCP server
   *
   * @return ErrorCode
   */
  ErrorCode init() noexcept;

  /**
   * @brief Main process TCP server
   */
  void run() noexcept;

  /**
   * @brief Stop server and close
   */
  void stop() noexcept;

 protected:
  /**
   * @brief Handle client connection
   *
   * @param socket Client
   */
  virtual void internal_run(int socket) noexcept { close(socket); };

 protected:
  std::atomic<bool> isRunning_{false};  ///< Running status

 private:
  /**
   * @brief Accept an incoming connection
   *
   * @return int client socket
   */
  int accept_connection() noexcept;

  /**
   * @brief Main process blocking mode
   */
  void run_blocking() noexcept;

  int listener_fd_;                   ///< Listener fd
  int port_;                          ///< Binding port
  struct sockaddr_in server_addr_;    ///< Server config
  std::vector<std::thread> threads_;  ///< Client threads
  Mode mode_;                         ///< connection handling mode
  int timeout_sec_;                   ///< accecpt connection timeout
};

}  // namespace qle

#endif  // UTILITIES_TCP_SERVER_H
