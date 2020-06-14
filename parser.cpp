#include "parser.hpp"



void parser::parse_file(string file_name)
{
	timer time_total("total");

	lex.tokenize_file(file_name);

	timer time("parse_file");

	root = nodes[parse_code_block()];
}

token parser::eat_token()
{
	token t = lex.tokens[index];
	index++;
	return t;
}

token parser::expect_token(token_type expected)
{
	token t = lex.tokens[index];

	if (t.type != expected)
	{
		cerr << "Se esperaba un token de tipo [" << token_type_name[(int)expected] << "], pero se encontro el token [" << lex.get_name(t) 
			<< ", linea: " << t.line << ", columna: " << t.position_in_line << ", tipo: " << token_type_name[(int)t.type] << "]\n";

		exit(-1);
	}

	index++;
	return t;
}

token parser::peek_ahead(int amount)
{
	int pos = index + amount;
	if (pos >= lex.tokens.size()) pos = lex.tokens.size() - 1;

	return  lex.tokens[pos];	
}



int parser::parse_expresion()		// @Bug: no estamos controlando que lo que nos pasen sea una epresion
{	
	int node = -1;

	if (lex.get_name(peek_ahead(0)).compare("var") == 0)				// Estamos declarando una variable
	{
		node = parse_declaration();
	}
	else if (peek_ahead(1).type == token_type::Equals)			// Estamos asignando una variable
	{
		node = parse_asigment();
	}
	else if (peek_ahead(0).type == token_type::Identifier && peek_ahead(1).type == token_type::OpenParentesis)	// Estamos invocando una funcion
	{
		node = parse_invocation();
	}
	else if (peek_ahead(0).type == token_type::Identifier)			// Estamos referenciando una variable
	{
		if (peek_ahead(1).type == token_type::BinaryOperation)	node = parse_binary_op();
		else													node = parse_identifier();
	}
	else
	{
		node = parse_literal();
	}

	return node;
}

int parser::parse_code_block()	// Un bloque esta compuesto de lineas de codigo separadas por ';'
{
	int node = nodes.size();
	nodes.push_back({ ast_type::Code_Block, -1, {} });

	while (lex.tokens[index].type != token_type::CloseBraket && lex.tokens[index].type != token_type::End_Of_File)
	{
		nodes[node].children.push_back(parse_expresion());
		expect_token(token_type::Semicolon);
	}

	eat_token();

	return node;
}

int parser::parse_declaration()
{
	int node = nodes.size();
	nodes.push_back({ ast_type::Declaration, -1, {} });

	expect_token(token_type::Identifier); // nos comemos la palabra clave 'var'

	nodes[node].children.push_back(parse_identifier());	// Nombre de la variable

	if (peek_ahead(0).type == token_type::Equals)	// Le damos un valor?
	{
		eat_token(); // nos comemos el '='
		nodes[node].children.push_back(parse_expresion());	// Valor de la variable;
		nodes[node].type = ast_type::Declaration_And_Asigment;
	}

	return node;
}

int parser::parse_asigment()
{
	int node = nodes.size();
	nodes.push_back({ ast_type::Asigment, -1, {} });


	nodes[node].children.push_back(parse_identifier());	// Nombre de la variable

	expect_token(token_type::Equals);

	nodes[node].children.push_back(parse_expresion());	// Valor de la variable;

	return node;
}


int parser::parse_invocation()
{
	int node = nodes.size();
	nodes.push_back({ ast_type::Invocation, -1, {} });


	nodes[node].children.push_back(parse_identifier());

	expect_token(token_type::OpenParentesis);

	while (true)	// Guarda los parametros
	{
		if (peek_ahead(0).type != token_type::CloseParentesis) nodes[node].children.push_back(parse_expresion());
		else
		{
			eat_token();
			break;
		}

		if (peek_ahead(0).type == token_type::Comma) eat_token();

	}

	return node;
}


int parser::parse_identifier()
{
	int node = nodes.size();
	nodes.push_back({ ast_type::Identifier, index, {} });

	eat_token();

	return node;
}

int parser::parse_literal()
{
	int node = nodes.size();
	nodes.push_back({ ast_type::Literal, index, {} });

	eat_token();

	return node;
}

int parser::parse_binary_op()	// @Todo
{
	int left = parse_expresion();
	eat_token(); // operator
	int rigt = parse_expresion();
}
