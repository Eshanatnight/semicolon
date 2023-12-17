#pragma once
#include "ast/PrototypeAST.h"
#include "KaleidoscopeJIT.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <memory>

namespace codegen {
	extern std::unique_ptr<llvm::LLVMContext> ctx;
	extern std::unique_ptr<llvm::Module> module;
	extern std::unique_ptr<llvm::IRBuilder<>> builder;
	extern std::map<std::string, llvm::AllocaInst*> named_values;
	extern std::unique_ptr<llvm::legacy::FunctionPassManager> func_pass_manager;
	extern std::unique_ptr<llvm::orc::KaleidoscopeJIT> jit;
	extern std::map<std::string, std::unique_ptr<AST::PrototypeAST>> function_protos;
	extern llvm::ExitOnError exit_on_err;

	auto get_function(std::string name) -> llvm::Function*;
	auto create_entry_block_alloca(llvm::Function* func, llvm::StringRef var_name)
		-> llvm::AllocaInst*;
}
