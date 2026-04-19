#include <netinet/in.h>
#include <socket/address.hpp>

#include <arpa/inet.h>
#include <string>

#define VALID_BIT (1ULL << 63)
#define PORT_SHIFT 32
#define PORT_MASK 0xFFFF
#define URL_MASK 0xFFFFFFFF
namespace peterfh::socket
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
  return ntohs(static_cast<uint16_t>((m_data >> PORT_SHIFT) & PORT_MASK));
}

std::string Address::url() const
{
  const auto url_data = static_cast<uint32_t>(m_data & URL_MASK);
  char buf[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &url_data, buf, sizeof(buf));
  return std::string{buf};
}

sockaddr_in Address::socket_address() const
{

  struct sockaddr_in socket_address;
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = static_cast<uint32_t>(m_data & URL_MASK);
  socket_address.sin_port = static_cast<uint16_t>((m_data >> PORT_SHIFT) & PORT_MASK);
  return socket_address;
}

bool Address::operator==(const Address& other) const
{
  return this->m_data == other.m_data;
}

} // namespace Socket
