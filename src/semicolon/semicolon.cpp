#include "semicolon/semicolon.h"

std::unique_ptr<llvm::LLVMContext> codegen::ctx;
std::unique_ptr<llvm::Module> codegen::module;
std::unique_ptr<llvm::IRBuilder<>> codegen::builder;
std::map<std::string, llvm::AllocaInst*> codegen::named_values;
std::unique_ptr<llvm::legacy::FunctionPassManager> codegen::func_pass_manager;
std::unique_ptr<llvm::orc::KaleidoscopeJIT> codegen::jit;
std::map<std::string, std::unique_ptr<AST::PrototypeAST>> codegen::function_protos;
llvm::ExitOnError codegen::exit_on_err;

auto codegen::get_function(std::string Name) -> llvm::Function* {
	// First, see if the function has already been added to the
	// current module.
	if(auto* func = codegen::module->getFunction(Name)) {
		return func;
	}

	// If not, check whether we can codegen the declaration from some
	// existing prototype.
	auto func_iter = codegen::function_protos.find(Name);
	if(func_iter != codegen::function_protos.end()) {
		return func_iter->second->codegen();
	}

	// If no existing prototype exists, return null.
	return nullptr;
}

auto codegen::create_entry_block_alloca(llvm::Function* func, llvm::StringRef var_name)
	-> llvm::AllocaInst* {
	llvm::IRBuilder<> temp_builder(&func->getEntryBlock(), func->getEntryBlock().begin());
	return temp_builder.CreateAlloca(llvm::Type::getDoubleTy(*codegen::ctx), nullptr, var_name);
}