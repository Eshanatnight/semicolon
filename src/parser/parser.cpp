#include "parser/parser.h"

#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/ForExprAST.h"
#include "ast/IfExprAST.h"
#include "ast/NumberExprAST.h"
#include "ast/UnaryExprAST.h"
#include "ast/VarExprAST.h"
#include "ast/VariableExprAST.h"
#include "lexer/lexer.h"
#include "lexer/tokens.h"
#include "logger/logger.h"
#include "parser/parser.h"
// #include "ast/PrototypeAST.h"
// #include "ast/FunctionAST.h"
#include "parser.h"

int Parser::current_tok;
std::map<char, int>
	Parser::binop_precedence; // ititialized in main.cpp mave initialize here instead?

auto Parser::get_next_token() -> int {
	return current_tok = Lexer::get_token();
}

auto Parser::get_tok_precedence() -> int {
	if(!isascii(current_tok)) {
		return -1;
	}

	// Make sure it's a declared binop.
	int tok_prec = binop_precedence[current_tok];
	if(tok_prec <= 0) {
		return -1;
	}
	return tok_prec;
}

auto Parser::parse_expr() -> std::unique_ptr<AST::ExprAST> {
	auto lhs = Parser::parse_unary();

	if(lhs == nullptr) {
		return nullptr;
	}
	return Parser::parse_bin_op_rhs(0, std::move(lhs));
}

auto Parser::parse_number_expr() -> std::unique_ptr<AST::ExprAST> {
	auto result = std::make_unique<AST::NumberExprAST>(Lexer::num_val);
	Parser::get_next_token();

	return std::move(result);
}

auto Parser::parse_paren_expr() -> std::unique_ptr<AST::ExprAST> {
	Parser::get_next_token();
	auto value = Parser::parse_expr();

	if(value == nullptr) {
		return nullptr;
	}

	if(Parser::current_tok != ')') {
		return Logger::LogError("Expected ')'");
	}

	Parser::get_next_token();

	return value;
}

auto Parser::parse_identifier_expr() -> std::unique_ptr<AST::ExprAST> {
	std::string ident_name = Lexer::ident_str;

	Parser::get_next_token(); // eat identifier.

	// Simple variable ref.
	if(Parser::current_tok != '(') {
		return std::make_unique<AST::VariableExprAST>(ident_name);
	}

	Parser::get_next_token();
	std::vector<std::unique_ptr<AST::ExprAST>> args;

	if(Parser::current_tok != ')') {
		while(true) {
			if(auto arg = Parser::parse_expr()) {
				args.push_back(std::move(arg));
			} else {
				return nullptr;
			}

			if(Parser::current_tok == ')') {
				break;
			}

			if(Parser::current_tok != ',') {
				return Logger::LogError("Expected ')' or ',' in argument list");
			}
			Parser::get_next_token();
		}
	}
	// Eat the ')'.
	Parser::get_next_token();

	return std::make_unique<AST::CallExprAST>(ident_name, std::move(args));
}

auto Parser::parse_if_expr() -> std::unique_ptr<AST::ExprAST> {
	Parser::get_next_token();

	auto condition_expr = Parser::parse_expr();

	if(condition_expr == nullptr) {
		return nullptr;
	}
	if(Parser::current_tok != tok_then) {
		return Logger::LogError("expected then");
	}
	Parser::get_next_token(); // eat the then

	auto then_expr = Parser::parse_expr();

	if(then_expr == nullptr) {
		return nullptr;
	}

	if(Parser::current_tok != tok_else) {
		return Logger::LogError("expected else");
	}

	Parser::get_next_token();

	auto else_expr = Parser::parse_expr();

	if(else_expr == nullptr) {
		return nullptr;
	}

	return std::make_unique<AST::IfExprAST>(
		std::move(condition_expr), std::move(then_expr), std::move(else_expr));
}

auto Parser::parse_for_expr() -> std::unique_ptr<AST::ExprAST> {
	Parser::get_next_token();

	if(Parser::current_tok != tok_identifier) {
		return Logger::LogError("expected identifier after for");
	}

	std::string ident_name = Lexer::ident_str;
	Parser::get_next_token();

	if(Parser::current_tok != '=') {
		return Logger::LogError("expected '=' after for");
	}
	Parser::get_next_token();

	auto start_expr = Parser::parse_expr();
	if(start_expr == nullptr) {
		return nullptr;
	}
	if(Parser::current_tok != ',') {
		return Logger::LogError("expected ',' after for start value");
	}
	Parser::get_next_token();

	auto end_expr = Parser::parse_expr();
	if(end_expr == nullptr) {
		return nullptr;
	}

	// The step value is optional.
	std::unique_ptr<AST::ExprAST> step_expr;
	if(Parser::current_tok == ',') {
		Parser::get_next_token();
		step_expr = Parser::parse_expr();
		if(step_expr == nullptr) {
			return nullptr;
		}
	}

	if(Parser::current_tok != tok_in) {
		return Logger::LogError("expected 'in' after for");
	}
	Parser::get_next_token();

	auto body_expr = Parser::parse_expr();
	if(body_expr == nullptr) {
		return nullptr;
	}

	return std::make_unique<AST::ForExprAST>(ident_name,
		std::move(start_expr),
		std::move(end_expr),
		std::move(step_expr),
		std::move(body_expr));
}

auto Parser::parse_var_expr() -> std::unique_ptr<AST::ExprAST> {
	Parser::get_next_token();
	std::vector<std::pair<std::string, std::unique_ptr<AST::ExprAST>>> var_names;

	// At least one variable name is required.
	if(Parser::current_tok != tok_identifier) {
		return Logger::LogError("expected identifier after var");
	}

	while(true) {
		std::string name = Lexer::ident_str;
		Parser::get_next_token();

		// Read the optional initializer.
		std::unique_ptr<AST::ExprAST> init = nullptr;
		if(Parser::current_tok == '=') {
			Parser::get_next_token();

			init = Parser::parse_expr();
			if(init == nullptr) {
				return nullptr;
			}
		}

		var_names.push_back(std::make_pair(name, std::move(init)));
		//              ^ probably use empalce_back here instead.

		// End of var list, exit loop.
		if(Parser::current_tok != ',') {
			break;
		}

		Parser::get_next_token(); // eat the ','

		if(Parser::current_tok != tok_identifier) {
			return Logger::LogError("expected identifier list after var");
		}
	}

	// At this point, we have to have 'in'.
	if(Parser::current_tok != tok_in) {
		return Logger::LogError("expected 'in' keyword after 'var'");
	}
	Parser::get_next_token(); // eat 'in'.

	auto body = Parser::parse_expr();
	if(body == nullptr) {
		return nullptr;
	}

	return std::make_unique<AST::VarExprAST>(std::move(var_names), std::move(body));
}

auto Parser::parse_primary() -> std::unique_ptr<AST::ExprAST> {
	switch(Parser::current_tok) {
	case tok_identifier :
		return Parser::parse_identifier_expr();
	case tok_number :
		return Parser::parse_number_expr();
	case '(' :
		return Parser::parse_paren_expr();
	case tok_if :
		return Parser::parse_if_expr();
	case tok_for :
		return Parser::parse_for_expr();
	case tok_var :
		return Parser::parse_var_expr();
	default :
		return Logger::LogError("unknown token when expecting an expression");
	}
}

auto Parser::parse_unary() -> std::unique_ptr<AST::ExprAST> {
	// If the current token is not an operator, it must be a primary
	// expr.
	if(!isascii(Parser::current_tok) || Parser::current_tok == '(' || Parser::current_tok == ',') {
		return Parser::parse_primary();
	}

	// If this is a unary operator, read it.
	int op_code = Parser::current_tok;
	Parser::get_next_token();

	if(auto operand = Parser::parse_unary()) {
		return std::make_unique<AST::UnaryExprAST>(op_code, std::move(operand));
	}

	return nullptr;
}

auto Parser::parse_prototype() -> std::unique_ptr<AST::PrototypeAST> {
	std::string fn_name;

	unsigned kind			   = 0; // 0 = identifier, 1 = unary, 2 = binary.
	unsigned binary_precedence = 30;

	switch(Parser::current_tok) {
	case tok_identifier :
		fn_name = Lexer::ident_str;
		kind	= 0;
		Parser::get_next_token();
		break;
	case tok_unary :
		Parser::get_next_token();
		if(!isascii(Parser::current_tok)) {
			return Logger::LogErrorP("Expected unary operator");
		}
		fn_name = "unary";
		fn_name += static_cast<char>(Parser::current_tok);
		kind = 1;
		Parser::get_next_token();
		break;

	case tok_binary :
		Parser::get_next_token();
		if(!isascii(Parser::current_tok)) {
			return Logger::LogErrorP("Expected binary operator");
		}

		fn_name = "binary";
		fn_name += static_cast<char>(Parser::current_tok);
		kind = 2;
		Parser::get_next_token();

		// Read the precedence if present.
		if(Parser::current_tok == tok_number) {
			if(Lexer::num_val < 1 || Lexer::num_val > 100) {
				return Logger::LogErrorP("Invalid precedence: must be 1..100");
			}
			binary_precedence = static_cast<unsigned>(Lexer::num_val);
			Parser::get_next_token();
		}
		break;
	default :
		return Logger::LogErrorP("Expected function name in prototype");
	}

	if(Parser::current_tok != '(') {
		return Logger::LogErrorP("Expected '(' in prototype");
	}

	std::vector<std::string> arg_names;

	while(Parser::get_next_token() == tok_identifier) {
		arg_names.push_back(Lexer::ident_str);
	}

	if(Parser::current_tok != ')') {
		return Logger::LogErrorP("Expected ')' in prototype");
	}

	Parser::get_next_token(); // eat ')'.

	// Verify right number of names for operator.
	if(kind && arg_names.size() != kind) {
		return Logger::LogErrorP("Invalid number of operands for operator");
	}

	return std::make_unique<AST::PrototypeAST>(
		fn_name, std::move(arg_names), kind != 0, binary_precedence);
}

auto Parser::parse_definition() -> std::unique_ptr<AST::FunctionAST> {
	Parser::get_next_token(); // eat def.
	auto proto = Parser::parse_prototype();

	if(proto == nullptr) {
		return nullptr;
	}

	if(auto expr = Parser::parse_expr()) {
		return std::make_unique<AST::FunctionAST>(std::move(proto), std::move(expr));
	}

	return nullptr;
}

auto Parser::parse_top_level_expr() -> std::unique_ptr<AST::FunctionAST> {
	if(auto expr = Parser::parse_expr()) {
		// Make an anonymous proto.
		auto proto = std::make_unique<AST::PrototypeAST>("__anon_expr", std::vector<std::string>());

		return std::make_unique<AST::FunctionAST>(std::move(proto), std::move(expr));
	}
	return nullptr;
}

auto Parser::parse_extern() -> std::unique_ptr<AST::PrototypeAST> {
	Parser::get_next_token(); // eat extern.
	return Parser::parse_prototype();
}

auto Parser::parse_bin_op_rhs(int expr_prec, std::unique_ptr<AST::ExprAST> lhs)
	-> std::unique_ptr<AST::ExprAST> {
	while(true) {
		int tok_prec = Parser::get_tok_precedence();

		// If this is a binop that binds at least as tightly as the
		// current binop, consume it, otherwise we are done.

		if(tok_prec < expr_prec) {
			return lhs;
		}

		int bin_op = Parser::current_tok;
		Parser::get_next_token(); // eat binop

		// Parse the primary expression after the binary operator.
		auto rhs = Parser::parse_unary();
		if(!rhs) {
			return nullptr;
		}

		// If binop binds less tightly with rhs than the operator
		// after rhs, let the pending operator take rhs as its lhs.
		int next_prec = Parser::get_tok_precedence();
		if(tok_prec < next_prec) {
			rhs = Parser::parse_bin_op_rhs(tok_prec + 1, std::move(rhs));
			if(rhs == nullptr) {
				return nullptr;
			}
		}

		// Merge lhs/rhs.
		lhs = std::make_unique<AST::BinaryExprAST>(bin_op, std::move(lhs), std::move(rhs));
	}
}
