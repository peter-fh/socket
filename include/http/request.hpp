#include <peterfh.hpp>
#pragma once
#include <http/url.hpp>

#include <string>
#include <vector>

namespace peterfh
{

namespace http
{
enum class Method {
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	PATCH,
};

class Request
{
public:
	Request(Method method, Url url, 
	 std::initializer_list<std::pair<std::string_view, std::string_view>> headers = {});

	Url url();
	std::vector<std::pair<std::string, std::string>> headers();
	std::vector<std::byte> data();
	std::string_view method();
private:
	Method m_method;
	std::vector<std::pair<std::string, std::string>> m_headers;
	Url m_url;
};

} // namespace http
} // namespace peterfh
