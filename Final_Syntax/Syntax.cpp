//
//  Syntax.cpp
//  Final_Syntax
//
//
/* This code is based off: https://github.com/Krunk-Juice/Syntactic-Analyzer-Predictive-Parser/blob/master/Syntactic%20Analyzer/Syntactic%20Analyzer.h
 
 **UPDATE from previous submissions**:
    // else production now functional
    // issues with comments are fixed
    // float declaration production now functional
    // corrected output: notifies user if string is fully parsed/accepted or not
    // created class for Symbol Table (not integrated as of 12/15/20)
    // Symbol Table is printed using vector
    // New functions -> printTable/printTableError: print the existing symbol table.
    
 */

#include <iostream>
#include <stack>
#include <iomanip>
#include <fstream>
//#include <string>
#include "Lexer.h"
#include "Syntax.h"
#include "Symbol_Table.h"


//==================HELPER BOOL MEMBER FUNCTIONS================================================================
//checks for spaces in the string
bool TDPP::isSpace(std::string currChar) {
    if (currChar.size() == 1 && (currChar[0] == ' ' || currChar[0] == '\t' || currChar[0] == '\n'))
        return true;
    return false;
}

//checks to see if incoming string is initialized, adds to symbol table
bool TDPP::isDuplicate(std::string s) {
    if (initialized.empty()) {
        initialized.push_back(s);
        return false;
    }
    else {
        for (int i = 0; i < initialized.size(); i++) {
            if (s == initialized[i])
                return true;
        }
        initialized.push_back(s);
        return false;
    }
}

void TDPP::printTable(std::vector<std::string> initialized) {
    std::cout << std::endl;
    std::cout.setf (std::ios::left, std::ios::adjustfield);
    std::cout << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    std::cout << ' ' << std::setw(10) << "Symbol Table " << std::endl;
    std::cout << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;

    outfile << std::endl;
    outfile.setf (std::ios::left, std::ios::adjustfield);
    outfile << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    outfile << ' ' << std::setw(10) << "Symbol Table " << std::endl;
    outfile << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    
    for (size_t index = 0; index < initialized.size(); index++) {
        std::cout << ' ' << std::setw(12) << initialized[index] << &initialized[index] <<std::endl;
        outfile << ' ' << std::setw(12) << initialized[index] << &initialized[index] <<std::endl;
    }
    
    std::cout << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    outfile << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    std::cout << std::endl;
    outfile << std::endl;
}

void TDPP::printTableError(std::vector<std::string> initialized) {
    std::cout << std::endl;
    std::cout.setf (std::ios::left, std::ios::adjustfield);
    std::cout << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    std::cout << ' ' << std::setw(10) << "Symbol Table " << std::endl;
    std::cout << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;

    outfile << std::endl;
    outfile.setf (std::ios::left, std::ios::adjustfield);
    outfile << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    outfile << ' ' << std::setw(10) << "Symbol Table " << std::endl;
    outfile << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    
    for (size_t index = 0; index < initialized.size()-1; index++) {
        std::cout << ' ' << std::setw(12) << initialized[index] << &initialized[index] <<std::endl;
        outfile << ' ' << std::setw(12) << initialized[index] << &initialized[index] <<std::endl;
    }
    
    std::cout << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    outfile << std::setfill('=') << std::setw(27) << " " << std::setfill(' ') << std::endl;
    std::cout << std::endl;
    outfile << std::endl;
}

//checks to see if incoming string is an operator
bool TDPP::isOperator(std::string currChar) {
    char operators[9][3] = { "+", "-", "*", "/","%", ">", "<", "=" };
    char token_str[currChar.length()];
    for (size_t i = 0; i < currChar.length(); ++i) {
        token_str[i] = currChar[i];
    }
    
    for (size_t i = 0; i < 10; i++)
        if (std::strcmp(operators[i], token_str) == 0)
            return true;
        return false;
}

//checks to see if incoming string is a separator
bool TDPP::isSeparator(std::string currChar) {
    char separators[9][3] = { ";", ",", ":", "(", ")", "{", "}", "[", "]" };
    char token_str[currChar.length()];
    for (size_t i = 0; i < currChar.length(); ++i) {
        token_str[i] = currChar[i];
    }
    
    for (size_t i = 0; i < 10; i++)
        if (std::strcmp(separators[i], token_str) == 0)
            return true;
        return false;
}

/* Check if string is terminal. */
bool TDPP::isTerminal(std::string c) {                                                //from Lex
    if (c == "id" || c == "num" || c == "boolean" || isSeparator(c) || isOperator(c) || isKey(c) || isSpace(c))
        return true;
    return false;
}

//==================END OF HELPER MEMBER BOOL FUNCTIONS================================================================





/*====================================================================================================================
 bool Parser(std::string s, std::vector<Token> tokens):
 The heart of the program. The function recieves an incoming string and using the table from class Pred_Table,
 the string is checked against the the table.
 
 If the string is accepted it prints its appropriate rule otherwise this bool functions returns false and states
 the error in the parsing production.
 //====================================================================================================================
 */
bool TDPP::Parser(std::string s, std::vector<Token> tokens) {
    outfile.open("Syn_out.txt");
    
    outfile << "============Initializing Lexical/Syntax Analyzer Program================\n";
    outfile << std::endl;
    outfile.setf(std::ios::left, std::ios::adjustfield);
    outfile << std::setfill('=') << std::setw(55) << " " << std::setfill(' ') << std::endl;
    outfile << ' ' << std::setw(21) << "Token " << "Lexeme " << std::endl;
    outfile << std::setfill('=') << std::setw(55) << " " << std::setfill(' ') << std::endl;
    
    bool prod_check = true;
    str = s;
    str = str + '$';

    stk.push("$");

    // Adds an extra element to the end of the vector to prevent out of bounds.
    tokens.push_back(Token("FIN", "$"));



    /* If use S -> idA & A -> =E. Use stack_.push("S").*/
    stk.push("S");

    while (!stk.empty()) {

        /* Top of Stack */
        std::string t = stk.top();

        /* Current token being evaluated by the parser. */
        std::string this_lexeme = tokens[index].lexeme_;

        std::pair<std::string, std::string> tp (tokens[index].lexeme_, tokens[index].token_);
        

        // Print Token:      Lexeme:
        if (prod_check) {
            std::cout << "Token: " << std::left << std::setw(15) << tokens[index].token_ <<
            "Lexeme: " << tokens[index].lexeme_ << std::endl;
            
//            std::cout << "Token: "  << tokens[index].token_  << ' ' << std::setw(15) << "Lexeme: " << tokens[index].lexeme_   << std::endl;
//            outfile << ' ' << std::setw(15) << "Token: "  << tokens[index].token_<< "Lexeme: " << tokens[index].lexeme_  << std::endl;

            
            outfile << "Token: " << std::left << std::setw(15) << tokens[index].token_ <<
                "Lexeme: " << tokens[index].lexeme_ << std::endl;

            prod_check = false;
        }
        // Is the top of stack a terminal? Yes.
        else if (isTerminal(t)) {
            /* Variables in Declaration Statement */
            if ((t == "id" && tokens[index].token_ == "Identifier") && inDec) {
                /* Is the variable a duplicate? No. */
                if (!isDuplicate(this_lexeme)) {
                    inDec = false;
                    stk.pop();
                    index++;
                    prod_check = true;
                }
                /* Is the variable a duplicate? Yes. */
                else {
                    printTableError(initialized);
                    std::cout << "[ERROR]: " << "'" << this_lexeme << "' has already been declared." << std::endl;
                    outfile << "[ERROR]: " << "'" << this_lexeme << "' has already been declared." << std::endl;
                    break;
                }
            }
            /* Variables NOT in Declaration Statement */
            else if ((t == "id" && tokens[index].token_ == "Identifier") && !inDec) {
                /* Has the variable been declared yet? Yes. */
                if (isDuplicate(this_lexeme)) {
                    stk.pop();
                    index++;
                    prod_check = true;
                }
                /* Has the variable been declared yet? No. */
                else {
                    std::cout << "[ERROR]: " << "'" << this_lexeme << "' was not initialized." << std::endl;
                    std::cout << std::setfill('=') << std::setw(55) << " " << std::setfill(' ') << std::endl;
                    std::cout<< std::endl;
                    
                    outfile << "[ERROR]: " << "'" << this_lexeme << "' was not initialized." << std::endl;
                    outfile << std::setfill('=') << std::setw(55) << " " << std::setfill(' ') << std::endl;
                    outfile << std::endl;
                    
                    printTableError(initialized);
                    break;
                }
            }
            /* Handle numerical terminals. */
            else if (t == "num" && tokens[index].token_ == "Integer") {
                stk.pop();
                index++;
                prod_check = true;
            }
            /* Handle boolean terminals. */
            else if (t == "boolean" && (tokens[index].lexeme_  == "true" || tokens[index].lexeme_ == "false")) {
                stk.pop();
                index++;
                prod_check = true;
            }
            /* Handle remaining terminals. */
            else if (t == this_lexeme) {
                stk.pop();
                index++;
                prod_check = true;
            }

            else {
                std::cout<< "[ERROR]: "<< "Top of stack " << t << " != character input " << this_lexeme << std::endl;
                outfile << "[ERROR]: " << "Top of stack " << t << " != character input " << this_lexeme << std::endl;
                break;
            }
        }
        // Is the top of stack a terminal? No. Get corresponding production rule.
        else {
            int l = getRow(t);

            int k = getCol(tp);

            if (k == -1) {
                std::cout << "String rejected" << std::endl;
                std::cout << "At character: " << this_lexeme << std::endl;
                outfile << "String rejected" << std::endl;
                outfile << "At character: " << this_lexeme << std::endl;
                break;
            }
            
            if (k == 24) {
                printTable(initialized);
                std::cout << std::endl;
                std::cout << "Syntax program is Terminating." << std::endl;
                
                outfile << std::endl;
                outfile << "Syntax program is Terminating." << std::endl;
                exit(1);
            }

            /* Was an "NIL" cell read? No. */
            if (Pred.table[l][k] != "NIL") {
                stk.pop();

                Print_Prod(t, Pred.table[l][k]);

                /* Pushes strings in the table in reverse onto the stack. */
                if (Pred.table[l][k] == "id")
                    stk.push("id");
                else if (Pred.table[l][k] == "id=E") {
                    stk.push("E");
                    stk.push("=");
                    stk.push("id");
                }
                else if (Pred.table[l][k] == "while(C){S}S") {
                    stk.push("S");
                    stk.push("}");
                    stk.push("S");
                    stk.push("{");
                    stk.push(")");
                    stk.push("C");
                    stk.push("(");
                    stk.push("while");
                }
                else if (Pred.table[l][k] == "if(C){S}S") {
                    stk.push("S");
                    stk.push("}");
                    stk.push("S");
                    stk.push("{");
                    stk.push(")");
                    stk.push("C");
                    stk.push("(");
                    stk.push("if");
                }
                
                else if (Pred.table[l][k] == "else{S}S") {
                    stk.push("S");
                    stk.push("}");
                    stk.push("S");
                    stk.push("{");
                    stk.push("else");
                }

                else if (Pred.table[l][k] == "intidJ") {
                    stk.push("J");
                    stk.push("id");
                    stk.push("int");
                    inDec = true;
                }
                else if (Pred.table[l][k] == "boolidK") {
                    stk.push("K");
                    stk.push("id");
                    stk.push("bool");
                    inDec = true;
                }
                
                else if (Pred.table[l][k] == "floatidL") {
                    stk.push("L");
                    stk.push("id");
                    stk.push("float");
                    inDec = true;
                }
                                
                else if (Pred.table[l][k] == "=numJ") {
                    stk.push("J");
                    stk.push("num");
                    stk.push("=");
                    inDec = true;
                }
                else if (Pred.table[l][k] == ",idJ") {
                    stk.push("J");
                    stk.push("id");
                    stk.push(",");
                    inDec = true;
                }
                
                else if (Pred.table[l][k] == "=booleanK") {
                    stk.push("K");
                    stk.push("boolean");
                    stk.push("=");
                    inDec = true;
                }
                else if (Pred.table[l][k] == ",idK") {
                    stk.push("K");
                    stk.push("id");
                    stk.push(",");
                    inDec = true;
                }
                
                else if (Pred.table[l][k] == "=numL") {
                    stk.push("L");
                    stk.push("num");
                    stk.push("=");
                    inDec = true;
                }

                else if (Pred.table[l][k] == ",idL") {
                    stk.push("L");
                    stk.push("id");
                    stk.push(",");
                    inDec = true;
                }

                else if (Pred.table[l][k] == "idZ") {
                    stk.push("Z");
                    stk.push("id");
                }
                
                else if (Pred.table[l][k] == "numZ") {
                    stk.push("Z");
                    stk.push("num");
                }
                
                else {
                    for (int x = Pred.table[l][k].length() - 1; x >= 0; x--) {
                        stk.push(std::string(1, Pred.table[l][k][x]));
                    }
                }
            }
            //"NIL" cell read
            else {
                std::cout << "[ERROR]: Program has hit NIL cell at Production: '" << t << "' and Terminal: '" << Pred.table[0][k] << "'." << std::endl;
                outfile << "[ERROR]: Program has hit NIL cell at Production: '" << t << "' and Terminal: '" << Pred.table[0][k] << "'." << std::endl;
                break;
            }
        }
    }
    // Deletes "FIN", "$" stack marker.
    tokens.pop_back();

    if (stk.empty())
        return true;
    else
        return false;
}

//gets the row
int TDPP::getRow(std::string c) {
    if (c == "S")
        return 1;
    
    else if (c == "A")
        return 2;
    
    else if (c == "W")
        return 3;
    
    else if (c == "I")
        return 4;

    else if (c == "B")
        return 5;

    else if (c == "C")
        return 6;
    
    else if (c == "P")
        return 7;
    
    else if (c == "D")
        return 8;

    else if (c == "J")
        return 9;

    else if (c == "K")
        return 10;
    
    else if (c == "L")
        return 11;

    else if (c == "E")
        return 12;

    else if (c == "Q")
        return 13;

    else if (c == "T")
        return 14;

    else if (c == "R")
        return 15;

    else if (c == "F")
        return 16;

    else
        return 17;

}

// Gets table column
int TDPP::getCol(std::pair<std::string, std::string> c) {
    if (c.second == "Identifier")
        return 1;
    
    else if (c.second == "Real" || c.second == "Integer")
        return 2;
    
    else if (c.first == "true" || c.first == "false")
        return 3;
    
    else if (c.first == "+")
        return 4;
    
    else if (c.first == "-")
        return 5;
    
    else if (c.first == "*")
        return 6;
    
    else if (c.first == "/")
        return 7;
    
    else if (c.first == "=")
        return 8;
    
    else if (c.first == "(")
        return 9;
    
    else if (c.first == ")")
        return 10;
    
    else if (c.first == ";")
        return 11;
    
    else if (c.first == "while")
        return 12;
    
    else if (c.first == "<")
        return 13;
    
    else if (c.first == ">")
        return 14;
    
    else if (c.first == "{")
        return 15;
    
    else if (c.first == "}")
        return 16;
    
    else if (c.first == "int")
        return 17;
    
    else if (c.first == "bool")
        return 18;

    else if (c.first == "float")
        return 19;

    else if (c.first == ",")
        return 20;
    
    else if (c.first == "if")
        return 21;
    
    else if (c.first == "else")
        return 22;

    else if (c.first == "$")
        return 23;
    
    //this is not found on the table but allows for the program to terminate.
    else if (c.first == "FIN") {
        std::cout << "[SUCCESS]: End of file reached. String accepted." << std::endl;
        std::cout << std::setfill('=') << std::setw(55) << " " << std::setfill(' ') << std::endl;
        std::cout << std::endl;
        
        outfile << "[SUCCESS]: End of file reached. String accepted." << std::endl;
        outfile << std::setfill('=') << std::setw(55) << " " << std::setfill(' ') << std::endl;
        outfile << std::endl;

        return 24;
    }
    
    else
        return -1;
}

//production print
void TDPP::Print_Prod(std::string t, std::string s) {
    if (s == "A") {
        std::cout << "<Statement> -> <Assign>" << std::endl;
        std::cout << "<Assign> -> <Identifier> = <Expression>" << std::endl;
        std::cout << std::endl;
        outfile << "<Statement> -> <Assign>" << std::endl;
        outfile << "<Assign> -> <Identifier> = <Expression>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "W") {
        std::cout << "<Statement> -> <Loop>" << std::endl;
        std::cout << "<Loop> -> while ( <Conditional> ) { <Statement> }" << std::endl;
        std::cout << std::endl;
        outfile << "<Statement> -> <Loop>" << std::endl;
        outfile << "<Loop> -> while ( <Conditional> ) { <Statement> }" << std::endl;
        outfile << std::endl;
    }

    else if (s == "I") {
        std::cout << "<Statement> -> <Selection>" << std::endl;
        std::cout << std::endl;
        outfile << "<Statement> -> <Selection>" << std::endl;
        outfile << std::endl;
    }
    
    else if (s == "if(C){S}S") {
        std::cout << "<Selection> -> if ( <Conditional> ) { <Statement> }" << std::endl;
        std::cout << std::endl;
        outfile << "<Selection> -> if ( <Conditional> ) { <Statement> }" << std::endl;
        outfile << std::endl;
    }
    

    else if (s == "B") {
        std::cout << "<Statement> -> <Selection>" << std::endl;
        std::cout << std::endl;
        outfile << "<Statement> -> <Selection>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "else{S}S") {
        std::cout << "<Selection> -> else { <Statement> }  <Statement>" << std::endl;
        std::cout << std::endl;
        outfile << "<Selection> -> else { <Statement> }  <Statement>" << std::endl;
        outfile << std::endl;
    }


    else if (s == "EP") {
        std::cout << "<Conditional> -> <Expression> <Conditional Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Conditional> -> <Expression> <Conditional Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "<E") {
        std::cout << "<Conditional Prime> -> < <Expression>" << std::endl;
        std::cout << std::endl;
        outfile << "<Conditional Prime> -> < <Expression>" << std::endl;
        outfile << std::endl;
    }

    else if (s == ">E") {
        std::cout << "<Conditional Prime> -> > <Expression>" << std::endl;
        std::cout << std::endl;
        outfile << "<Conditional Prime> -> > <Expression>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "D") {
        std::cout << "<Statement> -> <Declaration>" << std::endl;
        std::cout << std::endl;
        outfile << "<Statement> -> <Declaration>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "intidJ") {
        std::cout << "<Declaration> -> int <Identifier> <Declaration> <Number>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration> -> int <Identifier> <Declaration> <Number>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "boolidK") {
        std::cout << "<Declaration> -> bool <Identifier> <Declaration Boolean>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration> -> bool <Identifier> <Declaration Boolean>" << std::endl;
        outfile << std::endl;
    }
    
    
    else if (s == "floatidL") {
        std::cout << "<Declaration> -> float <Identifier> <Declaration> <Number>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration> -> float <Identifier> <Declaration> <Number>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "=numJ") {
        std::cout << "<Declaration Number> -> = <Number> <Declaration Number>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration Number> -> = <Number> <Declaration Number>" << std::endl;
        outfile << std::endl;
    }

    else if (s == ",idJ") {
        std::cout << "<Declaration Number> -> , <Identifier> <Declaration Number>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration Number> -> , <Identifier> <Declaration Number>" << std::endl;
        outfile << std::endl;
    }
    
    
    else if (t == "J" && s == ";S") {
        std::cout << "Declaration Number> -> ; <Statement>" << std::endl;
        std::cout << std::endl;
        outfile << "Declaration Number> -> ; <Statement>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "=booleanK") {
        std::cout << "<Declaration Boolean> -> = <Number> <Declaration Boolean>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration Boolean> -> = <Number> <Declaration Boolean>" << std::endl;
        outfile << std::endl;
    }

    else if (s == ",idK") {
        std::cout << "<Declaration Boolean> -> , <Identifier> <Declaration Boolean>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration Boolean> -> , <Identifier> <Declaration Boolean>" <<std::endl;
        outfile << std::endl;
    }

    else if (t == "K" && s == ";S") {
        std::cout << "Declaration Boolean> -> ; <Statement>" << std::endl;
        std::cout << std::endl;
        outfile << "Declaration Boolean> -> ; <Statement>" << std::endl;
        outfile << std::endl;
    }
    

    else if (s == "=numL") {
        std::cout << "<Declaration Number> -> = <Number> <Declaration Number>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration Number> -> = <Number> <Declaration Number>" << std::endl;
        outfile << std::endl;
    }

    else if (s == ",idL") {
        std::cout << "<Declaration Number> -> , <Identifier> <Declaration Number>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration Number> -> , <Identifier> <Declaration Number>" << std::endl;
        outfile << std::endl;
    }

    else if (t == "L" && s == ";S") {
        std::cout << "<Declaration Float> -> ; <Statement>" << std::endl;
        std::cout << std::endl;
        outfile << "<Declaration Float> -> ; <Statement>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "TQ") {
        std::cout << "<Expression> -> <Term> <Expression Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Expression> -> <Term> <Expression Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "+TQ") {
        std::cout << "<Expression Prime> -> + <Term> <Expression Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Expression Prime> -> + <Term> <Expression Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "-TQ") {
        std::cout << "<Expression Prime> -> - <Term> <Expression Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Expression Prime> -> - <Term> <Expression Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "FR") {
        std::cout << "<Term> -> <Factor> <Term Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Term> -> <Factor> <Term Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "*FR") {
        std::cout << "<Term Prime> -> * <Factor> <Term Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Term Prime> -> * <Factor> <Term Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "/FR") {
        std::cout << "<Term Prime> -> / <Factor> <Term Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Term Prime> -> / <Factor> <Term Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "id") {
        std::cout << "<Factor> -> <Identifier>" << std::endl;
        std::cout << std::endl;
        outfile << "<Factor> -> <Identifier>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "(E)") {
        std::cout << "<Factor> -> ( <Expression> )" << std::endl;
        std::cout << std::endl;
        outfile << "<Factor> -> ( <Expression> )" << std::endl;
        outfile << std::endl;
    }

    else if (t == "Q" && s == "\0") {
        std::cout << "<Expression Prime> -> <Epsilon>" << std::endl;
        std::cout << std::endl;
        outfile << "<Expression Prime> -> <Epsilon>" << std::endl;
        outfile << std::endl;
    }

    else if (t == "R" && s == "\0") {
        std::cout << "<Term Prime> -> <Epsilon>" << std::endl;
        std::cout << std::endl;
        outfile << "<Term Prime> -> <Epsilon>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "idZ") {
        std::cout << "<Factor> -> <Identifier> <Factor Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Factor> -> <Identifier> <Factor Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "numZ") {
        std::cout << "<Factor> -> <Number> <Factor Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Factor> -> <Number> <Factor Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (s == "(E)Z") {
        std::cout << "<Factor> -> ( <Expression> ) <Factor Prime>" << std::endl;
        std::cout << std::endl;
        outfile << "<Factor> -> ( <Expression> ) <Factor Prime>" << std::endl;
        outfile << std::endl;
    }

    else if (t == "Z" && s == ";S") {
        std::cout << "<Factor Prime> -> ; <Statement>" << std::endl;
        std::cout << std::endl;
        outfile << "<Factor Prime> -> ; <Statement>" << std::endl;
        outfile << std::endl;
    }

    else if (t == "Z" && s == "\0") {
        std::cout << "<Factor Prime> -> <Epsilon>" << std::endl;
        std::cout << std::endl;
        outfile << "<Factor Prime> -> <Epsilon>" << std::endl;
        outfile << std::endl;
    }

    else if (t == "S" && s == "\0") {
        std::cout << "<Statement> -> <Epsilon>" << std::endl;
        std::cout << std::endl;
        outfile << "<Statement> -> <Epsilon>" << std::endl;
        outfile << std::endl;
    }

    else {

    }
}
