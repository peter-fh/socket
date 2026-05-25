#include "gtest/gtest.h"
#include <http/client.hpp>
#include <http/types.hpp>

namespace peterfh
{

class HttpClientTest : public testing::Test {};

TEST_F(HttpClientTest, Get)
{
  http::Request req(http::Method::GET, "http://httpbin.org/get");
  //ASSERT_EQ(req.url().addr().url(), "104.18.26.120");
  //ASSERT_EQ(req.url().addr(), addr);
  const auto res = http::send(std::move(req));
  ASSERT_TRUE(res.successful()) << to_string(res.status());
  ASSERT_TRUE(res.has_value());
  std::vector<std::byte> data = *res;
  auto requestString = std::string{reinterpret_cast<const char*>(data.data()), data.size()};
  ASSERT_EQ(requestString, R"raw(HTTP/1.1 200 OK
Date: Wed, 29 Apr 2026 09:02:09 GMT
Content-Type: application/json
Content-Length: 255
Connection: keep-alive
Server: gunicorn/19.9.0
Access-Control-Allow-Origin: *
Access-Control-Allow-Credentials: true

{
  "args": {},
  "headers": {
    "Accept": "*/*",
    "Host": "httpbin.org",
    "User-Agent": "curl/8.7.1",
    "X-Amzn-Trace-Id": "Root=1-69f1c911-68f311981b2b2b552bc9bcba"
  },
  "origin": "207.164.135.100",
  "url": "http://httpbin.org/get"
})raw");
}

} // namespace peterfh
