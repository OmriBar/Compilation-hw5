#include "attributes.h"

NonTermFunc::NonTermFunc() : DataObj(NONE) {}

NonTermFunc::NonTermFunc(std::string name , std::list<TypeNameEnum> parameters , TypeNameEnum retType) :
    DataObj(name,NONE) , Parameters(parameters) , RetType(retType) {}
