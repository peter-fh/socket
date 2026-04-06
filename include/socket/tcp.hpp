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
  ~Tcp() noexcept;

  Tcp(const Tcp& other) = delete;
  Tcp& operator=(const Tcp& other) = delete;

  Tcp(Tcp&& other) noexcept;
  Tcp& operator=(Tcp&& other) noexcept;

  std::optional<Error> connect(Address addr) noexcept;
  std::optional<Error> bind(Address addr) noexcept;
  std::optional<Error> listen(int max_requests) noexcept;
  std::expected<int, Error> accept() noexcept;
  std::expected<std::vector<std::byte>, Error> receive(size_t size) noexcept;
  std::optional<Error> send(std::span<const std::byte> buff) noexcept;
  std::expected<Address, Error> peername() const noexcept;
  std::expected<Address, Error> sockname() const noexcept;
  int handle() const noexcept;
  explicit operator bool() const noexcept;

private:
  // Open and close are handled via RAII and should not be called by the integrator
  std::optional<Error> open() noexcept;
  std::optional<Error> close() noexcept;

  int m_handle;
};

} // namespace Socket
