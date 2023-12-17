#include "ast/CallExprAST.h"

#include "logger/logger.h"
#include "semicolon/semicolon.h"

auto AST::CallExprAST::codegen() -> llvm::Value* {
	// Look up the name in the global module table.
	llvm::Function* func = codegen::get_function(m_callee);
	if(func == nullptr) {
		return Logger::LogErrorV("Unknown function referenced");
	}
	// If argument mismatch error.
	if(func->arg_size() != m_args.size()) {
		return Logger::LogErrorV("Incorrect # arguments passed");
	}

	// construct call
	std::vector<llvm::Value*> args;
	for(unsigned i = 0, size = m_args.size(); i != size; ++i) {
		args.push_back(m_args[i]->codegen());
		if(args.back() == nullptr) {
			return nullptr;
		}
	}

	return codegen::builder->CreateCall(func, args, "calltmp");
}