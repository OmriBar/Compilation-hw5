#include "attributes.h"

NonTermByte::NonTermByte(WorkReg dataReg) : DataObj(dataReg) {
this->type = TYPE_BYTE;
}

NonTermByte::NonTermByte(std::string name , WorkReg dataReg) : DataObj(name,dataReg) {this->type = TYPE_BYTE;}

bool NonTermByte::IsValidByte(Node * node) {
    int number = (dynamic_cast<NumVal*>(node))->getVal();
    return (number >=0 && number <=255) ? true : false;
}