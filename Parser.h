#pragma once

#include "Lexer.h"

enum class ast_type
{
	CodeBlock,
	Declaration,
	Expresion,
	Statement,
	Lambda,
	Invocation,
	Literal,
	Identifier,
	UnaryOperator,
	BinaryOperator,
	Declaration,
	ConstantDeclaration,
	Asigment,
};

struct Ast_node
{
	ast_type type;

	string value;

	vector<Ast_node*> children;
};

struct Parser
{
	Lexer lexer;
	int index = 0;

	Ast_node* root;

	deque<Ast_node> nodes;

	bool parse_file(string file_name);

	bool expect_token(token_type token); 
	token_type peek(int offset);

	bool is_invocation();
	bool is_declaration();
	bool is_asigment();
	bool is_literal();
	bool is_unary();
	bool is_binary();


	Ast_node* parse_code_block();
	Ast_node* parse_statement();
	Ast_node* parse_expresion();

	Ast_node* parse_declaration();
	Ast_node* parse_asigment();
	Ast_node* parse_invocation();

	Ast_node* parse_unary_operator();
	Ast_node* parse_binary_operator();


	Ast_node* parse_identifier();
	Ast_node* parse_literal();
	
};

