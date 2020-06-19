//#include "Json_Parser.hpp"

#include "Lexer.h"


void prueba_lexer()
{
	Lexer lex;

	lex.tokenize_file("salida.txt");

	for (Token t : lex.tokens)
	{
		cout << "Type: " << token_name[(int)t.type] << "\t" << lex.text.substr(t.position, t.size) << endl;
	}
}



int main()
{
	prueba_lexer();

	return 0;
}