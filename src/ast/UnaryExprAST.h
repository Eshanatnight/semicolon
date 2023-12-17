#pragma once
#include "ast/ExprAST.h"

namespace AST {
	/**
	 * contains unary expressions (can hold other expressions!)
	 */
	class UnaryExprAST : public ExprAST {
		char m_opcode;
		std::unique_ptr<AST::ExprAST> m_operand;

	public:

		UnaryExprAST(char opcode, std::unique_ptr<AST::ExprAST> operand) :
			m_opcode(opcode), m_operand(std::move(operand)) {}

		auto codegen() -> llvm::Value* override;
	};

}
