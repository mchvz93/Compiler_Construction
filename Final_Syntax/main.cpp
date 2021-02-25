//
//  main.cpp
//  Final_Syntax
//
//

#include <iostream>
#include <fstream>
#include <vector>

#include "Lexer.h"
#include "Syntax.h"
#include "Symbol_Table.h"




int main(int argc, const char * argv[]) {
    std::cout << "============Initializing Lexical/Syntax Analyzer Program================\n";
    std::ifstream inFile;
    std::string inputFilename;
    std::vector<Token> tokens;
    bool accepted = true;
    
    Lex lex;
    TDPP syn;
//    Sym_Table table;

    std::cout << "Enter your text file name to read: ";
    std::getline(std::cin, inputFilename);
    inFile.open(inputFilename.c_str());


    if (inFile.fail()) {
        std::cerr << "[ERROR] File: " << inputFilename << " could not be opened. Terminating program." <<std::endl;
        exit(1);
    }

    
    std::cout << std::endl;
    std::cout.setf(std::ios::left, std::ios::adjustfield);
    std::cout << std::setfill('=') << std::setw(50) << " " << std::setfill(' ') << std::endl;
    std::cout << ' ' << std::setw(21) << "Token " << "Lexeme " << std::endl;
    std::cout << std::setfill('=') << std::setw(50) << " " << std::setfill(' ') << std::endl;
        
    //     Read file contents as one string.
    std::string str((std::istreambuf_iterator<char>(inFile)),
                (std::istreambuf_iterator<char>()));

    tokens = lex.Lexer(str);
    

    if (!syn.Parser(str, tokens))
        accepted = false;

    if (accepted)
        std::cout << "String Accepted." << std::endl;
    else
        std::cout << "String Rejected." << std::endl;

    inFile.close();

    return 0;
}

