#include "ast/VariableExprAST.h"

#include "logger/logger.h"
#include "semicolon/semicolon.h"

#include <llvm/IR/Instructions.h>

auto AST::VariableExprAST::codegen() -> llvm::Value* {
	llvm::AllocaInst* alloc = codegen::named_values[m_name];
	if(!alloc /*== nullptr*/) {
		return Logger::LogErrorV("Unknown variable name");
	}

	// Load the value.
	return codegen::builder->CreateLoad(alloc->getAllocatedType(), alloc, m_name.c_str());
}