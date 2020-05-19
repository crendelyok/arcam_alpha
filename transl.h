#ifndef _TRANSL_H
#define _TRANSL_H

#include "tree.h"
#include "cvector.h"

#define MAX_LITERALS 256

#define TOKEN_TYPE (((vec -> buf)[*token]).type)
#define TOKEN_OPER (((vec -> buf)[*token]).token_oper)
#define TOKEN_SYNT (((vec -> buf)[*token]).token_synt)
#define TOKEN_VAR  (((vec -> buf)[*token]).token_var)
#define TOKEN_VAL  (((vec -> buf)[*token]).token_val)

typedef struct token_t token_t;
typedef vector_define(token_t) vec_t;

struct token_t
{
        enum node_types_enum type;
        union {
                double token_val;
                enum token_oper {
                        #define OPERATOR(name, str, act) TOKEN_OPER_##name, 
                        #include "NotationRules/list_operators"
                        #undef OPERATOR
                }token_oper;
                enum token_var {
                        #define VARIABLE(name) TOKEN_VAR_##name, 
                        #include "NotationRules/list_variables"
                        #undef VARIABLE
                }token_var;
                enum token_synt {
                        #define SYNTAX(name, symbol) TOKEN_SYNT_##name,  
                        #include "NotationRules/list_syntax"   
                        #undef SYNTAX
                }token_synt;
        };

};


tree_t* GetTreeByFile (const char* file_name);
node_t* GetExpression (vec_t*, int*);
node_t* GetNumber     (vec_t*, int*);
node_t* GetVar        (vec_t*, int*);
node_t* GetSum        (vec_t*, int*);
node_t* GetMul        (vec_t*, int*);
node_t* GetBrackets   (vec_t*, int*);
node_t* GetFunc       (vec_t*, int*);

enum node_types_enum GetLiterals(vec_t*, char**); 

#endif //_TRANSL_H

