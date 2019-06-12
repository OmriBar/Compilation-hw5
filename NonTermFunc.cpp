#include "attributes.h"

NonTermFunc::NonTermFunc() {}

NonTermFunc::NonTermFunc(std::string name , std::list<TypeNameEnum> parameters , TypeNameEnum retType) :
    DataObj(name) , Parameters(parameters) , RetType(retType) {}
