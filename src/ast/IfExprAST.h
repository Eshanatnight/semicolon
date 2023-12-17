#pragma once

#include "ast/ExprAST.h"

namespace AST {
	class IfExprAST : public ExprAST {
		std::unique_ptr<ExprAST> m_cond;
		std::unique_ptr<ExprAST> m_then;
		std::unique_ptr<ExprAST> m_else;

	public:

		IfExprAST(std::unique_ptr<AST::ExprAST> Cond,
			std::unique_ptr<AST::ExprAST> Then,
			std::unique_ptr<AST::ExprAST> Else) :
			m_cond(std::move(Cond)), m_then(std::move(Then)), m_else(std::move(Else)) {}

		auto codegen() -> llvm::Value* override;
	};

}
