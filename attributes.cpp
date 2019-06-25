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

Node* TypeNameToExp(TypeNameEnum type , WorkReg workReg){
        switch(type){
            case TYPE_FUNC: return new NonTermFunc() ; break; // A Function cant be an ID in expression , so it must be an error!
            case TYPE_INT: return new NonTermInt(workReg); break;
            case TYPE_BYTE: return new NonTermByte(workReg); break;
            case TYPE_BOOL: return new NonTermBool(workReg); break;
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

std::string NumToStr(int x) {

    if (!x)
        return "0";
    bool IsNegative = x < 0 ? true : false;
    int abs = x < 0 ? -x : x;
    int absDup = abs;
    int absLen = 0;
    while(absDup){
      absDup /= 10;
      absLen ++;
    }
    std::string s = std::string(absLen,'0');
    int index = 0;
    while(abs) {
        s[index++]=(abs%10 + '0');
        abs /= 10;
    }
    int finalSize = IsNegative ? s.size()+1 : s.size();
    std::string s2 = std::string(finalSize,'0');
    int size = s.size();
    for(int i=0 ; i<size ; i++){
        s2[i] = s[size-1-i];
    }
    if(IsNegative){
        size++;
        for(int i= size-1 ; i>=0 ; i--){
        s2[i] = s2[i-1];
        }
        s2[0]='-';
    }
    return s2;
}
