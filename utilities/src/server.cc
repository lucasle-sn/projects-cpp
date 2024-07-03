#include <utilities/tcp/server.h>

#include <cstring>
#include <iostream>

const int cBufferSize = 1024;
/**
 * Reference:
 * https://www.ibm.com/docs/en/i/7.4?topic=designs-using-poll-instead-select
 *
 */

namespace qle {

Server::Server(int port, Mode mode, int timeout_sec) noexcept
    : port_(port), mode_(mode), timeout_sec_(timeout_sec) {}

ErrorCodes Server::init() noexcept {
  // Create an AF_INET (v4) stream socket to receive incoming connections on
  listener_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listener_fd_ == -1) {
    fprintf(stderr, "Socket setup failed\n");
    return ErrorCodes::ERROR;
  }

  // Set socket options: Allow socket descriptor to be reuseable
  int opt = 1;
  if (setsockopt(listener_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) == -1) {
    fprintf(stderr, "Socket setup failed\n");
    close(listener_fd_);
    return ErrorCodes::ERROR;
  }

  server_addr_.sin_family = AF_INET;
  server_addr_.sin_addr.s_addr = INADDR_ANY;
  server_addr_.sin_port = htons(port_);

  // Bind the socket to the network address and port
  if (bind(listener_fd_, (struct sockaddr *)&server_addr_,
           sizeof(server_addr_)) < 0) {
    fprintf(stderr, "Socket binding failed\n");
    close(listener_fd_);
    return ErrorCodes::ERROR;
  }

  if (listen(listener_fd_, 3) < 0) {
    fprintf(stderr, "Error socket listening\n");
    close(listener_fd_);
    return ErrorCodes::ERROR;
  }

  // Set the timeout for the accept call
  if (timeout_sec_ > 0) {
    fprintf(stderr, "Setup timeout %d seconds\n", timeout_sec_);
    struct timeval timeout;
    timeout.tv_sec = timeout_sec_;
    timeout.tv_usec = 0;

    if (setsockopt(listener_fd_, SOL_SOCKET, SO_RCVTIMEO,
                   (const char *)&timeout, sizeof(timeout)) < 0) {
      fprintf(stderr, "Fail to setup timeout for connection accept\n");
      close(listener_fd_);
      return ErrorCodes::ERROR;
    }
  }

  fprintf(stdout, "Successfully bind port %d\n", port_);
  isRunning_.store(true);
  return ErrorCodes::SUCCESS;
}

void Server::stop() noexcept {
  fprintf(stdout, "Stop server\n");
  isRunning_ = false;
  close(listener_fd_);

  for (auto &thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void Server::run() noexcept {
  if (mode_ == Mode::BLOCKING) {
    run_blocking();
  }
}

int Server::accept_connection() noexcept {
  // Accept an incoming connection that queues up on the listening socket
  struct sockaddr_in peer_addr;
  int peer_addr_len = sizeof(peer_addr);
  int client_socket = accept(listener_fd_, (struct sockaddr *)&peer_addr,
                             (socklen_t *)&peer_addr_len);
  if (client_socket < 0) {
    fprintf(stderr, "Accept failed\n");
    return client_socket;
  }

  threads_.emplace_back(&Server::internal_run, this, client_socket);
  fprintf(stderr, "Client socket %d\n", client_socket);
  return client_socket;
}

void Server::run_blocking() noexcept {
  while (isRunning_) {
    auto client_socket = accept_connection();
    if (client_socket < 0) {
      return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

}  // namespace qle
