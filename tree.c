#include "tree.h"

tree_t* TreeCreate() 
{
	tree_t* tree = calloc(1, sizeof(tree_t));
	if (tree == NULL)
		return NULL;
	return tree;	
}

int SubTreeDestroy(node_t* node) 
{
	assert(node != NULL);
	return VisitorLRN(node, NodeDestroy);	
}

int TreeDestroy(tree_t* tree) 
{
	assert(tree != NULL);
	int ret = SubTreeDestroy(tree -> head);	
	free(tree);
	return ret;
}

int NodeDestroy(node_t* node) 
{
	if (node == NULL) 
		return -1;
	free(node);
	return 0;
}

node_t* CreateNode(val_t val, node_type_t type)
{
	node_t* new_node = calloc(1, sizeof(node_t));
	new_node -> val  = val;
	new_node -> type = type;
	return new_node;
}

node_t* CreateNodeLR(val_t val, node_type_t type, node_t* left, node_t* right)
{
	node_t* new_node  = calloc(1, sizeof(node_t));
	new_node -> val   = val;
	new_node -> type  = type;
	new_node -> left  = left;
	new_node -> right = right;
	return new_node;
}

node_t* AddNodeLeft(node_t* node, val_t val, node_type_t type)
{
	node_t* new_node = calloc(1, sizeof(node_t));
	new_node -> parent = node;
	new_node -> val    = val;
	new_node -> type   = type;
	return new_node;	
}

int VisitorNLR(node_t* node, int (*func)(node_t*))
{
	if (node == NULL) 
		return -1;
	if (func(node) != 0)
		return -1;	
	if (node -> left != NULL)
		if (VisitorNLR(node -> left, func) != 0)
			return -1;
	if (node -> right != NULL)
		if (VisitorNLR(node -> right, func) != 0)
			return -1;
	return 0;
}

int VisitorLRN(node_t* node, int (*func)(node_t*))
{
	if (node == NULL) 
		return -1;
	if (node -> left != NULL)
		if (VisitorLRN(node -> left, func) != 0)
			return -1;
	if (node -> right != NULL)
		if (VisitorLRN(node -> right, func) != 0)
			return -1;
	return func(node);
}

node_t* CopySubTree(node_t* node) 
{
	if (node == NULL)
		return NULL;	
	node_t* new  = CreateNode(node -> val, node -> type);
	new -> left  = CopySubTree(node -> left);
	new -> right = CopySubTree(node -> right);
	if (new -> left != NULL)
		new -> left -> parent = new;
	if (new -> right != NULL)
		new -> right -> parent = new;
	
	return new;
}

int PrintNode(node_t* node)
{
	assert(node != NULL);
	printf("\n------------\n");
	printf("*node = %p\n", node);
	printf("*node -> parent = %p\n", node -> parent);
	if (node -> type == NUM)
		printf("node -> type = NUM\n");
	if (node -> type == OPER)
		printf("node -> type = OPER\n");
	if (node -> type == VAR)
		printf("node -> type = VAR\n");
	if (node -> type == NUM)
		printf("node -> val = %f\n", node -> val);
	
	if (node -> type == OPER) {
		#define OPERATOR(name, str, act)                     \
			if (node -> val == OP_##name) {              \
				printf("node -> val = %s\nn", str); \
			}
			#include "NotationRules/list_operators"
		#undef OPERATOR
	}	
	
	printf("node -> left = %p\n", node -> left);
	printf("node -> right= %p\n", node -> right);
	printf("\n------------\n");
	return 0;
}

void TreeDump(tree_t* tree) 
{
	printf("\nTREE DUMP:\n");
	printf("\ntree\n{\n");
	printf("\ttree -> head = %p", tree -> head);
	printf("\n{\n");
	if (VisitorNLR(tree -> head, PrintNode) != 0)
		printf("TREE IS DAMAGED!!!!!!");
}
