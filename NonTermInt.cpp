#include "attributes.h"

NonTermInt::NonTermInt() {}

NonTermInt::NonTermInt(Node * numericValueNode) : NumericValue((dynamic_cast<NumVal*>(numericValueNode))->getVal()) {this->type = TYPE_INT;}

NonTermInt::NonTermInt(std::string name) : DataObj(name) {this->type = TYPE_INT;}

int NonTermInt::GetNumericValue() {return NumericValue;}