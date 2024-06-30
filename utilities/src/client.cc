#include <utilities/tcp/client.h>

namespace unimelb {

Client::Client(const std::string &server_address, int port)
    : server_address_(server_address), port_(port) {}

Client::~Client() { disconnectServer(); }

bool Client::connectServer() {
  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ == -1) {
    fprintf(stderr, "Socket creation error");
    return false;
  }

  server_info_.sin_family = AF_INET;
  server_info_.sin_port = htons(port_);

  if (inet_pton(AF_INET, server_address_.c_str(), &server_info_.sin_addr) <=
      0) {
    fprintf(stderr, "Invalid address/ Address not supported");
    return false;
  }

  if (connect(sock_, (struct sockaddr *)&server_info_, sizeof(server_info_)) <
      0) {
    fprintf(stderr, "Connection Failed");
    return false;
  }

  return true;
}

ssize_t Client::sendData(const void *data, size_t data_size) {
  return send(sock_, data, data_size, 0);
}

ssize_t Client::receiveData(void *buffer, size_t buffer_size) {
  return recv(sock_, buffer, buffer_size, 0);
}

void Client::disconnectServer() {
  if (sock_ != -1) {
    close(sock_);
    sock_ = -1;
  }
}

}  // namespace unimelb
