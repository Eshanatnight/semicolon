#pragma once
#include <llvm/IR/Function.h>
#include <string>
#include <vector>

namespace AST {
	class PrototypeAST {
		std::string m_name;
		std::vector<std::string> m_args;
		bool m_is_operator;
		unsigned m_precedence; // Precedence if a binary op.

	public:

		PrototypeAST(const std::string& Name,
			std::vector<std::string> Args,
			bool IsOperator = false,
			unsigned Prec	= 0) :
			m_name(Name), m_args(std::move(Args)), m_is_operator(IsOperator), m_precedence(Prec) {}

		auto codegen() -> llvm::Function*;

		[[nodiscard]] auto get_name() const -> const std::string& {
			return m_name;
		}

		[[nodiscard]] auto is_unary_op() const -> bool {
			return m_is_operator && m_args.size() == 1;
		}
		[[nodiscard]] auto is_binary_op() const -> bool {
			return m_is_operator && m_args.size() == 2;
		}

		[[nodiscard]] auto get_operator_name() const -> char {
			assert(is_unary_op() || is_binary_op());
			return m_name[m_name.size() - 1];
		}

		[[nodiscard]] auto get_binary_precedence() const -> unsigned {
			return m_precedence;
		}
	};

}
