#pragma once

#include "ast/ExprAST.h"

namespace AST {
	/**
	 * contains reference to a Variable
	 */
	class VariableExprAST : public ExprAST {
		std::string m_name;

	public:

		/* explicit */ VariableExprAST(const std::string& name) : m_name(name) {}

		auto codegen() -> llvm::Value* override;
		auto getName() const -> const std::string& /*const*/ {
			return m_name;
		}
	};

}
