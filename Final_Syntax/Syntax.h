//
//  Syntax.h
//  Final_Syntax
//
//

#ifndef Syntax_h
#define Syntax_h

#include <stack>
#include <iomanip>
#include <fstream>

#include "Lexer.h"
#include "Symbol_Table.h"

/*UPDATE from previous submissions**:
   // else production now functional
   // issues with comments are fixed
   // float declaration production now functional (multiple and single)
   // corrected output: notifies user if string is fully parsed/accepted or not
   // created class for Symbol Table (not integrated as of 12/15/20)
   // Symbol Table is printed using vector
   // New functions -> printTable/printTableError: print the existing symbol table.
*/

//initializing TDPP
class TDPP;

class Pred_Table {
    friend TDPP;
protected:

    /* Production Rules:
        S -> A | W | D | I | B \0           // Statement production
        A -> id=E                           // Assignment production
        W -> while(C){S}S                   // While Statement production
        I -> if(C){S}S                      // If Statement production
        B -> else{S}S                       // Else Statement production
        C -> EP                             // epsilon for terminals
        P -> <E | >E                        // handles epsilon cases using '{', '}' for statements
        D -> intidJ | boolidK | floatidL    // declarative statement production (int, bool, float)
        J -> ,idJ | =numJ | ;S              // handles int declaration
        K -> ,idK | =booleanK | ;S          // handles bool declaration
        L -> ,idL | =numL | ;S              // handles float declaration
        E -> TQ                             // Expression production
        Q -> +TQ  | -TQ | \0                // Expression Prime production
        T -> FR                             // Term production
        R ->  *FR | /FR | \0                // Term Prime production
        F ->  idZ | numZ | (E)Z             // Factor production
        Z -> ;S | \0                        // Factor Prime production
    */

std::vector<std::vector<std::string>> table = {
    { "NIL", "id", "num", "boolean", "+", "-", "*", "/", "=", "(", ")", ";", "while", "<", ">", "{", "}", "int", "bool", "float", ",", "if", "else", "$" },
    { "S","A","NIL","NIL","NIL","NIL","NIL","NIL", "NIL", "NIL", "NIL", "NIL", "W", "NIL", "NIL", "NIL", "\0","D","D","D","NIL", "I", "B", "\0" },
    { "A", "id=E", "NIL", "NIL", "NIL", "NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL", "NIL","NIL","NIL","NIL","NIL","NIL" },
    { "W","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","while(C){S}S","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL" },
    { "I","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL", "if(C){S}S","NIL","NIL" },
    { "B","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL", "NIL","else{S}S","NIL" },
    { "C","EP","EP","NIL","NIL","NIL","NIL","NIL","NIL","EP","NIL","NIL","NIL","EP","EP","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL" },
    { "P","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","<E",">E","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL" },
    { "D","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","intidJ", "boolidK","floatidL", "NIL","NIL","NIL","NIL" },
    { "J","NIL","NIL","NIL","NIL","NIL","NIL","NIL","=numJ","NIL","NIL",";S","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL",",idJ","NIL","NIL","NIL" },
    { "K","NIL","NIL","NIL","NIL","NIL","NIL","NIL","=booleanK","NIL","NIL",";S","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL",",idK","NIL","NIL","NIL" },
    { "L","NIL","NIL","NIL","NIL","NIL","NIL","NIL","=numL","NIL","NIL",";S","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL",",idL","NIL","NIL","NIL" },
    { "E","TQ","TQ","NIL","NIL","NIL","NIL","NIL","NIL","TQ","NIL","NIL","NIL", "NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL"},
    { "Q","NIL","NIL","NIL","+TQ","-TQ","NIL","NIL","NIL","NIL","\0","NIL","NIL","\0","\0","NIL","\0","NIL","NIL","NIL","NIL", "NIL","NIL","\0" },
    { "T","FR","FR","NIL","NIL","NIL","NIL","NIL","NIL","FR","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL"},
    { "R","NIL","NIL","NIL","\0","\0","*FR","/FR","NIL","NIL","\0","NIL","NIL","\0","\0","NIL","\0","NIL","NIL","NIL","NIL","NIL","NIL","\0" },
    { "F","idZ","numZ","NIL","NIL","NIL","NIL","NIL","NIL","(E)Z","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL","NIL" },
    { "Z","NIL","NIL","NIL","\0","\0","\0","\0","NIL","NIL","\0",";S","NIL","\0","\0","NIL","\0","NIL","NIL","NIL","NIL","NIL","NIL","\0" }
    };
};

class TDPP : public Lex {
public:
    bool Parser(std::string, std::vector<Token> tokens);
    
private:
    Pred_Table Pred;
    Sym_Table Symbol_Table;                             // class producing a hash table as Symbol table... trying to make it more efficient than current vector
    std::ofstream outfile;             
    std::stack<std::string> stk;
    std::vector<std::string> initialized;               // vector as a symbol table
    std::string str;                                    // incoming string
    
    int index = 0;
    bool inDec = false;                                 // returns true when in Declarative Statement
    
    bool isTerminal(std::string);                       // returns true/false if string terminals
    bool isOperator(std::string currChar);              // returns true/false if string is an operator
    bool isSeparator(std::string currChar);             // returns true/false if string is a separator
    bool isDigit(std::string currChar);                 // returns true/false if string is a number
    bool isSpace(std::string currChar);                 // returns true/false if string has space
    
    bool isDuplicate(std::string);                      /* returns true/false if incoming string/variable is declared
                                                           uses vector<std::string>initialized as symbol table
                                                        */
    
    void printTable(std::vector<std::string> initialized);          // prints symbol table.
    void printTableError(std::vector<std::string> initialized);     // prints symbol table.
    
    int getRow(std::string);                            // returns int value derived from Pred_Table
    int getCol(std::pair<std::string, std::string>);    // returns int value derived from Pred_Table
    void Print_Prod(std::string, std::string);          // prints production derived from getRow/getCol

};

#endif /* Syntax_h */
