#include <http/types.hpp>

namespace peterfh
{

std::string_view to_string(http::Error err)
{
  using namespace std::string_view_literals;

  switch (err)
  {
    case http::Error::RECEIVE_PARTIAL:
      return "RECEIVE_PARTIAL"sv;
    case http::Error::RECEIVE_FAILED:
      return "RECEIVE_FAILED"sv;
    case http::Error::SEND_CANCELLED:
      return "SEND_CANCELLED"sv;
    case http::Error::CONNECTION_REFUSED:
      return "CONNECTION_REFUSED"sv;
    case http::Error::FAILED:
      return "FAILED"sv;
    case http::Error::PENDING:
      return "PENDING"sv;
  }

  return "UNKNOWN"sv;
}
}
