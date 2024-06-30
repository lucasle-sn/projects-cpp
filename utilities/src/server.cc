#include <utilities/tcp/server.h>

#include <cstring>
#include <iostream>

const int cBufferSize = 1024;

namespace unimelb {

Server::Server(int port) noexcept : port_(port), addrlen_(sizeof(address_)) {}

ErrorCodes Server::init() noexcept {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ == -1) {
    fprintf(stderr, "Socket setup failed\n");
    return ErrorCodes::ERROR;
  }

  int opt = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) == -1) {
    fprintf(stderr, "Socket setup failed\n");
    return ErrorCodes::ERROR;
  }
  address_.sin_family = AF_INET;
  address_.sin_addr.s_addr = INADDR_ANY;
  address_.sin_port = htons(port_);

  if (bind(fd_, (struct sockaddr *)&address_, sizeof(address_)) < 0) {
    fprintf(stderr, "Socket binding failed\n");
    return ErrorCodes::ERROR;
  }

  return ErrorCodes::SUCCESS;
}

void Server::stop() noexcept {
  if (!isRunning_) {
    return;
  }

  isRunning_ = false;
  close(fd_);

  for (auto &thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

ErrorCodes Server::run() noexcept {
  if (listen(fd_, 10) < 0) {
    fprintf(stderr, "Error socket listening\n");
    return ErrorCodes::ERROR;
  }
  isRunning_ = true;

  while (isRunning_) {
    int socket =
        accept(fd_, (struct sockaddr *)&address_, (socklen_t *)&addrlen_);
    if (socket < 0) {
      fprintf(stderr, "Accept failed\n");
      continue;
    }

    threads_.emplace_back(&Server::handleClient, this, socket);
  }

  return ErrorCodes::SUCCESS;
}

void Server::handleClient(int socket) noexcept {
  char buffer[cBufferSize] = {0};
  read(socket, buffer, sizeof(buffer));
  std::cout << "Message received: " << buffer << std::endl;

  std::string hello = "Hello from server";
  send(socket, hello.c_str(), hello.size(), 0);
  std::cout << "Hello message sent" << std::endl;

  close(socket);
}

}  // namespace unimelb
