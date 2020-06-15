#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>
#include <assert.h>
using namespace std;


/*
	TODO list

	add token keywords for basic types
	add token for casting. not sure of the sintax but i'm thinking of "identifier as type"
	add token for get memory address and derreference pointer

	maybe add checks when tokenizing ints. Right now saying "a := 15sadf;" is considered ok by the lexer 
	maybe add token for type diferent from token for identifier
	maybe revert all the /= += ... to simple tokens, it seems to make it easier for the parser
*/

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
	Arrow,
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
	ColonEquals,
	ColonColon,
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
	Void,
	String,	// this are literals
	Int,
	Float,
	End_Of_File
};

struct Token
{
	token_type type;
	unsigned int position, line, colum, size;	

	Token(token_type t, unsigned int p, unsigned int l, unsigned int c, unsigned int s);
};

struct Lexer
{
	string text;
	vector<Token> tokens;

	string message = "File tokenized successfully";
	bool error = false;

	unsigned int index = 0;
	char caracter;

	unsigned int line = 1, colum = 1;

	void load_file(string file_name);
	bool tokenize_file(string file_name);

	token_type look_back(unsigned int n);

	bool advance();
	bool skip_white_space();
	bool skip_coments();

	bool get_next_token();
	bool get_number();
	bool get_int();
	bool get_string();
	bool get_identifier();

	bool get_equals();
	bool get_colon();
	bool get_more_than();

};

