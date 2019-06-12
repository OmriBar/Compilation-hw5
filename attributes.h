#ifndef ATTR_H
#define ATTR_H

#include <iostream>
#include "list"
#include "vector"
#include "string"
#include "typeEnums.h"
#include "AidFunctions.h"

using namespace std;


class Node {
    public:
    Node(){}
    virtual ~Node() {}
};

//======================================token's Classes=====================================//

class Type : public Node {
    public:
    TypeNameEnum name;
        Type(TypeNameEnum _typeName) : name(_typeName) {}
};

class Op : public Node {
    opTypeEnum type;
    public:
        Op(opTypeEnum _opType) : type(_opType) {}
        Op(char * _opType) {
            std::string strType(_opType);
            if(strType == "=="){
                type = OP_EQU;
            }
            else if(strType == "!="){
                type = OP_NEQ;
            }
            else if(strType == "!="){
                type = OP_NEQ;
            }
            else if(strType == "<"){
                type = OP_RIGHTBIG;
            }
            else if(strType == ">"){
                type = OP_LEFTBIG;
            }
            else if(strType == "<="){
                type = OP_RIGHTBIGEQ;
            }
            else if(strType == ">="){
                type = OP_LEFTBIGEQ;
            }
        }
};

class BoolVal : public Node {
    ValBoolTypeEnum type;
    public:
        BoolVal(ValBoolTypeEnum _ValboolType) : type(_ValboolType) {}
};

class CmdWord : public Node {
    cmdTypeEnum type;
    public:
        CmdWord(cmdTypeEnum _cmdType) : type(_cmdType)  {}
};

class ScopeVal : public Node {
    scopeValEnum type;
    public:
        ScopeVal(scopeValEnum _scopeType) : type(_scopeType) {}

};

class NumVal : public Node {
    int val;
    std::string str;
    public:
        NumVal(char * strNum) : val(CharPointerToInt(strNum)) , str(strNum) {}
        int getVal() {return val;}
        std::string getStr() {return str;}
};

class IdVal : public Node {
    
    public:
    std::string IdStr;
        IdVal(char * strId) : IdStr(strId) {}
        std::string GetVal() {return IdStr;}
};

class StrVal : public Node {
    std::string str;
    public:
        StrVal(char * strVal) : str(strVal) {}
};

//======================================Data Objects Classes=====================================//

class DataObj : public Node {
    public:
        TypeNameEnum type;
        std::string Name;
        bool IsVar;
        DataObj(std::string name ) : Name(name) {
            IsVar = true;
        }
        DataObj() {
            IsVar = false;
        }
        bool IsItVar() {return IsVar;}
        virtual ~DataObj() {}
};

class NonTermBool : public DataObj {
    std::string name;
    bool IsVar;
    public:
    static bool IsValidBoolExp(Node * node);
    static bool IsValidBoolExp(Node * node1, Node * node2 , Node * node3);
    static bool IsValidBoolExpRelExp(Node * node1, Node * node2 , Node * node3);
    NonTermBool();
    NonTermBool(std::string name);
    bool IsItVar() {return IsItVar();}
};

class NonTermStr : public DataObj {
     public:
     NonTermStr();
     NonTermStr(std::string name);
};

class NonTermInt : public DataObj {
    int NumericValue;
    public:
    NonTermInt();
    NonTermInt(Node * numericValueNode);
    NonTermInt(std::string name);
    int GetNumericValue();
};

class NonTermByte : public DataObj {
    public:
    static bool IsValidByte(Node * node1);
    NonTermByte();
    NonTermByte(std::string name);
};

class IDNotExists : public DataObj {
    std::string name;
    public:
    IDNotExists();
    IDNotExists(std::string name);
};

class NonTermFunc : public DataObj {
    std::string name;
    std::list<TypeNameEnum> Parameters;
    TypeNameEnum RetType;
    public:
        NonTermFunc();
        NonTermFunc(std::string name , std::list<TypeNameEnum> parameters , TypeNameEnum retType);
        TypeNameEnum GetRetType() {return RetType;}
};

class NonTermVoid : public DataObj {
    std::string name;
    public:
    NonTermVoid();
    NonTermVoid(std::string name);
};

//======================================Program Flow Classes=====================================//

class ParaListObj : public Node {
    std::list<TypeNameEnum> paraList;
    public:
        ParaListObj() {}
        ParaListObj(TypeNameEnum newPara) {
            paraList.push_front(newPara);
        }
        ParaListObj(ParaListObj * oldList , TypeNameEnum newPara) : paraList(oldList->paraList) {
            paraList.push_front(newPara);
        }
        ParaListObj(ParaListObj * oldList , ParaListObj * anotherListOfOneObj) : paraList(oldList->paraList) {
            paraList.push_front(anotherListOfOneObj->GetParaList().back());
        }
        std::list<TypeNameEnum> GetParaList() {return paraList;}
        int GetParaListSize() {return paraList.size();}
};
    
class PreCondListObj : public Node {
    int numCond;
    public:
        PreCondListObj(PreCondListObj * oldPreCondList) {
            if(oldPreCondList!=NULL){
                numCond=(oldPreCondList->numCond) + 1;
            }
            else{
                numCond=1;
            }
        }
        int GetNumCond() {return numCond;}

};

//======================================Attributes-related Functions=====================================//

bool IsItConstOrExistingSymbol(DataObj * dataObject);

Node* TypeNameToExp(TypeNameEnum type);

TypeNameEnum ExpToTypeName(Node * node);

std::vector<string> ParaListToStrings(std::list<TypeNameEnum> paraList1);

std::vector<string> CallToRetType(std::list<TypeNameEnum> paraList1);

std::string TypeToString(TypeNameEnum type);

TypeNameEnum TypeNameToTypeEnum(Node * node);

int yyerror(char const* message);

#define YYSTYPE Node*

#endif