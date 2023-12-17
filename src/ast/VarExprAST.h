#pragma once
#include "ast/ExprAST.h"
namespace AST {
	class VarExprAST : public ExprAST {
		std::vector<std::pair<std::string, std::unique_ptr<AST::ExprAST>>> m_var_names;
		std::unique_ptr<AST::ExprAST> m_body;

	public:

		VarExprAST(std::vector<std::pair<std::string, std::unique_ptr<AST::ExprAST>>> VarNames,
			std::unique_ptr<AST::ExprAST> Body) :
			m_var_names(std::move(VarNames)), m_body(std::move(Body)) {}

		auto codegen() -> llvm::Value* override;
	};

}
