#include "diff.h"

int Simplify(tree_t* tree)
{
	assert(tree != NULL);
	node_t* ex_head = tree -> head;
	tree -> head = SimplifyNode(tree -> head);
	if (ex_head == tree -> head)
		return 0;
	return -1;
}

node_t* SimplifyPLUS(node_t* node)
{
	if (LEFT_TYPE == NUM && RIGHT_TYPE == NUM) {
		node_t* new = CreateNode(LEFT_VAL + RIGHT_VAL, NUM);
		new -> parent = node -> parent;				
		SubTreeDestroy(node);
		return new;
	}
	if (LEFT_TYPE == NUM && LEFT_VAL == 0) {
		node_t* ex = node;
		node = CopySubTree(node -> right);
		SubTreeDestroy(ex);
		return node;
	}
	if (RIGHT_TYPE == NUM && RIGHT_VAL == 0) {
		node_t* ex = node;
		node = CopySubTree(node -> left);
		SubTreeDestroy(ex);
		return node;
	}
	return node;	
}

node_t* SimplifyMINUS(node_t* node) 
{
	if (LEFT_TYPE == NUM && RIGHT_TYPE == NUM) {
		node_t* new = CreateNode(LEFT_VAL - RIGHT_VAL, NUM);
		new -> parent = node -> parent;				
		SubTreeDestroy(node);
		return new;
	}
	if (LEFT_TYPE == NUM && LEFT_VAL == 0) {
		node_t* ex = node;
		node = CopySubTree(node -> right);
		SubTreeDestroy(ex);
		return node;
	}
	if (RIGHT_TYPE == NUM && RIGHT_VAL == 0) {
		node_t* ex = node;
		node = CopySubTree(node -> left);
		SubTreeDestroy(ex);
		return node;
	}
	return node;	
}

node_t* SimplifyDIV(node_t* node) 
{
	if (LEFT_TYPE == NUM && RIGHT_TYPE == NUM) {
		node_t* new = CreateNode(LEFT_VAL / RIGHT_VAL, NUM);
		new -> parent = node -> parent;				
		SubTreeDestroy(node);
		return new;
	}
	if (LEFT_TYPE == NUM && LEFT_VAL == 0) {
		node_t* new = CreateNode(0, NUM);
		new -> parent = node -> parent;
		SubTreeDestroy(node);
		return new;
	}
	return node;
}

node_t* SimplifyMUL(node_t* node) 
{
	if (LEFT_TYPE == NUM && RIGHT_TYPE == NUM) {
		node_t* new = CreateNode(LEFT_VAL * RIGHT_VAL, NUM);
		new -> parent = node -> parent;				
		SubTreeDestroy(node);
		return new;
	}
	if ((LEFT_TYPE == NUM && RIGHT_TYPE == VAR && LEFT_VAL == 0) ||
	     (RIGHT_TYPE == NUM && LEFT_TYPE == VAR && RIGHT_VAL == 0)) {
		node_t* new = CreateNode(0, NUM);
		new -> parent = node -> parent;				
		SubTreeDestroy(node);
		return new;
	}
	if (LEFT_TYPE == NUM && LEFT_VAL == 0) {
		node_t* new = CreateNode(0, NUM);
		new -> parent = node -> parent;
		SubTreeDestroy(node);
		return new;
	}
	if (RIGHT_TYPE == NUM && RIGHT_VAL == 0) {
		node_t* new = CreateNode(0, NUM);
		new -> parent = node -> parent;
		SubTreeDestroy(node);
		return new;
	}
	if (LEFT_TYPE == NUM && LEFT_VAL == 1) {
		node_t* ex = node;
		node = CopySubTree(node -> right);
		SubTreeDestroy(ex);
		return node;
	}
	if (RIGHT_TYPE == NUM && RIGHT_VAL == 1) {
		node_t* ex = node;
		node = CopySubTree(node -> left);
		SubTreeDestroy(ex);
		return node;
	}
	return node;	
}

node_t* SimplifySIN(node_t* node) 
{
	if (LEFT_TYPE == NUM) {
		node_t* ex = node;
		node_t* new = CreateNode(sin(node -> left -> val), NUM); 
		new -> parent = node -> parent;
		SubTreeDestroy(ex);
		return new;
	}
	return node;
		
}
node_t* SimplifyCOS(node_t* node) 
{
	if (LEFT_TYPE == NUM) {
		node_t* ex = node;
		node_t* new = CreateNode(cos(node -> left -> val), NUM); 
		new -> parent = node -> parent;
		SubTreeDestroy(ex);
		return new;
	}
	return node;

}
node_t* SimplifyLN(node_t* node) 
{
	if (LEFT_TYPE == NUM) {
		node_t* ex = node;
		node_t* new = CreateNode(log(node -> left -> val), NUM); 
		new -> parent = node -> parent;
		SubTreeDestroy(ex);
		return new;
	}
	return node;

}

node_t* SimplifyTAN(node_t* node) 
{
	if (LEFT_TYPE == NUM) {
		node_t* ex = node;
		node_t* new = CreateNode(tan(node -> left -> val), NUM);
		new -> parent = node -> parent;
		SubTreeDestroy(ex);
		return new;
	}
	return node;
}

node_t* SimplifyEXP(node_t* node) 
{
	if (LEFT_TYPE == NUM) {
		node_t* ex = node;
		node_t* new = CreateNode(exp(node -> left -> val), NUM); 
		new -> parent = node -> parent;
		SubTreeDestroy(ex);
		return new;
	}
	return node;
}

node_t* SimplifyNode(node_t* node)
{
	if (node == NULL)
		return NULL;
	if (TYPE == OPER) {
		node -> left = SimplifyNode(node -> left);
		node -> right = SimplifyNode(node -> right);
		
		#define OPERATOR(name, str, act)             \
			if(VAL == OP_##name) {               \
				return Simplify##name(node); \
			}
		#include "NotationRules/list_operators"
		#undef OPERATOR
	}
	
	return node;
}

tree_t* Differ(tree_t* tree)
{
	assert(tree != NULL);
	tree_t* new = TreeCreate();
	assert(new != NULL);
	new -> head = DifferNode(tree -> head);	
	/*
	TreeDump(tree);
	printf("\n-------------------------------\n");
	TreeDump(new);
	*/
	return new;
}

node_t* DifferNode(node_t* node)
{
	assert(node != NULL);

	if (TYPE == NUM) 
		return CreateNodeLR(0, NUM, NULL, NULL);	
	if (TYPE == VAR)
		return CreateNodeLR(1, NUM, NULL, NULL);
	if (TYPE == OPER) {
		#define OPERATOR(name, str, act) \
			if(VAL == OP_##name) {   \
				return act;      \
			}
		#include "NotationRules/list_operators"
		#undef OPERATOR
	}
	return NULL;
}

int PrintTex(tree_t* tree, FILE* out)
{
	assert(tree != NULL);
	fprintf(out, "\\documentclass[20pt,a4paper]{article}\n");
	fprintf(out, "\\begin {document}\n");
	fprintf(out, "$");
	PrintDiffered(tree -> head, 0, stdout);	
	PrintDiffered(tree -> head, 0, out);
	fprintf(out, "$");
	fprintf(out, "\n\\end {document}\n");
	return 0;
}

#define NOT_FUNC (VAL != OP_SIN && VAL != OP_COS  \
		 && VAL != OP_LN && VAL != OP_EXP \
		 && VAL != OP_TAN)

int PrintDiffered(node_t* node, int bracket, FILE* out) 
{
	assert(out != NULL);
	if (node == NULL)
	return 0;

	if (TYPE == NUM) {
		if (VAL < 0)
			fprintf(out, "(%.2f)", VAL);
		else
			fprintf(out, "%.2f", VAL);

	}

	else if (TYPE == VAR) {
		if (VAL == 0)
			fprintf(out, "x");
	}

	else if (TYPE == OPER) {
		if (NOT_FUNC) {
			if (VAL == OP_PLUS) {	
				if (bracket) 
					fprintf (out, "(");
	                        PrintDiffered(node -> left, 0, out);
                        	fprintf(out, " + ");
	                        PrintDiffered(node -> right, 0, out);
				if (bracket) 
					fprintf (out , ")");
			}	
			if (VAL == OP_MINUS) {	
				if (bracket) 
					fprintf (out, "(");
	                        PrintDiffered(node -> left, 0, out);
                        	fprintf(out, " - ");
	                        PrintDiffered(node -> right, 0, out);
				if (bracket) 
					fprintf (out, ")");
			}	
			if (VAL == OP_MUL) {	
	                        PrintDiffered(node -> left, 1, out);
                        	fprintf(out, " \\cdot ");
	                        PrintDiffered(node -> right, 1, out);
			}	
			if (VAL == OP_DIV) {	
				fprintf(out, "\\frac {");
	                        PrintDiffered(node -> left, 1, out);
				fprintf(out, "}{");
	                        PrintDiffered(node -> right, 1, out);
				fprintf(out, "}");
			}	
		}
		else {
                	if (VAL == OP_SIN) {

                        	fprintf(out, "sin( ");
	                        PrintDiffered(node -> left, 0, out);
				fprintf(out, ")");
	                }
	                if (VAL == OP_COS) {
	                        fprintf(out, "cos( ");
	                        PrintDiffered(node -> left, 0, out);
				fprintf(out, ")");
	                }
	                if (VAL == OP_EXP) {
	                        fprintf(out, "e^{ ");
	                        PrintDiffered(node -> left, 0, out);
				fprintf(out, "}");
	                }
        	        if (VAL == OP_LN) {
                	        fprintf(out, "ln( ");
	                        PrintDiffered(node -> left, 0, out);
				fprintf(out, ")");
	                }
	                if (VAL == OP_TAN) {
	                        fprintf(out, "tan( ");
	                        PrintDiffered(node -> left, 0, out);
				fprintf(out, ")");
	                }

		}
	}
	return 1;
}

/*
int PrintDiffered(node_t* node, int bracket, FILE* out)
{
	assert(out != NULL);
	if (node == NULL)
		return 0;
	if (bracket == 1)
		fprintf(out, "(");
	if (TYPE == OPER && NOT_FUNC) {
		if (VAL == OP_MUL) {	
			if (LEFT_TYPE != VAR && LEFT_TYPE != NUM)
				PrintDiffered(node -> left, 0, out);
			else
				PrintDiffered(node -> left, 0, out);
		}
		else if (VAL == OP_DIV) {
			fprintf(out, "\\frac {");
			PrintDiffered(node -> left, 0, out);
			fprintf(out, "}");
		}
		else 
			PrintDiffered(node -> left, 0, out);
	}	
	if (TYPE == NUM) {
		if (VAL < 0) 
			fprintf(out, "(%.0f)", VAL);
		else 
			fprintf(out, "%.0f", VAL);
	}
	if (TYPE == VAR) {
		if (VAL == 0)
			fprintf(out, "x");
		else 
			fprintf(out, "y");
	}
	if (TYPE == OPER) {
		if (VAL == OP_PLUS)
			fprintf(out, " + ");
		if (VAL == OP_MINUS)
			fprintf(out, " - ");
		if (VAL == OP_MUL)
			fprintf(out, " \\cdot ");
		if (VAL == OP_DIV)
			//fprintf(out, "");
		if (VAL == OP_SIN) {
			fprintf(out, "sin ");
			PrintDiffered(node -> left, 1, out);
		}
		if (VAL == OP_COS) {
			fprintf(out, "cos ");
			PrintDiffered(node -> left, 1, out);
		}
		if (VAL == OP_EXP) {
			fprintf(out, "e^{ ");
			PrintDiffered(node -> left, 2, out);
		}
		if (VAL == OP_LN) {
			fprintf(out, "ln ");
			PrintDiffered(node -> left, 1, out);
		}
		if (VAL == OP_TAN) {
			fprintf(out, "tan ");
			PrintDiffered(node -> left, 1, out);
		}
	}
	if (TYPE == OPER && VAL == OP_MUL) {
		if (RIGHT_TYPE != VAR && RIGHT_TYPE != NUM && !(NOT_FUNC))
			PrintDiffered(node -> right, 0, out);
		else
			PrintDiffered(node -> right, 0, out);
	}
	else if (TYPE == OPER && VAL == OP_DIV) {
		fprintf(out, "{");
		PrintDiffered(node -> right, 0, out);
		fprintf(out, "}");
	}
	else if (TYPE == OPER)
		PrintDiffered(node -> right, 0, out);
	if (bracket == 1)
		fprintf(out, ")");
	if (bracket == 2)
		fprintf(out, "}");
	return 0;
}
*/
#undef NOT_FUNC
