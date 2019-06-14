#include "attributes.h"

NonTermInt::NonTermInt(WorkReg dataReg) : DataObj(dataReg) {}

NonTermInt::NonTermInt(Node * numericValueNode , WorkReg dataReg) : DataObj(dataReg) , NumericValue((dynamic_cast<NumVal*>(numericValueNode))->getVal()) {this->type = TYPE_INT;}

NonTermInt::NonTermInt(std::string name , WorkReg dataReg ) : DataObj(name,dataReg) {this->type = TYPE_INT;}

int NonTermInt::GetNumericValue() {return NumericValue;}