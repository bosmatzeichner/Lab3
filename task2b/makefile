#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task2b

# Tool invocations
# Executable "main[1]" depends on the files main[1].o 
task2b: main.o toy_printf.o
	gcc -g -Wall -o task2b main.o toy_printf.o

# Depends on the source and header files
main.o: main.c 
	gcc -g -Wall -c -o main.o main.c
toy_printf.o: toy_printf.c 
	gcc -g -Wall -c -o toy_printf.o toy_printf.c
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task2b