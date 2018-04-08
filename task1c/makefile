#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task1c

# Tool invocations
# Executable "main[1]" depends on the files main[1].o 
task1c: addresses.o
	gcc -g -Wall -o task1c addresses.o

addresses.o: addresses.c
	gcc -g -Wall -c -o addresses.o addresses.c
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1c