#include "attributes.h"
#include "output.hpp"
#include <stdlib.h> 
extern int yylineno;

bool IsItConstOrExistingSymbol(DataObj * dataObject){
    if(!(dataObject->IsItVar())){
        return true;
    }
    if(dynamic_cast<IDNotExists*>(dataObject)!=NULL){
        return true;
    }
    return false;
}

Node* TypeNameToExp(TypeNameEnum type){
        switch(type){
            case TYPE_FUNC: return new NonTermFunc() ; break; // A Function cant be an ID in expression , so it must be an error!
            case TYPE_INT: return new NonTermInt(); break;
            case TYPE_BYTE: return new NonTermByte(); break;
            case TYPE_BOOL: return new NonTermBool(); break;
            case TYPE_STR: return new NonTermStr(); break;
        }
    return NULL;  
}

TypeNameEnum TypeNameToTypeEnum(Node * node){
    Type* typeName = dynamic_cast<Type*>(node);
    return typeName->name;
}

TypeNameEnum ExpToTypeName(Node * node){
    if(dynamic_cast<NonTermBool*>(node)!= NULL){
        return TYPE_BOOL;
    }
    else if(dynamic_cast<NonTermInt*>(node)!= NULL){
        return TYPE_INT;
    }
    else if(dynamic_cast<NonTermStr*>(node)!= NULL){
        return TYPE_STR;
    }
    else if(dynamic_cast<NonTermByte*>(node)!= NULL){
        return TYPE_BYTE;
    }
    return TYPE_NONEXIST;
}

std::vector<string> ParaListToStrings(std::list<TypeNameEnum> paraList){
    std::vector<string> vec = std::vector<string>();
    while(!paraList.empty()){
        TypeNameEnum type = paraList.front();
        std::string currPar = ""; 
        switch(type){
            case TYPE_INT: currPar="INT"; break;
            case TYPE_BYTE: currPar="BYTE"; break;
            case TYPE_BOOL: currPar="BOOL"; break;
            case TYPE_STR: currPar="STRING"; break;
        }
        vec.push_back(currPar);
        paraList.pop_front();
    }
    return vec;

}

std::string TypeToString(TypeNameEnum type){
    switch(type){
            case TYPE_INT: return std::string("INT"); break;
            case TYPE_BYTE: return std::string("BYTE"); break;
            case TYPE_BOOL: return std::string("BOOL"); break;
            case TYPE_STR: return std::string("STRING"); break;
            case TYPE_VOID: return std::string("VOID"); break;
            }
    return "none";
}

int yyerror(char const* message)
{
    output::errorSyn(yylineno);
	exit(0);
}
