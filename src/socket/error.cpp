#include <socket/error.hpp>
#include <cerrno>

namespace Socket
{

Error parse_errno()
{
  switch (errno)
  {
    case EAFNOSUPPORT:
      return Error::INVALID_ADDRESS_FAMILY;
    case EACCES:
      return Error::TYPE_OR_PROTOCOL_DENIED;
    case EMFILE:
      return Error::PROCESS_FILE_DESCRIPTOR_LIST_FULL;
    case ENFILE:
      return Error::SYSTEM_FILE_DESCRIPTOR_LIST_FULL;
    case ENOBUFS:
      return Error::INSUFFICIENT_BUFFER_SPACE;
    case ENOMEM:
      return Error::INSUFFICIENT_MEMORY;
    case EPROTONOSUPPORT:
      return Error::PROTOCOL_NOT_SUPPORTED;
    case EPROTOTYPE:
      return Error::SOCKET_TYPE_NOT_SUPPORTED;
    default:
      return Error::UNSPECIFIED;
  }
}

} // namespace Socket

std::string_view to_string(Socket::Error err)
{
  using namespace std::string_view_literals;
  switch (err)
  {
    case Socket::Error::UNSPECIFIED: return "UNSPECIFIED"sv;
    case Socket::Error::INVALID_ADDRESS_FAMILY: return "INVALID_ADDRESS_FAMILY"sv;
    case Socket::Error::TYPE_OR_PROTOCOL_DENIED: return "TYPE_OR_PROTOCOL_DENIED"sv;
    case Socket::Error::PROCESS_FILE_DESCRIPTOR_LIST_FULL: return "PROCESS_FILE_DESCRIPTOR_LIST_FULL"sv;
    case Socket::Error::SYSTEM_FILE_DESCRIPTOR_LIST_FULL: return "SYSTEM_FILE_DESCRIPTOR_LIST_FULL"sv;
    case Socket::Error::INSUFFICIENT_BUFFER_SPACE: return "INSUFFICIENT_BUFFER_SPACE"sv;
    case Socket::Error::INSUFFICIENT_MEMORY: return "INSUFFICIENT_MEMORY"sv;
    case Socket::Error::PROTOCOL_NOT_SUPPORTED: return "PROTOCOL_NOT_SUPPORTED"sv;
    case Socket::Error::SOCKET_TYPE_NOT_SUPPORTED: return "SOCKET_TYPE_NOT_SUPPORTED"sv;
  }
}
