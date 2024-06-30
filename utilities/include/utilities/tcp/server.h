#ifndef UTILITIES_TCP_SERVER_H
#define UTILITIES_TCP_SERVER_H

#include <public_types/error_codes.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <thread>
#include <vector>

namespace unimelb {

class Server {
 public:
  /**
   * @brief Delete default constructor
   */
  Server() = delete;

  /**
   * @brief Delete copy constructor
   */
  Server(const Server &) = delete;

  /**
   * @brief Delete move constructor
   */
  Server(Server &&) = delete;

  /**
   * @brief Delete copy assignment
   */
  Server &operator=(const Server &) = delete;

  /**
   * @brief Delete move assignment
   */
  Server &operator=(Server &&) = delete;

  /**
   * @brief Constructor
   * @param port Server port
   */
  explicit Server(int port);

  /**
   * @brief Destructor
   */
  ~Server() { stop(); };

  /**
   * @brief Initialize TCP server
   * @return ErrorCodes
   */
  ErrorCodes init();

  /**
   * @brief Main process TCP server
   * @return ErrorCodes
   */
  ErrorCodes run();

  /**
   * @brief Stop server and close
   */
  void stop();

 private:
  /**
   * @brief Handle client connection
   * @param socket Client
   */
  void handleClient(int socket);

  int fd_;    // File descriptor
  int port_;  // Port
  struct sockaddr_in address_;
  size_t addrlen_;
  bool isRunning_{false};             // Running status
  std::vector<std::thread> threads_;  // Client threads
};

}  // namespace unimelb

#endif  // UTILITIES_TCP_SERVER_H
