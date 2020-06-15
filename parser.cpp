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

    if (current == token_type::Identifier)
    {
        token_type next_token = peek(1);

        if (next_token == token_type::Colon) return parse_declaration();    // TODO shoud I handle constant declarations here?
        if (next_token == token_type::Equals) return parse_asigment();      // TODO how do I handle the diferent types of segments?
        if(next_token == token_type::OpenParentesis) return parse_invocation();
    }
    else if (current == token_type::If) // TODO: shoud the if statement keep the else statement inside?
    {

    }
    else if (current == token_type::Else)
    {

    }
    return nullptr;
}

Ast_node* Parser::parse_expresion()
{
    return nullptr;
}

Ast_node* Parser::parse_declaration()
{
    token_type modifier = peek(2);
    if (modifier == token_type::Colon)
    {
        nodes.push_back({ ast_type::ConstantDeclaration });
    }


    Ast_node* node = &nodes.back();

    node->children.push_back(parse_identifier());


}

Ast_node* Parser::parse_asigment()
{
    return nullptr;
}
