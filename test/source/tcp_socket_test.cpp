#include "gtest/gtest.h"
#include <socket/tcp.hpp>
#include <span>
#include <string>

class TcpSocketTest : public testing::Test {};

TEST_F(TcpSocketTest, Open)
{
  Socket::Tcp socket;
  //const auto res = socket.open();
  //ASSERT_FALSE(res.has_value()) << to_string(*res);
  int fd = socket.handle();
  ASSERT_LE(0, fd);
  printf("FD: %d\n", fd);
}

TEST_F(TcpSocketTest, Connect)
{
  Socket::Tcp socket;
  //const auto open_result = socket.open();
  //ASSERT_FALSE(open_result.has_value()) << to_string(*open_result);

  using namespace std::string_view_literals;
  Socket::Address addr("104.18.26.120"sv, 80); // >nslookup example.com
  const auto connect_result = socket.connect(addr);
  ASSERT_FALSE(connect_result.has_value()) << to_string(*connect_result);
  printf("Passed connect\n");
}

TEST_F(TcpSocketTest, ClientServer)
{
  Socket::Tcp client;
  Socket::Tcp server;
  ASSERT_TRUE(client);
  ASSERT_TRUE(server);
  Socket::Address addr("127.0.0.1", 8008);

  const auto bind_result = server.bind(addr);
  EXPECT_FALSE(bind_result.has_value()) << to_string(*bind_result);
  EXPECT_FALSE(server.listen(5).has_value());
  const auto connect_result = client.connect(addr);
  EXPECT_FALSE(connect_result.has_value()) << to_string(*connect_result);
  auto res = server.accept();
  EXPECT_TRUE(res.has_value()) << to_string(res.error());
  if (!res.has_value()) return;
  Socket::Tcp connection(res.value());
  EXPECT_EQ(client.peername().value(), addr);
  EXPECT_EQ(client.sockname().value(), connection.peername().value());
  EXPECT_EQ(server.sockname().value(), connection.sockname().value());

  std::string msg("ping");
  client.send(std::as_bytes(std::span{msg}));
  const auto receive_result = connection.recv(msg.size());
  ASSERT_TRUE(receive_result.has_value()) << to_string(receive_result.error());
  std::string received_message {reinterpret_cast<const char*>(receive_result.value().data()), receive_result.value().size()};
  ASSERT_EQ(received_message, msg);
}

