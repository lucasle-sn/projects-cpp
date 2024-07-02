#include <utilities/tcp/client.h>

namespace qle {

Client::Client(const char *server_address, int port) noexcept
    : server_address_(server_address), port_(port) {}

ErrorCodes Client::init() noexcept {
  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ == -1) {
    fprintf(stderr, "Socket creation error");
    return ErrorCodes::ERROR;
  }

  server_addr_.sin_family = AF_INET;
  server_addr_.sin_port = htons(port_);

  // Convert IPv4 and IPv6 addresses to binary form
  if (inet_pton(AF_INET, server_address_, &server_addr_.sin_addr) <= 0) {
    fprintf(stderr, "Invalid address/ Address not supported");
    close(sock_);
    return ErrorCodes::ERROR;
  }
  return ErrorCodes::SUCCESS;
}

ErrorCodes Client::connectServer() noexcept {
  if (connect(sock_, (struct sockaddr *)&server_addr_, sizeof(server_addr_)) <
      0) {
    fprintf(stderr, "Connection Failed");
    return ErrorCodes::ERROR;
  }
  return ErrorCodes::SUCCESS;
}

void Client::disconnectServer() noexcept {
  if (sock_ != -1) {
    close(sock_);
    sock_ = -1;
  }
}

ssize_t Client::sendData(const void *data, size_t data_size) noexcept {
  return send(sock_, data, data_size, 0);
}

ssize_t Client::receiveData(void *buffer, size_t buffer_size) noexcept {
  return recv(sock_, buffer, buffer_size, 0);
}

}  // namespace qle
