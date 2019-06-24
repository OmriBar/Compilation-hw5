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

//PreConditions -> PreConditions M PreCondition

Node* PreConditionsAction2(Node* node1 , Node* node2, Node* node3 , CodeBuffer& codeBuffer) {
    PreCondListObj* preCondObj = dynamic_cast<PreCondListObj*>(node3);
    NonTermMMarker* marker = dynamic_cast<NonTermMMarker*>(node2);
    PreCondListObj* preCondListObj = dynamic_cast<PreCondListObj*>(node1);
    std::vector<int> TrueList;
    std::vector<int> FalseList;
    if(node1 != NULL){
        codeBuffer.bpatch(preCondListObj->GetTrueList(),marker->GetLabel());
        TrueList = preCondObj->GetTrueList();
        FalseList = codeBuffer.merge(preCondObj->GetFalseList(),preCondListObj->GetFalseList());
    }
    else{
        TrueList = preCondObj->GetTrueList();
        FalseList = preCondObj->GetFalseList();
    }
    return new PreCondListObj(dynamic_cast<PreCondListObj*>(node1),TrueList,FalseList);
}

//PreCondition -> PRECOND LPAREN Exp RPAREN

Node* PreConditionAction1(Node* node1 , Node* node2 , Node* node3 , Node* node4 , CodeBuffer& codeBuffer) {
    TypeNameEnum type = ExpToTypeName(node3);
    if(type!=TYPE_BOOL){
        output::errorMismatch(yylineno);
        exit(0);
    }
    NonTermBool* boolExp = dynamic_cast<NonTermBool*>(node3);
    return new PreCondListObj(NULL,boolExp->GetTrueList(),boolExp->GetFalseList());
}

//Statements -> Statment

Node * StatementsAction1(Node* node1){
    return node1;
}

// Statment -> LBRACE <MARKER> Statements <MARKER> RBRACE
Node* StatmentAction1(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3
 , Node* node4 , Node* node5 , RegManagment& regManagment , CodeBuffer& codeBuffer){
     return node3;
 }

//Statements -> Statements Statement

Node* StatementsAction2(Node* node1 , Node* node2 , CodeBuffer& codeBuffer){
    NonTermStatments* nonTermStatments1 = dynamic_cast<NonTermStatments*>(node1);
    NonTermStatments* nonTermStatments2 = dynamic_cast<NonTermStatments*>(node2);
    std::vector<int> breakList = codeBuffer.merge(nonTermStatments1->GetBreakList(),nonTermStatments2->GetBreakList());
    std::vector<int> continueList = codeBuffer.merge(nonTermStatments1->GetContinueList() , nonTermStatments2->GetContinueList());
    return new NonTermStatments(breakList,continueList);

}

// Statment -> LBRACE <MARKER> Statements <MARKER> RBRACE

Node* StatmentAction1(Node * node){
     return node;
  }
   // TODO

//Statment -> Type ID SC

Node* StatmentAction2(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3 ,
 RegManagment& regManagment , CodeBuffer& codeBuffer){
    std::string name = (dynamic_cast<IdVal*>(node2))->IdStr;
    Symbol* sym = symTable.GetSymbol(name);
    if(sym != NULL){
        output::errorDef(yylineno,(dynamic_cast<IdVal*>(node2))->IdStr); // already exists in the symbol table
        exit(0);
    }
    TypeNameEnum type = TypeNameToTypeEnum(node1);
    symTable.AddVariableSymbol(name , symTable.getCurrentIndex()+1 ,type);
    AddVarToBuffer(regManagment,codeBuffer);
    return new NonTermStatments();
}

//Statment -> Type ID ASSIGN Exp SC

Node* StatmentAction3(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3, Node* node4, Node* node5
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
    if(type==TYPE_BOOL){
        NonTermBool* nonTermBool = (dynamic_cast<NonTermBool*>(node4));
        AddAndAssignBoolVarToBuffer(nonTermBool,regManagment,codeBuffer);
    }
    else{
        WorkReg reg = (dynamic_cast<DataObj*>(node4))->getWorkReg();
        AddAndAssignNonBoolVarToBuffer(reg,regManagment,codeBuffer);
        regManagment.FreeReg(reg);
    }
    return new NonTermStatments();
}

//Statment -> ID ASSIGN Exp SC

Node* StatmentAction4(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3, Node* node4
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
    int varOffset = -(sym->GetIndex() * 4);
    if(type==TYPE_BOOL){        
        NonTermBool* nonTermBool = (dynamic_cast<NonTermBool*>(node3));
        AssignBoolVarToBuffer(nonTermBool,varOffset,regManagment,codeBuffer);
    }
    else{
        AssignNonBoolVarToBuffer(reg,varOffset,regManagment,codeBuffer);
        regManagment.FreeReg(reg);
    }
    return new NonTermStatments();
} // To Observe Later

//Statment -> IF_SUFFIX

Node* StatmentAction5(Node* node1 , RegManagment& regManagment , CodeBuffer& codeBuffer){
    NonTermIfSuffix* nonTermIfSuffix = dynamic_cast<NonTermIfSuffix*>(node1);
    NonTermBool* nonTermBool = nonTermIfSuffix->GetNonTermBool();
    NonTermStatments* statments = nonTermIfSuffix->GetStatments();
    int JumpToEndPos = nonTermIfSuffix->GetJumpToEndPos();
    std::string label1 = nonTermIfSuffix->GetLabel();
    std::string label2 = codeBuffer.genLabel();
    codeBuffer.bpatch(nonTermBool->GetTrueList(),label1);
    codeBuffer.bpatch(nonTermBool->GetFalseList(),label2);
    std::string label3 = codeBuffer.genLabel();
    codeBuffer.bpatch(codeBuffer.makelist(JumpToEndPos),label3);
    return statments;
}

//Statment -> IF_SUFFIX ELSE <Marker> M Statement

Node* StatmentAction6(Node* node1 , Node* node2, Node* node3, Node* node4 , Node* node5
 , RegManagment& regManagment , CodeBuffer& codeBuffer){
    NonTermStatments* nonTermStatments2 = dynamic_cast<NonTermStatments*>(node5);
    NonTermIfSuffix* nonTermIfSuffix = dynamic_cast<NonTermIfSuffix*>(node1);
    NonTermStatments* nonTermStatments1= nonTermIfSuffix->GetStatments();
    NonTermBool* nonTermBool = nonTermIfSuffix->GetNonTermBool();
    int JumpToEndPos = nonTermIfSuffix->GetJumpToEndPos();
    std::string label1 = nonTermIfSuffix->GetLabel();
    std::string label2 = (dynamic_cast<NonTermMMarker*>(node4))->GetLabel();
    codeBuffer.bpatch(nonTermBool->GetTrueList(),label1);
    codeBuffer.bpatch(nonTermBool->GetFalseList(),label2);
    std::string label3 = codeBuffer.genLabel();
    codeBuffer.bpatch(codeBuffer.makelist(JumpToEndPos),label3);
    std::vector<int> breakList = codeBuffer.merge(nonTermStatments1->GetBreakList(),nonTermStatments2->GetBreakList());
    std::vector<int> continueList = codeBuffer.merge(nonTermStatments1->GetContinueList(),nonTermStatments2->GetContinueList());
    return new NonTermStatments(breakList,continueList);
} 
 
// Statment -> WHILE LPAREN <MARKER> M Exp <MARKER> RPAREN M Statement

Node* StatmentAction7(Node* node1 , Node* node2, Node* node3, Node* node4 , Node* node5 
, Node* node6 , Node* node7 , Node* node8 , Node* node9 , RegManagment& regManagment , CodeBuffer& codeBuffer){
    NonTermStatments* nonTermStatments = dynamic_cast<NonTermStatments*>(node9);
    NonTermBool* boolExp = dynamic_cast<NonTermBool*>(node5);
    NonTermMMarker* nonTermMMarker1 = dynamic_cast<NonTermMMarker*>(node4);
    NonTermMMarker* nonTermMMarker2 = dynamic_cast<NonTermMMarker*>(node8);
    std::string label1 = nonTermMMarker1->GetLabel();
    std::string label2 = nonTermMMarker2->GetLabel();
    codeBuffer.emit("j "+label1);
    std::string label4 = codeBuffer.genLabel();
    codeBuffer.bpatch(boolExp->GetTrueList(),label2);
    codeBuffer.bpatch(boolExp->GetFalseList(),label4);
    codeBuffer.bpatch(nonTermStatments->GetBreakList(),label4);
    codeBuffer.bpatch(nonTermStatments->GetContinueList(),label1);
    return new NonTermStatments();
    /*
    label1:
        <Caculating Bolean Exp of while>
    label2:
        .....
    label3:
        j label1:
    label4:
     */


}

void ExitScopeStack(SymbolTable& symTable , CodeBuffer& codeBuffer){
    int offset = symTable.GetCurrentScope().size() * 4;
    std::stringstream toStr;
    toStr << offset;
    //std::cout << "heyyyyyyyyyyyyyyyyyyyyyyyyyy" << std::endl;
    codeBuffer.emit("addu $sp , $sp ,"+toStr.str());
}

//Statment -> BREAK SC 

Node* StatmentAction8(int in_while_flag , CodeBuffer& codeBuffer){
    if(in_while_flag<=0){
     output::errorUnexpectedBreak(yylineno);
     exit(0);
    }
    int breakPoint = codeBuffer.emit("j ");
    std::vector<int> breakList = codeBuffer.makelist(breakPoint);
    std::vector<int> continueList = std::vector<int>();
    return new NonTermStatments(breakList,continueList); 
    
}

//Statment -> CONTINUE SC

Node* StatmentAction9(int in_while_flag , CodeBuffer& codeBuffer){
    if(in_while_flag<=0){
     output::errorUnexpectedContinue(yylineno);
     exit(0);
    }
    int continuePoint = codeBuffer.emit("j ");
    std::vector<int> breakList = std::vector<int>();
    std::vector<int> continueList = codeBuffer.makelist(continuePoint);
    return new NonTermStatments(breakList,continueList); 
}

//Statment -> RETURN SC

Node* StatmentAction10(SymbolTable& symTable, CodeBuffer& codeBuffer){
    FunctionSymbol* funcSym = symTable.GetCurrentFunction();
    TypeNameEnum type = funcSym->GetRetType();
    if(!(type==TYPE_VOID)){
        output::errorMismatch(yylineno);
        exit(0);
    }
    int offset = (symTable.GetCurrentFunction()->GetParametersList().size() * 4);
    std::stringstream toStr;
    toStr << offset;
    codeBuffer.emit("move $sp , $fp");
    codeBuffer.emit("jr $ra");
    return new NonTermStatments();
}

//Statment -> RETURN Exp SC

Node* StatmentAction11(SymbolTable& symTable , Node * node1 , Node * node2, RegManagment& regManagment , CodeBuffer& codeBuffer ){
    TypeNameEnum currtype = ExpToTypeName(node2);
    FunctionSymbol* funcSym = symTable.GetCurrentFunction();
    TypeNameEnum definedType = funcSym->GetRetType();
    
    if(!(currtype==definedType) && !(definedType==TYPE_INT && currtype == TYPE_BYTE)){
        output::errorMismatch(yylineno);
        exit(0);
    }
    DataObj * dataObj1 = dynamic_cast<DataObj*>(node2);
    if(currtype == TYPE_BOOL){
        NonTermBool* nonTermBool = dynamic_cast<NonTermBool*>(node2);
        ReturnBool(nonTermBool,  regManagment ,codeBuffer);
    }
    else{
        codeBuffer.emit("move $v0, $"+ WorkRegEnumToStr(dataObj1->getWorkReg()));
        dataObj1->freeWorkReg(regManagment);
        codeBuffer.emit("move $sp , $fp");
        codeBuffer.emit("jr $ra");
    }
    return new NonTermStatments();
}

//Statement -> Call SC

Node* StatmentAction12(SymbolTable& symTable , Node * node1 , RegManagment& regManagment){
    if(dynamic_cast<NonTermVoid*>(node1) == NULL){
        DataObj* dataObj = dynamic_cast<DataObj*>(node1);
        dataObj->freeWorkReg(regManagment);
    }
    return new NonTermStatments();
}

//IF_SUFFIX -> IF LPAREN <Marker> Exp <Marker> RPAREN M Statement

Node* IfActionAction(Node * node1 , Node * node2 , Node * node3 , Node * node4 , Node * node5 , Node * node6 ,Node * node7, Node * node8
 , RegManagment& regManagment , CodeBuffer& codeBuffer){
    if(dynamic_cast<NonTermBool*>(node4)==NULL){
        output::errorMismatch(yylineno);
        exit(0);
    }
    NonTermStatments* statments = dynamic_cast<NonTermStatments*>(node8);
    NonTermBool* boolExp = dynamic_cast<NonTermBool*>(node4);
    NonTermMMarker * nonTermMMarker = dynamic_cast<NonTermMMarker*>(node7);
    int JumpToEndPos = codeBuffer.emit("j ");
    return new NonTermIfSuffix(nonTermMMarker->GetLabel(),boolExp,statments,JumpToEndPos);
}




Node* check(Node* node1,RegManagment& regManagment, CodeBuffer& codeBuffer ){
    TypeNameEnum type = ExpToTypeName(node1);
    if(type == TYPE_BOOL){
        if((dynamic_cast<DataObj*>(node1))->getWorkReg() != NONE){
            (dynamic_cast<DataObj*>(node1))->freeWorkReg(regManagment);
        }
        NonTermBool* nonTermBool = dynamic_cast<NonTermBool*>(node1);
        WorkReg reg = AssignBoolVarToExpFromList(nonTermBool,regManagment,codeBuffer);
        Node* newNonTermBool = TypeNameToExp(TYPE_BOOL,reg);
        return newNonTermBool;
    }
    return node1;
}

//ExpList -> Exp COMMA ExpList 
Node* ExpListAction1(Node* node1 , Node* node2 , Node* node3 , RegManagment& regManagment, CodeBuffer& codeBuffer) {
        TypeNameEnum type = ExpToTypeName(node1);
    //if(type != TYPE_BOOL){
        return new ExpListObj(dynamic_cast<ExpListObj*>(node3),dynamic_cast<DataObj*>(node1));
    //}
    //else {
       // NonTermBool* nonTermBool = dynamic_cast<NonTermBool*>(node1);
       //WorkReg reg = AssignBoolVarToExpFromList(nonTermBool,regManagment,codeBuffer);
        //Node* newNonTermBool = TypeNameToExp(TYPE_BOOL,reg);
       //return new ExpListObj(dynamic_cast<ExpListObj*>(node3),dynamic_cast<DataObj*>(newNonTermBool));
    //}
} 

//ExpList -> Exp

Node* ExpListAction2(Node* node1 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    TypeNameEnum type = ExpToTypeName(node1);
    if(type != TYPE_BOOL){
        return new ExpListObj(dynamic_cast<DataObj*>(node1));
    }
    else {
        NonTermBool* nonTermBool = dynamic_cast<NonTermBool*>(node1);
        WorkReg reg = AssignBoolVarToExpFromList(nonTermBool,regManagment,codeBuffer);
        Node* newNonTermBool = TypeNameToExp(TYPE_BOOL,reg);
        return new ExpListObj(dynamic_cast<DataObj*>(newNonTermBool));
    }
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

Node* CallAction1(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 , Node* node4
 , RegManagment& regManagment , CodeBuffer& codeBuffer){
    Symbol * sym = symTable.GetSymbol((dynamic_cast<IdVal*>(node1))->IdStr);
    if(sym == NULL || sym->GetType() != TYPE_FUNC ){
        output::errorUndefFunc(yylineno,(dynamic_cast<IdVal*>(node1))->IdStr);
        exit(0);
    }
    std::list<TypeNameEnum> symParas = (dynamic_cast<FunctionSymbol*>(sym))->GetParametersList();
    std::list<TypeNameEnum> expListparas = (dynamic_cast<ExpListObj*>(node3))->GetTypesListObj();
    std::vector<string> vector_symParas = ParaListToStrings(symParas);
    if(!AreParaListsEqual(expListparas,symParas)){
        output::errorPrototypeMismatch(yylineno,(dynamic_cast<IdVal*>(node1))->IdStr,vector_symParas);
        exit(0);
        //yyerror("error!");
    }
    FunctionSymbol* funcSym = dynamic_cast<FunctionSymbol*>(sym);
    WorkReg reg;
    if(funcSym->GetName()=="print"){
        std::list<DataObj*> expList = (dynamic_cast<ExpListObj*>(node3))->GetExpListObj();
        NonTermStr* nonTermStr = dynamic_cast<NonTermStr*>(expList.back());
        std::list<WorkReg> currentTakenRegsList = regManagment.GetCurrentTakenRegsList();
        backUpTakenRegisters(currentTakenRegsList,codeBuffer);
        WorkReg ret=callPrintToBuffer(nonTermStr->GetLabel(),regManagment,codeBuffer);
        recoverTakenRegisters(currentTakenRegsList,codeBuffer);
        regManagment.FreeReg(ret);
        return new NonTermVoid();
    }
    else if(funcSym->GetName()=="printi"){
        std::list<DataObj*> expList = (dynamic_cast<ExpListObj*>(node3))->GetExpListObj();
        DataObj* nonTermInt = expList.back();
        std::list<WorkReg> currentTakenRegsList = regManagment.GetCurrentTakenRegsList();
        backUpTakenRegisters(currentTakenRegsList,codeBuffer);
        WorkReg workReg = nonTermInt->getWorkReg();
        callPrintiToBuffer(workReg,regManagment,codeBuffer);
        recoverTakenRegisters(currentTakenRegsList,codeBuffer);
        regManagment.FreeReg(workReg);
        return new NonTermVoid();
    }
    else{
        std::list<DataObj*> expList = (dynamic_cast<ExpListObj*>(node3))->GetExpListObj();
        expList.reverse();
        DataObj* data;
        std::list<WorkReg> currentTakenRegsList = regManagment.GetCurrentTakenRegsList();
        backUpTakenRegisters(currentTakenRegsList,codeBuffer);
        codeBuffer.emit("subu $sp, $sp, 4");
        codeBuffer.emit("sw $fp, 0($sp)");
        codeBuffer.emit("subu $sp, $sp, 4");
        codeBuffer.emit("sw $ra, 0($sp)");
        for(std::list<DataObj*>::iterator it = expList.begin(); it != expList.end(); it++){
            data = *it;
            codeBuffer.emit("subu $sp, $sp, 4");
            codeBuffer.emit("sw $" +WorkRegEnumToStr(data->getWorkReg()) + ", ($sp)");
            regManagment.FreeReg(data->getWorkReg());
        }
        ExpListObj* expListObj = dynamic_cast<ExpListObj*>(node3);
        //expListObj->freeAllocRegisters(regManagment);
        codeBuffer.emit("jal func_"+funcSym->GetName());
        std::stringstream toStrSizeStack;
        toStrSizeStack <<(expList.size()*4);   
        codeBuffer.emit("addu $sp, $sp, " + toStrSizeStack.str());
        codeBuffer.emit("lw $ra, 0($sp)");
        codeBuffer.emit("lw $fp, 4($sp)");
        codeBuffer.emit("addu $sp, $sp, 8");
        recoverTakenRegisters(currentTakenRegsList,codeBuffer);
        if(funcSym->GetRetType() != TYPE_VOID){
            reg = regManagment.AllocateReg();
            codeBuffer.emit("move $" + WorkRegEnumToStr(reg) + ", $v0");
            return TypeNameToExp(funcSym->GetRetType(),reg);
        }
        return new NonTermVoid();
    }
}

void backUpTakenRegisters(std::list<WorkReg> currentTakenRegsList,CodeBuffer& codeBuffer){
    WorkReg reg;
    for(std::list<WorkReg>::iterator it = currentTakenRegsList.begin(); it != currentTakenRegsList.end(); it++){
        reg= *it;
        codeBuffer.emit("subu $sp, $sp, 4");
        codeBuffer.emit("sw $" + WorkRegEnumToStr(reg) + ", 0($sp)");
    }
}

void recoverTakenRegisters(std::list<WorkReg> currentTakenRegsList,CodeBuffer& codeBuffer){
    currentTakenRegsList.reverse();
    WorkReg reg;
    for(std::list<WorkReg>::iterator it = currentTakenRegsList.begin(); it != currentTakenRegsList.end(); it++){
        reg= *it;
        codeBuffer.emit("lw $"+ WorkRegEnumToStr(reg) +", 0($sp)");
        codeBuffer.emit("addu $sp, $sp, 4");
    }
}


// Call -> ID LPAREN RPAREN

Node* CallAction2(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    Symbol * sym = symTable.GetSymbol((dynamic_cast<IdVal*>(node1))->IdStr);
    if(sym == NULL || sym->GetType() != TYPE_FUNC ){
        output::errorUndefFunc(yylineno,(dynamic_cast<IdVal*>(node1))->IdStr);
        exit(0);
    }
    WorkReg reg;
    std::list<TypeNameEnum> symParas = (dynamic_cast<FunctionSymbol*>(sym))->GetParametersList(); 
    std::list<TypeNameEnum> expListparas = std::list<TypeNameEnum>();
    if(symParas != expListparas){
        std::vector<string> vector_symParas = ParaListToStrings(symParas);
        output::errorPrototypeMismatch(yylineno,(dynamic_cast<IdVal*>(node1))->IdStr,vector_symParas);
        exit(0);
    }
    FunctionSymbol* funcSym = dynamic_cast<FunctionSymbol*>(sym);
    std::list<WorkReg> currentTakenRegsList = regManagment.GetCurrentTakenRegsList();
    backUpTakenRegisters(currentTakenRegsList,codeBuffer);
    codeBuffer.emit("subu $sp, $sp, 4");
    codeBuffer.emit("sw $fp, 0($sp)");
    codeBuffer.emit("subu $sp, $sp, 4");
    codeBuffer.emit("sw $ra, 0($sp)");
    codeBuffer.emit("jal func_"+funcSym->GetName());    
    codeBuffer.emit("lw $ra, 0($sp)");
    codeBuffer.emit("lw $fp, 4($sp)");
    codeBuffer.emit("addu $sp, $sp, 8");
    recoverTakenRegisters(currentTakenRegsList,codeBuffer);
    if(funcSym->GetRetType() != TYPE_VOID){
        reg = regManagment.AllocateReg();
        codeBuffer.emit("move $"+WorkRegEnumToStr(reg)+", $v0");
        return TypeNameToExp(funcSym->GetRetType(),reg);
    }
    return new NonTermVoid();
    
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

Node* ExpAction1(Node* node1 , Node* node2 , Node* node3 , RegManagment& regManagment, CodeBuffer& codeBuffer) {
    /* TypeNameEnum type = ExpToTypeName(node2);
    if(type != TYPE_BOOL){
            output::errorMismatch(yylineno);
            exit(0);
        }
    DataObj * dataObj = dynamic_cast<DataObj*>(node2);
    return TypeNameToExp(TYPE_BOOL,dataObj->getWorkReg());*/
    return node2;
}

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
    bool IsTheResByte = (dynamic_cast<NonTermByte*>(node1) != NULL) && (dynamic_cast<NonTermByte*>(node3) != NULL);
    opTypeEnum op = (dynamic_cast<BinaryOp*>(node2))->getType();
    BinopCmdToBuffer(dataObj1->getWorkReg(),op,dataObj3->getWorkReg(),resReg,regManagment,codeBuffer);
    if(IsTheResByte){
        codeBuffer.emit("and $"+WorkRegEnumToStr(resReg)+", $"+WorkRegEnumToStr(resReg)+", 0x000000ff");
    }
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
    int offset = -(sym->GetIndex() * 4);
    std::stringstream toStr;
    toStr << offset;
    WorkReg reg = regManagment.AllocateReg();
    codeBuffer.emit("lw $"+WorkRegEnumToStr(reg)+", "+ toStr.str() + "($fp)");
    if(sym->GetType() == TYPE_BOOL){
        int falseListPos = codeBuffer.emit("beq $"+WorkRegEnumToStr(reg)+", 0, ");
        int trueListPos = codeBuffer.emit("j ");
        std::vector<int> falseList = codeBuffer.makelist(falseListPos);
        std::vector<int> trueList = codeBuffer.makelist(trueListPos);
        return new NonTermBool(trueList,falseList);
    }
    else{

    }
    return TypeNameToExp(sym->GetType(),reg);
}

// Exp -> Call 

Node* ExpAction4(Node* node , RegManagment& regManagment, CodeBuffer& codeBuffer){
    if(dynamic_cast<NonTermBool*>(node) == NULL){
      return node;  
    }
    NonTermBool* boolExp = dynamic_cast<NonTermBool*>(node);
    WorkReg reg = boolExp->getWorkReg();
    int falseListPos = codeBuffer.emit("beq $"+WorkRegEnumToStr(reg)+", 0, ");
    int trueListPos = codeBuffer.emit("j ");
    std::vector<int> falseList = codeBuffer.makelist(falseListPos);
    std::vector<int> trueList = codeBuffer.makelist(trueListPos);
    regManagment.FreeReg(reg);
    return new NonTermBool(trueList,falseList);
}

// Exp -> NUM

Node* ExpAction5(Node* node , RegManagment& regManagment, CodeBuffer& codeBuffer){
    //regManagment.printFree();
    std::string numberStr = (dynamic_cast<NumVal*>(node))->getStr();
    std::stringstream command;
    WorkReg resReg = regManagment.AllocateReg();
    command << "li ";
    command << "$" << WorkRegEnumToStr(resReg) << ", ";
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
    command << "$" << WorkRegEnumToStr(resReg) << ", ";
    command << numberStr;
    codeBuffer.emit(command.str());
    DataObj* dataObj = dynamic_cast<DataObj*>(node1);
    return new NonTermByte(resReg);
}

// Exp -> STRING

Node* ExpAction7(Node* node1 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    std::string text = (dynamic_cast<StrVal*>(node1))->GetStr();
    std::string label = SaveStringToData(text,regManagment,codeBuffer);
    return new NonTermStr(label);
}

// Exp -> TRUE

Node* ExpAction8(RegManagment& regManagment, CodeBuffer& codeBuffer){
    int truthAddr = codeBuffer.emit("j ");
    std::vector<int> trueList;
    std::vector<int> falseList;
    trueList.push_back(truthAddr);
    return new NonTermBool(trueList,falseList);
}

// Exp -> FALSE

Node* ExpAction9(RegManagment& regManagment, CodeBuffer& codeBuffer){
    int falseAddr = codeBuffer.emit("j ");
    std::vector<int> trueList;
    std::vector<int> falseList;
    falseList.push_back(falseAddr);
    return new NonTermBool(trueList,falseList);
}

// Exp -> Exp AND M Exp

Node* ExpAction10(Node* node1 , Node* node2 , Node* node3 , Node* node4 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    if(!(NonTermBool::IsValidBoolExp(node1,node2,node4)))
        {
            output::errorMismatch(yylineno);
            exit(0);
        }
    NonTermBool* left = dynamic_cast<NonTermBool*>(node1);
    NonTermBool* right = dynamic_cast<NonTermBool*>(node4);
    NonTermMMarker* nonTermMMarker = dynamic_cast<NonTermMMarker*>(node3);
    std::vector<int> trueList = right->GetTrueList();
    std::vector<int> falseList = codeBuffer.merge(left->GetFalseList(),right->GetFalseList());
    codeBuffer.bpatch(left->GetTrueList(),nonTermMMarker->GetLabel());
    return new NonTermBool(trueList,falseList);

}

// Exp -> Exp OR M Exp

Node* ExpAction11(Node* node1 , Node* node2 , Node* node3 , Node* node4, RegManagment& regManagment, CodeBuffer& codeBuffer){
    if(!(NonTermBool::IsValidBoolExp(node1,node2,node4)))
        {
            output::errorMismatch(yylineno);
            exit(0);
        }
    NonTermBool* left = dynamic_cast<NonTermBool*>(node1);
    NonTermBool* right = dynamic_cast<NonTermBool*>(node4);
    NonTermMMarker* nonTermMMarker = dynamic_cast<NonTermMMarker*>(node3);
    std::vector<int> trueList = codeBuffer.merge(left->GetTrueList(),right->GetTrueList());
    std::vector<int> falseList = right->GetFalseList();
    codeBuffer.bpatch(left->GetFalseList(),nonTermMMarker->GetLabel());
    return new NonTermBool(trueList,falseList);
}

// Exp -> Exp RELOP Exp

Node* ExpAction12(Node* node1 , Node* node2 , Node* node3 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    if(!(NonTermBool::IsValidBoolExpRelExp(node1,node2,node3)))
        {
            output::errorMismatch(yylineno);
            exit(0);
        }
    WorkReg leftReg = (dynamic_cast<DataObj*>(node1))->getWorkReg();
    WorkReg rightReg =(dynamic_cast<DataObj*>(node3))->getWorkReg();
    opTypeEnum op = (dynamic_cast<RelativeOp*>(node2))->getType();
    NonTermBool* nonTermBool = RelopCmdToBuffer(leftReg,op,rightReg,regManagment,codeBuffer);
    (dynamic_cast<DataObj*>(node1))->freeWorkReg(regManagment);
    (dynamic_cast<DataObj*>(node3))->freeWorkReg(regManagment);
    return nonTermBool;
}

// Exp -> NOT Exp

Node* ExpAction13(Node* node1 , Node* node2 , RegManagment& regManagment, CodeBuffer& codeBuffer){
    if(!(NonTermBool::IsValidBoolExp(node2)))
        {
            output::errorMismatch(yylineno);
            exit(0);
        }
    NonTermBool* nonTermBool = dynamic_cast<NonTermBool*>(node2);
    std::vector<int> trueList = nonTermBool->GetFalseList();
    std::vector<int> falseList = nonTermBool->GetTrueList();
    return new NonTermBool(trueList,falseList);
}

Node * MMarkerAction(CodeBuffer& codeBuffer){
    NonTermMMarker* nonTermMMarker = new NonTermMMarker(codeBuffer.genLabel());
    return nonTermMMarker;
}

//=================================== Handling Home Work's Special Functions ===============================================

void CallToEnterGlobalScope(SymbolTable& symTable){
    symTable.EnterScope();
}

void CallToExitGlobalScope(SymbolTable& symTable){
    //output::endScope();
    //printIDsInGlobalScope(symTable);
    symTable.ExitScope();
}

void CallToEnterFunctionScope(SymbolTable& symTable){
    symTable.EnterScope();
    symTable.StartEnteringFuncParas();
}

void CallToExitFunctionScope(SymbolTable& symTable, CodeBuffer& codeBuffer){
    //output::endScope();
    //FunctionSymbol * funcSym = symTable.GetCurrentFunction();
    //if(funcSym->GetName()=="main"){
    //    //codeBuffer.emit("move $sp, $fp");
    //    codeBuffer.emit("li $v0, 10");
	//	codeBuffer.emit("syscall");
    //}
    //output::printPreconditions(funcSym->GetName(),funcSym->GetPreCondNum());
    //printIDsInFunctionScope(symTable);
    symTable.ExitScope();
}

void CallToEnterInnerScope(SymbolTable& symTable){
    symTable.EnterScope();
}

void CallToExitInnerScope(SymbolTable& symTable){
    //output::endScope();
    //printIDsInInnerScope(symTable);
    symTable.ExitScope();
}


void EnterWhile(int &in_while_flag) {
    in_while_flag++;
}

void ExitWhile(int &in_while_flag) {
    in_while_flag--;
}

// assosiated with : FuncDecl -> RetType ID  <MARKER> LPAREN Formals  RPAREN <MARKER> PreConditions 

void addFunction(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 ,
        Node* node4 , Node* node5 , Node* node6 , Node* node7 , Node* node8 , RegManagment& regManagment , CodeBuffer& codeBuffer){
    symTable.StopEnteringFuncParas();
    TypeNameEnum type = (dynamic_cast<Type*>(node1))->name;
    std::string name = (dynamic_cast<IdVal*>(node2))->GetVal();
    Symbol * sym = symTable.GetSymbol(name);
    if(sym!=NULL){
        output::errorDef(yylineno,name);
        exit(0);
    }
    int num = (node8 != NULL) ? (dynamic_cast<PreCondListObj*>(node8))->GetNumCond() : 0;
    std::list<TypeNameEnum> typesList = (dynamic_cast<ParaListObj*>(node5))->GetParaList();
    //typesList.reverse();
    if(name=="main"){
        if(typesList.size()==0 && type==TYPE_VOID){
            symTable.FoundMainFunc();
        }
    }
    PreCondListObj* paraListObj = dynamic_cast<PreCondListObj*>(node8);
    FuncDeclToBuffer(symTable,name,codeBuffer,regManagment,paraListObj);
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

void BinopCmdToBuffer(WorkReg left , opTypeEnum op , WorkReg right , WorkReg res , RegManagment& regManagment , CodeBuffer& codeBuffer){
    if(op == OP_DIV){
        int JumpPoint = codeBuffer.emit("bne $"+WorkRegEnumToStr(right)+",0 ");
        PrintErrorMsToBuffer("DivideZeroError",regManagment,codeBuffer);
        codeBuffer.emit("li $v0, 10");
	    codeBuffer.emit("syscall");
        std::string JumpLabel = codeBuffer.genLabel();
        codeBuffer.bpatch(codeBuffer.makelist(JumpPoint),JumpLabel);
    }
    std::string opStr;
    switch(op){
        case OP_SUM : opStr="addu";break;
        case OP_SUB : opStr="subu";break;
        case OP_MUL : opStr="mul";break;
        case OP_DIV : opStr="div";break;
    }
    std::stringstream command;
    command << opStr;
    command << " $" << WorkRegEnumToStr(res) << ",";
    command << " $" << WorkRegEnumToStr(left) << ",";
    command << " $" << WorkRegEnumToStr(right);
    codeBuffer.emit(command.str());
}
NonTermBool* RelopCmdToBuffer(WorkReg left , opTypeEnum op , WorkReg right, RegManagment& regManagment , CodeBuffer& codeBuffer){
    int trueListAddr;
    int falseListAddr;
    std::string opStr;
    switch(op){
        case OP_EQU : opStr="beq";break;
        case OP_NEQ : opStr="bne";break;
        case OP_LEFTBIGEQ : opStr="bge";break;
        case OP_LEFTBIG : opStr="bgt";break;
        case OP_RIGHTBIGEQ : opStr="ble"; break;
        case OP_RIGHTBIG : opStr="blt"; break;
    }
    std::stringstream command1;
    command1 << opStr;
    command1 << " $" << WorkRegEnumToStr(left) << ",";
    command1 << " $" << WorkRegEnumToStr(right) << ",";
    std::stringstream command2;
    command2 << "j ";
    trueListAddr = codeBuffer.emit(command1.str());
    falseListAddr = codeBuffer.emit(command2.str());
    std::vector<int> trueList = codeBuffer.makelist(trueListAddr);
    std::vector<int> falseList = codeBuffer.makelist(falseListAddr);
    return new NonTermBool(trueList,falseList);
}

//====== Function decleration Related Functions ==============

void FuncLabelToBuffer(Node * node2 , CodeBuffer& codeBuffer){
    std::string funcName = (dynamic_cast<IdVal*>(node2))->GetVal();
    if(funcName != "main"){
		codeBuffer.emit("func_" + funcName + ": ");
	}
	else{
		codeBuffer.emit(funcName + ": ");
	}
    codeBuffer.emit("move $fp, $sp");
}

void FuncDeclToBuffer(SymbolTable& symTable , std::string funcName , CodeBuffer& codeBuffer ,
 RegManagment& regManagment , PreCondListObj* paraListObj){
	
    if(paraListObj != NULL){
        //_FalseLabel_:
        codeBuffer.bpatch(paraListObj->GetFalseList(),codeBuffer.genLabel());
        //          li $<tempReg>, 1
        PrintErrorMsToBuffer("preCondError",regManagment,codeBuffer);
        codeBuffer.emitData("funcName_"+funcName+": .asciiz \""+funcName+"\"");
        callPrintToBuffer("funcName_"+funcName,regManagment,codeBuffer);
        codeBuffer.emit("li $v0, 10");
	    codeBuffer.emit("syscall");
        // _TrueLabel_:
	    codeBuffer.bpatch(paraListObj->GetTrueList(), codeBuffer.genLabel());
    }
    if(funcName!= "main"){
        int offset = (symTable.GetCurrentFunction()->GetParametersList().size() * 4);
        std::stringstream toStr;
        toStr << offset;
        //codeBuffer.emit("sw $ra , "+toStr.str()+"($fp)");
    }
}

void AddFuncPrintPrintiToBuffer(CodeBuffer& codeBuffer){
    codeBuffer.emit("printi:");
	codeBuffer.emit("lw $a0, 0($sp)");
	codeBuffer.emit("li $v0, 1");
	codeBuffer.emit("syscall");
	codeBuffer.emit("jr $ra");

	codeBuffer.emit("print:");
	codeBuffer.emit("lw $a0, 0($sp)");
	codeBuffer.emit("li $v0, 4");
	codeBuffer.emit("syscall");
	codeBuffer.emit("jr $ra");
}

std::string SaveStringToData(std::string text  , RegManagment& regManagment , CodeBuffer& codeBuffer){
    static int stringCounter=0;
    std::stringstream counterToStr;
    counterToStr << (stringCounter++);
    std::string stringCounterStr = counterToStr.str();
    std::stringstream label;
    label << "_strData" << (stringCounter++) << "_";
    codeBuffer.emitData( label.str() + ": .asciiz " + text);
    return label.str();
}

WorkReg callPrintToBuffer(std::string label , RegManagment& regManagment , CodeBuffer& codeBuffer){
        codeBuffer.emit("subu $sp, $sp, 4");
        codeBuffer.emit("sw $fp, 0($sp)");
        codeBuffer.emit("subu $sp, $sp, 4");
        codeBuffer.emit("sw $ra, 0($sp)");
        codeBuffer.emit("subu $sp, $sp , 4");
		WorkReg tempReg = regManagment.AllocateReg();
		codeBuffer.emit("la $"+ WorkRegEnumToStr(tempReg) + ", " + label);
		codeBuffer.emit("sw $" + WorkRegEnumToStr(tempReg) + ", 0($sp)");
		codeBuffer.emit("jal print");
        codeBuffer.emit("addu $sp, $sp, 4");
        codeBuffer.emit("lw $ra, 0($sp)");
        codeBuffer.emit("lw $fp, 4($sp)");
        codeBuffer.emit("addu $sp, $sp, 8");
        return tempReg;
}


void callPrintiToBuffer(WorkReg workReg , RegManagment& regManagment , CodeBuffer& codeBuffer){
        codeBuffer.emit("subu $sp, $sp, 4");
        codeBuffer.emit("sw $fp, 0($sp)");
        codeBuffer.emit("subu $sp, $sp, 4");
        codeBuffer.emit("sw $ra, 0($sp)");
        codeBuffer.emit("subu $sp, $sp , 4");
		codeBuffer.emit("sw $" + WorkRegEnumToStr(workReg) + ", 0($sp)");
		codeBuffer.emit("jal printi");
        codeBuffer.emit("addu $sp, $sp, 4");
        codeBuffer.emit("lw $ra, 0($sp)");
        codeBuffer.emit("lw $fp, 4($sp)");
        codeBuffer.emit("addu $sp, $sp, 8");
}


//=========== Variables Related Functions ====================

// Non-Bool

void AddAndAssignNonBoolVarToBuffer(WorkReg reg  , RegManagment& regManagment , CodeBuffer& codeBuffer) {
    codeBuffer.emit("subu $sp, $sp , 4");
	codeBuffer.emit("sw $" + WorkRegEnumToStr(reg) + ", ($sp)");
}

void AddVarToBuffer(RegManagment& regManagment , CodeBuffer& codeBuffer) {
    codeBuffer.emit("subu $sp, $sp , 4");
	WorkReg tempReg = regManagment.AllocateReg();
	codeBuffer.emit("li $" + WorkRegEnumToStr(tempReg) + ", 0");
	codeBuffer.emit("sw $" + WorkRegEnumToStr(tempReg) + ", ($sp)");
	regManagment.FreeReg(tempReg);
}

void AssignNonBoolVarToBuffer(WorkReg reg , int varOffset , RegManagment& regManagment , CodeBuffer& codeBuffer) {
    std::stringstream str;
    str << "sw $" << WorkRegEnumToStr(reg) << ", " << varOffset << "($fp)";
    codeBuffer.emit(str.str());
}

// Bool

void AddAndAssignBoolVarToBuffer(NonTermBool* nonTermBool , RegManagment& regManagment , CodeBuffer& codeBuffer) {
    WorkReg tempReg = regManagment.AllocateReg();
    //_TrueLabel_:
    codeBuffer.bpatch(nonTermBool->GetTrueList(),codeBuffer.genLabel());
    //          li $<tempReg>, 1
    codeBuffer.emit("li $"+WorkRegEnumToStr(tempReg)+", 1");
    //          j _EndLabel_
    int JumpToEnd = codeBuffer.emit("j ");
    // _FalseLabel_:
	codeBuffer.bpatch(nonTermBool->GetFalseList(), codeBuffer.genLabel());
    //          li $<TempReg>, 0
	codeBuffer.emit("li $" + WorkRegEnumToStr(tempReg) + ", 0");
    // _EndLabel_:
	codeBuffer.bpatch(codeBuffer.makelist(JumpToEnd), codeBuffer.genLabel());
    codeBuffer.emit("subu $sp, $sp , 4");
	codeBuffer.emit("sw $" + WorkRegEnumToStr(tempReg) + ", ($sp)");
    regManagment.FreeReg(tempReg);
}

void AssignBoolVarToBuffer(NonTermBool* nonTermBool , int offset  , RegManagment& regManagment , CodeBuffer& codeBuffer) {
    std::stringstream toStr;
    toStr << offset;
    WorkReg tempReg = regManagment.AllocateReg();
    //_TrueLabel_:
    codeBuffer.bpatch(nonTermBool->GetTrueList(),codeBuffer.genLabel());
    //          li $<tempReg>, 1
    codeBuffer.emit("li $"+WorkRegEnumToStr(tempReg)+", 1");
    //          j _EndLabel_
    int JumpToEnd = codeBuffer.emit("j ");
    // _FalseLabel_:
	codeBuffer.bpatch(nonTermBool->GetFalseList(), codeBuffer.genLabel());
    //          li $<TempReg>, 0
	codeBuffer.emit("li $" + WorkRegEnumToStr(tempReg) + ", 0");
    // _EndLabel_:
	codeBuffer.bpatch(codeBuffer.makelist(JumpToEnd), codeBuffer.genLabel());
	codeBuffer.emit("sw $" + WorkRegEnumToStr(tempReg) + ", " + toStr.str() + "($fp)");
    regManagment.FreeReg(tempReg);
}

WorkReg AssignBoolVarToExpFromList(NonTermBool* nonTermBool, RegManagment& regManagment , CodeBuffer& codeBuffer) {
    WorkReg reg = regManagment.AllocateReg();
    //_TrueLabel_:
    codeBuffer.bpatch(nonTermBool->GetTrueList(),codeBuffer.genLabel());
    //          li $<tempReg>, 1
    codeBuffer.emit("li $"+WorkRegEnumToStr(reg)+", 1");
    //          j _EndLabel_
    int JumpToEnd = codeBuffer.emit("j ");
    // _FalseLabel_:
	codeBuffer.bpatch(nonTermBool->GetFalseList(), codeBuffer.genLabel());
    //          li $<TempReg>, 0
	codeBuffer.emit("li $" + WorkRegEnumToStr(reg) + ", 0");
    // _EndLabel_:
	codeBuffer.bpatch(codeBuffer.makelist(JumpToEnd), codeBuffer.genLabel());
    return reg;
}

void FuncEndToBuffer(SymbolTable& symTable , Node * node2 , CodeBuffer& codeBuffer){
    std::string name = (dynamic_cast<IdVal*>(node2))->GetVal();
    codeBuffer.emit("move $sp , $fp");
    codeBuffer.emit("jr $ra");
}

void PrintErrorMsToBuffer(std::string MsgLabelStr , RegManagment& regManagment , CodeBuffer& codeBuffer){
    WorkReg reg = regManagment.AllocateReg();
    codeBuffer.emit("subu $sp, $sp, 4");
    codeBuffer.emit("sw $fp, 0($sp)");
    codeBuffer.emit("subu $sp, $sp, 4");
    codeBuffer.emit("sw $ra, 0($sp)");
     codeBuffer.emit("subu $sp, $sp , 4");
    codeBuffer.emit("la $" + WorkRegEnumToStr(reg) + " , " + MsgLabelStr);
    codeBuffer.emit("sw $" + WorkRegEnumToStr(reg) + " , 0($sp) ");
    codeBuffer.emit("jal print");
	codeBuffer.emit("addu $sp, $sp, 4");
    codeBuffer.emit("lw $ra, 0($sp)");
    codeBuffer.emit("lw $fp, 4($sp)");
    codeBuffer.emit("addu $sp, $sp, 8");
    regManagment.FreeReg(reg);
}

void PrintDataToBuffer(CodeBuffer& codeBuffer){
    codeBuffer.emitData("preCondError: .asciiz \"Precondition hasn't been satisfied for function \"");
    codeBuffer.emitData("DivideZeroError: .asciiz \"Error division by zero\n\"");
}

void ReturnBool(NonTermBool* nonTermBool, RegManagment& regManagment , CodeBuffer& codeBuffer) {
    //_TrueLabel_:
    codeBuffer.bpatch(nonTermBool->GetTrueList(),codeBuffer.genLabel());
    //          li $<tempReg>, 1
    codeBuffer.emit("li $v0, 1");
    //          j _EndLabel_
    int JumpToEnd = codeBuffer.emit("j ");
    // _FalseLabel_:
	codeBuffer.bpatch(nonTermBool->GetFalseList(), codeBuffer.genLabel());
    //          li $<TempReg>, 0
	codeBuffer.emit("li $v0, 0");
    // _EndLabel_:
	codeBuffer.bpatch(codeBuffer.makelist(JumpToEnd), codeBuffer.genLabel());
    codeBuffer.emit("move $sp , $fp");
    codeBuffer.emit("jr $ra");
}