/**
 * @file ast.h
 * @details: to be treated as a pch file
 * @brief This file includes all the AST (Abstract Syntax Tree)
 *  related header files.
 *
 * The AST is used to represent the structure of your code in a format
 * that's easy to process and manipulate.
 * This file includes headers for various types of expressions
 * (binary, call, for, if, etc.) and other AST components like prototypes and
 * functions.
 */

#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/ExprAST.h"
#include "ast/ForExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/IfExprAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/UnaryExprAST.h"
#include "ast/VarExprAST.h"
#include "ast/VariableExprAST.h"
