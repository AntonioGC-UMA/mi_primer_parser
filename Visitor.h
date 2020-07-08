#pragma once

#include "Parser.h"

#include <unordered_map>



struct Visitor
{
	int tabs = -1;
	ofstream file;

	void visit_tree(Ast_node* node, string file_name);
	void visit(Ast_node* node);
	void visit_code_block(Ast_node* node);
	void visit_identifier(Ast_node* node);
	void visit_binary_operator(Ast_node* node);
	void visit_invocation(Ast_node* node);
	void visit_declaration(Ast_node* node);
	void visit_assigment(Ast_node* node);
	void visit_lambda(Ast_node* node);
};

