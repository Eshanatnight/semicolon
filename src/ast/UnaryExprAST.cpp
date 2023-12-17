#include "ast/UnaryExprAST.h"

#include "logger/logger.h"
#include "semicolon/semicolon.h"

auto AST::UnaryExprAST::codegen() -> llvm::Value* {
	llvm::Value* operand_val = m_operand->codegen();
	if(operand_val == nullptr) {
		return nullptr;
	}

	llvm::Function* func = codegen::get_function(std::string("unary") + m_opcode);
	if(func == nullptr) {
		return Logger::LogErrorV("Unknown unary operator");
	}
	return codegen::builder->CreateCall(func, operand_val, "unop");
}