//
//  Lexer.cpp
//  Final_Syntax
//
//  Created by XxTheEyeOfSauronxX
//
/*
 this code was based on https://www.programmingnotes.org/?p=4699
 added a few things such as comments, keywords, separators into the enum and created a new state table
 */

#include <iostream>
#include <fstream>
#include <sstream>
//#include <iterator>
//#include <functional>
//#include <set>
#include <vector>
//#include <string>
#include <iomanip>
#include "Lexer.h"
#include "Syntax.h"
#include "Symbol_Table.h"



enum State {
    REJECT = 0,
    COMM,
    INTEGER,
    REAL,
    SPACE,
    STRING,
    KEYWORD,
    SEPARATOR,
    OPERATOR,
    UNKNOWN
};

int StateTable[][10] = {
    { 0 , COMM, INTEGER, REAL, SPACE, STRING, KEYWORD, SEPARATOR, OPERATOR, UNKNOWN },
    { COMM, REJECT, COMM, COMM, COMM, COMM, COMM, COMM, COMM, COMM },
    { INTEGER, COMM, INTEGER, INTEGER, REJECT, INTEGER, REJECT, REJECT, REJECT, REJECT},
    { REAL, COMM, INTEGER, UNKNOWN, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT },
    { SPACE, COMM, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT},
    { STRING, COMM, STRING, REJECT, REJECT, STRING, REJECT, REJECT, REJECT, REJECT },
    { KEYWORD, COMM, REJECT, REJECT, REJECT, REJECT, KEYWORD, SEPARATOR, REJECT, REJECT},
    { SEPARATOR, COMM, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT},
    { OPERATOR, COMM, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT, REJECT},
    { UNKNOWN, COMM, UNKNOWN, UNKNOWN, REJECT, REJECT, REJECT , UNKNOWN, UNKNOWN, UNKNOWN }
};

//
////----------------------------------------------------------------------------------
//void Lex::set_map_key() {
//    keydict.insert("int");
//    keydict.insert("float");
//    keydict.insert("bool");
//    keydict.insert("true");
//    keydict.insert("false");
//    keydict.insert("if");
//    keydict.insert("else");
//    keydict.insert("then");
//    keydict.insert("then");
//    keydict.insert("endif");
//    keydict.insert("while");
//    keydict.insert("whileend");
//    keydict.insert("do");
//    keydict.insert("doend");
//    keydict.insert("for");
//    keydict.insert("forend");
//    keydict.insert("input");
//    keydict.insert("output");
//    keydict.insert("and");
//    keydict.insert("or");
//    keydict.insert("not");
//}
////------------------------------------------------------------------------------------------
//void Lex::set_map_op() {
//    opdict.insert('+');
//    opdict.insert('-');
//    opdict.insert('%');
//    opdict.insert('*');
//    opdict.insert('=');
//    opdict.insert('/');
//    opdict.insert('<');
//    opdict.insert('>');
//    opdict.insert('.');
//}
////------------------------------------------------------------------------------------------
//void Lex::set_map_sep() {
//    sepdict.insert('(');
//    sepdict.insert(')');
//    sepdict.insert('[');
//    sepdict.insert(']');
//    sepdict.insert(':');
//    sepdict.insert('{');
//    sepdict.insert('}');
//    sepdict.insert(',');
//    sepdict.insert(';');
//}

//==========start of working bools =============================
bool Lex::isKey(std::string currentToken) {
    char token_str[currentToken.length()];
    for (int i = 0; i < currentToken.length(); ++i) {
        token_str[i] = currentToken[i];
    }
    
    for (size_t i = 0; i <20; i++)
        if (std::strcmp(keywords[i], token_str) == 0)
            return true;
        return false;
}


bool Lex::isCom(char currChar) {
    if (currChar == '!')
        return true;
    return false;
}
//==========end of working bools =============================


//=====start of useless bools ignore them===============
//bool Lex::isOp(char currChar) {
//    std::multiset<char>::iterator oit;
//    oit = opdict.find(currChar);
////    std::multiset<char>::value_compare comp = opdict.value_comp();
//
//
//    while (oit != opdict.end()) {
////        if (comp(*oit, currChar)) {
//        if (*oit == currChar){
//            f_ostr = true;
//            return f_ostr;
//        }
//    }
//    return f_ostr;
//}
//
//bool Lex::isSep(char currChar) {
//    std::multiset<char>::iterator sep_it;
//    sep_it = sepdict.find(currChar);
////    sep_it = sepdict.begin();
//    while (sep_it != sepdict.end()) {
//        if (*sep_it == currChar)
//            f_spstr = true;
//    }
//    return f_spstr;
//}
//=====end of useless bools ignore them===============

//------------------------------END--OF--BOOLS------------------------------------------------


//-------------------------------------------------------------------------------------------
// using enum as state transition
int Lex::getColState(char currChar) {
    
    if (isspace(currChar)) {
        return SPACE;
    }
    
    else if (isalpha(currChar) || currChar == '$' ) {
        return STRING;
    }
    
    else if (isKey(currentToken)) {
        return KEYWORD;
    }

    
    else if (isCom(currChar)) {
        return COMM;
    }

    else if (ispunct(currChar) && currChar != ';' && currChar != ',' && currChar != '(' && currChar != ')' && currChar != '{' && currChar != '}' && currChar != '.') {
        return OPERATOR;
    }

    else if (currChar == ';' || currChar == ',' || currChar == '(' || currChar == ')' || currChar == '{' || currChar == '}') {
        return SEPARATOR;
    }
    else if (isdigit(currChar)) {
        return INTEGER;
    }
    else if (isdigit(currChar) || currChar == '.') {
        return REAL;
    }
    else
        return UNKNOWN;
};

std::string Lex::setTokenName(int lexeme) {
    switch(lexeme) {
        case COMM:
            return "Comment";
            break;
        case INTEGER:
            return "Integer";
            break;
        case REAL:
            return "Real";
            break;
        case SPACE:
            return "Space";
            break;
        case STRING:
            if(isKey(currentToken))
                return "Keyword";
            return "Identifier";
            break;
        case KEYWORD:
            return "Keyword";
            break;
        case SEPARATOR:
            return "Separator";
            break;
        case OPERATOR:
            return "Operator";
            break;
        case UNKNOWN:
            return "Unknown";
            break;
            
        default:
            return "[ERROR]: INVALID STATE.";
            break;
    }
}


//-------------------------------------------------------------------------------
std::vector<Token>Lex::Lexer(std::string input_str) {
    std::vector<Token> tokens;
    currentToken = "";
    currChar = ' ';

    
    int col = REJECT;
    int thisState = REJECT;
    int prev_State = REJECT;
    
    if (is_inCom == true) {
        thisState = COMM;
        is_inCom = false;
    }
    
    for (size_t x = 0; x < input_str.length();) {
        currChar = input_str[x];
        
        col = getColState(currChar);
        
        thisState = StateTable[thisState][col];
        
        if(thisState == REJECT) {
            if(prev_State != SPACE && prev_State != COMM) {
                tk.lexeme_ = currentToken;
                tk.token_ = setTokenName(prev_State);
                tokens.push_back(tk);
            }
            if (prev_State == COMM)
                x++;
            //resetting the token to blank.
            currentToken = "";
        }
        else if (thisState == COMM)
            x++;
        else {
            currentToken += currChar;
            x++;
        }
        prev_State = thisState;

    }
    
    if (thisState != SPACE && currentToken != "" && thisState != COMM) {
        tk.lexeme_ = currentToken;
        tk.token_ = (setTokenName(thisState));
        tokens.push_back(tk);
    }
    
    else if (thisState == COMM)
        is_inCom = true;
    

    return tokens;
}

//in case we need to print for future projects.
//void Lex::print(std::vector<Token> tokens) {
//    std::cout << std::endl;
//    std::cout.setf (std::ios::left, std::ios::adjustfield);
//    std::cout << ' ' << std::setw(10) << "Lexeme " << "Token " << std::endl;
//    std::cout << std::setfill('-') << std::setw(20) << " " << std::setfill(' ') << std::endl;
//
//// loop for formatted printing of lexemes and tokens
//    for (size_t index = 0; index < tokens.size(); index++) {
//        std::cout << ' ' << std::setw(10) << tokens[index].lexeme_ << tokens[index].token_ << std::endl;
//    }
////
////    for (size_t index = 0; index < tokens.size(); index++) {
////        if (tokens[index].token_ == "Identifier") {
//////            table.hashFunction(tokens[index].lexeme_);
////
////        }
////    }
//
//    std::cout << std::endl;
//}
