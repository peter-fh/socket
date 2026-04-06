#pragma once
#include <socket/error.hpp>
#include <socket/address.hpp>

#include <expected>
#include <optional>
#include <span>
#include <vector>

namespace Socket
{

class Tcp
{
public:
  Tcp() noexcept;
  Tcp(int handle) noexcept;
  std::optional<Error> open() noexcept;
  std::optional<Error> connect(Address addr) noexcept;
  std::optional<Error> bind(Address addr) noexcept;
  std::optional<Error> listen(int max_requests) noexcept;
  std::expected<Tcp, Error> accept() noexcept;
  std::expected<std::vector<std::byte>, Error> receive(size_t size=1024) noexcept;
  std::optional<Error> send(std::span<const std::byte> buff) noexcept;
  std::optional<Error> close() noexcept;
  std::expected<Address, Error> peername() const noexcept;
  std::expected<Address, Error> sockname() const noexcept;
  int handle() const noexcept;

private:
  int m_handle;
};

} // namespace Socket
