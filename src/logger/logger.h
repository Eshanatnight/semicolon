#pragma once

#include "ast/ExprAST.h"
#include "ast/PrototypeAST.h"

#include <memory>

namespace Logger {
	auto LogError(const char* Str) -> std::unique_ptr<AST::ExprAST>;
	auto LogErrorP(const char* Str) -> std::unique_ptr<AST::PrototypeAST>;
	auto LogErrorV(const char* str) -> llvm::Value*;

	template<typename T>
	std::unique_ptr<T> log_error(const char* str) {
		fprintf(stderr, "Error: %s\n", str);
		return nullptr;
	}
}
