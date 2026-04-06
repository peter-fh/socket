#include <netinet/in.h>
#include <socket/address.hpp>

#include <arpa/inet.h>
#include <string>

#define VALID_BIT (1ULL << 63)
#define PORT_SHIFT 32;
namespace Socket
{

Address::Address(sockaddr_in addr)
{
  m_data = 0ULL;
  m_data |= static_cast<uint64_t>(addr.sin_addr.s_addr);
  m_data |= static_cast<uint64_t>(addr.sin_port) << PORT_SHIFT;
  m_data |= VALID_BIT;
}

Address::Address(std::string_view address, uint16_t port)
{
  m_data = 0ULL;
  std::string s(address);
  uint32_t addr;
  int res = inet_pton(AF_INET, s.c_str(), &addr);
  if (res <= 0)
  {
    m_data = 0;
    return;
  }
  m_data |= addr;
  m_data |= static_cast<uint64_t>(htons(port)) << PORT_SHIFT;
  m_data |= VALID_BIT;
}

Address::Address(std::string_view address)
: Address(address, 0) {}

bool Address::valid() const
{
  return (m_data & VALID_BIT) != 0;
}

uint16_t Address::port() const
{
  return static_cast<uint16_t>((m_data >> 32) & 0xFFFF);
}

uint32_t Address::url() const
{
  return static_cast<uint32_t>(m_data & 0xFFFFFFFF);
}

bool Address::operator==(const Address& other) const
{
  return this->m_data == other.m_data;
}

} // namespace Socket
