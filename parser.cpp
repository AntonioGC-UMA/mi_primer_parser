#include "parser.hpp"


parser::parser()
{
	root.type = ast_type::Code_Block;
}

void parser::parse_file(string file_name)
{
	lex.tokenize_file(file_name);

	root = *parse_code_block();
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
		cerr << "No se esperaba el token de tipo: " << token_type_name[(int)t.type] << " en la linea: " << t.line
			<< " posicion: " << t.position_in_line << "\nSe esperaba un token de tipo: " << token_type_name[(int)expected] << endl;

		assert(false);
		exit(-1);
	}

	index++;
	return t;
}

token parser::peek_ahead(int amount)
{
	return  lex.tokens[index + amount];
}



ast_node* parser::parse_expresion()
{
	ast_node* node = nullptr;


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
		node = parse_identifier();
	}
	else
	{
		node = parse_literal();
	}

	return node;
}

ast_node* parser::parse_code_block()	// Un bloque esta compuesto de lineas de codigo separadas por ';'
{
	ast_node* node = new ast_node();	// @Leak

	node->type = ast_type::Code_Block;

	while (index < lex.tokens.size())
	{
		if (lex.tokens[index].type == token_type::CloseBraket)	// Si encontramos '}' lo quitamos y nos salimos
		{
			eat_token();
			break;
		}

		node->children.push_back(parse_expresion());
		expect_token(token_type::Semicolon);
	}

	return node;
}

ast_node* parser::parse_declaration()
{
	ast_node* node = new ast_node();	// @Leak

	expect_token(token_type::Identifier); // nos comemos la palabra clave 'var'

	node->children.push_back(parse_identifier());	// Nombre de la variable

	if (peek_ahead(0).type == token_type::Equals)	// Le damos un valor?
	{
		eat_token(); // nos comemos el '='
		node->children.push_back(parse_expresion());	// Valor de la variable;
		node->type = ast_type::Declaration_And_Asigment;
	}
	else
	{
		node->type = ast_type::Declaration;
	}

	return node;
}

ast_node* parser::parse_asigment()
{
	ast_node* node = new ast_node();	// @Leak

	node->type = ast_type::Asigment;

	node->children.push_back(parse_identifier());	// Nombre de la variable

	expect_token(token_type::Equals);

	node->children.push_back(parse_expresion());	// Valor de la variable;

	return node;
}


ast_node* parser::parse_invocation()
{
	ast_node* node = new ast_node();	// @Leak

	node->type = ast_type::Invocation;
	node->children.push_back(parse_identifier());

	expect_token(token_type::OpenParentesis);

	while (true)	// Guarda los parametros
	{
		if (peek_ahead(0).type != token_type::CloseParentesis) node->children.push_back(parse_expresion());
		else
		{
			eat_token();
			break;
		}

		if (peek_ahead(0).type == token_type::Comma) eat_token();

	}

	return node;
}


ast_node* parser::parse_identifier()
{
	ast_node* node = new ast_node();	// @Leak

	node->type = ast_type::Identifier;
	node->value_position = index;
	eat_token();

	return node;
}

ast_node* parser::parse_literal()
{
	ast_node* node = new ast_node();	// @Leak

	node->type = ast_type::Literal;
	node->value_position = index;
	eat_token();

	return node;
}
