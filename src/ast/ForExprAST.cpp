#include "ast/ForExprAST.h"

#include "semicolon/semicolon.h"

auto AST::ForExprAST::codegen() -> llvm::Value* {
	llvm::Function* func = codegen::builder->GetInsertBlock()->getParent();

	// Create an alloca for the variable in the entry block.
	llvm::AllocaInst* alloc = codegen::create_entry_block_alloca(func, m_var_name);

	// Emit the start code first, without 'variable' in scope.
	llvm::Value* start_val = m_start->codegen();
	if(start_val == nullptr) {
		return nullptr;
	}

	// Store the value into the alloca.
	codegen::builder->CreateStore(start_val, alloc);

	// Make the new basic block for the loop header, inserting after
	// current block.
	llvm::BasicBlock* loop_basic_block = llvm::BasicBlock::Create(*codegen::ctx, "loop", func);

	// Insert an explicit fall through from the current block to the
	// LoopBB.
	codegen::builder->CreateBr(loop_basic_block);

	// Start insertion in LoopBB.
	codegen::builder->SetInsertPoint(loop_basic_block);

	// Within the loop, the variable is defined equal to the PHI node.
	// If it shadows an existing variable, we have to restore it, so
	// save it now.
	llvm::AllocaInst* old_val		  = codegen::named_values[m_var_name];
	codegen::named_values[m_var_name] = alloc;

	// Emit the body of the loop.  This, like any other expr, can
	// change the current BB.  Note that we ignore the value computed
	// by the body, but don't allow an error.
	if(m_body->codegen() == nullptr) {
		return nullptr;
	}

	// Emit the step value.
	llvm::Value* step_val = nullptr;
	if(m_step != nullptr) {
		step_val = m_step->codegen();
		if(step_val == nullptr) {
			return nullptr;
		}
	} else {
		// If not specified, use 1.0.
		step_val = llvm::ConstantFP::get(*codegen::ctx, llvm::APFloat(1.0));
	}

	// Compute the end condition.
	llvm::Value* end_cond = m_end->codegen();
	if(end_cond == nullptr) {
		return nullptr;
	}

	// Reload, increment, and restore the alloca.  This handles the
	// case where the body of the loop mutates the variable.
	llvm::Value* cur_val =
		codegen::builder->CreateLoad(alloc->getAllocatedType(), alloc, m_var_name.c_str());
	llvm::Value* next_var = codegen::builder->CreateFAdd(cur_val, step_val, "nextvar");
	codegen::builder->CreateStore(next_var, alloc);

	// Convert condition to a bool by comparing non-equal to 0.0.
	end_cond = codegen::builder->CreateFCmpONE(
		end_cond, llvm::ConstantFP::get(*codegen::ctx, llvm::APFloat(0.0)), "loopcond");

	// Create the "after loop" block and insert it.
	llvm::BasicBlock* after_basic_block =
		llvm::BasicBlock::Create(*codegen::ctx, "afterloop", func);

	// Insert the conditional branch into the end of LoopEndBB.
	codegen::builder->CreateCondBr(end_cond, loop_basic_block, after_basic_block);

	// Any new code will be inserted in AfterBB.
	codegen::builder->SetInsertPoint(after_basic_block);

	// Restore the unshadowed variable.
	if(old_val != nullptr) {
		codegen::named_values[m_var_name] = old_val;
	} else {
		codegen::named_values.erase(m_var_name);
	}
	// for expr always returns 0.0.
	return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(*codegen::ctx));
}
