#pragma once
#include <cstdint>
#include <string_view>

struct sockaddr_in;

namespace Socket
{
class Address
{
public:
  Address(sockaddr_in);
  Address(std::string_view address);
  Address(std::string_view address, uint16_t port);

  bool valid() const;
  uint32_t url() const;
  uint16_t port() const;
  sockaddr_in socket_address() const;

  bool operator==(const Address& other) const;

private:
  // Data is a single 64 bit integer to ensure it fits on a 64 bit register
  uint64_t m_data;
};

} // namespace Socket
