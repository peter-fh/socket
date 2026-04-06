#include <cerrno>
#include <iostream>
#include <socket/tcp.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>

#include <expected>
#include <optional>
#include <cassert>

namespace Socket
{

Tcp::Tcp() noexcept
{
  m_handle = -1;
  const auto res = open();
  if (res.has_value())
  {
    std::cout << "Failed to open: " << to_string(*res) << "\n";
    assert(false);
  }
}

Tcp::~Tcp() noexcept
{
  if (*this)
  {
    const auto res = close();
    if (res.has_value())
    {
      std::cout << "Failed to close: " << to_string(*res) << "\n";
    }
  }
}
Tcp::Tcp(Tcp&& other) noexcept 
{
  m_handle = other.m_handle;
  other.m_handle = -1;
}
Tcp& Tcp::operator=(Tcp&& other) noexcept
{
  if (this != &other)
  {
    if (*this)
    {
      close();
    }
    m_handle = other.m_handle;
    other.m_handle = -1;
  }
  return *this;
}

Tcp::operator bool() const noexcept {
  return m_handle >= 0;
}
Tcp::Tcp(int handle) noexcept : m_handle(handle) {}

std::optional<Error> Tcp::open() noexcept
{
  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) return parse_errno();

  m_handle = fd;
  return std::nullopt;
}

std::optional<Error> Tcp::connect(Address addr) noexcept
{
  ssize_t res = ::fcntl(m_handle, F_SETFL, O_NONBLOCK);
  if (res < 0) return parse_errno();

  struct sockaddr_in socket_address = addr.socket_address();

  struct timeval timeout{ .tv_sec = 5, .tv_usec = 0 };
  const auto sockopt_result = ::setsockopt(m_handle, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
  if (sockopt_result < 0) return parse_errno();

  res = ::connect(m_handle, reinterpret_cast<sockaddr*>(&socket_address), sizeof(socket_address));
  if (res < 0 && errno != EINPROGRESS) return parse_errno();

  return std::nullopt;
}

std::optional<Error> Tcp::bind(Address addr) noexcept
{
  int opt = 1;
  auto sockopt_result = ::setsockopt(m_handle, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (sockopt_result < 0) return parse_errno();

  struct timeval timeout{ .tv_sec = 5, .tv_usec = 0 };
  sockopt_result = ::setsockopt(m_handle, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
  if (sockopt_result < 0) return parse_errno();

  struct sockaddr_in socket_address = addr.socket_address();

  const auto bind_result = ::bind(m_handle, reinterpret_cast<sockaddr*>(&socket_address), sizeof(socket_address));
  if (bind_result < 0)
    return parse_errno();

  return std::nullopt;
}

std::optional<Error> Tcp::listen(int max_requests) noexcept
{
  ssize_t res = ::fcntl(m_handle, F_SETFL, O_NONBLOCK);
  if (res < 0) return parse_errno();

  res = ::listen(m_handle, max_requests);
  if (res < 0) return parse_errno();
  return std::nullopt;
}

std::expected<Tcp, Error> Tcp::accept() noexcept
{
  sockaddr_in peer_address;
  socklen_t peer_socket_length = sizeof(peer_address);
  const auto res = ::accept(m_handle, reinterpret_cast<sockaddr*>(&peer_address), &peer_socket_length);
  if (res < 0) return std::unexpected(parse_errno());

  Tcp connection_socket(res);
  assert(connection_socket.peername()->socket_address().sin_addr.s_addr == peer_address.sin_addr.s_addr);
  assert(connection_socket.peername()->socket_address().sin_port == peer_address.sin_port);
  return connection_socket;
}

// TODO: Add receive some method for whatever is available
std::expected<std::vector<std::byte>, Error> Tcp::receive(size_t size) noexcept
{
  size_t total = 0;
  std::vector<std::byte> buff;
  buff.resize(size);
  while (total < size)
  {
    ssize_t res = ::recv(m_handle, buff.data() + total, size - total, 0);
    if (res < 0)
    {
      if (errno == EINTR) continue;
      return std::unexpected(parse_errno());
    }
    if (res == 0) return std::unexpected(Error::CONNECTION_CLOSED);
    total += static_cast<size_t>(res);
  }
  return buff;
}

std::expected<std::vector<std::byte>, Error> Tcp::receive_available() noexcept
{
  const constexpr size_t size = 4096;
  std::vector<std::byte> data;
  std::array<std::byte, size> buff;
  for (;;)
  {
    const ssize_t result = ::recv(m_handle, buff.data(), buff.size(), 0);
    if (result < 0)
    {
      if (errno == EINTR) continue;
      if (errno == EWOULDBLOCK || errno == EAGAIN) break;
      return std::unexpected(parse_errno());
    }
    if (result == 0) break;
    data.insert(data.end(), buff.begin(), buff.begin() + static_cast<size_t>(result));
  }
  return data;
}

std::optional<Error> Tcp::send(std::span<const std::byte> buff) noexcept
{
  size_t total = 0;
  while (total < buff.size())
  {
    const ssize_t res = ::send(m_handle, buff.data() + total, buff.size() - total, 0);
    if (res < 0)
    {
      if (errno == EINTR) continue;
      return parse_errno();
    }
    if (res == 0) return Error::CONNECTION_CLOSED;
    total += static_cast<size_t>(res);
  }
  return std::nullopt;
}

std::optional<Error> Tcp::close() noexcept
{
  const auto res = ::close(m_handle);
  m_handle = -1;
  if (res < 0)
  {
    return parse_errno();
  }
  return std::nullopt;
}

int Tcp::handle() const noexcept
{
  return m_handle;
}

std::expected<Address, Error> Tcp::peername() const noexcept
{
  sockaddr_in peer_address;
  socklen_t peer_socket_length = sizeof(peer_address);

  const auto res = ::getpeername(m_handle, reinterpret_cast<sockaddr*>(&peer_address), &peer_socket_length);
  if (res < 0) return std::unexpected(parse_errno());

  return Address(peer_address);
}

std::expected<Address, Error> Tcp::sockname() const noexcept
{
  sockaddr_in peer_address;
  socklen_t peer_socket_length = sizeof(peer_address);

  const auto res = ::getsockname(m_handle, reinterpret_cast<sockaddr*>(&peer_address), &peer_socket_length);
  if (res < 0) return std::unexpected(parse_errno());

  return Address(peer_address);
}

} // namespace Socket

