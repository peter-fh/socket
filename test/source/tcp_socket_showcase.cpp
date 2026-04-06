#include <chrono>
#include <ostream>
#include <socket/tcp.hpp>
#include <gtest/gtest.h>
#include <span>
#include <string>
#include <thread>

class TcpSocketShowcase : public testing::Test {};


std::span<const std::byte> to_buffer(std::string str)
{
  return std::as_bytes(std::span{str});
}

std::string from_buffer(std::vector<std::byte> buff)
{
  return {reinterpret_cast<const char*>(buff.data()), buff.size()};
}
using namespace std::chrono_literals;
void wait(std::chrono::milliseconds ms)
{
  std::this_thread::sleep_for(ms);
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

  // Wait for connection
  wait(5ms);

  // Complete the handshake and get the server's new connection socket
  auto accept_result = server.accept();
  Socket::Tcp connection(std::move(*accept_result));

  // Send the message
  std::string msg("ping");
  client.send(to_buffer(msg));

  // Wait for send
  wait(5ms);

  // Receive the message
  auto receive_result = connection.receive(msg.size());
  std::string received_message(from_buffer(receive_result.value()));

  // Assert that we received the same message that was sent
  ASSERT_EQ(received_message, msg);

  // Reply to the client
  msg = "pong";
  connection.send(to_buffer(msg));
  wait(5ms);
  receive_result = client.receive(msg.size());
  received_message = from_buffer(receive_result.value());
  ASSERT_EQ(received_message, msg);
}

TEST_F(TcpSocketShowcase, Client_Server_ErrorHandling)
{
  // Initiate and open the sockets
  Socket::Tcp client;
  Socket::Tcp server;

  // Setup the address that will be used for the connection
  Socket::Address address("127.0.0.1", 8008);

  // Start listening on the address
  auto result = server.bind(address);
  if (result.has_value())
  {
    std::cout << "Bind failed with error: " << to_string(*result) << std::endl;
    return;
  }

  result = server.listen(5);
  if (result.has_value())
  {
    std::cout << "Listen failed with error: " << to_string(*result) << std::endl;
    return;
  }

  // Connect the client to this address
  result = client.connect(address);
  if (result.has_value())
  {
    std::cout << "Listen failed with error: " << to_string(*result) << std::endl;
    return;
  }

  // Wait for connection
  wait(5ms);

  // Complete the handshake and get the server's new connection socket
  auto accept_result = server.accept();
  if (!accept_result.has_value())
  {
    std::cout << "Accept failed with error: " << to_string(accept_result.error()) << std::endl;
    return;
  }
  Socket::Tcp connection(std::move(*accept_result));

  // Send the message
  std::string msg("ping");
  result = client.send(to_buffer(msg));
  if (result.has_value())
  {
    std::cout << "Send failed with error: " << to_string(*result) << std::endl;
    return;
  }

  // Wait for send
  wait(5ms);

  // Receive the message
  auto receive_result = connection.receive(msg.size());
  if (!receive_result.has_value())
  {
    std::cout << "Receive failed with error: " << to_string(receive_result.error()) << std::endl;
    return;
  }
  std::string received_message(from_buffer(receive_result.value()));

  // Assert that we received the same message that was sent
  ASSERT_EQ(received_message, msg);

  // Reply to the client
  msg = "pong";
  result = connection.send(to_buffer(msg));
  if (result.has_value())
  {
    std::cout << "Send failed with error: " << to_string(*result) << std::endl;
    return;
  }
  wait(5ms);
  receive_result = client.receive(msg.size());
  if (!receive_result.has_value())
  {
    std::cout << "Receive failed with error: " << to_string(receive_result.error()) << std::endl;
    return;
  }
  received_message = from_buffer(receive_result.value());
  ASSERT_EQ(received_message, msg);
}
