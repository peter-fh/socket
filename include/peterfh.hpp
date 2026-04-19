/*
 * Header-only library for convenience while developing personal cpp projects
*/

#pragma once
#include <memory>
#include <optional>

// Helper macros
#if defined(_DEBUG) || !defined(NDEBUG)
#define PETERFH_BUILD_DEBUG
#endif

#ifdef PETERFH_BUILD_DEBUG
#include <cstdlib>
#include <iostream>
#if defined(_MSC_VER)
#define PETERFH_DEBUGBREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define PETERFH_DEBUGBREAK() __builtin_trap()
#elif defined(SIGTRAP)
#include <csignal>
#define PETERFH_DEBUGBREAK() raise(SIGTRAP)
#else
#define PETERFH_DEBUGBREAK() std::abort()
#endif

#define _LOG_BASE(msg) std::cout << msg << std::endl
#define PETERFH_LOG(msg) _LOG_BASE("LOG [" << __FILE__ << ":" << __LINE__ << "]: " << msg)
#define PETERFH_ASSERT(cond, msg) \
	if (!(cond)) \
	{ \
		_LOG_BASE("ASSERTION FAILED [" << __FILE__ << ":" << __LINE__ << ":" << __func__ << "] (" << #cond << "): " << msg); \
		PETERFH_DEBUGBREAK(); \
	} 

#define PETERFH_ASSERT_ALWAYS(msg) \
	PETERFH_LOG(msg); \
	PETERFH_DEBUGBREAK();
#else
#define PETERFH_LOG(msg)
#define PETERFH_ASSERT(cond, msg)
#define PETERFH_ASSERT_ALWAYS(msg)
#endif

// Personal namespace to avoid collisions
namespace peterfh
{

struct Void {};

template <typename TValue, typename TError>
class Result
{
public:
	Result();
	Result(TValue value);
	Result(TError err);

	bool successful() const;
	bool has_value() const;

	// Available when successful is true
	const TValue& operator*() const;
	TValue& operator*();
	const TValue* operator->() const;
	TValue* operator->();

	// Available when successful is false
	TError err() const;
private:
	std::optional<TValue> m_value;
	std::optional<TError> m_error;
};

/*
* Inline template implementations
*/

template <typename TValue, typename TError>
Result<TValue, TError>::Result() {}

template <typename TValue, typename TError>
Result<TValue, TError>::Result(TValue value) : m_value(std::move(value)), m_error(std::nullopt) {}

template <typename TValue, typename TError>
Result<TValue, TError>::Result(TError error) : m_value(std::nullopt), m_error(error) {}

template <typename TValue, typename TError>
bool Result<TValue, TError>::successful() const
{
	return !m_error.has_value();
}

template <typename TValue, typename TError>
bool Result<TValue, TError>::has_value() const
{
	return m_value.has_value();
}

template <typename TValue, typename TError>
TError Result<TValue, TError>::err() const
{
	return *m_error;
}

template <typename TValue, typename TError>
const TValue& Result<TValue, TError>::operator*() const
{
	return *m_value;
}

template <typename TValue, typename TError>
TValue& Result<TValue, TError>::operator*()
{
	return *m_value;
}

template <typename TValue, typename TError>
const TValue* Result<TValue, TError>::operator->() const
{
	return std::addressof(*m_value);
}

template <typename TValue, typename TError>
TValue* Result<TValue, TError>::operator->()
{
	return std::addressof(*m_value);
}

#if defined(PETERFH_COMMON_IMPLEMENTATION)
#endif

} // namespace peterfh
