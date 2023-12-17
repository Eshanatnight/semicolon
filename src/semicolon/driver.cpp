#include "semicolon/driver.h"

#include "lexer/tokens.h"
#include "parser/parser.h"
#include "semicolon/semicolon.h"

#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>

auto driver::initialize_module_and_pass_manager() -> void {
	// Open a new module.
	codegen::ctx	= std::make_unique<llvm::LLVMContext>();
	codegen::module = std::make_unique<llvm::Module>("my cool jit", *codegen::ctx);
	codegen::module->setDataLayout(codegen::jit->getDataLayout());

	// Create a new builder for the module.
	codegen::builder = std::make_unique<llvm::IRBuilder<>>(*codegen::ctx);

	// Create a new pass manager attached to it.
	codegen::func_pass_manager =
		std::make_unique<llvm::legacy::FunctionPassManager>(codegen::module.get());

	// Promote allocas to registers.
	codegen::func_pass_manager->add(llvm::createPromoteMemoryToRegisterPass());
	// Do simple "peephole" optimizations and bit-twiddling optzns.
	codegen::func_pass_manager->add(llvm::createInstructionCombiningPass());
	// Reassociate expressions.
	codegen::func_pass_manager->add(llvm::createReassociatePass());
	// Eliminate Common SubExpressions.
	codegen::func_pass_manager->add(llvm::createGVNPass());
	// Simplify the control flow graph (deleting unreachable blocks,
	// etc).
	codegen::func_pass_manager->add(llvm::createCFGSimplificationPass());

	codegen::func_pass_manager->doInitialization();
}

auto driver::handle_definition() -> void {
	if(auto func = Parser::parse_definition()) {
		if(auto* func_ir = func->codegen()) {
			fprintf(stderr, "Read function definition:");
			func_ir->print(llvm::errs());
			fprintf(stderr, "\n");
			codegen::exit_on_err(codegen::jit->addModule(
				llvm::orc::ThreadSafeModule(std::move(codegen::module), std::move(codegen::ctx))));
			driver::initialize_module_and_pass_manager();
		}
	} else {
		// Skip token for error recovery.
		Parser::get_next_token();
	}
}

auto driver::handle_extern() -> void {
	if(auto proto = Parser::parse_extern()) {
		if(auto* func_ir = proto->codegen()) {
			fprintf(stderr, "Read extern: ");
			func_ir->print(llvm::errs());
			fprintf(stderr, "\n");
			codegen::function_protos[proto->get_name()] = std::move(proto);
		}
	} else {
		// Skip token for error recovery.
		Parser::get_next_token();
	}
}

auto driver::handle_top_level_expression() -> void {
	// Evaluate a top-level expression into an anonymous function.
	if(auto func = Parser::parse_top_level_expr()) {
		if(func->codegen()) {
			// Create a ResourceTracker to track JIT'd memory
			// allocated to our anonymous expression -- that way we
			// can free it after executing.
			auto resource_tracker = codegen::jit->getMainJITDylib().createResourceTracker();

			auto module =
				llvm::orc::ThreadSafeModule(std::move(codegen::module), std::move(codegen::ctx));
			codegen::exit_on_err(codegen::jit->addModule(std::move(module), resource_tracker));
			driver::initialize_module_and_pass_manager();

			// Search the JIT for the __anon_expr symbol.
			auto expr_symbol = codegen::exit_on_err(codegen::jit->lookup("__anon_expr"));

			// Get the symbol's address and cast it to the right type
			// (takes no arguments, returns a double) so we can call
			// it as a native function.
			double (*func_ptr)() = expr_symbol.getAddress().toPtr<double (*)()>();
			fprintf(stderr, "Evaluated to %f\n", func_ptr());

			// Delete the anonymous expression module from the JIT.
			codegen::exit_on_err(resource_tracker->remove());
		}
	} else {
		// Skip token for error recovery.
		Parser::get_next_token();
	}
}

/// top ::= definition | external | expression | ';'
auto driver::main_loop() -> void {
	while(true) {
		fprintf(stderr, "ready> ");
		switch(Parser::current_tok) {
		case tok_eof :
			return;
		case ';' : // ignore top-level semicolons.
			Parser::get_next_token();
			break;
		case tok_def :
			driver::handle_definition();
			break;
		case tok_extern :
			driver::handle_extern();
			break;
		default :
			driver::handle_top_level_expression();
			break;
		}
	}
}