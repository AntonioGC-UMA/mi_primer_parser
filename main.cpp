//#include "Json_Parser.hpp"

#include "Lexer.h"
#include "Parser.h"
#include "Visitor.h"

void prueba_lexer()
{
	Lexer lex;

	lex.tokenize_file("salida.txt");

	for (Token t : lex.tokens)
	{
		cout << "Type: " << token_name[(int)t.type] << "\t" << lex.text.substr(t.position, t.size) << endl;
	}
}

void prueba_parser()
{
	Parser par;

	par.parse_file("salida.txt");

	Visitor vis;

	vis.visit_tree(par.root, "salida.py");
}

int main()
{
	//prueba_lexer();

	prueba_parser();



	return 0;
}