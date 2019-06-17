#include "SemanticActions.h"
#include "output.hpp"
#include <stdlib.h> 
#include "RegManagment.h"

// RetType -> TYPE 

Node * RetTypeAction1(Node * node1){
    return node1;
}

// RetType -> VOID 

Node * RetTypeAction2(){
    return new Type(TYPE_VOID);
}

// Formals -> epsilon

Node * FormalsAction1(){
    return new ParaListObj();
}

// Formals -> FormalsList

Node * FormalsAction2(Node * node1){
    return node1;
}

//FormalsList -> FormalDecl

Node * FormalsListAction1(Node * node1){
    return node1;
}

//FormalsList ->  FormalDecl COMMA FormalsList

Node * FormalsListAction2(Node * node1 , Node * node2 , Node * node3){
    ParaListObj* list1 = dynamic_cast<ParaListObj*>(node1);
    ParaListObj* list3 = dynamic_cast<ParaListObj*>(node3);
    return new ParaListObj(list3,list1);
}

//FormalDecl -> Type ID

Node * FormalDeclAction1(SymbolTable& symTable , Node * node1 , Node * node2){
    TypeNameEnum type = TypeNameToTypeEnum(node1);
    std::string name = (dynamic_cast<IdVal*>(node2))->GetVal();
    Symbol* sym = symTable.GetSymbol(name);
    if(sym != NULL){
        output::errorDef(yylineno,name); // already exists!
        exit(0);
      //  yyerror("error!");
    }
    symTable.AddVariableSymbol(name , symTable.getCurrentIndex()+1 ,type);
    return new ParaListObj(type);
}
// PreConditions -> epsilon

Node* PreConditionsAction1() {
    return NULL;
}

// PreConditions -> PreConditions PreCondition

Node* PreConditionsAction2(Node* node1 , Node* node2) {
    return new PreCondListObj(dynamic_cast<PreCondListObj*>(node1));
}

//PreCondition -> PRECOND LPAREN Exp RPAREN

void PreConditionAction1(Node* node1 , Node* node2 , Node* node3 , Node* node4) {
    TypeNameEnum type = ExpToTypeName(node3);
    if(type!=TYPE_BOOL){
        output::errorMismatch(yylineno);
        exit(0);
        //yyerror("");
    }
}
//Statment -> Type ID SC

void StatmentAction1(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3 , RegManagment& regManagment , CodeBuffer& codeBuffer){
    std::string name = (dynamic_cast<IdVal*>(node2))->IdStr;
    Symbol* sym = symTable.GetSymbol(name);
    if(sym != NULL){
        output::errorDef(yylineno,(dynamic_cast<IdVal*>(node2))->IdStr); // already exists in the symbol table
        exit(0);
    }
    TypeNameEnum type = TypeNameToTypeEnum(node1);
    symTable.AddVariableSymbol(name , symTable.getCurrentIndex()+1 ,type);
    AddNonBoolVarToBuffer(regManagment,codeBuffer);
    
}

//Statment -> Type ID ASSIGN Exp SC

void StatmentAction2(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3, Node* node4, Node* node5
, RegManagment& regManagment , CodeBuffer& codeBuffer){
    std::string name = (dynamic_cast<IdVal*>(node2))->IdStr;
    Symbol* sym = symTable.GetSymbol(name);
    
    if(sym != NULL){
        output::errorDef(yylineno,(dynamic_cast<IdVal*>(node2))->IdStr); // alrady exists in the symbol table
        exit(0);
    }
    if(dynamic_cast<Type*>(node1)->name != ExpToTypeName(node4) &&
        !(dynamic_cast<Type*>(node1)->name == TYPE_INT && ExpToTypeName(node4) == TYPE_BYTE)){ // mismatch
        output::errorMismatch(yylineno);
        exit(0);
    }
    TypeNameEnum type = TypeNameToTypeEnum(node1);
    symTable.AddVariableSymbol(name , symTable.getCurrentIndex()+1 ,type);
    WorkReg reg = regManagment.AllocateReg();
    AddAndAssignNonBoolVarToBuffer(reg,regManagment,codeBuffer);
}

//Statment -> ID ASSIGN Exp SC
void StatmentAction3(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3, Node* node4
        ,RegManagment& regManagment , CodeBuffer& codeBuffer){
    std::string name = (dynamic_cast<IdVal*>(node1))->IdStr;
    Symbol* sym = symTable.GetSymbol(name);
    if(sym == NULL || sym->GetType() == TYPE_FUNC){
        output::errorUndef(yylineno, (dynamic_cast<IdVal*>(node1))->IdStr); // not exists in the symbol table
        exit(0);
    }
    TypeNameEnum type = sym->GetType();
    if((type != ExpToTypeName(node3)) &&
     !(type == TYPE_INT && ExpToTypeName(node3)== TYPE_BYTE)){ // mismatch
        output::errorMismatch(yylineno);
        exit(0);
        //yyerror("error!");
    }
    WorkReg reg = (dynamic_cast<DataObj*>(node3))->getWorkReg();
    int varOffset = sym->GetIndex();
    AssignNonBoolVarToBuffer(reg,varOffset,regManagment,codeBuffer);
}

//Statment -> BREAK SC 

void StatmentAction4(int in_while_flag){
    if(in_while_flag<=0){
     output::errorUnexpectedBreak(yylineno);
     exit(0);
     //yyerror("error!");
    }
}

//Statment -> CONTINUE SC

void StatmentAction5(int in_while_flag){
    if(in_while_flag<=0){
     output::errorUnexpectedContinue(yylineno);
     exit(0);
     //yyerror("error!");
    }
}

//Statment -> RETURN SC

void StatmentAction6(SymbolTable& symTable){
    FunctionSymbol* funcSym = symTable.GetCurrentFunction();
    TypeNameEnum type = funcSym->GetRetType();
    if(!(type==TYPE_VOID)){
        output::errorMismatch(yylineno);
        exit(0);
    }
}

//Statment -> RETURN Exp SC

void StatmentAction7(SymbolTable& symTable , Node * node1 , Node * node2){
    TypeNameEnum currtype = ExpToTypeName(node2);
    FunctionSymbol* funcSym = symTable.GetCurrentFunction();
    TypeNameEnum definedType = funcSym->GetRetType();

    if(!(currtype==definedType) && !(definedType==TYPE_INT && currtype == TYPE_BYTE)){
        output::errorMismatch(yylineno);
        exit(0);
    }
}

//ExpList -> Exp COMMA ExpList 

Node* ExpListAction1(Node* node1 , Node* node2 , Node* node3) {
        ParaListObj* paralist = dynamic_cast<ParaListObj*>(node3);
        TypeNameEnum type = ExpToTypeName(node1);
        return new ParaListObj(paralist,type);
} 

//ExpList -> Exp

Node* ExpListAction2(Node* node1){
    TypeNameEnum type = ExpToTypeName(node1);
    return new ParaListObj(ExpToTypeName(node1));
}

bool AreParaListsEqual(std::list<TypeNameEnum> list1 , std::list<TypeNameEnum> list2){
    if(list1.size() != list2.size()){
        return false;
    }
    std::list<TypeNameEnum>::iterator it1 = list1.begin();
    std::list<TypeNameEnum>::iterator it2 = list2.begin();
    while(it1!=list1.end()){
        if((*it1)!=(*it2)){
            if((*it1)==TYPE_BYTE && (*it2)==TYPE_INT){
                it1++;
                it2++;
                continue;
            }
            else{
                return false;
            }
        }
        it1++;
        it2++;
    }
    return true;

}

// Call -> ID LPAREN ExpList RPAREN

Node* CallAction1(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 , Node* node4 , RegManagment regManagment){
    Symbol * sym = symTable.GetSymbol((dynamic_cast<IdVal*>(node1))->IdStr);
    if(sym == NULL || sym->GetType() != TYPE_FUNC ){
        output::errorUndefFunc(yylineno,(dynamic_cast<IdVal*>(node1))->IdStr);
        exit(0);
    }
    std::list<TypeNameEnum> symParas = (dynamic_cast<FunctionSymbol*>(sym))->GetParametersList();
    std::list<TypeNameEnum> expListparas = (dynamic_cast<ParaListObj*>(node3))->GetParaList();
    std::vector<string> vector_symParas = ParaListToStrings(symParas);
    /*std::vector<string> list1 = ParaListToStrings(symParas);
    std::vector<string> list2 = ParaListToStrings(expListparas);
    std::cout << "start" << std::endl;
    for(std::vector<string>::iterator it = list1.begin() ; it != list1.end() ; it++){
        std::cout << std::endl << "type : " << (*it) << std::endl;
    }
    std::cout << "end" << std::endl;
    std::cout << "start" << std::endl;
    for(std::vector<string>::iterator it = list2.begin() ; it != list2.end() ; it++){
        std::cout << std::endl << "type : " << (*it) << std::endl;
    }
    std::cout << "end" << std::endl;*/
    if(!AreParaListsEqual(expListparas,symParas)){
        output::errorPrototypeMismatch(yylineno,(dynamic_cast<IdVal*>(node1))->IdStr,vector_symParas);
        exit(0);
        //yyerror("error!");
    }
    FunctionSymbol* funcSym = dynamic_cast<FunctionSymbol*>(sym);
    return TypeNameToExp(funcSym->GetRetType(),regManagment.AllocateReg());
}

// Call -> ID LPAREN RPAREN

Node* CallAction2(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 , RegManagment regManagment){
    Symbol * sym = symTable.GetSymbol((dynamic_cast<IdVal*>(node1))->IdStr);
    if(sym == NULL || sym->GetType() != TYPE_FUNC ){
        output::errorUndefFunc(yylineno,(dynamic_cast<IdVal*>(node1))->IdStr);
        exit(0);
        //yyerror("error!");
    }
    std::list<TypeNameEnum> symParas = (dynamic_cast<FunctionSymbol*>(sym))->GetParametersList(); 
    std::list<TypeNameEnum> expListparas = std::list<TypeNameEnum>();
    if(symParas != expListparas){
        std::vector<string> vector_symParas = ParaListToStrings(symParas);
        output::errorPrototypeMismatch(yylineno,(dynamic_cast<IdVal*>(node1))->IdStr,vector_symParas);
        exit(0);
        //yyerror("error!");
    }
    FunctionSymbol* funcSym = dynamic_cast<FunctionSymbol*>(sym);
    return TypeNameToExp(funcSym->GetRetType(),regManagment.AllocateReg());
}

// Type -> INT

Node* TypeAction1(){
    return new Type(TYPE_INT);
}

// Type -> BYTE

Node* TypeAction2(){
    return new Type(TYPE_BYTE);
}

// Type -> BOOL

Node* TypeAction3(){
    return new Type(TYPE_BOOL);
}

// Exp -> LPAREN Exp RPAREN

Node* ExpAction1(Node* node1 , Node* node2 , Node* node3) {
    TypeNameEnum type = ExpToTypeName(node2);
    if(type != TYPE_BOOL){
            output::errorMismatch(yylineno);
            exit(0);
            //yyerror("error!");
        }
    DataObj * dataObj = dynamic_cast<DataObj*>(node2);
    return TypeNameToExp(TYPE_BOOL,dataObj->getWorkReg());
}// TODO: add error here

// Exp -> Exp BINOP Exp

Node* ExpAction2(Node* node1 , Node* node2 , Node* node3 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    TypeNameEnum node1Type = ExpToTypeName(node1);
    TypeNameEnum node2Type = ExpToTypeName(node3);
    if((node1Type != TYPE_INT && node1Type != TYPE_BYTE)
        || (node2Type != TYPE_INT && node2Type != TYPE_BYTE) ){
            output::errorMismatch(yylineno);
            exit(0);
        }
    DataObj * dataObj1 = dynamic_cast<DataObj*>(node1);
    DataObj * dataObj3 = dynamic_cast<DataObj*>(node3);
    dataObj1->freeWorkReg(regManagment);
    dataObj3->freeWorkReg(regManagment);
    WorkReg resReg = regManagment.AllocateReg();

    opTypeEnum op = (dynamic_cast<BinaryOp*>(node2))->getType();
    BinopCmdToBuffer(dataObj1->getWorkReg(),op,dataObj3->getWorkReg(),resReg,regManagment,codeBuffer);

    if(node1Type==TYPE_INT || node2Type==TYPE_INT ){
        return TypeNameToExp(TYPE_INT,resReg);
    }
    else{
        return TypeNameToExp(TYPE_BYTE,resReg);
    }
}

// Exp -> ID

Node* ExpAction3(SymbolTable& symTable , Node* node1 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    std::string name = (dynamic_cast<IdVal*>(node1))->GetVal();
    Symbol* sym = symTable.GetSymbol(name);
    if(sym == NULL){
        output::errorUndef(yylineno,name);
        exit(0);
    }
    else if(sym->GetType() == TYPE_FUNC){
        output::errorUndef(yylineno,name);
        exit(0);
    }
    return TypeNameToExp(sym->GetType(),regManagment.AllocateReg());
}

// Exp -> Call 

Node* ExpAction4(Node* node , RegManagment& regManagment, CodeBuffer& codeBuffer){
    return node;
}

// Exp -> NUM

Node* ExpAction5(Node* node , RegManagment& regManagment, CodeBuffer& codeBuffer){
    std::string numberStr = (dynamic_cast<NumVal*>(node))->getStr();
    std::stringstream command;
    WorkReg resReg = regManagment.AllocateReg();
    command << "li ";
    command << "$" << resReg << ", ";
    command << numberStr;
    codeBuffer.emit(command.str());
    return new NonTermInt(node,resReg);
}

// Exp -> NUM B

Node* ExpAction6(Node* node1 , Node* node2 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    if( !(NonTermByte::IsValidByte(node1)) )
        {
            std::string str = (dynamic_cast<NumVal*>(node1))->getStr();
            output::errorByteTooLarge(yylineno,str);
            exit(0);
        }
    std::string numberStr = (dynamic_cast<NumVal*>(node1))->getStr();
    std::stringstream command;
    WorkReg resReg = regManagment.AllocateReg();
    command << "li ";
    command << "$" << resReg << ", ";
    command << numberStr;
    codeBuffer.emit(command.str());
    DataObj* dataObj = dynamic_cast<DataObj*>(node1);
    return new NonTermByte(resReg);
}

// Exp -> STRING

Node* ExpAction7(RegManagment& regManagment, CodeBuffer& codeBuffer){
    return new NonTermStr();
}

// Exp -> TRUE

Node* ExpAction8(RegManagment& regManagment, CodeBuffer& codeBuffer){
    std::stringstream command;
    WorkReg resReg = regManagment.AllocateReg();
    command << "li ";
    command << "$" << resReg << ", ";
    command << "1";
    codeBuffer.emit(command.str());
    return new NonTermBool(resReg);
}

// Exp -> FALSE

Node* ExpAction9(RegManagment& regManagment, CodeBuffer& codeBuffer){
        std::stringstream command;
    WorkReg resReg = regManagment.AllocateReg();
    command << "li ";
    command << "$" << resReg << ", ";
    command << "1";
    codeBuffer.emit(command.str());
    return new NonTermBool(resReg);
}

// Exp -> Exp AND Exp

Node* ExpAction10(Node* node1 , Node* node2 , Node* node3 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    if(!(NonTermBool::IsValidBoolExp(node1,node2,node3)))
        {
            output::errorMismatch(yylineno);
            exit(0);
             //yyerror("error!");
        }
    DataObj * dataObj1 = dynamic_cast<DataObj*>(node1);
    DataObj * dataObj3 = dynamic_cast<DataObj*>(node3);
    dataObj1->freeWorkReg(regManagment);
    dataObj3->freeWorkReg(regManagment);
    return new NonTermBool(regManagment.AllocateReg());
}

// Exp -> Exp OR Exp

Node* ExpAction11(Node* node1 , Node* node2 , Node* node3 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    if(!(NonTermBool::IsValidBoolExp(node1,node2,node3)))
        {
            output::errorMismatch(yylineno);
            exit(0);
             //yyerror("error!");
        }
    DataObj * dataObj1 = dynamic_cast<DataObj*>(node1);
    DataObj * dataObj3 = dynamic_cast<DataObj*>(node3);
    dataObj1->freeWorkReg(regManagment);
    dataObj3->freeWorkReg(regManagment);
    return new NonTermBool(regManagment.AllocateReg());
}

// Exp -> Exp RELOP Exp

Node* ExpAction12(Node* node1 , Node* node2 , Node* node3 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    if(!(NonTermBool::IsValidBoolExpRelExp(node1,node2,node3)))
        {
            output::errorMismatch(yylineno);
            exit(0);
        }
    DataObj * dataObj1 = dynamic_cast<DataObj*>(node1);
    DataObj * dataObj3 = dynamic_cast<DataObj*>(node3);
    dataObj1->freeWorkReg(regManagment);
    dataObj3->freeWorkReg(regManagment);
    return new NonTermBool(regManagment.AllocateReg());
}

// Exp -> NOT Exp

Node* ExpAction13(Node* node1 , Node* node2 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    if(!(NonTermBool::IsValidBoolExp(node2)))
        {
            output::errorMismatch(yylineno);
            exit(0);
             //yyerror("error!");
        }
    DataObj * dataObj = dynamic_cast<DataObj*>(node2);
    dataObj->freeWorkReg(regManagment);
    return new NonTermBool(regManagment.AllocateReg());
}

//=================================== Handling Home Work's Special Functions ===============================================

void CallToEnterGlobalScope(SymbolTable& symTable){
    symTable.EnterScope();
}

void CallToExitGlobalScope(SymbolTable& symTable){
    output::endScope();
    //printIDsInGlobalScope(symTable);
    symTable.ExitScope();
}

void CallToEnterFunctionScope(SymbolTable& symTable){
    symTable.EnterScope();
    symTable.StartEnteringFuncParas();
}

void CallToExitFunctionScope(SymbolTable& symTable){
    output::endScope();
    FunctionSymbol * funcSym = symTable.GetCurrentFunction();
    //output::printPreconditions(funcSym->GetName(),funcSym->GetPreCondNum());
    //printIDsInFunctionScope(symTable);
    symTable.ExitScope();
}

void CallToEnterInnerScope(SymbolTable& symTable){
    symTable.EnterScope();
}

void CallToExitInnerScope(SymbolTable& symTable){
    output::endScope();
    //printIDsInInnerScope(symTable);
    symTable.ExitScope();
}


void EnterWhile(int &in_while_flag) {
    in_while_flag++;
}

void ExitWhile(int &in_while_flag) {
    in_while_flag--;
}

// assosiated with : FuncDecl -> RetType ID  <MARKER> LPAREN Formals  RPAREN  PreConditions 

void addFunction(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 ,
        Node* node4 , Node* node5 , Node* node6 , Node* node7 , CodeBuffer& codeBuffer){
    symTable.StopEnteringFuncParas();
    TypeNameEnum type = (dynamic_cast<Type*>(node1))->name;
    std::string name = (dynamic_cast<IdVal*>(node2))->GetVal();
    Symbol * sym = symTable.GetSymbol(name);
    if(sym!=NULL){
        output::errorDef(yylineno,name);
        exit(0);
    }
    int num = (node7 != NULL) ? (dynamic_cast<PreCondListObj*>(node7))->GetNumCond() : 0;
    std::list<TypeNameEnum> typesList = (dynamic_cast<ParaListObj*>(node5))->GetParaList();
    //typesList.reverse();
    if(name=="main"){
        if(typesList.size()==0 && type==TYPE_VOID){
            symTable.FoundMainFunc();
        }
        // else {
        //     output::errorMainMissing();
        //     exit(0);
        // }
    }
    FuncDeclToBuffer(name ,codeBuffer);
    symTable.AddFuncSymbol(name,0,TYPE_FUNC,typesList,type,num);
}


void checkIfBoolInWhileIf(Node* node){
    TypeNameEnum type = ExpToTypeName(node);
    if(type != TYPE_BOOL){
        output::errorMismatch(yylineno);
        exit(0);
    }
}

void printIDsInGlobalScope(SymbolTable& symTable){
    std::list<Symbol*> funcList = symTable.GetCurrentScope();
    for(std::list<Symbol*>::iterator it = funcList.begin(); it != funcList.end() ; it++){
        FunctionSymbol* funcSym = dynamic_cast<FunctionSymbol*>(*it);
        const string retTypeStr = TypeToString(funcSym->GetRetType());
        std::vector<std::string> typesVec = ParaListToStrings(funcSym->GetParametersList());
        output::printID(funcSym->GetName(),0,output::makeFunctionType(retTypeStr,typesVec));
        
    }
}

void printIDsInFunctionScope(SymbolTable& symTable){
    FunctionSymbol* funcSym = symTable.GetCurrentFunction();
    int numParas = funcSym->GetParametersList().size();
    std::list<Symbol*> IDList = symTable.GetCurrentScope();
    int i=0;
    int j=0;
    for(std::list<Symbol*>::iterator it_scope = IDList.begin() ; 
        it_scope != IDList.end() ; it_scope++){
            if(i<numParas){
                j--;
                output::printID((*it_scope)->GetName(),j,TypeToString((*it_scope)->GetType()));
                i++;
            }
            else if(i==numParas){
                j=0;
                output::printID((*it_scope)->GetName(),j,TypeToString((*it_scope)->GetType()));
                i++;
            }
            else{
                j++;
                output::printID((*it_scope)->GetName(),j,TypeToString((*it_scope)->GetType()));
            }
        }

}

void printIDsInInnerScope(SymbolTable& symTable){
    std::list<Symbol*> IDList = symTable.GetCurrentScope();
    FunctionSymbol* funcSym = symTable.GetCurrentFunction();
    for(std::list<Symbol*>::iterator it_scope = IDList.begin() ; 
        it_scope != IDList.end() ; it_scope++){
            int memLoc = (*it_scope)->GetIndex();
            memLoc -= funcSym->GetParametersList().size();
            output::printID((*it_scope)->GetName(),memLoc,TypeToString((*it_scope)->GetType()));
        }
}

void mainCheck(SymbolTable& symTable){
    if(!(symTable.IsMainExists())){
        output::errorMainMissing();
        exit(0);
    }
}

//================================================= Buffer Related Functions ============================================================

void BinopCmdToBuffer(WorkReg left , opTypeEnum op , WorkReg right , WorkReg res , RegManagment regManagment , CodeBuffer& codeBuffer){
    std::string opStr;
    switch(op){
        case OP_SUM : opStr="addu";
        case OP_SUB : opStr="subu";
        case OP_MUL : opStr="mul";
        case OP_DIV : opStr="div";
    }
    std::stringstream command;
    command << opStr;
    command << " $" << left << ",";
    command << " $" << right << ",";
    command << res;
    codeBuffer.emit(command.str());
}

void FuncDeclToBuffer(std::string funcName , CodeBuffer& codeBuffer){
    if(funcName != "main"){
		codeBuffer.emit("func_" + funcName + ": ");
	}
	else{
		codeBuffer.emit(funcName + ": ");
	}
	codeBuffer.emit("move $fp, $sp");
}

// Non-Bool

void AddAndAssignNonBoolVarToBuffer(WorkReg reg  , RegManagment& regManagment , CodeBuffer& codeBuffer) {
    codeBuffer.emit("subu $sp, $sp , 4");
	codeBuffer.emit("sw $" + WorkRegEnumToStr(reg) + ", ($sp)");
	regManagment.FreeReg(reg);
}

void AddNonBoolVarToBuffer(RegManagment& regManagment , CodeBuffer& codeBuffer) {
    codeBuffer.emit("subu $sp, $sp , 4");
	WorkReg tempReg = regManagment.AllocateReg();
	codeBuffer.emit("li $" + WorkRegEnumToStr(tempReg) + ", 0");
	codeBuffer.emit("sw $" + WorkRegEnumToStr(tempReg) + ", ($sp)");
	regManagment.FreeReg(tempReg);
}

void AssignNonBoolVarToBuffer(WorkReg reg , int varOffset , RegManagment& regManagment , CodeBuffer& codeBuffer) {
    std::stringstream str;
    str << "sw " << WorkRegEnumToStr(reg) << ", " << varOffset << "($fp)";
    codeBuffer.emit(str.str());
	regManagment.FreeReg(reg);
}

// Bool

void AddAndAssignBoolVarToBuffer(WorkReg reg  , RegManagment& regManagment , CodeBuffer& codeBuffer) {
    codeBuffer.emit("subu $sp, $sp , 4");
	codeBuffer.emit("sw $" + WorkRegEnumToStr(reg) + ", ($sp)");
	regManagment.FreeReg(reg);
}

void AddBoolVarToBuffer(WorkReg reg  , RegManagment& regManagment , CodeBuffer& codeBuffer) {
    codeBuffer.emit("subu $sp, $sp , 4");
	codeBuffer.emit("sw $" + WorkRegEnumToStr(reg) + ", ($sp)");
	regManagment.FreeReg(reg);
}

void AssignBoolVarToBuffer(WorkReg reg  , RegManagment& regManagment , CodeBuffer& codeBuffer) {
    codeBuffer.emit("subu $sp, $sp , 4");
	codeBuffer.emit("sw $" + WorkRegEnumToStr(reg) + ", ($sp)");
	regManagment.FreeReg(reg);
}