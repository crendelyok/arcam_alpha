#ifndef _TREE_H
#define _TREE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef double      val_t;
typedef struct node node_t;
typedef int         node_type_t;
typedef struct tree tree_t;

struct node
{
	node_t*     left;
	node_t*     right;
	node_t*     parent;
	val_t       val; 
	node_type_t type;
};

struct tree
{
	node_t* head;
};

//Types of tokens
enum node_types_enum
{
	UNDEFINED_TYPE = 0,
	NUM,
	OPER,
	VAR,
	SYNT
};


//enum of OPER type
//e.g. PLUS, MINUS, TAN, EXP etc.
enum val_type_op_t
{
	#define OPERATOR(name, string, act) OP_##name,
	#include "NotationRules/list_operators"
	#undef OPERATOR
};


tree_t* TreeCreate     ();
int     TreeDestroy    (tree_t*);
int     SubTreeDestroy (node_t*);
int     NodeDestroy    (node_t*);
node_t* AddNodeLeft    (node_t*, val_t, node_type_t);
node_t* CreateNode     (val_t, node_type_t);
node_t* CreateNodeLR   (val_t, node_type_t, node_t*, node_t*);
int     VisitorNLR     (node_t*, int (*func)(node_t*));
int     VisitorLRN     (node_t*, int (*func)(node_t*));
node_t* CopySubTree    (node_t*);
int     PrintNode      (node_t*);
void    TreeDump       (tree_t*);

#endif //_TREE_H
