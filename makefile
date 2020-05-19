all:
	gcc -Wall -Wextra 4transl.c diff.c main.c tree.c -lm
	#./a.out <input_file_name> to make out.tex
	#then complie out.tex in any tex editor to make pdf
clean:
	rm *.dvi *.log *.gz a.out *.aux *.pdf *.tex
texmaker:
	./a.out input
	texmaker out.tex
