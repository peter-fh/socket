#include "gtest/gtest.h"
#include <http/request.hpp>

namespace peterfh
{

class HttpRequestTestUnit : public testing::Test {};

TEST_F(HttpRequestTestUnit, Headers)
{
  const char* url = "http://example.com";
  http::Request req(http::Method::GET, url, {
    {"a", "1"},
    {"b", "2"},
    {"c", "3"}
  });

  auto headers = req.headers();
  ASSERT_EQ(headers[0], std::make_pair("Host", "example.com"));
  ASSERT_EQ(headers[1], std::make_pair("a", "1"));
  ASSERT_EQ(headers[2], std::make_pair("b", "2"));
  ASSERT_EQ(headers[3], std::make_pair("c", "3"));

  ASSERT_EQ(url, req.url().str());
}

TEST_F(HttpRequestTestUnit, Data)
{
  const char* url = "http://example.com";
  http::Request req(http::Method::GET, url, {
    {"a", "1"},
    {"b", "2"},
    {"c", "3"}
  });
  const char* expectedData=
    "GET / HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "a: 1\r\n"
    "b: 2\r\n"
    "c: 3\r\n"
    "\r\n";
  std::vector<std::byte> data = req.data();
  auto requestString = std::string{reinterpret_cast<const char*>(data.data()), data.size()};
  ASSERT_EQ(expectedData, requestString);
}

TEST_F(HttpRequestTestUnit, Data_QueryPath)
{
  const char* url = "http://example.com/search?q=hello&page=2";
  http::Request req(http::Method::GET, url, {});
  const char* expectedData =
    "GET /search?q=hello&page=2 HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "\r\n";
  std::vector<std::byte> data = req.data();
  auto requestString = std::string{reinterpret_cast<const char*>(data.data()), data.size()};
  ASSERT_EQ(expectedData, requestString);
}

TEST_F(HttpRequestTestUnit, Data_Delete)
{
  const char* url = "http://example.com/users/42";
  http::Request req(http::Method::DELETE, url, {
    {"Authorization", "Bearer token"}
  });
  const char* expectedData =
    "DELETE /users/42 HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "Authorization: Bearer token\r\n"
    "\r\n";
  std::vector<std::byte> data = req.data();
  auto requestString = std::string{reinterpret_cast<const char*>(data.data()), data.size()};
  ASSERT_EQ(expectedData, requestString);
}

TEST_F(HttpRequestTestUnit, Data_EmptyHeaderValue)
{
  const char* url = "http://example.com";
  http::Request req(http::Method::GET, url, {
    {"X-Custom-Header", ""}
  });
  const char* expectedData =
    "GET / HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "X-Custom-Header: \r\n"
    "\r\n";
  std::vector<std::byte> data = req.data();
  auto requestString = std::string{reinterpret_cast<const char*>(data.data()), data.size()};
  ASSERT_EQ(expectedData, requestString);
}

TEST_F(HttpRequestTestUnit, Data_ManyHeaders)
{
  const char* url = "http://example.com/resource";
  http::Request req(http::Method::GET, url, {
    {"Accept", "text/html"},
    {"Accept-Encoding", "gzip, deflate, br"},
    {"Accept-Language", "en-US,en;q=0.9"},
    {"Authorization", "Bearer abc123token"},
    {"Cache-Control", "no-cache"},
    {"Connection", "keep-alive"},
    {"User-Agent", "MyApp/1.0"}
  });
  const char* expectedData =
    "GET /resource HTTP/1.1\r\n"
    "Host: example.com\r\n"
    "Accept: text/html\r\n"
    "Accept-Encoding: gzip, deflate, br\r\n"
    "Accept-Language: en-US,en;q=0.9\r\n"
    "Authorization: Bearer abc123token\r\n"
    "Cache-Control: no-cache\r\n"
    "Connection: keep-alive\r\n"
    "User-Agent: MyApp/1.0\r\n"
    "\r\n";
  std::vector<std::byte> data = req.data();
  auto requestString = std::string{reinterpret_cast<const char*>(data.data()), data.size()};
  ASSERT_EQ(expectedData, requestString);
}

}

