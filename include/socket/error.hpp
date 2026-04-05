#pragma once
#include <string_view>

namespace Socket
{
enum class Error
{
  UNSPECIFIED,
  INVALID_ADDRESS_FAMILY,            // EAFNOSUPPORT
  TYPE_OR_PROTOCOL_DENIED,           // EACCES
  PROCESS_FILE_DESCRIPTOR_LIST_FULL, // EMFILE
  SYSTEM_FILE_DESCRIPTOR_LIST_FULL,  // ENFILE
  INSUFFICIENT_BUFFER_SPACE,         // ENOBUFS
  INSUFFICIENT_MEMORY,               // ENOMEMS
  PROTOCOL_NOT_SUPPORTED,            // EPROTONOSUPPORT
  SOCKET_TYPE_NOT_SUPPORTED,         // EPROTOTYPE
};

Error parse_errno();
}

std::string_view to_string(Socket::Error err);
