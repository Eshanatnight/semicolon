#include "ast/FunctionAST.h"

#include "logger/logger.h"
#include "parser/parser.h"
#include "semicolon/semicolon.h"

#include <llvm/IR/Verifier.h>

auto AST::FunctionAST::codegen() -> llvm::Function* {
	// Transfer ownership of the prototype to the FunctionProtos map,
	// but keep a reference to it for use below.
	auto& proto									  = *m_proto;
	codegen::function_protos[m_proto->get_name()] = std::move(m_proto);
	llvm::Function* func						  = codegen::get_function(proto.get_name());
	if(func == nullptr) {
		return nullptr;
	}

	// If this is an operator, install it.
	if(proto.is_binary_op()) {
		Parser::binop_precedence[proto.get_operator_name()] = proto.get_binary_precedence();
	}

	// Create a new basic block to start insertion into.
	llvm::BasicBlock* basic_block = llvm::BasicBlock::Create(*codegen::ctx, "entry", func);
	codegen::builder->SetInsertPoint(basic_block);

	// Record the function arguments in the NamedValues map.
	codegen::named_values.clear();
	for(auto& arg: func->args()) {
		// Create an alloca for this variable.
		llvm::AllocaInst* alloc = codegen::create_entry_block_alloca(func, arg.getName());

		// Store the initial value into the alloca.
		codegen::builder->CreateStore(&arg, alloc);

		// Add arguments to variable symbol table.
		codegen::named_values[std::string(arg.getName())] = alloc;
	}

	if(llvm::Value* ret_val = m_body->codegen()) {
		// Finish off the function.
		codegen::builder->CreateRet(ret_val);

		// Validate the generated code, checking for consistency.
		llvm::verifyFunction(*func);

		// Run the optimizer on the function.
		codegen::func_pass_manager->run(*func);

		return func;
	}

	// Error reading body, remove function.
	func->eraseFromParent();

	if(proto.is_binary_op()) {
		Parser::binop_precedence.erase(proto.get_operator_name());
	}
	return nullptr;
}
