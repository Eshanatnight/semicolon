#include "ast/NumberExprAST.h"

#include "semicolon/semicolon.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"

auto AST::NumberExprAST::codegen() -> llvm::Value* {
	return llvm::ConstantFP::get(*codegen::ctx, llvm::APFloat(m_value));
}