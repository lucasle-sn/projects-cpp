#include <utilities/tcp/server.h>

#include <cstring>
#include <iostream>

const int cBufferSize = 1024;

namespace qle {

Server::Server(int port) noexcept : port_(port) {}

ErrorCodes Server::init() noexcept {
  // Create a socket
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ == -1) {
    fprintf(stderr, "Socket setup failed\n");
    return ErrorCodes::ERROR;
  }

  int opt = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) == -1) {
    fprintf(stderr, "Socket setup failed\n");
    close(fd_);
    return ErrorCodes::ERROR;
  }

  address_.sin_family = AF_INET;
  address_.sin_addr.s_addr = INADDR_ANY;
  address_.sin_port = htons(port_);

  // Bind the socket to the network address and port
  if (bind(fd_, (struct sockaddr *)&address_, sizeof(address_)) < 0) {
    fprintf(stderr, "Socket binding failed\n");
    close(fd_);
    return ErrorCodes::ERROR;
  }

  if (listen(fd_, 2) < 0) {
    fprintf(stderr, "Error socket listening\n");
    close(fd_);
    return ErrorCodes::ERROR;
  }

  fprintf(stdout, "Successfully bind port %d\n", port_);
  isRunning_ = true;
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
  if (!isRunning_) {
    fprintf(stderr, "Server is not running\n");
    return ErrorCodes::ERROR;
  }

  while (true) {
    int addrlen_ = sizeof(address_);
    int client_socket =
        accept(fd_, (struct sockaddr *)&address_, (socklen_t *)&addrlen_);
    if (client_socket < 0) {
      fprintf(stderr, "Accept failed\n");
      continue;
    }
    threads_.emplace_back(&Server::internal_run, this, client_socket);
  }

  return ErrorCodes::SUCCESS;
}

void Server::internal_run(int socket) noexcept {
  char buffer[cBufferSize] = {0};
  read(socket, buffer, sizeof(buffer));
  std::cout << "Message received: " << buffer << std::endl;

  std::string hello = "Hello from server";
  send(socket, hello.c_str(), hello.size(), 0);
  std::cout << "Hello message sent" << std::endl;

  close(socket);
}

}  // namespace qle
