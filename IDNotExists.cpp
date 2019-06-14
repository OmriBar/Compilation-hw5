#include "attributes.h"

IDNotExists::IDNotExists() : DataObj(NONE) {}
    
IDNotExists::IDNotExists(std::string name) : DataObj(name,NONE) {}