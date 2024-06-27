#ifndef TEMPLATE_TCP_CLIENT_H
#define TEMPLATE_TCP_CLIENT_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

namespace unimelb {

class Client {
 public:
  Client() = delete;
  Client(const Client &) = delete;
  Client(Client &&) = delete;
  Client &operator=(const Client &) = delete;
  Client &operator=(Client &&) = delete;

  explicit Client(const std::string &address, int port);
  ~Client() = default;

  bool connectServer();
  ssize_t sendData(const void *data, size_t data_size);
  ssize_t receiveData(void *buffer, size_t buffer_size);
  void disconnectServer();

 private:
  int sock_;
  std::string server_address_;
  int port_;
  struct sockaddr_in server_info_;
};

}  // namespace unimelb

#endif  // TEMPLATE_TCP_CLIENT_H
