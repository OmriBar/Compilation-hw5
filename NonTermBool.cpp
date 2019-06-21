#include "attributes.h"

/*NonTermBool::NonTermBool(WorkReg dataReg) : DataObj(dataReg) {
this->type = TYPE_BOOL;
}

NonTermBool::NonTermBool(std::string name , WorkReg dataReg) : DataObj(name,dataReg) {
this->type = TYPE_BOOL;
}*/

NonTermBool::NonTermBool(std::vector<int> truthList , std::vector<int> falseList) : 
DataObj("",NONE) , TrueList(truthList) , FalseList(falseList) {}

NonTermBool::NonTermBool(WorkReg workReg) : DataObj("",workReg) {}

bool NonTermBool::IsValidBoolExp(Node * node1, Node * node2 , Node * node3){
    TypeNameEnum type1 = ExpToTypeName(node1);
    TypeNameEnum type2 = ExpToTypeName(node3);
    if(type1 != TYPE_BOOL || type2 != TYPE_BOOL){
        return false;
    }
    return true;
}

bool NonTermBool::IsValidBoolExpRelExp(Node * node1, Node * node2 , Node * node3){
    TypeNameEnum type1 = ExpToTypeName(node1);
    TypeNameEnum type2 = ExpToTypeName(node3);
    if(!(type1 == TYPE_INT || type1 == TYPE_BYTE) || !(type2 == TYPE_INT || type2 == TYPE_BYTE)){
        return false;
    }
    return true;
}

bool NonTermBool::IsValidBoolExp(Node * node){
    
    return true;
}

std::vector<int> NonTermBool::GetTrueList() {
    return TrueList;
}
std::vector<int> NonTermBool::GetFalseList(){
    return FalseList;
}
