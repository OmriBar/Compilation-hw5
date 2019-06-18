#ifndef ATTR_H
#define ATTR_H

#include <iostream>
#include "list"
#include "vector"
#include "string"
#include "typeEnums.h"
#include "AidFunctions.h"
#include "RegManagment.h"

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

class BinaryOp : public Node {
    opTypeEnum type;
    public:
        BinaryOp(opTypeEnum _opType) : type(_opType) {}
        BinaryOp(char * _opType) {
            std::string strType(_opType);
            if(strType == "+"){
                type = OP_SUM;
            }
            else if(strType == "-"){
                type = OP_SUB;
            }
            else if(strType == "*"){
                type = OP_MUL;
            }
            else if(strType == "/"){
                type = OP_DIV;
            }
            else if(strType == "and"){
                type = OP_AND;
            }
            else if(strType == "or"){
                type = OP_OR;
            }
            else if(strType == "not"){
                type = OP_NOT;
            }
        }
        opTypeEnum getType(){return type;}
};

class RelativeOp : public Node {
    opTypeEnum type;
    public:
        RelativeOp(opTypeEnum _opType) : type(_opType) {}
        RelativeOp(char * _opType) {
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
        opTypeEnum getType(){return type;}
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
        std::string GetStr() {return str;}
};

//======================================Data Objects Classes=====================================//

class DataObj : public Node {
    public:
        TypeNameEnum type;
        WorkReg DataReg;
        std::string Name;
        bool IsVar;
        WorkReg getWorkReg() {
            return DataReg;
        }
        WorkReg freeWorkReg(RegManagment regManagment) {
            regManagment.FreeReg(DataReg);
        }
        DataObj(std::string name , WorkReg dataReg ) : Name(name) , DataReg(dataReg) {
            IsVar = true;
        }
        DataObj(WorkReg dataReg) : DataReg(dataReg) {
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
    NonTermBool(WorkReg dataReg);
    NonTermBool(std::string name , WorkReg dataReg);
    bool IsItVar() {return IsItVar();}
};

class NonTermStr : public DataObj { //check whats up with that inside registers
    private:
    std::string Label;
     public:
     NonTermStr();
     NonTermStr(std::string label);
     std::string GetLabel() {return Label;}
};

class NonTermInt : public DataObj {
    int NumericValue;
    public:
    NonTermInt(WorkReg dataReg);
    NonTermInt(Node * numericValueNode , WorkReg dataReg);
    NonTermInt(std::string name , WorkReg dataReg);
    int GetNumericValue();
};

class NonTermByte : public DataObj {
    public:
    static bool IsValidByte(Node * node1);
    NonTermByte(WorkReg dataReg);
    NonTermByte(std::string name , WorkReg dataReg);
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

class ExpListObj : public Node {
    std::list<DataObj*> ExpList;
    public:
        ExpListObj() {}
        ExpListObj(DataObj* newExp) {
            ExpList.push_front(newExp);
        }
        ExpListObj(ExpListObj * oldList , DataObj* newExp) : ExpList(oldList->ExpList) {
            ExpList.push_front(newExp);
        }
        std::list<DataObj*> GetExpListObj() {return ExpList;}
        std::list<TypeNameEnum> GetTypesListObj() {
            std::list<TypeNameEnum> typesList;
            std::list<DataObj*> dataList(ExpList);
            while(! dataList.empty()){
                DataObj* obj = dataList.back();
                TypeNameEnum type;
                if(dynamic_cast<NonTermBool*>(obj)){
                    type = TYPE_BOOL;
                }
                else if(dynamic_cast<NonTermInt*>(obj)){
                    type = TYPE_INT;
                }
                else if(dynamic_cast<NonTermByte*>(obj)){
                    type = TYPE_BYTE;
                }
                else if(dynamic_cast<NonTermStr*>(obj)){
                    type = TYPE_STR;
                }
                typesList.push_front(type);
                dataList.pop_back();
            }
            return typesList;
        }
        int GetExpListObjSize() {return ExpList.size();}
};

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

Node* TypeNameToExp(TypeNameEnum type , WorkReg WorkReg);

TypeNameEnum ExpToTypeName(Node * node);

std::vector<string> ParaListToStrings(std::list<TypeNameEnum> paraList1);

std::vector<string> CallToRetType(std::list<TypeNameEnum> paraList1);

std::string TypeToString(TypeNameEnum type);

TypeNameEnum TypeNameToTypeEnum(Node * node);

int yyerror(char const* message);

#define YYSTYPE Node*

#endif