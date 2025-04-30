CC = gcc
CFLAGS = -Wall -g

OBJS = main.o lexer.o parser.o

compiler: $(OBJS)
	$(CC) $(CFLAGS) -o compiler $(OBJS)

main.o: main.c compiler.h
lexer.o: lexer.c compiler.h
parser.o: parser.c compiler.h

clean:
	rm -f *.o compiler
