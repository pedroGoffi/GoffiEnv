#ifndef MAIN
#define MAIN 

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include "./frontend/Tools.h"
#include "./backend/goffi.cpp"
#include "./backend/Lexer.cpp"
#include "./backend/Parser.cpp"
#include "./backend/TypeCheck.cpp"

#define SHIFT shift(&argc, &argv)
int main(int argc, char** argv){
    std::string const program         = SHIFT;
    std::string inputFilePath;
    std::string outputFilePath        = "./output.gfsl";
    std::string flag;
    bool unsafe_mode = false;
    while(argc > 0){
        flag = SHIFT;

        if(flag == "-o"){
            if (argc <= 0){
                usage(stderr, program);
                fprintf(stderr, "Error: Flag -o has no uwu");
            }
            outputFilePath = SHIFT;
	} else if (flag == "-h"){
            usage(stdout, program);
            exit(0);	    
        } else if (flag == "--unsafe"){
	  unsafe_mode = true;
	} else{
	  inputFilePath = flag;
	}
    }
    if(inputFilePath.length()  == 0){
        usage(stderr, program);
        fprintf(stderr, "Error: No input file path was provided\n");
        exit(1);
    }
    std::fstream INPUT_FILE(inputFilePath, std::ios::in);
    assert(INPUT_FILE.is_open() && "Error: Could not open the file\n");


    std::vector<Token>  tokens = Lexer::lex(INPUT_FILE);
    Crossreference::analyze(tokens);
    std::vector<std::pair<VR, Token>>     instructions = Parser::parse(tokens);

    if(!unsafe_mode)
      type_checking_walk(instructions);
    Goffi::compile_program(instructions, outputFilePath);

    return 0;

}
#endif /* ifndef MAIN */
