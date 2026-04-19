#include <peterfh.hpp>
#pragma once

#include <socket/error.hpp>
#include <socket/address.hpp>

#include <span>
#include <vector>

namespace peterfh::socket
{

class Tcp
{
public:
  Tcp() noexcept;
  ~Tcp() noexcept;

  Tcp(const Tcp& other) = delete;
  Tcp& operator=(const Tcp& other) = delete;

  Tcp(Tcp&& other) noexcept;
  Tcp& operator=(Tcp&& other) noexcept;

  Result<Void, Error> connect(Address addr) noexcept;
  Result<Void, Error> bind(Address addr) noexcept;
  Result<Void, Error> listen(int max_requests) noexcept;
  Result<Tcp, Error> accept() noexcept;
  Result<std::vector<std::byte>, Error> receive(size_t size) noexcept;
  Result<std::pair<std::vector<std::byte>, bool>, Error> receive_available() noexcept;
  Result<Void, Error> send(std::span<const std::byte> buff) noexcept;
  Result<Address, Error> peername() const noexcept;
  Result<Address, Error> sockname() const noexcept;
  int handle() const noexcept;
  explicit operator bool() const noexcept;

private:
  Tcp(int handle) noexcept;

  // Open and close are handled via RAII and should not be called by the integrator
  Result<Void, Error> open() noexcept;
  Result<Void, Error> close() noexcept;

  int m_handle;
};

} // namespace Socket
