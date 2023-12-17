#pragma once
#include "ast/ExprAST.h"

namespace AST {
	/**
	 * contains Binary operations (can hold other expressions!)
	 */
	class BinaryExprAST : public ExprAST {
		char m_operator;
		std::unique_ptr<AST::ExprAST> m_lhs;
		std::unique_ptr<AST::ExprAST> m_rhs;

	public:

		BinaryExprAST(
			char Op, std::unique_ptr<AST::ExprAST> LHS, std::unique_ptr<AST::ExprAST> RHS) :
			m_operator(Op), m_lhs(std::move(LHS)), m_rhs(std::move(RHS)) {}

		auto codegen() -> llvm::Value* override;
	};
}