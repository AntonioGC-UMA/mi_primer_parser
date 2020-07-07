#include "Visitor.h"


void Visitor::visit_tree(Ast_node* node, string file_name)
{
	file.open(file_name);

	visit(node);
}

void Visitor::visit(Ast_node* node)
{
	switch (node->type)
	{
	case ast_type::CodeBlock:
		visit_code_block(node); break;
	case ast_type::Identifier :
		visit_identifier(node); break;
	case ast_type::Asigment :
		visit_assigment(node);	break;
	case ast_type::BinaryOperator:
		visit_binary_operator(node); break;
	case ast_type::Invocation:
		visit_invocation(node); break;
	case ast_type::Declaration:
		visit_declaration(node); break;
	case ast_type::Literal:
		visit_identifier(node); break;
	}
}

void Visitor::visit_code_block(Ast_node* node)
{	
	tabs++;


	for (auto child : node->children)
	{
		for (int i = 0; i < tabs; i++) file << "\t";

		visit(child);
		
		file << endl;
	}

	tabs--;
}

void Visitor::visit_identifier(Ast_node* node)
{
	file << node->value;
}

void Visitor::visit_binary_operator(Ast_node* node)
{
	file << "(";
	visit(node->children[0]);
	file << " " << node->value << " ";
	visit(node->children[1]);
	file << ")";
}

void Visitor::visit_invocation(Ast_node* node)
{
	visit(node->children[0]);
	file << "( ";
	for (int i = 1; i < node->children.size() - 1; i++)
	{
		visit(node->children[i]);
		file << ", ";
	}
	visit(node->children[node->children.size() - 1]);

	file << " )";
}

void Visitor::visit_declaration(Ast_node* node)
{
	visit(node->children[0]);
	file << " = ";
	visit(node->children[1]);
}

void Visitor::visit_assigment(Ast_node* node)
{
	visit(node->children[0]);
	file << " = ";
	visit(node->children[1]);
}

/*

void Visitor::visit(Ast_node* node)
{
	switch (node->type)
	{
	case ast_type::CodeBlock:
		visit_code_block(node); break;
	case ast_type::Identifier :
		visit_identifier(node); break;
	case ast_type::Asigment :
		visit_assigment(node);	break;
	case ast_type::BinaryOperator:
		visit_binary_operator(node); break;
	case ast_type::Invocation:
		visit_invocation(node); break;
	case ast_type::Declaration:
		visit_declaration(node); break;
	case ast_type::Literal:
		visit_identifier(node); break;
	}
}

void Visitor::visit_code_block(Ast_node* node)
{
	cout << "{\n";

	for (auto child : node->children)
	{
		visit(child);
		cout << ";\n";
	}

	cout << "}\n";
}

void Visitor::visit_identifier(Ast_node* node)
{
	cout << " " << node->value;
}

void Visitor::visit_binary_operator(Ast_node* node)
{
	cout << "( ";
	visit(node->children[0]);
	cout << " " << node->value << " ";
	visit(node->children[1]);
	cout << " )";
}

void Visitor::visit_invocation(Ast_node* node)
{
	cout << " ";
	visit(node->children[0]);
	cout << "( ";
	for (int i = 1; i < node->children.size(); i++)
	{
		visit(node->children[i]);
	}
	cout << " )";
}

void Visitor::visit_declaration(Ast_node* node)
{
	cout << " ";
	visit(node->children[0]);
	cout << " declared as: ";
	visit(node->children[1]);
}

void Visitor::visit_assigment(Ast_node* node)
{
	cout << " ";
	visit(node->children[0]);
	cout << " assigned the value: ";
	visit(node->children[1]);
}

*/