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

  bool valid();
  uint32_t url();
  uint16_t port();

  bool operator==(const Address& other) const;

private:
  uint32_t m_url;
  uint16_t m_port;
  bool m_isValid;
};

}
