/*************************************************************************/
/*  error_macros.h                                                       */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H

#include "core/typedefs.h"

class String;

enum ErrorHandlerType {
	ERR_HANDLER_ERROR,
	ERR_HANDLER_WARNING,
	ERR_HANDLER_SCRIPT,
	ERR_HANDLER_SHADER,
};

// Pointer to the error handler printing function. Reassign to any function to have errors printed.
// Parameters: userdata, function, file, line, error, explanation, type.
typedef void (*ErrorHandlerFunc)(void *, const char *, const char *, int p_line, const char *, const char *, ErrorHandlerType p_type);

struct ErrorHandlerList {

	ErrorHandlerFunc errfunc;
	void *userdata;

	ErrorHandlerList *next;

	ErrorHandlerList() {
		errfunc = 0;
		next = 0;
		userdata = 0;
	}
};

void add_error_handler(ErrorHandlerList *p_handler);
void remove_error_handler(ErrorHandlerList *p_handler);

// Functions used by the error macros.
void _err_print_error(const char *p_function, const char *p_file, int p_line, const char *p_error, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const String &p_error, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const char *p_error, const char *p_message, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const String &p_error, const char *p_message, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const char *p_error, const String &p_message, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const String &p_error, const String &p_message, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_index_error(const char *p_function, const char *p_file, int p_line, int64_t p_index, int64_t p_size, const char *p_index_str, const char *p_size_str, const char *p_message = "", bool fatal = false);
void _err_print_index_error(const char *p_function, const char *p_file, int p_line, int64_t p_index, int64_t p_size, const char *p_index_str, const char *p_size_str, const String &p_message, bool fatal = false);

#ifdef __GNUC__
//#define FUNCTION_STR __PRETTY_FUNCTION__ - too annoying
#define FUNCTION_STR __FUNCTION__
#else
#define FUNCTION_STR __FUNCTION__
#endif

// Don't use this directly; instead, use any of the CRASH_* macros
#ifdef _MSC_VER
#define GENERATE_TRAP() __debugbreak()
#else
#define GENERATE_TRAP() __builtin_trap()
#endif

// Used to strip debug messages in release mode
#ifdef DEBUG_ENABLED
#define DEBUG_STR(m_msg) m_msg
#else
#define DEBUG_STR(m_msg) ""
#endif

/**
 * Error macros.
 * WARNING: These macros work in the opposite way to assert().
 *
 * Unlike exceptions and asserts, these macros try to maintain consistency and stability.
 * In most cases, bugs and/or invalid data are not fatal. They should never allow a perfectly
 * running application to fail or crash.
 * Always try to return processable data, so the engine can keep running well.
 * Use the _MSG versions to print a meaningful message to help with debugging.
 *
 * Note: See https://stackoverflow.com/questions/257418/do-while-0-what-is-it-good-for
 */

// Index out of bounds error macros.
// These macros should be used instead of `ERR_FAIL_COND` for bounds checking.

// Integer index out of bounds error macros.
// Ensures an integer index `m_index` is less than `m_size` and greater than or equal to 0.

// The current function returns.

#define ERR_FAIL_INDEX(m_index, m_size)                                                                             \
	do {                                                                                                            \
		if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                     \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size)); \
			return;                                                                                                 \
		}                                                                                                           \
	} while (0)

#define ERR_FAIL_INDEX_MSG(m_index, m_size, m_msg)                                                                                    \
	do {                                                                                                                              \
		if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                                       \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), DEBUG_STR(m_msg)); \
			return;                                                                                                                   \
		}                                                                                                                             \
	} while (0)

// The current function returns m_retval.

#define ERR_FAIL_INDEX_V(m_index, m_size, m_retval)                                                                 \
	do {                                                                                                            \
		if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                     \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size)); \
			return m_retval;                                                                                        \
		}                                                                                                           \
	} while (0)

#define ERR_FAIL_INDEX_V_MSG(m_index, m_size, m_retval, m_msg)                                                                        \
	do {                                                                                                                              \
		if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                                       \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), DEBUG_STR(m_msg)); \
			return m_retval;                                                                                                          \
		}                                                                                                                             \
	} while (0)

// Only use CRASH macros if there is no sensible fallback, that is, the error is unrecoverable.

#define CRASH_BAD_INDEX(m_index, m_size)                                                                                      \
	do {                                                                                                                      \
		if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                               \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), "", true); \
			GENERATE_TRAP();                                                                                                  \
		}                                                                                                                     \
	} while (0)

#define CRASH_BAD_INDEX_MSG(m_index, m_size, m_msg)                                                                                         \
	do {                                                                                                                                    \
		if (unlikely((m_index) < 0 || (m_index) >= (m_size))) {                                                                             \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), DEBUG_STR(m_msg), true); \
			GENERATE_TRAP();                                                                                                                \
		}                                                                                                                                   \
	} while (0)

// Unsigned integer index out of bounds error macros.
// Ensures an unsigned integer index `m_index` is less than `m_size`

// The current function returns.

#define ERR_FAIL_UNSIGNED_INDEX(m_index, m_size)                                                                    \
	do {                                                                                                            \
		if (unlikely((m_index) >= (m_size))) {                                                                      \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size)); \
			return;                                                                                                 \
		}                                                                                                           \
	} while (0)

#define ERR_FAIL_UNSIGNED_INDEX_MSG(m_index, m_size, m_msg)                                                                           \
	do {                                                                                                                              \
		if (unlikely((m_index) >= (m_size))) {                                                                                        \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), DEBUG_STR(m_msg)); \
			return;                                                                                                                   \
		}                                                                                                                             \
	} while (0)

// The current function returns m_retval.

#define ERR_FAIL_UNSIGNED_INDEX_V(m_index, m_size, m_retval)                                                        \
	do {                                                                                                            \
		if (unlikely((m_index) >= (m_size))) {                                                                      \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size)); \
			return m_retval;                                                                                        \
		}                                                                                                           \
	} while (0)

#define ERR_FAIL_UNSIGNED_INDEX_V_MSG(m_index, m_size, m_retval, m_msg)                                                               \
	do {                                                                                                                              \
		if (unlikely((m_index) >= (m_size))) {                                                                                        \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), DEBUG_STR(m_msg)); \
			return m_retval;                                                                                                          \
		}                                                                                                                             \
	} while (0)

// Only use CRASH macros if there is no sensible fallback, that is, the error is unrecoverable.

#define CRASH_BAD_UNSIGNED_INDEX(m_index, m_size)                                                                             \
	do {                                                                                                                      \
		if (unlikely((m_index) >= (m_size))) {                                                                                \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), "", true); \
			GENERATE_TRAP();                                                                                                  \
		}                                                                                                                     \
	} while (0)

#define CRASH_BAD_UNSIGNED_INDEX_MSG(m_index, m_size, m_msg)                                                                                \
	do {                                                                                                                                    \
		if (unlikely((m_index) >= (m_size))) {                                                                                              \
			_err_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), DEBUG_STR(m_msg), true); \
			GENERATE_TRAP();                                                                                                                \
		}                                                                                                                                   \
	} while (0)

// Null reference error macros.
// Ensures a pointer `m_param` is not null.

// The current function returns.

#define ERR_FAIL_NULL(m_param)                                                                              \
	do {                                                                                                    \
		if (unlikely(!m_param)) {                                                                           \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null."); \
			return;                                                                                         \
		}                                                                                                   \
	} while (0)

#define ERR_FAIL_NULL_MSG(m_param, m_msg)                                                                                     \
	do {                                                                                                                      \
		if (unlikely(!m_param)) {                                                                                             \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", DEBUG_STR(m_msg)); \
			return;                                                                                                           \
		}                                                                                                                     \
	} while (0)

// The current function returns m_retval.

#define ERR_FAIL_NULL_V(m_param, m_retval)                                                                  \
	do {                                                                                                    \
		if (unlikely(!m_param)) {                                                                           \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null."); \
			return m_retval;                                                                                \
		}                                                                                                   \
	} while (0)

#define ERR_FAIL_NULL_V_MSG(m_param, m_retval, m_msg)                                                                         \
	do {                                                                                                                      \
		if (unlikely(!m_param)) {                                                                                             \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", DEBUG_STR(m_msg)); \
			return m_retval;                                                                                                  \
		}                                                                                                                     \
	} while (0)

// Error condition macros.
// Ensures that `m_cond` is not true.

// The current function returns.

#define ERR_FAIL_COND(m_cond)                                                                              \
	do {                                                                                                   \
		if (unlikely(m_cond)) {                                                                            \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true."); \
			return;                                                                                        \
		}                                                                                                  \
	} while (0)

#define ERR_FAIL_COND_MSG(m_cond, m_msg)                                                                                     \
	do {                                                                                                                     \
		if (unlikely(m_cond)) {                                                                                              \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true.", DEBUG_STR(m_msg)); \
			return;                                                                                                          \
		}                                                                                                                    \
	} while (0)

// The current function returns m_retval.

#define ERR_FAIL_COND_V(m_cond, m_retval)                                                                                            \
	do {                                                                                                                             \
		if (unlikely(m_cond)) {                                                                                                      \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. returned: " _STR(m_retval)); \
			return m_retval;                                                                                                         \
		}                                                                                                                            \
	} while (0)

#define ERR_FAIL_COND_V_MSG(m_cond, m_retval, m_msg)                                                                                                   \
	do {                                                                                                                                               \
		if (unlikely(m_cond)) {                                                                                                                        \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. returned: " _STR(m_retval), DEBUG_STR(m_msg)); \
			return m_retval;                                                                                                                           \
		}                                                                                                                                              \
	} while (0)

// The current loop continues.

#define ERR_CONTINUE(m_cond)                                                                                             \
	do {                                                                                                                 \
		if (unlikely(m_cond)) {                                                                                          \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Continuing."); \
			continue;                                                                                                    \
		}                                                                                                                \
	} while (0)

#define ERR_CONTINUE_MSG(m_cond, m_msg)                                                                                                    \
	do {                                                                                                                                   \
		if (unlikely(m_cond)) {                                                                                                            \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Continuing.", DEBUG_STR(m_msg)); \
			continue;                                                                                                                      \
		}                                                                                                                                  \
	} while (0)

// The current loop breaks.

#define ERR_BREAK(m_cond)                                                                                              \
	do {                                                                                                               \
		if (unlikely(m_cond)) {                                                                                        \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Breaking."); \
			break;                                                                                                     \
		}                                                                                                              \
	} while (0)

#define ERR_BREAK_MSG(m_cond, m_msg)                                                                                                     \
	do {                                                                                                                                 \
		if (unlikely(m_cond)) {                                                                                                          \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Breaking.", DEBUG_STR(m_msg)); \
			break;                                                                                                                       \
		}                                                                                                                                \
	} while (0)

// Only use CRASH macros if there is no sensible fallback, that is, the error is unrecoverable.

#define CRASH_COND(m_cond)                                                                                        \
	do {                                                                                                          \
		if (unlikely(m_cond)) {                                                                                   \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Condition \"" _STR(m_cond) "\" is true."); \
			GENERATE_TRAP();                                                                                      \
		}                                                                                                         \
	} while (0)

#define CRASH_COND_MSG(m_cond, m_msg)                                                                                               \
	do {                                                                                                                            \
		if (unlikely(m_cond)) {                                                                                                     \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Condition \"" _STR(m_cond) "\" is true.", DEBUG_STR(m_msg)); \
			GENERATE_TRAP();                                                                                                        \
		}                                                                                                                           \
	} while (0)

// Generic error macros.

// The current function returns.

#define ERR_FAIL()                                                                     \
	do {                                                                               \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/Function Failed."); \
		return;                                                                        \
	} while (0)

#define ERR_FAIL_MSG(m_msg)                                                                              \
	do {                                                                                                 \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/Function Failed.", DEBUG_STR(m_msg)); \
		return;                                                                                          \
	} while (0)

// The current function returns m_retval.

#define ERR_FAIL_V(m_retval)                                                                                      \
	do {                                                                                                          \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/Function Failed, returning: " __STR(m_value)); \
		return m_retval;                                                                                          \
	} while (0)

#define ERR_FAIL_V_MSG(m_value, m_msg)                                                                                              \
	do {                                                                                                                            \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/Function Failed, returning: " __STR(m_value), DEBUG_STR(m_msg)); \
		return m_value;                                                                                                             \
	} while (0)

// Print error message macros.

#define ERR_PRINT(m_msg)                                                      \
	do {                                                                      \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, DEBUG_STR(m_msg)); \
	} while (0)

// Only prints the error message once.

#define ERR_PRINT_ONCE(m_msg)                                                     \
	do {                                                                          \
		static bool first_print = true;                                           \
		if (first_print) {                                                        \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, DEBUG_STR(m_msg)); \
			first_print = false;                                                  \
		}                                                                         \
	} while (0)

// Print warning message macros.
// To warn about deprecated usage, use `WARN_DEPRECATED` or `WARN_DEPRECATED_MSG` instead.

#define WARN_PRINT(m_msg)                                                                          \
	do {                                                                                           \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, DEBUG_STR(m_msg), ERR_HANDLER_WARNING); \
	} while (0)

// Only prints the warning message once.

#define WARN_PRINT_ONCE(m_msg)                                                                         \
	do {                                                                                               \
		static bool first_print = true;                                                                \
		if (first_print) {                                                                             \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, DEBUG_STR(m_msg), ERR_HANDLER_WARNING); \
			first_print = false;                                                                       \
		}                                                                                              \
	} while (0)

// Print deprecated warning message macros.
// Only prints the warning message once.

#define WARN_DEPRECATED                                                                                                                                   \
	do {                                                                                                                                                  \
		static volatile bool warning_shown = false;                                                                                                       \
		if (!warning_shown) {                                                                                                                             \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "This method has been deprecated and will be removed in the future.", ERR_HANDLER_WARNING); \
			warning_shown = true;                                                                                                                         \
		}                                                                                                                                                 \
	} while (0)

#define WARN_DEPRECATED_MSG(m_msg)                                                                                                                                          \
	do {                                                                                                                                                                    \
		static volatile bool warning_shown = false;                                                                                                                         \
		if (!warning_shown) {                                                                                                                                               \
			_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "This method has been deprecated and will be removed in the future.", DEBUG_STR(m_msg), ERR_HANDLER_WARNING); \
			warning_shown = true;                                                                                                                                           \
		}                                                                                                                                                                   \
	} while (0)

// Only use CRASH macros if there is no sensible fallback, that is, the error is unrecoverable.

#define CRASH_NOW()                                                                           \
	do {                                                                                      \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Method/Function Failed."); \
		GENERATE_TRAP();                                                                      \
	} while (0)

#define CRASH_NOW_MSG(m_msg)                                                                                    \
	do {                                                                                                        \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Method/Function Failed.", DEBUG_STR(m_msg)); \
		GENERATE_TRAP();                                                                                        \
	} while (0)

#endif
