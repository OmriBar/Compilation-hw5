#include "attributes.h"

NonTermStr::NonTermStr(std::string label) : DataObj(label,NONE) , Label(label) {this->type = TYPE_STR;}