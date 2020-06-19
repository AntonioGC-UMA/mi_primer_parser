#include "Parser.h"
#include "Parser.h"

bool Parser::parse_file(string file_name)
{
    bool success = lexer.tokenize_file(file_name);

    if (!success)
    {
        cerr << lexer.error << endl;
        return false;
    }

    root = parse_code_block(); 

    return true;     // TODO check for errors
}

bool Parser::expect_token(token_type token)
{
    if (peek(0) == token) return true;

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
    token_type current = peek(0);

    Ast_node* res = nullptr;

    if(is_declaration())        res = parse_declaration();
    else if(is_asigment())      res = parse_asigment();
    else if(is_invocation())    res = parse_invocation();
    // TODO parse keywords here

    if (!expect_token(token_type::Semicolon)) res = nullptr;

    return res;
}

Ast_node* Parser::parse_expresion()
{
    token_type current = peek(0);
    Ast_node* res = nullptr;

    
    if (is_invocation())                                res = parse_invocation();
    else if (is_identifier())                           res = parse_identifier();
    else if (is_literal())                              res = parse_literal();
    else if (is_unary_operator())                       res = parse_unary_operator();
    else if (parse_parentesis())                        res = parse_parentesis();
    
    return parse_binary_operator(res);
}

Ast_node* Parser::parse_binary_operator(Ast_node* left)
{
    if (left == nullptr) return nullptr;

    Ast_node node;
    node.type = ast_type::BinaryOperator; // TODO this is way to little information
    node.children.push_back(left);

    switch (peek(0))
    {
    case token_type::Plus:
    case token_type::Minus:
    case token_type::Times:
    case token_type::Slash:
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
        Ast_node* right = parse_expresion();
        if (right == nullptr) return nullptr;
        node.children.push_back(right);
        nodes.push_back(node);
        return &nodes.back();         
    }  
    return left;    // This is the case where we did not have a binary operator       
}

Ast_node* Parser::parse_declaration()
{
    int offset = 2;     // the offset of the position i'm looking at
    token_type modifier = peek(offset);
    
    if (modifier == token_type::Identifier)
    {
        offset++;
        modifier = peek(offset); // parse the type info
    }

    if (modifier == token_type::Colon)
    {
        nodes.push_back({ ast_type::ConstantDeclaration });
    }    
    else if (modifier == token_type::Equals)
    {
        nodes.push_back({ ast_type::Declaration }); // all declarations are declaration and asigment
    }

    offset;

    Ast_node* node = &nodes.back();

    node->children.push_back(parse_identifier());

    // TODO add the type, either read it or infer it
    //node->children.push_back(type);

    index += offset;

    node->children.push_back(parse_expresion());

    return node;
}

Ast_node* Parser::parse_asigment()
{
    nodes.push_back({ ast_type::Asigment });
    Ast_node* node = &nodes.back();

    Ast_node* variable = parse_identifier();

    if (!variable) return nullptr;

    Ast_node* value = parse_expresion();

    if (!value) return nullptr;

    node->children.push_back(variable);
    node->children.push_back(value);

    return node;
}

Ast_node* Parser::parse_invocation()
{
    return nullptr;
}

Ast_node* Parser::parse_identifier()
{
    return nullptr;
}

Ast_node* Parser::parse_literal()
{
    return nullptr;
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
    return nullptr;
}

Ast_node* Parser::parse_parentesis()
{
    return nullptr;
}

bool Parser::is_declaration()
{
    return peek(0) == token_type::Identifier && peek(1) == token_type::Colon;
}

bool Parser::is_asigment()  // TODO
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