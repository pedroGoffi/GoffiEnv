cflags = -Wall -Wextra \
         -lstdc++ \
	 -Wswitch-enum \
	 -Wconversion -Wno-missing-braces \
	 -pedantic -fno-strict-aliasing \
	 -ggdb -std=c++2b -g
cc = gcc-11
output = debug-app
input = ./src/main.cpp 


compile:
	${cc} ${input} ${cflags} -o ${output} 

