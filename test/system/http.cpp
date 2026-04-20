#include "gtest/gtest.h"
#include <http/client.hpp>
#include <http/types.hpp>

namespace peterfh
{

class HttpClientTest : public testing::Test {};

TEST_F(HttpClientTest, Get)
{
  http::Request req(http::Method::GET, "http://example.com");
  const auto res = http::send(std::move(req));
  ASSERT_TRUE(res.successful()) << std::to_string(res.status());
  ASSERT_TRUE(res.has_value());
  std::vector<std::byte> data = *res;
  auto requestString = std::string{reinterpret_cast<const char*>(data.data()), data.size()};
  ASSERT_EQ(requestString, R"json(<!doctype html><html lang="en"><head><title>Example Domain</title><meta name="viewport" content="width=device-width, initial-scale=1"><style>body{background:#eee;width:60vw;margin:15vh auto;font-family:system-ui,sans-serif}h1{font-size:1.5em}div{opacity:0.8}a:link,a:visited{color:#348}</style></head><body><div><h1>Example Domain</h1><p>This domain is for use in documentation examples without needing permission. Avoid use in operations.</p><p><a href="https://iana.org/domains/example">Learn more</a></p></div></body></html>)json");
}

} // namespace peterfh
