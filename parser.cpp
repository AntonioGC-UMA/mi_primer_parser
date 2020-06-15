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

    if (!expect_token(token_type::Semicolon)) res = nullptr;

    return res;
}

Ast_node* Parser::parse_expresion()
{
    token_type current = peek(0);
    Ast_node* res = nullptr;

    
    if (is_invocation())                                res = parse_invocation();
    else if (is_literal())                              res = parse_literal();
    else if (is_binary())                               res = parse_binary_operator();
    else if (is_unary())                                res = parse_unary_operator();
    else if (current == token_type::Identifier)         res = parse_identifier();
    else if (current == token_type::OpenParentesis)
    {
        res = parse_expresion();
        if (!expect_token(token_type::CloseParentesis)) res = nullptr;
    }

    return res;
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
