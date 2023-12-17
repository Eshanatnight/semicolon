#pragma once
#include "ast/ExprAST.h"

namespace AST {
	class NumberExprAST : public ExprAST {
		double m_value;

	public:

		NumberExprAST(double val) : m_value(val) {}

		auto codegen() -> llvm::Value* override;
	};

}
