#include "gtest/gtest.h"
#include <socket/tcp.hpp>

class TcpSocketTest : public testing::Test
{
public:
private:
};

TEST_F(TcpSocketTest, Open)
{
  Socket::Tcp socket;
  const auto res = socket.open();
  ASSERT_FALSE(res.has_value()) << to_string(*res);
  int fd = socket.handle();
  ASSERT_LE(0, fd);
  printf("FD: %d\n", fd);
}

TEST_F(TcpSocketTest, Connect)
{
  Socket::Tcp socket;
  const auto open_result = socket.open();
  ASSERT_FALSE(open_result.has_value()) << to_string(*open_result);

  using namespace std::string_view_literals;
  Socket::Address addr("104.18.26.120"sv, 80); // >nslookup example.com
  const auto connect_result = socket.connect(addr);
  ASSERT_FALSE(connect_result.has_value()) << to_string(*connect_result);
}

TEST_F(TcpSocketTest, ClientServer)
{
  Socket::Tcp client;
  Socket::Tcp server;
  Socket::Address addr("127.0.0.1", 8088);
  EXPECT_FALSE(client.open().has_value());
  EXPECT_FALSE(server.open().has_value());
  const auto bind_result = server.bind(addr);
  EXPECT_FALSE(bind_result.has_value()) << to_string(*bind_result);
  EXPECT_FALSE(server.listen(5).has_value());
  const auto connect_result = client.connect(addr);
  EXPECT_FALSE(connect_result.has_value()) << to_string(*connect_result);
  auto res = server.accept();
  EXPECT_TRUE(res.has_value()) << to_string(res.error());
  if (res.has_value())
  {
    Socket::Tcp connection(res.value());
    EXPECT_EQ(client.peername().value(), addr);
    EXPECT_EQ(client.sockname().value(), connection.peername().value());
    connection.close();
  }
  client.close();
  server.close();
}
