#include "ast/VarExprAST.h"

#include "semicolon/semicolon.h"

auto AST::VarExprAST::codegen() -> llvm::Value* {
	std::vector<llvm::AllocaInst*> old_bindings;

	llvm::Function* func = codegen::builder->GetInsertBlock()->getParent();

	// Register all variables and emit their initializer.
	for(unsigned i = 0, size = m_var_names.size(); i != size; ++i) {
		const std::string& var_name = m_var_names[i].first;
		AST::ExprAST* init			= m_var_names[i].second.get();

		// Emit the initializer before adding the variable to scope,
		// this prevents the initializer from referencing the variable
		// itself, and permits stuff like this:
		//  var a = 1 in
		//    var a = a in ...   # refers to outer 'a'.
		llvm::Value* init_val;
		if(init != nullptr) {
			init_val = init->codegen();
			if(init_val == nullptr) {
				return nullptr;
			}
		} else { // If not specified, use 0.0.
			init_val = llvm::ConstantFP::get(*codegen::ctx, llvm::APFloat(0.0));
		}

		llvm::AllocaInst* alloc = codegen::create_entry_block_alloca(func, var_name);
		codegen::builder->CreateStore(init_val, alloc);

		// Remember the old variable binding so that we can restore
		// the binding when we unrecurse.
		old_bindings.push_back(codegen::named_values[var_name]);

		// Remember this binding.
		codegen::named_values[var_name] = alloc;
	}

	// Codegen the body, now that all vars are in scope.
	llvm::Value* body_val = m_body->codegen();
	if(body_val == nullptr) {
		return nullptr;
	}

	// Pop all our variables from scope.
	for(unsigned i = 0, size = m_var_names.size(); i != size; ++i) {
		codegen::named_values[m_var_names[i].first] = old_bindings[i];
	}

	// Return the body computation.
	return body_val;
}
