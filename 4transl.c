//2: added variables
//3: added sin\cos\ln\exp
//4: logica changed to tokens

#include "transl.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

long FileSize(FILE* in) 
{
	long file_size = fseek(in, 0L, SEEK_END);
	file_size = ftell(in);
	rewind(in);
	return file_size;
}

//returns tree_t* of the tree that was
//composed from file "file_name" 
//returns NULL if error appears
tree_t* GetTreeByFile(const char* file_name) 
{
	FILE* in = fopen(file_name, "r");
	if (in == NULL) 
		return NULL;

	char* str = (char*)calloc(FileSize(in), 1);
	if (str == NULL)
		return NULL;

	fread(str, 1, FileSize(in), in);

	tree_t* tree = TreeCreate();
	if (tree == NULL)
		return NULL;
	

	//colect tokens and create tree of this tokens
	char* cur_char = str;
	
	vector_spec(token_t) vec;
	vector_init(&vec);
	vector_reserve(&vec, MAX_LITERALS * sizeof(token_t));

	GetLiterals(&vec, &cur_char);
	int ofs = 0;
	tree -> head = GetExpression(&vec, &ofs);

	vector_destroy(&vec);
	fclose(in);
	free(str);
	return tree;
}	

//collect tokens then push them into vec_t vec
enum node_types_enum GetLiterals(vec_t* vec, char** cur_char)
{
	while(**cur_char == ' ')
		(*cur_char)++;
	token_t elem;

	int is_num = 0;
	val_t val = 0;
	while (**cur_char >= '0' && **cur_char <= '9') {
		is_num = 1;
		val *= 10;
		val += (**cur_char) - '0';
		(*cur_char)++;
	}
	
	if (is_num) {
		elem.type = NUM;
		elem.token_val = val;
		vector_push_back(vec, elem);	
		GetLiterals(vec, cur_char);
		return NUM;
	}
		

	#define OPERATOR(name, str, act)                         \
		if (strncmp(*cur_char, str, strlen(str)) == 0) { \
			*cur_char += strlen(str);                \
			elem.type = OPER;		         \
			elem.token_oper = TOKEN_OPER_##name;     \
			vector_push_back(vec, elem);             \
			GetLiterals(vec, cur_char);              \
			return OPER;			         \
		}
	#include "NotationRules/list_operators"
	#undef OPERATOR
	
	#define VARIABLE(name)                                         \
		if (strncmp(*cur_char, #name, strlen(#name)) == 0) {   \
			*cur_char += strlen(#name);                    \
			elem.type = VAR;		               \
			elem.token_var = TOKEN_VAR_##name;             \
			vector_push_back(vec, elem);                   \
			GetLiterals(vec, cur_char);                    \
			return VAR;			               \
		}
	#include "NotationRules/list_variables"
	#undef VARIABLE 
	
	#define SYNTAX(name, sym)                                      \
		if (strncmp(*cur_char, sym, strlen(sym)) == 0) {       \
			*cur_char += strlen(sym);                      \
			elem.type = SYNT;		               \
			elem.token_synt = TOKEN_SYNT_##name;           \
			vector_push_back(vec, elem);                   \
			GetLiterals(vec, cur_char);                    \
			return SYNT;			               \
		}
	#include "NotationRules/list_syntax"
	#undef SYNTAX

	return UNDEFINED_TYPE;
}

node_t* GetExpression(vec_t* vec, int* token)
{
	node_t* ret_node = GetSum(vec, token);
		
	assert (TOKEN_TYPE == SYNT && TOKEN_SYNT == TOKEN_SYNT_END);
	return ret_node;
	
}

node_t* GetSum(vec_t* vec, int* token) 
{
	node_t* node1 = GetMul(vec, token);
	while(TOKEN_TYPE == OPER && (TOKEN_OPER == TOKEN_OPER_PLUS 
				     || TOKEN_OPER == TOKEN_OPER_MINUS)) {
		token_t detect_token;
		detect_token.token_oper = TOKEN_OPER;
		
		(*token)++;
		node_t* node2 = GetMul(vec, token);
		node_t* op_node = NULL;

		if (detect_token.token_oper == TOKEN_OPER_PLUS)
			op_node = CreateNodeLR(OP_PLUS, OPER, node1, node2);  
		else
			op_node = CreateNodeLR(OP_MINUS, OPER, node1, node2);

		node1 -> parent = op_node;
		node2 -> parent = op_node;		
		node1 = op_node;
	}
	return node1;
}

node_t* GetFunc(vec_t* vec, int* token)
{
	if (TOKEN_TYPE == OPER) {
		if (TOKEN_OPER == TOKEN_OPER_SIN) {
			(*token)++;
			node_t* left = GetBrackets(vec, token);
			node_t* node = CreateNodeLR(OP_SIN, OPER, left, NULL);
			return node;
		}
		if (TOKEN_OPER == TOKEN_OPER_COS) {
			(*token)++;
			node_t* left = GetBrackets(vec, token);
			node_t* node = CreateNodeLR(OP_COS, OPER, left, NULL);
			return node;
		}
		if (TOKEN_OPER == TOKEN_OPER_EXP) {
			(*token)++;
			node_t* left = GetBrackets(vec, token);
			node_t* node = CreateNodeLR(OP_EXP, OPER, left, NULL);
			return node;
		}
		if (TOKEN_OPER == TOKEN_OPER_LN) {
			(*token)++;
			node_t* left = GetBrackets(vec, token);
			node_t* node = CreateNodeLR(OP_LN, OPER, left, NULL);
			return node;
		}
		if (TOKEN_OPER == TOKEN_OPER_TAN) {
			(*token)++;
			node_t* left = GetBrackets(vec, token);
			node_t* node = CreateNodeLR(OP_TAN, OPER, left, NULL);
			return node;
		}
	}
	return GetBrackets(vec, token);
}

node_t* GetMul(vec_t* vec, int* token) 
{
	node_t* node1 = GetFunc(vec, token);
	while (TOKEN_TYPE == OPER && (TOKEN_OPER == TOKEN_OPER_MUL ||
				      TOKEN_OPER == TOKEN_OPER_DIV)) {
		token_t detect_token;
		detect_token.token_oper = TOKEN_OPER;

		(*token)++;
		node_t* node2 = GetFunc(vec, token);
		node_t* op_node = NULL;
		if (detect_token.token_oper == TOKEN_OPER_MUL)
			op_node = CreateNodeLR(OP_MUL, OPER, node1, node2);  
		else
			op_node = CreateNodeLR(OP_DIV, OPER, node1, node2);

		node1 -> parent = op_node;
		node2 -> parent = op_node;		
		node1 = op_node;
	}
	return node1;
}

node_t* GetBrackets(vec_t* vec, int* token) 
{
	if (TOKEN_TYPE == SYNT && TOKEN_SYNT == TOKEN_SYNT_LBR) {
		(*token)++;
		
		node_t* node = GetSum(vec, token);
		
		assert(TOKEN_TYPE == SYNT && TOKEN_SYNT == TOKEN_SYNT_RBR);
		(*token)++;

		return node;
	}
	if (TOKEN_TYPE == VAR) 
		return GetVar(vec, token);

	return GetNumber(vec, token);
}

node_t* GetVar(vec_t* vec, int* token)
{
	node_t* ret = CreateNode(TOKEN_VAR, VAR);
	(*token)++;
	return ret;
}

node_t* GetNumber(vec_t* vec, int* token) 
{
	node_t* ret = CreateNode(TOKEN_VAL, NUM);	
	(*token)++;
	return ret;
}
