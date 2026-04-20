#include <peterfh.hpp>
#pragma once

namespace peterfh
{

namespace http
{

enum class Error
{
	PENDING,
	CONNECTION_REFUSED,
	SEND_CANCELLED,
	RECEIVE_FAILED,
	RECEIVE_PARTIAL,
	FAILED,
};

} // namespace http

std::string_view to_string(http::Error err);

} // namespace peterfh
