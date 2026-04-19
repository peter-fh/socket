#include <http/request.hpp>
#include <span>

namespace peterfh::http
{
Request::Request(Method method, Url url, std::initializer_list<std::pair<std::string_view, std::string_view>> headers)
: m_method(method), m_headers{}, m_url(std::move(url))
{
  m_headers.reserve(1 + headers.size());
  using namespace std::string_view_literals;
  m_headers.push_back(std::make_pair(std::string{"Host"}, std::string{m_url.domain()}));
  m_headers.insert(m_headers.end(), headers.begin(), headers.end());
}
std::string_view Request::method()
{
  using namespace std::string_view_literals;
  switch (m_method)
  {
    case Method::GET: return "GET"sv;
    case Method::HEAD: return "HEAD"sv;
    case Method::POST: return "POST"sv;
    case Method::PUT: return "PUT"sv;
    case Method::DELETE: return "DELETE"sv;
    case Method::PATCH: return "PATCH"sv;
  }
}

std::vector<std::pair<std::string, std::string>> Request::headers()
{
  return m_headers;
}

Url Request::url()
{
  return m_url;
}

std::vector<std::byte> Request::data()
{
  using namespace std::string_view_literals;

  std::string_view path_sv = m_url.path().size() != 0 ? m_url.path() : "/"sv;
  auto iterate_request_view = [&](auto&& fn)
  {
      fn({method(), " "sv, path_sv, " HTTP/1.1"sv, "\r\n"sv});
      for (const auto& [key, value] : m_headers)
        fn({key, ": "sv, value, "\r\n"sv});
      fn({"\r\n"sv});
  };

  size_t data_size{};
  iterate_request_view([&](std::initializer_list<std::string_view> views) {
    for (const auto view : views)
    {
      data_size += view.size();
    }
  });

  std::vector<std::byte> data;
  data.reserve(data_size);
  iterate_request_view([&](std::initializer_list<std::string_view> views) {
      for (const auto view : views)
      {
        std::span<const std::byte> data_view = std::as_bytes(std::span<const char>{view.data(), view.size()});
        data.insert(data.end(), data_view.begin(), data_view.end());
      }
  });

  return data;
}

}
