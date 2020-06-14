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
	Identifier,
	Dot,
	Comma,
	Semicolon,
	Colon,
	OpenBraket,
	CloseBraket,
	OpenParentesis,
	CloseParentesis,
	OpenBox,
	CloseBox,
	Equals,
	Plus,
	Minus,
	Times,
	Slash,
	LessThan,
	MoreThan,
	Not,
	EqualsEquals,
	PlusEquals,
	MinusEquals,
	TimesEquals,
	SlashEquals,
	LessThanEquals,
	MoreThanEquals,
	NotEquals,
	If,		// TODO add to the reserved keywords the basic types
	Else,
	For,
	While,
	Return,
	Continue,
	Break,
	Enum,
	Struct,
	Null,
	String,
	Int,
	Float,
	End_Of_File
};

struct Token
{
	token_type type;
	int position, line, colum, size;	
};

struct Lexer
{
	string text;
	vector<Token> tokens;

	string message = "File tokenized successfully";
	bool error = false;

	int index;
	char caracter;

	int line = 1, colum = 1;

	void load_file(string file_name);
	bool tokenize_file(string file_name);

	token_type look_back(int n);

	bool advance();
	bool skip_white_space();
	bool skip_coments();

	bool get_next_token();
	bool get_number();
	bool get_int();
	bool get_string();
	bool get_identifier();
	bool get_equals();

};

