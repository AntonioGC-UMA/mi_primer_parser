#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>
using namespace std;

enum class token_type
{
	String,
	Equals,
	Semicolon,
	Comma,
	Dot,
	OpenBraket,
	CloseBraket,
	OpenParentesis,
	CloseParentesis,
	Identifier,
	Number,
	Math,
	End_Of_File
};



struct token
{
	token_type type;
	int position;
	int size;

	int line;
	int position_in_line;

	token(token_type type, int position, int line, int position_in_line, int size);
};


extern vector<string> token_type_name;

struct lexer
{
	string text;
	vector<token> tokens;

	int index = 0;
	char caracter = ' ';
	int line = 1;
	int position_in_line = 1;

	void load_file(string file_name);
	void tokenize_file(string file_name);

	void advance();
	void skip_white_space();
	void skip_coments();
	token get_next_token();
	token collect_string();
	token collect_identifier();
	token collect_number();
	token advance_and_return(token t);

	string get_name(token t);
};

