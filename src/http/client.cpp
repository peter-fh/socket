#include <cstdlib>
#include <http/client.hpp>
#include <socket/tcp.hpp>
#include <socket/poll.hpp>
#include <thread>

namespace peterfh::http
{

Result<std::vector<std::byte>, Error> send(Request req)
{
  socket::Address addr = req.url().addr();
  socket::Tcp sock;
  socket::Poll poller;
  const auto connect_result = sock.connect(addr);
  if (!connect_result.successful())
  {
    PETERFH_LOG("Tcp error on connection: " << to_string(connect_result.status()));
    return Error::CONNECTION_REFUSED;
  }

  poller.wait(sock, socket::PollEvent::Writable);

  const auto send_result = sock.send(req.data());
  if (!send_result.successful())
  {
    PETERFH_LOG("Tcp error on send: " << to_string(send_result.status()));
    return Error::SEND_CANCELLED;
  }

  poller.wait(sock, socket::PollEvent::Readable);
  std::vector<std::byte> buff;
  struct ResponseInfo {
    size_t header_end;
    size_t body_begin;
    size_t body_length;
    bool initialized;
  };

  ResponseInfo responseInfo;
  while (!responseInfo.initialized)
  {
    auto res = sock.receive(1024);
    if (!res.successful())
    {
      PETERFH_LOG("Tcp error on receive: " << to_string(res.status()));
      return Error::RECEIVE_FAILED;
    }
  }
  auto res = sock.receive(1024);

  std::string response_begin{reinterpret_cast<const char*>(buff.data()), buff.size()};

  const char* content_length_header = "Content-Length";
  size_t content_length_pos = response_begin.find(content_length_header) + std::strlen(content_length_header);
  size_t content_length_end_pos = response_begin.find("\r\n", content_length_pos);
  int content_length = std::atoi(response_begin.substr(content_length_pos, content_length_end_pos).c_str());

  for (;;)
  {
    if (!res.successful())
    {
      PETERFH_LOG("Tcp error on receive: " << to_string(res.status()));
      return Error::RECEIVE_FAILED;
    }

    std::string response_begin{reinterpret_cast<const char*>(buff.data()), buff.size()};
    buff.insert(buff.end(), res->first.begin(), res->first.end());
    if (res->second)
    {
      break;
    }
    res = sock.receive_available();
  }
  return buff;
}

}
