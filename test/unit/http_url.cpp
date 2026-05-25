#include "gtest/gtest.h"
#include <http/url.hpp>

namespace peterfh
{

class HttpUrlTestUnit : public testing::Test {};

TEST_F(HttpUrlTestUnit, str)
{
  std::string url_data{"http://localhost/api"};
  http::Url url(url_data);
  ASSERT_EQ(url_data, url.str());
}

}
