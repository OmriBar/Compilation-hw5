#include "attributes.h"

NonTermStr::NonTermStr(){this->type = TYPE_STR;}

NonTermStr::NonTermStr(std::string name) : DataObj(name) {this->type = TYPE_STR;}