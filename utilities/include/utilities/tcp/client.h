#ifndef UTILITIES_TCP_CLIENT_H
#define UTILITIES_TCP_CLIENT_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

namespace qle {

/**
 * @brief TCP Client class
 */
class Client {
 public:
  /**
   * @brief Default constructor deleted
   */
  Client() = delete;

  /**
   * @brief Copy constructor deleted
   */
  Client(const Client &) = delete;

  /**
   * @brief Move constructor deleted
   */
  Client(Client &&) = delete;

  /**
   * @brief Copy assignment deleted
   */
  Client &operator=(const Client &) = delete;

  /**
   * @brief Move assignment deleted
   */
  Client &operator=(Client &&) = delete;

  /**
   * @brief Construct a new Client object
   *
   * @param address Server IP address
   * @param port Server binding port
   */
  explicit Client(const std::string &address, int port) noexcept;

  /**
   * @brief Destroy the Client object
   */
  ~Client() noexcept { disconnectServer(); }

  /**
   * @brief Connect client to server
   *
   * @return true/false for success/failed
   */
  bool connectServer() noexcept;

  /**
   * @brief Terminate connection
   */
  void disconnectServer() noexcept;

  /**
   * @brief Send data to server
   *
   * @param data Sending data
   * @param data_size Size of data
   * @return Size of data sent
   */
  ssize_t sendData(const void *data, size_t data_size) noexcept;

  /**
   * @brief Receive data from server
   *
   * @param buffer Buffer storing received data
   * @param buffer_size Size of buffer
   * @return Size of data received
   */
  ssize_t receiveData(void *buffer, size_t buffer_size) noexcept;

 private:
  int sock_;                        ///< Socket
  std::string server_address_;      ///< Server IP address
  int port_;                        ///< Server binding port
  struct sockaddr_in server_info_;  ///< Server configuration info
};

}  // namespace qle

#endif  // UTILITIES_TCP_CLIENT_H
