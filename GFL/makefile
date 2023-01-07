cflags = -lstdc++				\
	 -Wconversion -Wno-missing-braces	\
	 -pedantic -fno-strict-aliasing		\
	 -ggdb -std=c++2b -g

cc = gcc-11
output = debug-app
input = ./main.cpp

compile:
	@echo "[0] Compiling gfl compiler"
	${cc} ${input} ${cflags} -o ${output}

