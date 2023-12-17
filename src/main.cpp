
#include "parser/parser.h"
#include "semicolon/driver.h"
#include "semicolon/semicolon.h"

#include <cstdio>
#include <llvm/Support/TargetSelect.h>

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// ! add some more library functions, basic math functions, to start with

/// putchard - putchar that takes a double and returns 0.
extern "C" DLLEXPORT auto putchard(double str) -> double {
	fputc((char)str, stderr /*stdout*/);
	return 0;
}

/// printd - printf that takes a double prints it as "%f\n", returning
/// 0.
extern "C" DLLEXPORT auto printd(double number) -> double {
	fprintf(stderr /*stdout*/, "%f\n", number);
	return 0;
}

/* void printHelp() {
	printf("semicolon interpreter\n\n");
	printf("-i\t--include-file\t\tspecify a file with semicolon code to include / execute before "
		   "starting the console\n");
	printf("--help\tprint this help\n");
	printf("\n");

	exit(0);
}

void fromFile(char* filename) {
	FILE* stream = fopen(filename, "r");

	if(stream) {
		Lexer::stream = stream;
	}
	else {
		fprintf(stdout, "ERROR: couldn't read file: %s\n defaulting to stdin", filename);
	}

	Lexer::get_next_token();
	while(TopCompare() == 0){}

	Lexer::stream = stdin;
	fclose(stream);
}

void parseArgs(int argc, char** argv) {

	for(int i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
			case '-' :
				switch(argv[i][2]) {
				case '\0' :
					return;
				case 'h' :
					if(strcmp(argv[i], "--help") == 0) printHelp();
					break;
				case 'i' :
					if(strcmp(argv[i], "--include-file") == 0) fromFile(argv[++i]);
					break;
				default :
					fprintf(stdout, "ERROR: invalid option: %s", argv[i]);
					exit(0);
				};
				break;

			case '?' :
				printHelp();
				break;

			case 'i' :
				fromFile(argv[++i]);
				break;

			default :
				fprintf(stdout, "ERROR: invalid option: %s", argv[i]);
				exit(0);
			}
		} else {
			fprintf(stdout, "ERROR: invalid option: %s", argv[i]);
			exit;
		}
	}
} */

auto main() -> int {
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();
	llvm::InitializeNativeTargetAsmParser();

	// Install standard binary operators.
	// 1 is lowest precedence.
	Parser::binop_precedence['='] = 2;
	Parser::binop_precedence['<'] = 10;
	Parser::binop_precedence['+'] = 20;
	Parser::binop_precedence['-'] = 20;
	Parser::binop_precedence['*'] = 40; // highest.

	// Prime the first token.
	// Note: Change stderr to stdout
	fprintf(stderr, "ready> ");
	Parser::get_next_token();

	// when doing multi-line code also add file / stdin mode
	// top comapre is not there thats why it does not line multi-line code

	codegen::jit = codegen::exit_on_err(llvm::orc::KaleidoscopeJIT::Create());

	driver::initialize_module_and_pass_manager();

	// Run the main "interpreter loop" now.
	driver::main_loop();

	return 0;
}
