#pragma once
/*
#include "lexer.hpp"


// @Improvements: Mejorar la estructura para hacer más facil el acto de travesar los nodos

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

	// posicion del token que lo represente. Solo se usa para los valores, en el resto de casos se mantiene a -1
	int value_position = -1;

	vector<int> children; 
};

struct parser
{
	lexer lex;

	int index = 0;

	ast_node root;

	vector<ast_node> nodes;

	void parse_file(string file_name);

	token eat_token();
	token expect_token(token_type expected);
	token peek_ahead(int amount);

	int parse_expresion();	// Este es el más importante
	int parse_code_block();
	int parse_declaration();
	int parse_asigment();
	int parse_invocation();
	int parse_identifier();
	int parse_literal();
	int parse_binary_op();

};

*/