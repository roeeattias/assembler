CFLAGS = -Wall -ansi -pedantic
SRCS := $(wildcard source/*.c)
OBJS := $(SRCS:.c=.o)  # Replace .c with .o for object files

assembler: $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o assembler

# Compile each C file separately
%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm source/assembler.o source/macroTable.o source/state.o source/preProcessor.o source/symbolTable.o source/firstTransition.o source/utils.o source/opcode.o source/memory.o source/secondTransition.o source/exportFiles.o