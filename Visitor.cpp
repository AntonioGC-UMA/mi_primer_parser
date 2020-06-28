#include "Visitor.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

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
	llvm::Value* v;

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
