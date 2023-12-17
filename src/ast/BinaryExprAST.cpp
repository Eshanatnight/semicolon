#include "ast/BinaryExprAST.h"

#include "ast/VariableExprAST.h"
#include "logger/logger.h"
#include "semicolon/semicolon.h"

auto AST::BinaryExprAST::codegen() -> llvm::Value* {

	if(m_operator == '=') {
		AST::VariableExprAST* lhs = static_cast<AST::VariableExprAST*>(m_lhs.get());
		if(lhs == nullptr) {
			return Logger::LogErrorV("destination of '=' must be a variable");
		}

		llvm::Value* val = m_rhs->codegen();

		if(val == nullptr) {
			return nullptr;
		}

		llvm::Value* variable = codegen::named_values[lhs->getName()];
		if(!variable /* == nullptr*/) {
			return Logger::LogErrorV("Unknown variable name");
		}

		codegen::builder->CreateStore(val, variable);
		return val;
	}

	llvm::Value* left  = m_lhs->codegen();
	llvm::Value* right = m_rhs->codegen();
	if(left == nullptr || right == nullptr) {
		return nullptr;
	}

	switch(m_operator) {
	case '+' :
		return codegen::builder->CreateFAdd(left, right, "addtmp");
	case '-' :
		return codegen::builder->CreateFSub(left, right, "subtmp");
	case '*' :
		return codegen::builder->CreateFMul(left, right, "multmp");
	case '<' :
		left = codegen::builder->CreateFCmpULT(left, right, "cmptmp");
		return codegen::builder->CreateUIToFP(
			left, llvm::Type::getDoubleTy(*codegen::ctx), "booltmp");

	default :
		break;
	}

	llvm::Function* func = codegen::get_function(std::string("binary") + m_operator);
	assert(func && "binary operator not found!");

	llvm::Value* ops[2] = { left, right }; // use std:array
	return codegen::builder->CreateCall(func, ops, "binop");
}