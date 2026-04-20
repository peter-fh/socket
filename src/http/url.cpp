#include <http/url.hpp>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <charconv>

namespace peterfh::http
{

Url::Url(std::string_view url)
: m_addr(""), m_isValid(true)
{
  std::string_view protocol_delimiter = "://";
  size_t protocol_pos = url.find(protocol_delimiter);
  std::string_view protocol = url.substr(0, protocol_pos);
  if (protocol == "http")
  {
    m_protocol = Protocol::HTTP;
  }
  else if (protocol == "https")
  {
    m_protocol = Protocol::HTTPS;
  }
  else
  {
    PETERFH_LOG("Error: Created URL with invalid protocol");
    m_isValid = false;
    m_addr = socket::Address("");
    return;
  }

  url = url.substr(protocol_pos + protocol_delimiter.size(), url.size());
  size_t port_pos = url.find(':');
  size_t path_pos = url.find('/');
  if (port_pos != std::string::npos)
  {
    std::string_view domain = url.substr(0, port_pos);
    std::string_view port_data = url.substr(port_pos + 1, path_pos-port_pos);
    uint16_t port{};
    if (std::from_chars(port_data.data(), port_data.data() + port_data.size(), port).ec != std::errc{})
    {
      PETERFH_LOG("Error: Port is not a valid integer: " << url << ".substr(" << port_pos << ", " << path_pos << ")=" << port_data);
      m_isValid = false;
      m_addr = socket::Address("");
      return;
    }
    m_domain = domain;
    m_addr = socket::Address(to_ip(domain), port);
  }
  else if (path_pos != std::string::npos)
  {
    std::string_view domain = url.substr(0, path_pos);
    m_domain = domain;
    m_addr = socket::Address(to_ip(domain));
  }
  else
  {
    m_domain = url;
    m_addr = socket::Address(to_ip(url));
    return;
  }
  m_path = url.substr(path_pos);

}

std::string_view Url::protocol()
{
  using namespace std::string_view_literals;
  switch (m_protocol)
  {
    case Protocol::HTTP: return "http://"sv; 
    case Protocol::HTTPS: return "https://"sv;
    case Protocol::INVALID: return "invalid://"sv;
  }
}

std::string_view Url::domain()
{
  return m_domain;
}

std::string Url::port()
{
  return std::to_string(m_addr.port());
}

std::string_view Url::path()
{
  return m_path;
}

std::string Url::str()
{
  std::string url{};
  if (!m_isValid)
  {
    return url;
  }

  url += protocol();
  url += domain();
  if (m_addr.port())
  {
    url += ":" + port();
  }
  url += path();
  return url;
}


socket::Address Url::addr()
{
  return m_addr;
}

std::string Url::to_ip(std::string_view domain)
{
  std::string domain_data{domain};
  struct addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  struct addrinfo* res;
  int status = getaddrinfo(domain_data.c_str(), NULL, &hints, &res);
  if (status != 0)
  {
    PETERFH_LOG("Error getting address info: " << gai_strerror(status));
    m_isValid = false;
    return "";
  }
  char ip[INET_ADDRSTRLEN];
  inet_ntop(
    AF_INET,
    &reinterpret_cast<sockaddr_in*>(res->ai_addr)->sin_addr,
    ip,
    sizeof(ip)
  );
  return std::string(ip);

}

std::string Url::from_ip(std::string_view ip)
{
  std::string ip_data{ip};
  struct sockaddr_in sa{};
  sa.sin_family = AF_INET;
  inet_pton(AF_INET, ip_data.c_str(), &sa.sin_addr);

  char host[NI_MAXHOST];
  int status = getnameinfo(reinterpret_cast<sockaddr*>(&sa), sizeof(sa), host, sizeof(host), nullptr, 0, NI_NAMEREQD);

  if (status != 0)
  {
    PETERFH_LOG("Error getting domain info for domain " << ip << ": " << gai_strerror(status));
    return m_domain;
  }
  return std::string{host};
}


}
