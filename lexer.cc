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
#include <vector>

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR","REALNUM" , "BASE08NUM" , "BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };


bool ispdigit(char c) {
    return c >= '1' && c <= '9';
}

bool ispdigit8(char c) {
    return c >= '1' && c <= '7';
}

bool ispdigit16(char c) {
    return (c >= '1' && c <= '9') || (c >= 'A' && c <= 'F');
}

bool isdigit(char c) {
    return c == '0' || ispdigit(c);
}

bool isdigit8(char c) {
    return c == '0' || ispdigit8(c);
}

bool isdigit16(char c) {
    return c == '0' || ispdigit16(c);
}


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

/*bool LexicalAnalyzer::check_consecutive_zero(char c)
{
    if (c != '0')
    {
        return false;
    }
    else
    {	
    	char next_c;
        input.UngetChar(next_c);
        if (next_c =='0')
        {
            input.UngetChar(next_c);
            return true;
        }
        else
        {   
            input.UngetChar(next_c);
            return false;
        }
    }
}
*/
int LexicalAnalyzer::max_length_num(char c)
{   
    	
    /*if (check_consecutive_zero(c) == true)
    {
        return 1;
    }*/
    vector<char> characters;
    int length = 1;
    char next_c; 
    input.GetChar(next_c);
    while(isdigit(next_c) && !input.EndOfInput())
    {
        characters.push_back(next_c);
        input.GetChar(next_c);
        length +=1;
    }
    //as the loop breaks the current value of next_c is not a digit
    //so first do ungetchar(next_c)
    characters.push_back(next_c);
    while (!characters.empty())
    {	
    	char z;
    	z = characters.back();
    	characters.pop_back();
        input.UngetChar(z);
    }
    return length;
}


int LexicalAnalyzer::max_length_base8(char c)
{   
   /*if (check_consecutive_zero(c) == true)
    {
        return 0;
    }*/
    vector<char> characters;
    int length = 1;
    char next_c; 
    input.GetChar(next_c);
    while(isdigit8(next_c) && !input.EndOfInput())
    {   
        //cout<<"In loop"<<'\n';
        characters.push_back(next_c);
        input.GetChar(next_c);
        length +=1;
    }
    //after the loop breaks the next_c can be x so we have to check if next_c is x if it is not x then just return 0
    characters.push_back(next_c);
    if (next_c != 'x')
    {   while(!characters.empty())
        {
            input.UngetChar(characters.back());
            characters.pop_back();
        }
        return 0;
    }
    else
    {
        
        char first;
        first = next_c;
        char second;
        char three;
        input.GetChar(second);
        input.GetChar(three);
        characters.push_back(second);
        characters.push_back(three);
        if (first == 'x' && second =='0' && three =='8')
        {
            length = characters.size()+1;
        }
        else
        {
            length = 0;
        }
        while(!characters.empty())
        {
            input.UngetChar(characters.back());
            characters.pop_back();
        }
        return length;
    }
}
int LexicalAnalyzer::max_length_base16(char c)
{   
   /*if (check_consecutive_zero(c) == true)
    {
        return 0;
    }*/
    vector<char> characters;
    int length = 1;
    char next_c; 
    input.GetChar(next_c);
    while(isdigit16(next_c) && !input.EndOfInput())
    {
        characters.push_back(next_c);
        input.GetChar(next_c);
        length +=1;
    }
    //after the loop breaks the next_c can be x so we have to check if next_c is x if it is not x then just return 0
    characters.push_back(next_c);
    if (next_c != 'x')
    {   while(!characters.empty())
        {
            input.UngetChar(characters.back());
            characters.pop_back();
        }
        return 0;
    }
    else
    {
        
        char first;
        first = next_c;
        char second;
        char three;
        input.GetChar(second);
        input.GetChar(three);
        characters.push_back(second);
        characters.push_back(three);
        if (first == 'x' && second =='1' && three =='6')
        {
            length = characters.size()+1;
        }
        else
        {
            length = 0;
        }
        while(!characters.empty())
        {
            input.UngetChar(characters.back());
            characters.pop_back();
        }
        return length;
    }
}
int LexicalAnalyzer::max_length_real_num(char c)
{
   /*if (check_consecutive_zero(c) == true)
    {
        return 0;
    }*/ 
    vector<char> characters;
    int length = 1;
    char next_c; 
    input.GetChar(next_c);
    while(isdigit(next_c) && !input.EndOfInput())
    {
        characters.push_back(next_c);
        input.GetChar(next_c);
        length +=1;
    }
    //if this loop breaks then the value of next_c can be with . or something else
    // if next_c is not '.' then simply return 0 as this cannot be a real num
    // now that we have esatblished that there is a dot present..we need to check that there is atleast one digit on the right of the dot.
    characters.push_back(next_c);
    if (next_c != '.')
    {   
        while(!characters.empty())
        {
            input.UngetChar(characters.back());
            characters.pop_back();
        }
        return 0;
    }
    else
    {
        char decimal;
        input.GetChar(decimal);
        characters.push_back(decimal);
        if (!isdigit(decimal))
        {
            while(!characters.empty())
            {   
                
                input.UngetChar(characters.back());
                characters.pop_back();
            }
            return 0;
        }
        else
        {   
            length = characters.size();
            input.GetChar(decimal);
            while(isdigit(decimal) && !input.EndOfInput())
            {   
                characters.push_back(decimal);
                length +=1;
                input.GetChar(decimal);

            }
            characters.push_back(decimal);
            while(!characters.empty())
            {   
                //cout<<characters.back()<<'\n';
                input.UngetChar(characters.back());
                characters.pop_back();
            }
            return length+1;
        }
        
    }
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;
    input.GetChar(c);
    if (c == '0')
    {   
        char next_c;
        input.GetChar(next_c);
        if (next_c == 'x')
        {   
           char next_next_c;
           char next_three_c;
           input.GetChar(next_next_c);
           input.GetChar(next_three_c);
           if (next_next_c == '0' && next_three_c == '8')
            {
                tmp.lexeme = "0x08";
                tmp.token_type = BASE08NUM;
                tmp.line_no = line_no;
                return tmp;
            }
            else if (next_next_c == '1' && next_three_c == '6')
            {
                tmp.lexeme = "0x16";
                tmp.token_type = BASE16NUM;
                tmp.line_no = line_no;
                return tmp;
            } 
            else
            {   
                input.UngetChar(next_three_c);
                input.UngetChar(next_next_c);
                input.UngetChar(next_c);
                tmp.lexeme = "0";
                tmp.token_type = NUM;
                tmp.line_no = line_no;
                return tmp;
            }
        }
        else if (next_c == '.')
        {   
            vector <char>Decimals;
            char first_decimal;
            input.GetChar(first_decimal);
            if (!isdigit(first_decimal))
            {
                input.UngetChar(first_decimal);
                input.UngetChar(next_c);
                tmp.lexeme = "0";
                tmp.token_type = NUM;
                tmp.line_no = line_no;
                return tmp;
            }
            else
            {   
                tmp.lexeme = "0.";
                tmp.lexeme += first_decimal;
                char next_decimal;
                input.GetChar(next_decimal);
                while(isdigit(next_decimal) && !input.EndOfInput())
                {   
                    tmp.lexeme += next_decimal;
                    input.GetChar(next_decimal);
                }
                input.UngetChar(next_decimal);
                tmp.token_type = REALNUM;
                tmp.line_no = line_no;
                return tmp;
            }
        }
        else
        {
            input.UngetChar(next_c);
            tmp.lexeme = "0";
            tmp.token_type = NUM;
            tmp.line_no = line_no;
            return tmp;
        } 
    }
    else
    {
        
        int max_num = max_length_num(c);
        int max_base8 = max_length_base8(c);
        int max_base16 = max_length_base16(c);
        int max_real = max_length_real_num(c);

        //cout<<"NUM_size"<<max_num<<'\n';
        //cout<<"Base8_size"<<max_base8<<'\n';
        //cout<<"Base16_size"<<max_base16<<'\n';
        //cout<<"Real_size"<<max_real<<'\n';

        
        if (max_num > max_base8 && max_num > max_base16 && max_num > max_real)
        {
            tmp.token_type = NUM;
            tmp.line_no = line_no;
            tmp.lexeme = "";
            tmp.lexeme += c;
            for (int i = 0 ; i<max_num-1;i++)
            {   
                char eps;
                input.GetChar(eps); 
                tmp.lexeme += eps;
            }
            return tmp;
        }

        if (max_base8 > max_num && max_base8 > max_base16 && max_base8 > max_real)
        {
            tmp.token_type = BASE08NUM;
            tmp.line_no = line_no;
            tmp.lexeme = "";
            tmp.lexeme += c;
            for (int i = 0 ; i<max_base8-1;i++)
            {
                char eps;
                input.GetChar(eps); 
                tmp.lexeme += eps;
            }
            return tmp;
        }

        if (max_base16 > max_num && max_base16 > max_base8 && max_base16 > max_real)
        {
            tmp.token_type = BASE16NUM;
            tmp.line_no = line_no;
            tmp.lexeme = "";
            tmp.lexeme += c;
            for (int i = 0 ; i<max_base16-1;i++)
            {
                char eps;
                input.GetChar(eps); 
                tmp.lexeme += eps;
            }
            return tmp;
        }

        else if (max_real > max_num && max_real > max_base8 && max_real > max_base16)
        {
            tmp.token_type = REALNUM;
            tmp.line_no = line_no;
            tmp.lexeme = "";
            tmp.lexeme += c;
            for (int i = 0 ; i<max_real-1;i++)
            {   
                
                char eps;
                input.GetChar(eps); 
                tmp.lexeme += eps;
            }
            return tmp;
        }
        
        {
        if (!input.EndOfInput())
            {
                input.UngetChar(c);
            }
            tmp.lexeme = "";
            tmp.token_type = ERROR;
            tmp.line_no = line_no;
            return tmp; 
        }
    }
    
    
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

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
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
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
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
