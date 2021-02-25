//
//  Symbol_Table.cpp
//  Final_Syntax
//
//  Created by XxTheEyeOfSauronxX on 12/12/20.
// **UPDATE from previous submissions**:
//          else production now functional
//          float declaration production now functional
//          corrected: notifies user if string is fully parsed/accepted.
//          created class for Symbol Table (not working as of 12/11/20)


#include "Symbol_Table.h"
#include "Lexer.h"
#include <iostream>


int Sym_Table::hashFunction(std::string id) {
    unsigned int sum = 0;
    
    for (unsigned int i = 0; i < id.length(); i++) {
        sum = sum+id[i];
    }
    return (sum%100);
}

//bool Sym_Table::insert(std::string id, std::string Type, size_t lineno) {
//    int index = hashFunction(id);
//    Sym_Node* p = new Sym_Node(id, Type, lineno);
//
//    if (head[index] == nullptr) {
//        head[index] = p;
//        return true;
//    }
//    else {
//        Sym_Node* begin = head[index];
//        while(begin->next_ !=nullptr) {
//            begin = begin->next_;
//
//            begin->next_ = p;
//            return true;
//        }
//    }
//    return false;
//}
//
//std::string Sym_Table::find(std::string id) {
//    int index = hashFunction(id);
//
//    Sym_Node* begin = head[index];
//
//    if (begin == nullptr) {
//        return "-1";
//    }
//
//    while (begin != nullptr) {
//        if (begin->identifier_ == id) {
//            begin->print();
//        }
//        begin = begin->next_;
//    }
//
//    return "-1";
//
//}

//void Sym_Node::print() {
//        std::cout << "Identifier: " << identifier_ << "\nType: "<< type_ << "\nLine No: " << lineNo_ << std::endl;
//}

//bool Sym_Table::modify(std::string id, std::string d, size_t l) {
//
//}




