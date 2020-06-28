#pragma once

#include "Parser.h"

struct Visitor
{
	void visit(Ast_node* node);
	void visit_code_block(Ast_node* node);
	void visit_identifier(Ast_node* node);
	void visit_binary_operator(Ast_node* node);
	void visit_invocation(Ast_node* node);
	void visit_declaration(Ast_node* node);
	void visit_assigment(Ast_node* node);
};

