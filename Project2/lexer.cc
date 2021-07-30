/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR",
    "PRIVATE","PUBLIC", "LBRACE", "RBRACE"
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}
void LexicalAnalyzer::SkipComment()
{
    char first,second,third,fourth;
    input.GetChar(first);
    input.GetChar(second);
    if (first == '/' and second =='/')
    {   
        input.GetChar(third);
        while(third != '\n')
        {
            input.GetChar(third);
        }
        SkipSpace();
        input.GetChar(fourth);
        if (fourth == '/')
        {
            input.UngetChar(fourth);
            SkipComment();
        }
        else
        {
            input.UngetChar(fourth);
        }
    }
    else
    {
        input.UngetChar(second);
        input.UngetChar(first);
    }
}
bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;

    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0') {
            tmp.lexeme = "0";
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
        }
        tmp.token_type = NUM;
        tmp.line_no = line_no;
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);
    // cout << "ID me c: "<<c<<endl;

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

string LexicalAnalyzer::public_private_checker()
{
    string pripub ="";
    for(int i=0;i<6;i++)
    {
        char cc;
        input.GetChar(cc);
        if(input.EndOfInput())
        {
            break;
        }
        pripub += cc;
    }
    if(pripub == "public" || pripub == "PUBLIC")
        return "public";
    else if(pripub == "privat" || pripub == "PRIVAT")
    {
        char cc;
        input.GetChar(cc);
        if(cc == 'e' || cc == 'E') return "private";
        else
        {
            pripub+=cc;
        }
    }
    int spd = pripub.length();
    for(int i=spd-1;i>=0;i--)
    {
        input.UngetChar(pripub[i]);
    }
    return "";
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    SkipComment();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            tmp.lexeme = "DOT";
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            tmp.lexeme = "PLUS";
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            tmp.lexeme ="MINUS";
            return tmp;
        case '/':
            tmp.token_type = DIV;
            tmp.lexeme = "DIV";
            return tmp;
        case '*':
            tmp.token_type = MULT;
            tmp.lexeme = "MULT";
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            tmp.lexeme = "EQUAL";
            tmp.lexeme = "=";
            return tmp;
        case ':':
            tmp.token_type = COLON;
            tmp.lexeme = "COLON";
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            tmp.lexeme = "COMMA";
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            tmp.lexeme = "SEMICOLON";
            tmp.lexeme = ";";
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            tmp.lexeme = "LBRAC";
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            tmp.lexeme = "RBRAC";
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            tmp.lexeme = "LPAREN";
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            tmp.lexeme = "RPAREN";
            return tmp;
        case '{':   
            tmp.token_type = LBRACE;
            tmp.lexeme = "LBRACE";
            return tmp;
        case '}':
            tmp.token_type = RBRACE;
            tmp.lexeme = "RBRACE";
            tmp.lexeme = "}";
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) 
            {
                input.UngetChar(c);
                return ScanNumber();
            } 
            else if (isalpha(c)) 
            {
                input.UngetChar(c);
                if(c == 'p' || c=='P') 
                {
                    string resp = public_private_checker();
                    if(resp == "public")
                    {
                        tmp.token_type = PUBLIC;
                        tmp.lexeme = "PUBLIC";
                        return tmp;
                    }
                    else if(resp == "private")
                    {
                        tmp.token_type = PRIVATE;
                        tmp.lexeme = "PRIVATE";
                        return tmp;
                    }
                }
                return ScanIdOrKeyword();
            } 
            else if (input.EndOfInput())
            {
                tmp.token_type = END_OF_FILE;
                tmp.lexeme = "END_OF_FILE";
                return tmp;
            }
            else
            {   
                tmp.token_type = ERROR;
                tmp.lexeme = "ERROR";
                return tmp;
            }
    }
}
