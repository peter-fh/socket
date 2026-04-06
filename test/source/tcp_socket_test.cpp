#include "gtest/gtest.h"
#include <socket/tcp.hpp>
#include <span>
#include <string>

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
  printf("Passed connect\n");
}

TEST_F(TcpSocketTest, ClientServer)
{
  Socket::Tcp client;
  Socket::Tcp server;
  Socket::Address addr("127.0.0.1", 8008);
  EXPECT_FALSE(client.open().has_value());
  EXPECT_FALSE(server.open().has_value());
  const auto bind_result = server.bind(addr);
  EXPECT_FALSE(bind_result.has_value()) << to_string(*bind_result);
  EXPECT_FALSE(server.listen(5).has_value());
  const auto connect_result = client.connect(addr);
  EXPECT_FALSE(connect_result.has_value()) << to_string(*connect_result);
  auto res = server.accept();
  EXPECT_TRUE(res.has_value()) << to_string(res.error());
  if (!res.has_value())
  {
    client.close();
    server.close();
    return;
  }
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

  connection.close();
  client.close();
  server.close();
}

std::span<const std::byte> to_buffer(std::string str)
{
  return std::as_bytes(std::span{str});
}

std::string from_buffer(std::vector<std::byte> buff)
{
  return {reinterpret_cast<const char*>(buff.data()), buff.size()};
}

TEST_F(TcpSocketTest, ClientServerShowcase)
{
  // Setup the address that will be used for the connection
  Socket::Address addr("127.0.0.1", 8008);

  // Initiate and open the sockets
  Socket::Tcp client;
  Socket::Tcp server;
  client.open();
  server.open();

  // Start listening on the address
  server.bind(addr);
  server.listen(5);

  // Connect the client to this address
  client.connect(addr);

  // Complete the handshake and get the server's new connection socket
  auto accept_result = server.accept();
  Socket::Tcp connection(accept_result.value());

  // Send the message
  std::string msg("ping");
  client.send(to_buffer("ping"));

  // Receive the message
  const auto receive_result = connection.recv(msg.size());
  std::string received_message(from_buffer(receive_result.value()));

  // Assert that we received the same message that was sent
  ASSERT_EQ(received_message, msg);

  // Cleanup the connections
  connection.close();
  client.close();
  server.close();
}
