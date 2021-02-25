//
//  Lexer.h
//  Final_Syntax
//
// 
//

#ifndef Lexer_h
#define Lexer_h

#include <set>
#include <vector>
#include <string>
//#include "Symbol_Table.h"



//struct to hold in tokens and lexemes
struct Token {
    Token(): Token("", "") {}
    Token(const std::string& lexeme,  const std::string& token) : lexeme_(lexeme),  token_(token) {}
    
    friend std::ostream& operator<<(std::ostream& os, Token& tok) {
        return os << tok.lexeme_ << tok.token_;
    }
    
    friend std::istream& operator>>(std::istream& is, Token& tok) {
        return is >> tok.lexeme_ >> tok.token_;
    }
    
    friend bool operator==(const Token& lhs, const Token& rhs) {
        return lhs.lexeme_ == rhs.lexeme_ && lhs.token_ == rhs.token_;
    }
    
    std::string lexeme_;
    std::string token_;
};


class Lex {
public:
    friend class TDPP;
    friend class Sym_Node;
    friend class Sym_Table;
    //=========useless for now in the code========================================
//    void set_map_key(); //uses insert operation for key         (useless for now)
//    void set_map_op();  //uses insert operation for operators   (useless for now)
//    void set_map_sep(); //uses insert opertation for seperators (useless for now)
//    void set_map_int(); //uses insert operation for integers    (useless for now)
//
//    //these bool functions are to be used to see if strings match (all useless for now)
//    bool isOp(char currChar);  //uses iterator to find operator
//    bool isSep(char currChar); //uses iterator to find seperators
//    bool f_ostr = false;       //bool for operators    (useless for now)
//    bool f_spstr = false;      //bool for separators   (useless for now)
    //=========useless for now in the code=(END)=====================================
//    Sym_Table table;
    bool isCom(char);                                           //checks for comments
    bool is_inCom = false;                                      //bool for comments
    virtual bool isKey(std::string currentToken);               //searches through keywords[][] returns true if found.
    int getColState(char);                                      //returns column values
    std::string setTokenName(int lexeme);                       //returns token type
    virtual std::vector<Token> Lexer(std::string input_str);    //heart of Lex program, separates tokens and lexemes
    std::vector<Token> tokens;                                  //vector to hold tokens and lexemes


private:
//=========useless for now in the code=========================================================
//    std::multiset<std::string> keydict;   //creates the keyword dictionary (useless for now)
//    std::multiset<char> opdict;           //creates the operator dictionary (useless for now)
//    std::multiset<char> sepdict;          //creates the seperator dictionary (useless for now)
//=========useless for now in the code=(END)===================================================

    
    Token tk;                                                   //initializing token struct
    char keywords[30][15] = { "int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or", "not" };                         // array holding keywords
    char currChar = ' ';
    std::string currentToken = "";
};
#endif /* Lexer_h */
