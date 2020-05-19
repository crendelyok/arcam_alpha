#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "transl.h"
#include "diff.h"

#define OUTPUT_FILE "out.tex"

void PrintInput(tree_t* tree, FILE* out)
{
	fprintf(out, "\\documentclass[25pt, a4paper]{article}\n");
	fprintf(out, "\\begin {document}\n");
	fprintf(out, "\\section*{Utrem nos Stevenu Wolframu!}\n\n");
	fprintf(out, "Input equation:\\\\\\\\\n\n");
	fprintf(out, "$");
	PrintDiffered(tree -> head, 0, out);
	fprintf(out, "$");
}

void PrintAnswer(tree_t* tree, FILE* out) 
{
	fprintf(out, "\\\\\\\\\\\\\nAnswer is:\\\\\\\\\n\n");
	fprintf(out, "$");
	PrintDiffered(tree -> head, 0, out);
	fprintf(out, "$\n");	
}

void PrintDerivate(tree_t* tree, FILE* out)
{
	fprintf(out, "\\\\\\\\Derivating:\\\\\n");
	fprintf(out, "$");
	PrintDiffered(tree -> head, 0, out);
	fprintf(out, "$\n");
}

int Derivate(const char* input)
{
	tree_t* tree = GetTreeByFile(input);
	if (tree == NULL) {
		printf("Can't open or read an input file\n");
		return -1;
	}

	FILE* out = fopen(OUTPUT_FILE, "w");

	PrintInput(tree, out);

	Simplify(tree);	

	tree_t* dif = Differ(tree);
	assert(dif != NULL);

	Simplify(dif);
	PrintAnswer(dif, out);


	fprintf(out, "\\end{document}\n");

	fclose(out);
	TreeDestroy(dif);
	TreeDestroy(tree);
	return 0;	
	
}


//returns 0  on success
//returns -1 if error appears
int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("Incorrect number of arguments\n");
		return -1;	
	}
	return Derivate(argv[1]);
}
