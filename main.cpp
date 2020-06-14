//#include "Json_Parser.hpp"

#include "lexer.hpp"
#include "parser.hpp"


void prueba_lexer()
{
	lexer lex;

	lex.tokenize_file("salida.txt");

	cout << "El fichero es: \n" << lex.text << endl;

	for (token t : lex.tokens)
	{
		cout << "\nLine: " << t.line << " Position: [" << t.position_in_line << ", " << t.position_in_line + t.size - 1 << "]"
			" Type: " << token_type_name[(int)t.type] << " Value: " << lex.get_name(t);
	}
}

void prueba_parser()
{
	parser par;

	par.parse_file("salida.txt");

	

	for (auto a : par.nodes[0].children)
	{
		if (par.nodes[a].type == ast_type::Declaration_And_Asigment)
		{
			cout << "Declaramos la variable : " << par.lex.get_name(par.lex.tokens[par.nodes[par.nodes[a].children[0]].value_position]) << " = "
				<< par.lex.get_name(par.lex.tokens[par.nodes[par.nodes[a].children[1]].value_position]) << endl;
		}
		else if (par.nodes[a].type == ast_type::Invocation)
		{
			cout << "Invocamos : " << par.lex.get_name(par.lex.tokens[par.nodes[par.nodes[a].children[0]].value_position]) << "( "
				<< par.lex.get_name(par.lex.tokens[par.nodes[par.nodes[a].children[1]].value_position]) << ", "
				<< par.lex.get_name(par.lex.tokens[par.nodes[par.nodes[a].children[2]].value_position]) << " ) " << endl;
		}
	}
}

int main()
{
	//prueba_lexer();


	//prueba_parser();

	parser par;
	par.parse_file("salida.txt");

	return 0;
}