#include "ast/PrototypeAST.h"

#include "semicolon/semicolon.h"

auto AST::PrototypeAST::codegen() -> llvm::Function* {
	// Make the function type:  double(double,double) etc.
	std::vector<llvm::Type*> doubles(m_args.size(), llvm::Type::getDoubleTy(*codegen::ctx));
	llvm::FunctionType* func_type =
		llvm::FunctionType::get(llvm::Type::getDoubleTy(*codegen::ctx), doubles, false);

	llvm::Function* func = llvm::Function::Create(
		func_type, llvm::Function::ExternalLinkage, m_name, codegen::module.get());

	// Set names for all arguments.
	unsigned idx = 0;
	for(auto& arg: func->args()) {
		arg.setName(m_args[idx++]);
	}

	return func;
}
