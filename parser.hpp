#pragma once

#include "lexer.hpp"

enum class ast_type
{
	Code_Block,
	Asigment,
	Declaration,
	Declaration_And_Asigment,
	Literal,
	Invocation,
	Expresion,
	Identifier
	//Variable,
};

struct ast_node	// ast = abstract syntax tree
{
	ast_type type = ast_type::Code_Block;

	int value_position = -1;

	vector<ast_node*> children; // @Leak nunca los deleteamos
};

struct parser
{
	lexer lex;
	int index = 0;

	ast_node root;

	parser();

	void parse_file(string file_name);

	token eat_token();
	token expect_token(token_type expected);
	token peek_ahead(int amount);

	ast_node* parse_expresion();	// Este es el más importante
	ast_node* parse_code_block();
	ast_node* parse_declaration();
	ast_node* parse_asigment();
	ast_node* parse_invocation();
	ast_node* parse_identifier();
	ast_node* parse_literal();

};

