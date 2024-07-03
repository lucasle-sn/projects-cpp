#include <utilities/log.h>
#include <utilities/tcp/client.h>

#include <memory>

static auto logger = std::make_unique<qle::Logger>("Client");

namespace qle {

Client::Client(const char *server_address, int port) noexcept
    : server_address_(server_address), port_(port) {}

ErrorCodes Client::init() noexcept {
  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ == -1) {
    logger->error("Socket creation failed");
    return ErrorCodes::ERROR;
  }

  server_addr_.sin_family = AF_INET;
  server_addr_.sin_port = htons(port_);

  // Convert IPv4 and IPv6 addresses to binary form
  if (inet_pton(AF_INET, server_address_, &server_addr_.sin_addr) <= 0) {
    logger->error("Invalid address or Address not supported");
    close(sock_);
    return ErrorCodes::ERROR;
  }
  logger->info("Successfully initialize client");
  return ErrorCodes::SUCCESS;
}

ErrorCodes Client::connectServer() noexcept {
  if (connect(sock_, (struct sockaddr *)&server_addr_, sizeof(server_addr_)) <
      0) {
    logger->error("Connection Failed");
    close(sock_);
    return ErrorCodes::ERROR;
  }
  return ErrorCodes::SUCCESS;
}

void Client::disconnectServer() noexcept {
  logger->info("Disconnect server");
  if (sock_ != -1) {
    close(sock_);
    sock_ = -1;
  }
}

ssize_t Client::sendData(const void *data, size_t data_size) noexcept {
  // logger->info("Sending data %s", (const char *)data);
  return send(sock_, data, data_size, 0);
}

ssize_t Client::receiveData(void *buffer, size_t buffer_size) noexcept {
  // logger->info("Receiving data %s", (char *)buffer);
  return recv(sock_, buffer, buffer_size, 0);
}

}  // namespace qle
