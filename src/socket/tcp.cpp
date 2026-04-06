#include <socket/tcp.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include <expected>
#include <optional>
#include <cassert>

namespace Socket
{

Tcp::Tcp() noexcept = default;
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
  struct sockaddr_in socket_address;
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = addr.url();
  socket_address.sin_port = addr.port();

  struct timeval timeout{ .tv_sec = 5, .tv_usec = 0 };
  const auto sockopt_result = ::setsockopt(m_handle, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
  if (sockopt_result < 0) return parse_errno();

  const auto res = ::connect(m_handle, reinterpret_cast<sockaddr*>(&socket_address), sizeof(sockaddr));
  if (res < 0) return parse_errno();

  return std::nullopt;
}

std::optional<Error> Tcp::bind(Address addr) noexcept
{
  int opt = 1;
  auto sockopt_result = ::setsockopt(m_handle, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (sockopt_result < 0) return parse_errno();

  struct timeval timeout{ .tv_sec = 5, .tv_usec = 0 };
  sockopt_result = ::setsockopt(m_handle, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

  struct sockaddr_in socket_address;
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = addr.url();
  socket_address.sin_port = addr.port();

  const auto bind_result = ::bind(m_handle, reinterpret_cast<sockaddr*>(&socket_address), sizeof(sockaddr));
  if (bind_result < 0)
    return parse_errno();

  return std::nullopt;
}

std::optional<Error> Tcp::listen(int max_requests) noexcept
{

  const auto res = ::listen(m_handle, max_requests);
  if (res < 0) return parse_errno();
  return std::nullopt;
}

std::expected<Socket::Tcp, Error> Tcp::accept() noexcept
{
  sockaddr_in peer_address;
  socklen_t peer_socket_length = sizeof(peer_address);
  const auto res = ::accept(m_handle, reinterpret_cast<sockaddr*>(&peer_address), &peer_socket_length);
  if (res < 0) return std::unexpected(parse_errno());

  Tcp result_socket(res);
  assert(result_socket.peername() == Address(peer_address));
  return result_socket;
}

std::expected<std::vector<std::byte>, Error> Tcp::recv(size_t size) noexcept
{
  std::vector<std::byte> buff;
  buff.resize(size);
  const auto res = ::recv(m_handle, buff.data(), size, 0);
  if (res < 0) return std::unexpected(parse_errno());
  if (res == 0) return std::unexpected(Error::CONNECTION_CLOSED);

  buff.resize(size);

  return buff;
}

std::optional<Error> Tcp::send(std::span<const std::byte> buff) noexcept
{
  const auto res = ::send(m_handle, buff.data(), buff.size(), 0);
  if (res < 0) return parse_errno();
  return std::nullopt;
}

std::optional<Error> Tcp::close() noexcept
{
  const auto res = ::close(m_handle);
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



