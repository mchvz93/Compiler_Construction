//
//  Symbol_Table.h
//  Final_Syntax
//
//  Created by XxTheEyeOfSauronxX on 12/9/20.
//  code follows: https://www.geeksforgeeks.org/cpp-program-to-implement-symbol-table/

#ifndef Symbol_Table_h
#define Symbol_Table_h

#include "Lexer.h"

class Sym_Table;
const int MAX = 500;

//============================================CLASS FOR SYMBOL TABLE NODE==========================================================================
class Sym_Node : public Lex {
public:
    Sym_Node() { next_ = nullptr; }

    Sym_Node(std::string key, std::string type, size_t lineNo, Sym_Node* next=nullptr) {
        this->identifier_ = key;
        this->type_ = type;
        this->lineNo_ = lineNo;
        next_ = nullptr;
    }

    void print();
    
////==========================================================================================
//    Sym_Node() : Sym_Node(Token(), next_ = nullptr) {}
//    Sym_Node() { next_ = nullptr; }
//    Sym_Node(Token& token, Sym_Node* next_=nullptr) : token_(token), next_(nullptr) {}
//
//    friend std::istream& operator>>(std::istream& is, Sym_Node& no) {
//        return is >> no.token_;
//    }
////==========================================================================================

    friend class Sym_Table;

private:
    std::string identifier_;
    std::string type_;
    size_t lineNo_;
//    std::vector<Token> token_;
//    Token token_;
    Sym_Node* next_;
    
};

//============================================CLASS FOR SYMBOL TABLE==============================================================================
class Sym_Table {
public:
    Sym_Table() {
        for (size_t i = 0; i < MAX; i++) {
            head[i] = nullptr;
        }
    }
    
    int hashFunction(std::string id_);                                    // creates hash table w/ separate chaining
    bool insert(std::string id_, std::string Type, size_t lineno);        // inserts new entries to symbol table
    std::string find(std::string id_);                                    // searches for identifier in table
    //==============================================================================================================================
    bool deleteRecord(std::string id_);                                   // not working as of 12/11/20
    bool modify(std::string id_, std::string Type, size_t lineno);        // not working as of 12/11/20
    
    friend class Lex;
    friend class TDPP;
private:
    Sym_Node* head[MAX];
};


#endif /* Symbol_Table_h */
