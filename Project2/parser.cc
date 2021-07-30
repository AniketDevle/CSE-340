#include <iostream>
#include <vector>

#include "lexer.h"
#include "parser.h"

using namespace std;

vector<string> all_answers;
int left_parenthesis_count = 0;
int right_parenthesis_count = 0;

void syntax_error()
{
    cout << "Syntax Error"<<endl;
    exit(1); 
}

void add_symbol_table(string name, string scope, int type)
{
    //this funtion takes the name, scope and type(global=0,public=1,private=2) and adds the entry to the global symbol table that we maintain.
    name_scope_type current_var = {name, scope, type};
    symbol_table.push_back(current_var);
    return;
}


string search_symbol_table(string name, string variable_scope)
{   

    
    int symbol_table_size = symbol_table.size();
    for(int i=symbol_table_size-1;i>=0;i--)
    {
        if(symbol_table[i].var_name == name)
        {
            if(symbol_table[i].var_scope == variable_scope)
            {
                return symbol_table[i].var_scope;
            }
            else 
            {
                if(symbol_table[i].var_instance == 1 || symbol_table[i].var_instance == 0 )
                {
                    return symbol_table[i].var_scope;
                }
            }
        }
    }
    return "?";
}
void delete_from_symbol_table(string closed_scope)
{   
    //when we encounter an RBRACE, we have to remove all the variables of the current scope from the global symbol table
    //closed scope is the scope which is closed, so we remove the closed scope variables

    int table_size = symbol_table.size();
    if(table_size)
    {
        string current_scope = symbol_table[table_size-1].var_scope;
        while(table_size > 0 && current_scope == closed_scope)
        {
            symbol_table.pop_back();
            table_size = symbol_table.size();
            if(table_size > 0)
                current_scope = symbol_table[table_size-1].var_scope;
            else
                break;
        }
    }
}
void parse_stmt(string curr_scope)
{
    Token t1 = lexer.GetToken();
    if(t1.token_type == ID)
    {
        Token t2 = lexer.GetToken();
        if(t2.token_type == EQUAL)
        {
            Token t3 = lexer.GetToken();
            if(t3.token_type == ID)
            {
                Token t4 = lexer.GetToken();
                if(t4.token_type == SEMICOLON)
                {
                    string str_left_cont = search_symbol_table(t1.lexeme, curr_scope); 
                    string str_right_cont = search_symbol_table(t3.lexeme, curr_scope); 
                    if (str_left_cont == "::" && str_right_cont == "::")
                    {
                         string ans = str_left_cont+t1.lexeme+" = "+str_right_cont+t3.lexeme;
                         all_answers.push_back(ans);      
                    }
                    else if (str_left_cont == "::" && str_right_cont != "::")
                    {
                        string ans = str_left_cont+t1.lexeme+" = "+str_right_cont+"."+t3.lexeme;
                        all_answers.push_back(ans);
                    }
                    else if (str_left_cont != "::" && str_right_cont == "::")
                    {
                        string ans = str_left_cont+"."+t1.lexeme+" = "+str_right_cont+t3.lexeme;
                        all_answers.push_back(ans);
                    }
                    else
                    {
                        string ans = str_left_cont+"."+t1.lexeme+" = "+str_right_cont+"."+t3.lexeme;
                        all_answers.push_back(ans);
                    }
                }
                else
                {
                    syntax_error();
                }
            }
            else
            {
                syntax_error();
            }
        }
        else if(t2.token_type == LBRACE)
        {   
            
            lexer.UngetToken(t2);
            lexer.UngetToken(t1);
            parse_scope();
        }
        else
        {
            syntax_error();
        }
    }
    else
    {
        lexer.UngetToken(t1);
    }
}

void parse_stmt_list(string curr_scope)
{
    Token t1 = lexer.GetToken();
    if(t1.token_type == ID)
    {
        while(t1.token_type == ID)
        {
            lexer.UngetToken(t1);
            parse_stmt(curr_scope);
            t1 = lexer.GetToken();
        }
        if(t1.token_type != RBRACE)
        {
            syntax_error();
        }
        else
        {   
            
            lexer.UngetToken(t1);
        }
    }
    else
    {
        syntax_error();
    }
}



void parse_private_vars(string curr_scope)
{
    Token t1 = lexer.GetToken();
    if(t1.token_type == PRIVATE)
    {
        Token t2 = lexer.GetToken();
        if(t2.token_type == COLON)
        {
            vector<string> variable_list = parse_var_list();
            for(int i=0;i<=variable_list.size()-1;i++)
            {
                add_symbol_table(variable_list[i], curr_scope, 2);
            }
        }
        else
        {
            syntax_error();
        }
    }
    else
    {
        lexer.UngetToken(t1);
    }
}

void parse_public_vars(string curr_scope)
{
    Token t1 = lexer.GetToken();
    if(t1.token_type == PUBLIC)
    {
        Token t2 = lexer.GetToken();
        if(t2.token_type == COLON)
        {
            vector<string> variable_list = parse_var_list();
            for(int i=0;i<=variable_list.size()-1;i++)
            {
                add_symbol_table(variable_list[i], curr_scope, 1);
            }
        }
        else
        {
            syntax_error();
        }
    }
    else
    {
        lexer.UngetToken(t1);
    }
}

void parse_scope()
{
    Token t1 = lexer.GetToken();
    string curr_scope;
    if(t1.token_type == ID)
    {
        Token t2 = lexer.GetToken();
        if(t2.token_type == LBRACE)
        {   
            left_parenthesis_count = left_parenthesis_count + 1;
            curr_scope = t1.lexeme;
            parse_public_vars(curr_scope); 
            parse_private_vars(curr_scope);
            parse_stmt_list(curr_scope);

            Token t3 = lexer.GetToken();
            if(t3.token_type == RBRACE)
            {   
                right_parenthesis_count = right_parenthesis_count + 1;
                delete_from_symbol_table(curr_scope);
            }
            else
            {
                syntax_error();
            }
        }
    }
}

vector<string> parse_var_list()
{
    vector<string> variable_list;
    Token t1 = lexer.GetToken();
    if(t1.token_type == COMMA)
    {
        return parse_var_list();
    }
    else if(t1.token_type == SEMICOLON)
    {
        return variable_list;
    }
    else if(t1.token_type == ID)
    {
        variable_list.push_back(t1.lexeme);
        vector<string> parsed = parse_var_list();
        variable_list.insert(variable_list.end(), parsed.begin(), parsed.end());
        return variable_list;
    }
    else
    {
        syntax_error();
    }
    return vector<string>();
}

void parse_global_vars()
{
    
    Token t1 = lexer.GetToken();
    if(t1.token_type == ID)
    {
        Token t2 = lexer.GetToken();
        if(t2.token_type == COMMA)
        {
            lexer.UngetToken(t2);
            lexer.UngetToken(t1);
            vector<string> variable_list = parse_var_list();
            for(int i = 0; i<=variable_list.size()-1;i++)
            {
                add_symbol_table(variable_list[i], "::", 0);
            }
        }
    }
    else
    {
        syntax_error();
    }
}

void parse_program()
{
    Token t1 = lexer.GetToken();
    if(t1.token_type == ID)
    {
        Token t2 = lexer.GetToken();
        if( t2.token_type == COMMA )
        {
            lexer.UngetToken(t2);
            lexer.UngetToken(t1);
            parse_global_vars();
            parse_scope();
        }
        else if(t2.token_type == SEMICOLON )
        {
            add_symbol_table(t1.lexeme,"::",0);
            parse_scope();
        }
        else if( t2.token_type == LBRACE)
        {   
            
            lexer.UngetToken(t2);
            lexer.UngetToken(t1);
            parse_scope();
            
        }
        else
        {
            syntax_error();
        }

        Token check = lexer.GetToken();
        cout<<check.lexeme<<endl;
        if (check.token_type != END_OF_FILE || check.token_type != ERROR)
        {
            syntax_error();
        }
    }
    else
    {   
        syntax_error();
    }
    
}

int main()
{
    
    parse_program();
    //cout<<"right parenthesis"<<right_parenthesis_count<<endl;
    //cout<<"left parenthesis" <<left_parenthesis_count<<endl;
    
    /*
    Token T1 = lexer.GetToken();
    if (T1.token_type != END_OF_FILE)
    {
        syntax_error();
    }
    */

    for(int i=0;i<all_answers.size();i++)
    {
        cout << all_answers[i]<<endl;
    }
}