#include <peterfh.hpp>
#pragma once
#include <string_view>
#include <socket/address.hpp>

namespace peterfh
{

namespace http
{

class Url
{
public:
	Url(const char* url) : Url(std::string_view{url}) {}
	Url(std::string_view url);

	std::string_view protocol();
	std::string_view domain();
	std::string_view path();

	std::string port();
	std::string str();
	// TODO: Support queries

	socket::Address addr();
private:
	std::string to_ip(std::string_view domain);
	std::string from_ip(std::string_view ip);
	enum class Protocol
	{
		INVALID,
		HTTP,
		HTTPS,
	};
	Protocol m_protocol;
	socket::Address m_addr;
	std::string m_domain;
	std::string m_path;
	bool m_isValid;
};

} // namespace http
} // namespace peterfh
