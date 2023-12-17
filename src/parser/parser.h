#pragma once

#include "ast/ExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/PrototypeAST.h"

#include <map>

namespace Parser {
	extern int current_tok;
	extern std::map<char, int> binop_precedence;

	auto get_next_token() -> int;
	auto get_tok_precedence() -> int;

	auto parse_expr() -> std::unique_ptr<AST::ExprAST>;
	auto parse_number_expr() -> std::unique_ptr<AST::ExprAST>;
	auto parse_paren_expr() -> std::unique_ptr<AST::ExprAST>;
	auto parse_identifier_expr() -> std::unique_ptr<AST::ExprAST>;
	auto parse_if_expr() -> std::unique_ptr<AST::ExprAST>;
	auto parse_for_expr() -> std::unique_ptr<AST::ExprAST>;
	auto parse_var_expr() -> std::unique_ptr<AST::ExprAST>;
	auto parse_primary() -> std::unique_ptr<AST::ExprAST>;
	auto parse_unary() -> std::unique_ptr<AST::ExprAST>;
	auto parse_prototype() -> std::unique_ptr<AST::PrototypeAST>;
	auto parse_definition() -> std::unique_ptr<AST::FunctionAST>;

	auto parse_top_level_expr() -> std::unique_ptr<AST::FunctionAST>;
	auto parse_extern() -> std::unique_ptr<AST::PrototypeAST>;

	auto parse_bin_op_rhs(int expr_prec, std::unique_ptr<AST::ExprAST> lhs)
		-> std::unique_ptr<AST::ExprAST>;
}
