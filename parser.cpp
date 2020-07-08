#include "Parser.h"
#include "Parser.h"

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

bool Parser::parse_file(string file_name)
{
    bool success = lexer.tokenize_file(file_name);


    root = parse_code_block(); 

    return true;     // TODO check for errors
}

bool Parser::expect_token(token_type token)
{
    index++;

    if (peek(-1) == token) return true;

    cerr << "Unexpected token" << endl;

    return false;
}

token_type Parser::peek(int offset)
{
    int pos = index + offset;

    if (pos < 0 || pos >= lexer.tokens.size()) return token_type::End_Of_File;

    return lexer.tokens[pos].type;
}



Ast_node* Parser::parse_code_block()
{
    nodes.push_back({ ast_type::CodeBlock });
    Ast_node* node = &nodes.back();

    while (true)
    {
        Ast_node* child = parse_statement();
        if (child == nullptr) break;
        node->children.push_back(child);
    }

    return node;
}

Ast_node* Parser::parse_statement()
{
    Ast_node* res = nullptr;

    if(is_declaration())        res = parse_declaration();
    else if(is_asigment())      res = parse_asigment();
    else if(is_invocation())    res = parse_invocation();
    // TODO parse keywords here

    if (!expect_token(token_type::Semicolon))
    {
        index--;
        res = nullptr;
    }

    return res;
}

Ast_node* Parser::parse_expresion()
{
    token_type current = peek(0);
    Ast_node* res = nullptr;

    
    if (is_invocation())              res = parse_invocation();
    else if (is_identifier())         res = parse_identifier();
    else if (is_literal())            res = parse_literal();
    else if (is_unary_operator())     res = parse_unary_operator();
    else if (is_parentesis())      res = parse_parentesis();
    
    return parse_binary_operator(res);
}

int Parser::get_precedence()
{
    switch (peek(0))
    {
    case token_type::Plus:  return 20;
    case token_type::Minus: return 20;
    case token_type::Times: return 40;
    case token_type::Slash: return 40;
    case token_type::PlusEquals:
    case token_type::MinusEquals:
    case token_type::TimesEquals:
    case token_type::SlashEquals:
    case token_type::LessThan:
    case token_type::MoreThan:
    case token_type::LessThanEquals:
    case token_type::MoreThanEquals:
    case token_type::NotEquals:
    case token_type::EqualsEquals:
        return 0;
    default:
        return -1;
    }
}

int Parser::get_precedence(string token)
{
    if (token.compare("+") == 0 || token.compare("-") == 0) return 20;
    if (token.compare("*") == 0 || token.compare("/") == 0) return 40;
    return -1;
}

Ast_node* Parser::parse_binary_operator(Ast_node* left)
{
    if (left == nullptr) return nullptr;

    int p1 = get_precedence();

    if (p1 < 0) return left; // This is the case where we did not have a binary operator    
    
    nodes.push_back({ ast_type::BinaryOperator });

    Ast_node* node = &nodes.back();
    node->type = ast_type::BinaryOperator;
    node->value = lexer.text.substr(lexer.tokens[index].position, lexer.tokens[index].size);
    node->children.push_back(left);

    index++; 

    Ast_node* right = parse_expresion();

    int p2 = get_precedence(right->value);

    if (p1 < p2 || p2 < 0) // adoptar
    {
        node->children.push_back(right);
        return node;
    }
    else    // robar
    {
        Ast_node* res = right;

        Ast_node* prev = right;

        while (right->children.size() > 0 && right->children[0]->type == ast_type::BinaryOperator)
        {
            prev = right;
            right = right->children[0];
        }

        if (get_precedence(right->value) > p1) right = prev;

        node->children.push_back(right->children[0]);
        right->children[0] = node;
        return res;
    }

}

Ast_node* Parser::parse_declaration()
{
    token_type modifier = peek(1);
    

    if (modifier == token_type::ColonColon)
    {
        nodes.push_back({ ast_type::ConstantDeclaration });
    }    
    else if (modifier == token_type::ColonEquals)
    {
        nodes.push_back({ ast_type::Declaration }); // TODO change this to Declaration and asigment
    }
    else if(modifier == token_type::Colon)
    {
        nodes.push_back({ ast_type::Declaration }); 
    }

    Ast_node* node = &nodes.back();

    node->children.push_back(parse_identifier());

    index++; //skip the :: or :=

    if (peek(0) == token_type::OpenParentesis)  // check for function declaration
    {
        int offset = 1;
        int parentesis = 1;
        while (parentesis != 0)
        {
            if (peek(offset) == token_type::OpenParentesis) parentesis++;
            else if (peek(offset) == token_type::CloseParentesis) parentesis--;
            offset++;
        }

        if (peek(offset) == token_type::Arrow)
        {
            node->type = ast_type::Lambda;
            index++;    // skip (
            while (peek(0) != token_type::CloseParentesis)
            {
                node->children.push_back(parse_identifier());
                if (peek(0) != token_type::CloseParentesis) index++;
            }
            index++;    // skip )
            index++;    // skip ->
            index++;    // skip {

            node->children.push_back(parse_code_block());

            index++;    // skip }
        }
    }
    else if (modifier != token_type::Colon)
    {
        node->children.push_back(parse_expresion());
    }



    return node;
}

Ast_node* Parser::parse_asigment()
{
    nodes.push_back({ ast_type::Asigment });
    Ast_node* node = &nodes.back();

    Ast_node* variable = parse_identifier();

    if (!variable) return nullptr;

    index++;

    Ast_node* value = parse_expresion();

    if (!value) return nullptr;

    node->children.push_back(variable);
    node->children.push_back(value);

    return node;
}

Ast_node* Parser::parse_invocation()
{
    nodes.push_back({ ast_type::Invocation });
    Ast_node* node = &nodes.back();

    Ast_node* name = parse_identifier();

    if (!name) return nullptr;

    node->children.push_back(name);

    expect_token(token_type::OpenParentesis);

    if (peek(0) == token_type::CloseParentesis)
    {
        index++;
        return node;    // No arguments
    }

    Ast_node* arg;

    while (arg = parse_expresion())
    {
        node->children.push_back(arg);

        if (peek(0) != token_type::Comma) break;

        index++;
    }

    expect_token(token_type::CloseParentesis);

    return node;
}

Ast_node* Parser::parse_identifier()
{
    nodes.push_back({ ast_type::Identifier });
    Ast_node* node = &nodes.back();

    Token t = lexer.tokens[index];
    node->value = lexer.text.substr(t.position, t.size);
    index++;
    return node;
}

Ast_node* Parser::parse_literal()
{
    nodes.push_back({ ast_type::Literal });
    Ast_node* node = &nodes.back();

    Token t = lexer.tokens[index];
    node->value = lexer.text.substr(t.position, t.size);
    index++;
    return node;
}


bool Parser::is_invocation()
{
    return peek(0) == token_type::Identifier && peek(1) == token_type::OpenParentesis;
}

bool Parser::is_identifier()
{
    return peek(0) == token_type::Identifier;
}

Ast_node* Parser::parse_unary_operator()
{
    nodes.push_back({ ast_type::Identifier });
    Ast_node* node = &nodes.back();

    node->children.push_back(parse_expresion());    // @Incomplete
    return node;
}

Ast_node* Parser::parse_parentesis()
{
    expect_token(token_type::OpenParentesis);
    Ast_node* node = parse_expresion();
    expect_token(token_type::CloseParentesis);

    return node;
}

bool Parser::is_declaration()
{
    return peek(0) == token_type::Identifier && (peek(1) == token_type::ColonColon || peek(1) == token_type::ColonEquals);
}

bool Parser::is_asigment()
{
    return peek(0) == token_type::Identifier && peek(1) == token_type::Equals;
}

bool Parser::is_literal()
{
    token_type type = peek(0);
    return type == token_type::String || type == token_type::Int || type == token_type::Float;
}

bool Parser::is_unary_operator() // TODO
{
    token_type type = peek(0);
    return type == token_type::Minus || type == token_type::Not;
}

bool Parser::is_parentesis()
{
    token_type type = peek(0);
    return type == token_type::OpenParentesis;
}