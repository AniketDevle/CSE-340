#include <iostream>
#include <vector>

#include "lexer.h"

using namespace std;

struct info_variable{
    string var_name;
    string var_scope;
    int var_instance; // 1->public, 2->private
};

vector<struct info_variable> symbol_table;

int get_symbol_table_size()
{
    return symbol_table.size();
}

void add_to_symbol_table(string var_name, string var_scope, int var_instance)
{
    info_variable entry = {var_name, var_scope, var_instance};
    symbol_table.push_back(entry);
    cout << "\tadded:\t"<<var_name<<"\t"<<var_scope<<"\t"<<var_instance<<endl;
    return;
}

void delete_from_symbol_table(string closed_scope)
{
    int table_size = get_symbol_table_size();
    if(table_size > 0)
    {
        string variable_scope = symbol_table[table_size-1].var_scope;
        while(table_size > 0 && variable_scope == closed_scope)
        {
            symbol_table.pop_back();
            cout << "deleted an entry from symbol table" <<endl;
            table_size = get_symbol_table_size();
            if(table_size > 0)
                variable_scope = symbol_table[table_size-1].var_scope;
            else
                break;
        }
    }
}

string search_symbol_table(string variable_name, string variable_scope)
{
    int table_size = get_symbol_table_size();
    for(int i=table_size-1;i>=0;i--)
    {
        if(symbol_table[i].var_name == variable_name)
        {
            if(symbol_table[i].var_scope == variable_scope)
            {
                cout << "search_result:\t" <<symbol_table[i].var_name<<"\t"<<symbol_table[i].var_scope<<"\t"<<symbol_table[i].var_instance<<endl;
                return symbol_table[i].var_scope;
            }
            else // TODO: do we need to have a separate elseif for when the scope is global
            {
                if(symbol_table[i].var_instance == 1 || symbol_table[i].var_instance == 0 )
                {
                    cout << "search_result:\t" <<symbol_table[i].var_name<<"\t"<<symbol_table[i].var_scope<<"\t"<<symbol_table[i].var_instance<<endl;
                    return symbol_table[i].var_scope;
                }
            }
        }
    }
    return "?";
}

void parse_program()
{
    t1 = lexer.getToken();
    if (t1.token_type == ID)
    {
        t2 = getToken();
        if (t2.token_type == SEMICOLON || t2.token_type == COMMA)
        {
            ungetToken (t2);
            ungetToken (t1);
            parse_global_vars();
            parse_scope();
        }
        else if (t2.token_type == LBRACE)
        {
            ungetToken(t2);
            ungetToken(t1);
            parse_scope();
        }
        else
        {
            //syntax error
        }
    }
    else{
        //syntax_error
    }        
}

void parse_global_vars()
{
    t1 = getToken();
    if (t1.token_type == ID)
    {
        t2 = getToken();
        if (t2 == COMMA)
        {   
            ungetToken(t2);
            ungetToken(t1);
            vector<string> global_vars = parse_var_list();
            for (int i = 0; i<global_vars.size();i++)
            {
                add_to_symbol_table(global_vars[i],"::" ,0);
            }
        }
    }
    else
    {
        //syntax_error
    } 
}
vector<string> parse_var_list()
{
    vector <string> var_names;
    t1 = getToken();
    if (t1.token_type == SEMICOLON)
    {
        return var_names;
    }
    else if (t1.token_type == COMMA)
    {
        return parse_var_list();
    }
    else if (t1.token_type == ID)
    {
        var_names.push_back(t1.lexeme);
        vector<string> new_vec = parse_var_list();
        var_names.insert(var_names.end(),new_vec.begin(),new_vec.end());
        return var_names;
    }
    else{
        //syntax_error
    }
}

void parse_scope()
{
  t1 = getToken();
  string current_scope;
  if (t1.token_type == ID)
  {
      t2 = getToken();
      if (t2.token_type == LBRACE)
      {
          current_scope = t1.lexeme;
          parse_public_vars(current_scope);
          parse_private_vars(current_scope);
          parse_stmt_list(current_scope);
          t3 = getToken();
          if (t3 != RBRACE)
          {
              //syntax error
          }
          else
          {
              //delete all the values from the vector table symbol_table
          }
      }
      else
      {
          //syntax_error
      }
  }   
}

void parse_public_vars(string current_scope)
{
    t1 = getToken();
    if (t1.token_type == PUBLIC)
    {
        t2 = getToken();
        if (t2.token_type == COLON)
        {
            vector<string> var_names = parse_var_list();
            for (int i = 0; i < var_names.size(); i++)
            {
                add_to_symbol_table(var_names[i],current_scope,1);
            }
        }
        else{
            //syntax_error
        }
    }
    else
    {
        ungetToken(t1);
    }

}

void parse_private_vars(string current_scope)
{
    t1 = getToken();
    if (t1.token_type == PRIVATE)
    {
        t2 = getToken();
        if (t2.token_type == COLON)
        {
            vector<string> var_names = parse_var_list();
            for (int i = 0; i < var_names.size(); i++)
            {
                add_to_symbol_table(var_names[i],current_scope,2);
            }
        }
        else{
            //syntax_error
        }
    }
    else
    {
        ungetToken(t1);
    }

}

void parse_stmt_list(string current_scope)
{
    t1 = getToken()l;
    if (t1.token_type == ID)
    {
        while(t1.token_type == ID)
        {
            ungetToken(t1);
            parse_stmt(current_scope);
            t1 = get_token();
        }
        if (t1.token_type != RBRACE)
        {
            //syntax_error
        } 
        
    }
    else
    {
        //syntax_error
    }
}
void parse_stmt(string current_scope)
{
    t1 = getToken();
    if (t1.token_type == ID)
    {
        t2 = getToken();
        if (t2.token_type == EQUAL)
        {
            t3 = getToken();
            t4 = getToken();
            if (t3.token_type == ID && t4.token_type == SEMICOLON)
            {   
                //aniket check this once again and see if you completely understand the logic behind this
                //noice now you have to write the code for searching stuff from the global vector
                string var1 = search_symbol_table(t1.lexeme,current_scope);
                string var2 = search_symbol_table(t3.lexeme,current_scope);
                //print properly
            }
            else
            {
                //syntax_error
            }
        }
        else if (t2.token_type == LBRACE)
        {
            ungetToken(t2);
            ungetToken(t1);
            parse_scope();
        }
        else
        {
            //syntax_error
        }
    }
    else
    {
        ungetToken(t1);
    }
}
