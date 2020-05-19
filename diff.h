#ifndef _DIFF_H
#define _DIFF_H

#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define VAL        (node -> val)
#define TYPE       (node -> type)
#define LEFT_TYPE  (node -> left -> type)
#define LEFT_VAL   (node -> left -> val)
#define RIGHT_TYPE (node -> right -> type)
#define RIGHT_VAL  (node -> right -> val)

#define dL DifferNode  (node -> left)
#define dR DifferNode  (node -> right)
#define cL CopySubTree (node -> left)
#define cR CopySubTree (node -> right)

#define _ADD(left, right)   CreateNodeLR(OP_PLUS, OPER, left, right)
#define _MINUS(left, right) CreateNodeLR(OP_MINUS, OPER, left, right)
#define _MUL(left, right)   CreateNodeLR(OP_MUL, OPER, left, right)
#define _DIV(left, right)   CreateNodeLR(OP_DIV, OPER, left, right)
#define _COS(left)          CreateNodeLR(OP_COS, OPER, left, NULL)
#define _SIN(left)          CreateNodeLR(OP_SIN, OPER, left, NULL)
#define _EXP(left)          CreateNodeLR(OP_EXP, OPER, left, NULL)
#define _NEGATIVE           CreateNode(-1, NUM)


int Simplify(tree_t*);

#define OPERATOR(name, str, act)        \
	node_t* Simplify##name(node_t*);
#include "NotationRules/list_operators"
#undef OPERATOR

node_t* SimplifyNode(node_t*);

tree_t* Differ        (tree_t*);
node_t* DifferNode    (node_t*);
int     PrintDiffered (node_t*, int, FILE*);
int     PrintTex      (tree_t*, FILE*);

#endif //_DIFF_H
