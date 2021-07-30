#include <iostream>
#include <vector>

#include "lexer.h"

using namespace std;

struct name_scope_type{
    string var_name;
    string var_scope;
    int var_instance; // 0-> global 1->public, 2->private
};

LexicalAnalyzer lexer;

vector<struct name_scope_type> symbol_table;




void add_to_symbol_table(string var_name, string var_scope, int var_instance);

void delete_from_symbol_table(string closed_scope);

string search_symbol_table(string variable_name, string variable_scope);

void parse_stmt(string curr_scope);

void parse_stmt_list(string curr_scope);

void parse_private_vars(string curr_scope);

void parse_public_vars(string curr_scope);

void parse_scope();

vector<string> parse_var_list();

void parse_global_vars();

void parse_program();