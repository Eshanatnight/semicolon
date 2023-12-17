#pragma once

namespace driver {
	auto initialize_module_and_pass_manager() -> void;
	auto handle_definition() -> void;
	auto handle_extern() -> void;
	auto handle_top_level_expression() -> void;
	auto main_loop() -> void;
}