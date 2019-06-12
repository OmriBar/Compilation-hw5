#include "attributes.h"

NonTermByte::NonTermByte() {
this->type = TYPE_BYTE;
}

NonTermByte::NonTermByte(std::string name) : DataObj(name) {this->type = TYPE_BYTE;}

bool NonTermByte::IsValidByte(Node * node){
    int number = (dynamic_cast<NumVal*>(node))->getVal();
    return (number >=0 && number <=255) ? true : false;
}