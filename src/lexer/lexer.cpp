#include "lexer/lexer.h"

#include "lexer/tokens.h"

std::string Lexer::ident_str;
double Lexer::num_val;

/// gettok - Return the next token from standard input.
auto Lexer::get_token() -> int {
	static int last_char = ' ';

	// Skip any whitespace.
	while(isspace(last_char)) {
		last_char = getchar();
	}

	if(isalpha(last_char)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
		Lexer::ident_str = last_char;
		while(isalnum((last_char = getchar()))) {
			Lexer::ident_str += last_char;
		}

		if(Lexer::ident_str == "def") return tok_def;
		if(Lexer::ident_str == "extern") return tok_extern;
		if(Lexer::ident_str == "if") return tok_if;
		if(Lexer::ident_str == "then") return tok_then;
		if(Lexer::ident_str == "else") return tok_else;
		if(Lexer::ident_str == "for") return tok_for;
		if(Lexer::ident_str == "in") return tok_in;
		if(Lexer::ident_str == "binary") return tok_binary;
		if(Lexer::ident_str == "unary") return tok_unary;
		if(Lexer::ident_str == "var") return tok_var;
		return tok_identifier;
	}

	if(isdigit(last_char) || last_char == '.') { // Number: [0-9.]+
		std::string num_str;
		do {
			num_str += last_char;
			last_char = getchar();
		} while(isdigit(last_char) || last_char == '.');

		Lexer::num_val = strtod(num_str.c_str(), nullptr);
		return tok_number;
	}

	if(last_char == '#') {
		// Comment until end of line.
		do {
			last_char = getchar();
		} while(last_char != EOF && last_char != '\n' && last_char != '\r');

		if(last_char != EOF) {
			return get_token();
		}
	}

	// Check for end of file.  Don't eat the EOF.
	if(last_char == EOF) {
		return tok_eof;
	}

	// Otherwise, just return the character as its ascii value.
	int this_char = last_char;
	last_char	  = getchar();
	return this_char;
}