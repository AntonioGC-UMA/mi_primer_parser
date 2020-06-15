#include "Lexer.h"


Token::Token(token_type t, unsigned int p, unsigned int l, unsigned int c, unsigned int s) : type(t), position(p), line(l), colum(c), size(s)
{

}

void Lexer::load_file(string file_name)
{
	ifstream in;

	in.open(file_name);

	if (in) {
		ostringstream ss;
		ss << in.rdbuf();
		text = ss.str();
	}

	in.close();
}

bool Lexer::tokenize_file(string file_name)
{
	load_file(file_name);

	if (text.size() < 1) return false;

	caracter = text[index];

	while (get_next_token()) {}

	cout << message << endl;
	return error;
}

token_type Lexer::look_back(unsigned int n)
{
	if (tokens.size() < n) return token_type::End_Of_File;

	return tokens[tokens.size() - n].type;
}


bool Lexer::advance()
{
	index++;
	colum++;

	if (index >= text.size()) return false;

	caracter = text[index];

	return true;
}

bool Lexer::skip_white_space()
{
	while (isspace(caracter))
	{
		if (!advance()) return false;
	}

	return true;
}

bool Lexer::skip_coments()
{
	if ((index + 1) >= tokens.size()) return true;

	if (text[index + 1] == '/')
	{
		while (caracter != '\n')
		{
			if (!advance()) return false;
		}

		line++;
		colum = 0;
		return true;
	}
	else if (text[index + 1] == '*')
	{
		while (advance())
		{
			if (caracter == '\n')
			{
				line++;
				colum = 0;
			}
			else if (caracter == '/' && advance() && caracter == '*') return true;
		}

		return false;
	}


	return true;
}

bool Lexer::get_next_token()
{
	skip_white_space();
	skip_coments();

	switch (caracter)
	{
	case '+':
		tokens.emplace_back(token_type::Plus, index, line, colum, 1); return advance();
	case '-':
		tokens.emplace_back(token_type::Minus, index, line, colum, 1); return advance();
	case '*':
		tokens.emplace_back(token_type::Times, index, line, colum, 1); return advance();
	case '/':
		tokens.emplace_back(token_type::Slash, index, line, colum, 1); return advance();
	case '{':
		tokens.emplace_back(token_type::OpenBraket, index, line, colum, 1); return advance();
	case '}':
		tokens.emplace_back(token_type::CloseBraket, index, line, colum, 1); return advance();
	case '(':
		tokens.emplace_back(token_type::OpenParentesis, index, line, colum, 1); return advance();
	case ')':
		tokens.emplace_back(token_type::CloseParentesis, index, line, colum, 1); return advance();
	case '[':
		tokens.emplace_back(token_type::OpenBox, index, line, colum, 1); return advance();
	case ']':
		tokens.emplace_back(token_type::CloseBox, index, line, colum, 1); return advance();
	case '<':
		tokens.emplace_back(token_type::LessThan, index, line, colum, 1); return advance();
	case '>':
		return get_more_than();
	case '=':
		return get_equals();
	case '!':
		tokens.emplace_back(token_type::Not, index, line, colum, 1); return advance();
	case '.':
		tokens.emplace_back(token_type::Dot, index, line, colum, 1); return advance();
	case ',':
		tokens.emplace_back(token_type::Comma, index, line, colum, 1); return advance();
	case ';':
		tokens.emplace_back(token_type::Semicolon, index, line, colum, 1); return advance();
	case ':':
		return get_colon();
	case '"':


	default:		
		if ('0' <= caracter && caracter <= '9') return get_number();
		else if (('a' <= caracter && caracter <= 'z') || ('A' <= caracter && caracter <= 'Z') || caracter == '_') return get_identifier();
		return advance(); // TODO should this be an error?
	}
}


bool Lexer::get_number()	// Valid for ints and floats
{
	token_type previous_token = look_back(1);

	if (previous_token != token_type::Dot) return get_int();

	get_int();

	tokens[tokens.size() - 3].size += 1 + tokens.back().size;	// changes the first int token to be a float
	tokens[tokens.size() - 3].type = token_type::Float;

	tokens.pop_back(); // the number
	tokens.pop_back(); // the dot

	return true;
}

bool Lexer::get_int()
{
	Token token = { token_type::Int, index, line, colum, 0 };

	while ('0' <= caracter && caracter <= '9')
	{
		if (!advance()) return false;
		token.size++;
	}

	tokens.push_back(token);

	return true;
}

bool Lexer::get_string()
{
	if (!advance()) return false;

	Token token = { token_type::String, index, line, colum, 0 };

	while (caracter != '"')
	{
		if (!advance()) return false;
		token.size++;
	}

	tokens.push_back(token);

	return true;
}

bool Lexer::get_identifier()
{
	Token token = { token_type::Identifier, index, line, colum, 0 };

	while ((('a' <= caracter && caracter <= 'z') || ('A' <= caracter && caracter <= 'Z') || ('0' <= caracter && caracter <= '9') || caracter == '_'))
	{
		token.size++;
		if (!advance()) return false;
	}

	string name = text.substr(token.position, token.size);

	if (!name.compare("if"))		// Check for keywords. Use the ! because compare returns 0 if they are equal
	{
		token.type = token_type::If;
	}
	else if (!name.compare("else"))
	{
		token.type = token_type::Else;
	}
	else if (!name.compare("for"))
	{
		token.type = token_type::For;
	}
	else if (!name.compare("while"))
	{
		token.type = token_type::While;
	}
	else if (!name.compare("return"))
	{
		token.type = token_type::Return;
	}
	else if (!name.compare("continue"))
	{
		token.type = token_type::Continue;
	}
	else if (!name.compare("break"))
	{
		token.type = token_type::Break;
	}
	else if (!name.compare("enum"))
	{
		token.type = token_type::Enum;
	}
	else if (!name.compare("struct"))
	{
		token.type = token_type::Struct;
	}
	else if (!name.compare("null"))
	{
		token.type = token_type::Null;
	}
	else if (!name.compare("void"))	// TODO add void enum
	{
		token.type = token_type::Void;
	}

	tokens.push_back(token);

	return true;
}

bool Lexer::get_equals()
{
	token_type previous_token = look_back(1);

	switch (previous_token)	// If we find an = we need to check and maybe modify the previous token
	{
	case token_type::Plus:
		tokens.back().type = token_type::PlusEquals;
		tokens.back().size += 1;
		break;
	case token_type::Minus:
		tokens.back().type = token_type::MinusEquals;
		tokens.back().size += 1;
		break;
	case token_type::Times:
		tokens.back().type = token_type::TimesEquals;
		tokens.back().size += 1;
		break;
	case token_type::Slash:
		tokens.back().type = token_type::SlashEquals;
		tokens.back().size += 1;
		break;
	case token_type::LessThan:
		tokens.back().type = token_type::LessThanEquals;
		tokens.back().size += 1;
		break;
	case token_type::MoreThan:
		tokens.back().type = token_type::MoreThanEquals;
		tokens.back().size += 1;
		break;
	case token_type::Not:
		tokens.back().type = token_type::NotEquals;
		tokens.back().size += 1;
		break;
	case token_type::Colon:
		tokens.back().type = token_type::ColonEquals;
		tokens.back().size += 1;
		break;
	case token_type::Equals:
		tokens.back().type = token_type::EqualsEquals;
		tokens.back().size += 1;
		break;
	case token_type::Identifier:
		tokens.emplace_back(token_type::Equals, index, line, colum, 0); return advance();
	default:
		message = "Error tokenizing '='";
		error = true;
		return false;
	}

	return advance();
}

bool Lexer::get_colon()
{
	token_type previous_token = look_back(1);

	if (previous_token == token_type::Colon)
	{
		tokens.back().type = token_type::ColonColon;
		tokens.back().size += 1;
		return true;
	}

	tokens.emplace_back(token_type::Colon, index, line, colum, 1);

	return advance();
}

bool Lexer::get_more_than()
{
	token_type previous_token = look_back(1);

	if (previous_token == token_type::Minus)
	{
		tokens.back().type = token_type::Arrow;
		tokens.back().size += 1;
	}
	else
	{
		tokens.emplace_back(token_type::MoreThan, index, line, colum, 1);
	}

	return advance();
}

