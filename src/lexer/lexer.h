#pragma once

#include <string>

namespace Lexer {
	extern std::string ident_str;
	extern double num_val;

	auto get_token() -> int;
}
