#pragma once
#include <llvm/IR/BasicBlock.h>

namespace AST {

	class ExprAST {
	public:

		virtual ~ExprAST() = default;

		virtual llvm::Value* codegen() = 0;
	};
}