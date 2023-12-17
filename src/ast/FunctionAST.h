#pragma once

#include "ast/ExprAST.h"
#include "ast/PrototypeAST.h"

namespace AST {
	class FunctionAST {
		std::unique_ptr<AST::PrototypeAST> m_proto;
		std::unique_ptr<AST::ExprAST> m_body;

	public:

		FunctionAST(std::unique_ptr<AST::PrototypeAST> Proto, std::unique_ptr<AST::ExprAST> Body) :
			m_proto(std::move(Proto)), m_body(std::move(Body)) {}

		auto codegen() -> llvm::Function*;
	};

}
