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
  Socket::Address addr("127.0.0.1", 8090);
  ASSERT_FALSE(client.open().has_value());
  ASSERT_FALSE(server.open().has_value());
  ASSERT_FALSE(server.bind(addr).has_value());
  ASSERT_FALSE(server.listen(5).has_value());
  ASSERT_FALSE(client.connect(addr).has_value());
  auto res = server.accept();
  ASSERT_TRUE(res.has_value()) << to_string(res.error());
  ASSERT_EQ(client.peername().value(), addr);
  ASSERT_EQ(client.sockname().value(), res.value().peername().value());
}
