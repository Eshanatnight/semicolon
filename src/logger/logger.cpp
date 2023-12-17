#include "logger/logger.h"

/// LogError* - These are little helper functions for error handling.
auto Logger::LogError(const char* Str) -> std::unique_ptr<AST::ExprAST> {
	fprintf(stderr, "Error: %s\n", Str);
	return nullptr;
}

auto Logger::LogErrorP(const char* Str) -> std::unique_ptr<AST::PrototypeAST> {
	Logger::LogError(Str);
	return nullptr;
}

auto Logger::LogErrorV(const char* str) -> llvm::Value* {
	Logger::LogError(str);
	return nullptr;
}
