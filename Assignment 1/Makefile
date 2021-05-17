# Do not edit the contents of this file.
all: warnings_OK

warnings_BAD: *.c
	gcc -Werror -Wall -g -std=gnu99 -o processor_management *.c -lrt
	
warnings_OK: *.c
	gcc -Wall -g -std=gnu99 -o processor_management *.c -lrt
	
clean: *.c
	rm processor_management 
