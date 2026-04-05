#include <netinet/in.h>
#include <socket/address.hpp>

#include <arpa/inet.h>
#include <string>

namespace Socket
{

Address::Address(sockaddr_in addr)
: m_url(addr.sin_addr.s_addr), m_port(addr.sin_port), m_isValid(true) {}

Address::Address(std::string_view address, uint16_t port)
{
  std::string s(address);
  int res = inet_pton(AF_INET, s.c_str(), &m_url);
  if (res <= 0)
  {
    m_isValid = false;
    return;
  }
  m_isValid = true;
  m_port = htons(port);
}

Address::Address(std::string_view address)
: Address(address, 0) {}

uint16_t Address::port()
{
  return m_port;
}

uint32_t Address::url()
{
  return m_url;
}

bool Address::operator==(const Address& other) const
{
  return this->m_url == other.m_url &&
         this->m_port == other.m_port &&
         this->m_isValid == other.m_isValid;
}
}
