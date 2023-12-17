#pragma once
#include "ast/ExprAST.h"
namespace AST {
	/**
	 * contains for loop (holds other expressions!)
	 */
	class ForExprAST : public ExprAST {
		std::string m_var_name;
		std::unique_ptr<AST::ExprAST> m_start;
		std::unique_ptr<AST::ExprAST> m_end;
		std::unique_ptr<AST::ExprAST> m_step;
		std::unique_ptr<AST::ExprAST> m_body;

	public:

		ForExprAST(const std::string& VarName,
			std::unique_ptr<AST::ExprAST> Start,
			std::unique_ptr<AST::ExprAST> End,
			std::unique_ptr<AST::ExprAST> Step,
			std::unique_ptr<AST::ExprAST> Body) :
			m_var_name(VarName),
			m_start(std::move(Start)),
			m_end(std::move(End)),
			m_step(std::move(Step)),
			m_body(std::move(Body)) {}

		auto codegen() -> llvm::Value* override;
	};

}
