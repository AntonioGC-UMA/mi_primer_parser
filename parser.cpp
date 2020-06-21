#include "Parser.h"
#include "Parser.h"

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

    if (!expect_token(token_type::Semicolon)) res = nullptr;

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
    else if (parse_parentesis())      res = parse_parentesis();
    
    return res;//parse_binary_operator(res, 0);
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

Ast_node* Parser::parse_binary_operator(Ast_node* left, int precedence)
{
    if (left == nullptr) return nullptr;

    int my_precedence = get_precedence();

    if (my_precedence > precedence)
    {
        nodes.push_back({ ast_type::BinaryOperator });

        Ast_node* node = &nodes.back();
        node->type = ast_type::BinaryOperator;
        node->children.push_back(left);

        index++; 

        Ast_node* right = parse_expresion();

        int next_precedence;

        while ((next_precedence = get_precedence()) != -1)  // mientras haya mas operadores binarios
        {
            if (my_precedence < next_precedence)
            {
                right = parse_binary_operator(right, my_precedence + 1);
                node->children.push_back(right);
            }
            else
            {
                node->children.push_back(right);
                node = parse_binary_operator(node, next_precedence + 1);
            }
        }

        return node;
    }

    return left;    // This is the case where we did not have a binary operator       
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
        nodes.push_back({ ast_type::Declaration });
    }

    Ast_node* node = &nodes.back();

    node->children.push_back(parse_identifier());

    index++; //skip the :: or :=

    node->children.push_back(parse_expresion());

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

    if (peek(0) == token_type::CloseParentesis) return node;    // No arguments

    Ast_node* arg;

    while (arg = parse_expresion())
    {
        node->children.push_back(arg);
        if (peek(0) == token_type::Comma) continue;
        if (peek(0) == token_type::CloseParentesis) break;
        return nullptr; // ERROR
    }

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