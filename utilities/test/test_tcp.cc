#include <utilities/tcp/client.h>
#include <utilities/tcp/server.h>

#include <gtest/gtest.h>
#include <memory>

namespace {

const char *cLocalhost{"127.0.0.1"};

class TestTcp : public ::testing::Test {};

class ServerAccessor : public qle::Server {
 public:
  explicit ServerAccessor(int port,
                          qle::Server::Mode mode = qle::Server::Mode::BLOCKING,
                          int timeout_sec = -1) noexcept
      : Server(port, mode, timeout_sec) {}

 protected:
  void internal_run(int socket) noexcept override {
    char buffer[1024] = {0};
    read(socket, buffer, sizeof(buffer));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::string str_return = "Server: " + std::string(buffer);

    send(socket, str_return.c_str(), str_return.size(), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    close(socket);
  }
};

TEST_F(TestTcp, BasicUsage) {
  const int cPort = 8080;
  const char *str = "Hello";
  const char *str_return = "Server: Hello";

  auto run_server = [&]() {
    auto server =
        std::make_unique<ServerAccessor>(cPort, qle::Server::Mode::BLOCKING, 3);
    ASSERT_EQ(server->init(), qle::ErrorCode::SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    server->run();
  };

  auto run_client = [&]() {
    auto client = std::make_unique<qle::Client>(cLocalhost, cPort);
    ASSERT_EQ(client->init(), qle::ErrorCode::SUCCESS);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(1000));  // wait for server up
    ASSERT_EQ(client->connectServer(), qle::ErrorCode::SUCCESS);

    EXPECT_EQ(client->sendData(str, sizeof(str)), sizeof(str));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    char buff[1024]{};
    EXPECT_EQ(client->receiveData(buff, sizeof(buff)), strlen(str_return));

    ASSERT_TRUE(str_return);
    EXPECT_TRUE((strlen(buff) == strlen(str_return)) &&
                (strcmp(buff, str_return) == 0));
  };

  std::thread server_thread(run_server);
  std::thread client_thread(run_client);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  client_thread.join();
  server_thread.join();
}

}  // namespace
