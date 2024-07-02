#ifndef UTILITIES_TCP_SERVER_H
#define UTILITIES_TCP_SERVER_H

#include <public_types/error_codes.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <thread>
#include <vector>

namespace qle {

/**
 * @brief TCP Server class
 */
class Server {
 public:
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
  explicit Server(int port) noexcept;

  /**
   * @brief Destroy the Server object
   */
  ~Server() noexcept { stop(); };

  /**
   * @brief Initialize TCP server
   *
   * @return ErrorCodes
   */
  ErrorCodes init() noexcept;

  /**
   * @brief Main process TCP server
   *
   * @return ErrorCodes
   */
  ErrorCodes run() noexcept;

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
  virtual void internal_run(int socket) noexcept;

 private:
  int fd_;                            ///< File descriptor
  int port_;                          ///< Binding port
  struct sockaddr_in address_;        ///< Server config
  bool isRunning_{false};             ///< Running status
  std::vector<std::thread> threads_;  ///< Client threads
};

}  // namespace qle

#endif  // UTILITIES_TCP_SERVER_H
