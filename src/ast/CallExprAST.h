#pragma once
#include "ast/ExprAST.h"
namespace AST {
	class CallExprAST : public ExprAST {
		std::string m_callee;
		std::vector<std::unique_ptr<AST::ExprAST>> m_args;

	public:

		CallExprAST(const std::string& callee, std::vector<std::unique_ptr<AST::ExprAST>> args) :
			m_callee(callee), m_args(std::move(args)) {}

		auto codegen() -> llvm::Value* override;
	};

}
