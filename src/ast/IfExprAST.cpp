#include "ast/IfExprAST.h"

#include "semicolon/semicolon.h"

auto AST::IfExprAST::codegen() -> llvm::Value* {
	llvm::Value* condition_val = m_cond->codegen();
	if(condition_val == nullptr) {
		return nullptr;
	}
	// Convert condition to a bool by comparing non-equal to 0.0.
	condition_val = codegen::builder->CreateFCmpONE(
		condition_val, llvm::ConstantFP::get(*codegen::ctx, llvm::APFloat(0.0)), "ifcond");

	llvm::Function* func = codegen::builder->GetInsertBlock()->getParent();

	// Create blocks for the then and else cases.  Insert the 'then'
	// block at the end of the function.
	llvm::BasicBlock* then_basic_block	= llvm::BasicBlock::Create(*codegen::ctx, "then", func);
	llvm::BasicBlock* else_basic_block	= llvm::BasicBlock::Create(*codegen::ctx, "else");
	llvm::BasicBlock* merge_basic_block = llvm::BasicBlock::Create(*codegen::ctx, "ifcont");

	codegen::builder->CreateCondBr(condition_val, then_basic_block, else_basic_block);

	// Emit then value.
	codegen::builder->SetInsertPoint(then_basic_block);

	llvm::Value* then_val = m_then->codegen();
	if(then_val == nullptr) {
		return nullptr;
	}

	codegen::builder->CreateBr(merge_basic_block);
	// Codegen of 'Then' can change the current block, update ThenBB
	// for the PHI.
	then_basic_block = codegen::builder->GetInsertBlock();

	// Emit else block.
	func->insert(func->end(), else_basic_block);
	codegen::builder->SetInsertPoint(else_basic_block);

	llvm::Value* else_val = m_else->codegen();
	if(else_val == nullptr) {
		return nullptr;
	}
	codegen::builder->CreateBr(merge_basic_block);
	// Codegen of 'Else' can change the current block, update ElseBB
	// for the PHI.
	else_basic_block = codegen::builder->GetInsertBlock();

	// Emit merge block.
	func->insert(func->end(), merge_basic_block);
	codegen::builder->SetInsertPoint(merge_basic_block);
	llvm::PHINode* phi_node =
		codegen::builder->CreatePHI(llvm::Type::getDoubleTy(*codegen::ctx), 2, "iftmp");

	phi_node->addIncoming(then_val, then_basic_block);
	phi_node->addIncoming(else_val, else_basic_block);
	return phi_node;
}