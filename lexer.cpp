#include "lexer.hpp"

vector<string> token_type_name = { 
	"String",
	"Equals",
	"Semicolon",
	"Comma",
	"Dot",
	"OpenBraket",
	"CloseBraket",
	"OpenParentesis",
	"CloseParentesis",
	"Identifier",
	"Number",
	"Math" };

token::token(token_type type, int position, int line, int position_in_line, int size = 1) 
	: type(type), position(position), line(line), position_in_line(position_in_line), size(size) {}

void lexer::load_file(string file_name)
{
	ifstream in;

	in.open(file_name);

	if (in) {
		ostringstream ss;
		ss << in.rdbuf(); // reading data
		text = ss.str();
	}

	in.close();
}

void lexer::tokenize_file(string file_name)
{
	load_file(file_name);
	while (index < text.size())
	{
		tokens.push_back(get_next_token());
	}
}

void lexer::advance()
{
	if (index < text.size())
	{
		index++;
		caracter = text[index];
		position_in_line++;
	}
}

void lexer::skip_white_space()
{
	while (isspace(caracter))
	{
		if (index >= text.size()) return;
		if (caracter == '\n')
		{
			line++;
			position_in_line = 0;
		}
		advance();
	}
}

void lexer::skip_coments()
{
	if (caracter != '/') return;
	if (text[index+1] != '/') return;

	while (caracter != '\n' || index < text.size())
	{
		advance();
	}
}

token lexer::get_next_token()
{
	caracter = text[index];
	skip_white_space();
	skip_coments();

	if (index < text.size())
	{

		if(isalpha(caracter)) return collect_identifier();

		if (isalnum(caracter)) return collect_number();

		switch (caracter)
		{
		case '"':
			return collect_string();
		case '=':
			return advance_and_return({ token_type::Equals, index, line, position_in_line });
		case '(':
			return advance_and_return({ token_type::OpenParentesis, index, line, position_in_line });
		case ')':
			return advance_and_return({ token_type::CloseParentesis, index, line, position_in_line });
		case '{':
			return advance_and_return({ token_type::OpenBraket, index, line, position_in_line });
		case '}':
			return advance_and_return({ token_type::CloseBraket, index, line, position_in_line });
		case ',':
			return advance_and_return({ token_type::Comma, index, line, position_in_line });
		case '.':
			return advance_and_return({ token_type::Dot, index, line, position_in_line });
		case ';':
			return advance_and_return({ token_type::Semicolon, index, line, position_in_line });
		case '+':
		case '-':
		case '*':
		case '/':			
			return advance_and_return({ token_type::Math, index, line, position_in_line });


		default:
			break;
		}
	}
	
	advance();

	return { token_type::Semicolon, index, line, position_in_line };
}

token lexer::collect_string()
{
	advance();

	token res = { token_type::String, index, line, position_in_line, 0 };

	while (caracter != '"')
	{
		res.size++;
		advance();
	}

	advance();

	return res;
}

token lexer::collect_number()
{
	token res = { token_type::Number, index, line, position_in_line, 0 };

	while (caracter >= '0' && caracter <= '9' || caracter == '.')
	{
		res.size++;
		advance();
	}

	return res;
}

token lexer::collect_identifier()
{
	token res = { token_type::Identifier, index, line, position_in_line, 0 };

	while (isalnum(caracter)) 
	{
		res.size++;
		advance();
	}

	return res;
}

token lexer::advance_and_return(token t)
{
	advance();

	return t;
}

string lexer::get_name(token t)
{
	return text.substr(t.position, t.size);
}

