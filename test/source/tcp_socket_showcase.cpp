#include <socket/tcp.hpp>
#include <gtest/gtest.h>
#include <span>
#include <string>

class TcpSocketShowcase : public testing::Test {};


std::span<const std::byte> to_buffer(std::string str)
{
  return std::as_bytes(std::span{str});
}

std::string from_buffer(std::vector<std::byte> buff)
{
  return {reinterpret_cast<const char*>(buff.data()), buff.size()};
}

TEST_F(TcpSocketShowcase, Client_Server)
{
  // Initiate and open the sockets
  Socket::Tcp client;
  Socket::Tcp server;

  // Setup the address that will be used for the connection
  Socket::Address address("127.0.0.1", 8008);

  // Start listening on the address
  server.bind(address);
  server.listen(5);

  // Connect the client to this address
  client.connect(address);

  // Complete the handshake and get the server's new connection socket
  auto accept_result = server.accept();
  Socket::Tcp connection(std::move(*accept_result));

  // Send the message
  std::string msg("ping");
  client.send(to_buffer(msg));

  // Receive the message
  auto receive_result = connection.receive(msg.size());
  std::string received_message(from_buffer(receive_result.value()));

  // Assert that we received the same message that was sent
  ASSERT_EQ(received_message, msg);

  // Reply to the client
  msg = "pong";
  connection.send(to_buffer(msg));
  receive_result = client.receive(msg.size());
  received_message = from_buffer(receive_result.value());
  ASSERT_EQ(received_message, msg);
}
