#include "SymbolTable.h"
#include <stdlib.h> 
SymbolTable::SymbolTable() : isThereMain(false), CurrentRetType(TYPE_NONEXIST) , ScopesList(), CurrentScopeLevel(0) ,CurrentIndexInScope(0) {
    ScopesList.push_back(std::list<Symbol*>());
    std::list<TypeNameEnum> typesList1 = std::list<TypeNameEnum>();
    typesList1.push_back(TYPE_STR);
    AddFuncSymbol("print",0,TYPE_FUNC,typesList1,TYPE_VOID,0);
    std::list<TypeNameEnum> typesList2 = std::list<TypeNameEnum>();
    typesList2.push_back(TYPE_INT);
    AddFuncSymbol("printi",0,TYPE_FUNC,typesList2,TYPE_VOID,0);
}

void SymbolTable::SetCurrentRetType(Node* node){
    CurrentRetType = TypeNameToTypeEnum(node);
}

TypeNameEnum SymbolTable::GetCurrentRetType(){
    return CurrentRetType;
}

void SymbolTable::EnterScope() {
    ScopesList.push_back(std::list<Symbol*>());
}

bool SymbolTable::ExitScope() {
    for(std::list<Symbol*>::iterator it_scope = ScopesList.back().begin() ; 
                it_scope != ScopesList.back().end() ; it_scope++) {
                    delete (*it_scope);
                }
    ScopesList.pop_back();
}

int SymbolTable::getCurrentIndex(){
    return ScopesList.back().size();
}

void SymbolTable::AddSymbol(std::string name , int index , TypeNameEnum type){
    ScopesList.back().push_back(new Symbol(name,GetNCurrIndexInMem(),type));
}

void SymbolTable::AddVariableSymbol(std::string name , int index , TypeNameEnum type){
    ScopesList.back().push_back(new VariableSymbol(name,GetNCurrIndexInMem(),type));
}

void SymbolTable::AddFuncSymbol(std::string name , int index , TypeNameEnum type ,
 std::list<TypeNameEnum> parametersList , TypeNameEnum retType , int precondNum){

    ScopesList.front().push_back(new FunctionSymbol(name,index,type,parametersList,retType,precondNum));  // sould be changed for Functions!
}

FunctionSymbol* SymbolTable::GetCurrentFunction() {
    return dynamic_cast<FunctionSymbol*>(ScopesList.front().back());

}

void SymbolTable::FoundMainFunc(){
    isThereMain=true;
}

bool SymbolTable::IsMainExists(){
    return isThereMain;
}

Symbol* SymbolTable::GetSymbol(std::string name){
    for(std::list<std::list<Symbol*> >::iterator it_scopes = ScopesList.begin() ; 
        it_scopes != ScopesList.end() ; it_scopes++){
            for(std::list<Symbol*>::iterator it_scope = it_scopes->begin() ; 
                it_scope != it_scopes->end() ; it_scope++) {
                    if((*it_scope)->GetName() == name){
                        Symbol* ptr = (*it_scope);
                        if(dynamic_cast<VariableSymbol*>(ptr) != NULL){
                            VariableSymbol * varPtr = dynamic_cast<VariableSymbol*>(ptr);
                            Symbol * copiedSymbolPtr = new VariableSymbol(*varPtr);
                            return copiedSymbolPtr; 
                        }
                        else if(dynamic_cast<FunctionSymbol*>(ptr) != NULL){
                            FunctionSymbol * funcPtr = dynamic_cast<FunctionSymbol*>(ptr);
                            Symbol * copiedSymbolPtr = new FunctionSymbol(*funcPtr); 
                            return copiedSymbolPtr;
                        }
                    }
                }
        }
    return NULL;
}

void SymbolTable::printTable(){
     for(std::list<std::list<Symbol*> >::iterator it_scopes = ScopesList.begin() ; 
        it_scopes != ScopesList.end() ; it_scopes++){
            for(std::list<Symbol*>::iterator it_scope = it_scopes->begin() ; 
                it_scope != it_scopes->end() ; it_scope++) {
                    std::cout << "symbol name: " << (*it_scope)->GetName() << std::endl;
                }
        }
}

std::list<Symbol*> SymbolTable::GetCurrentScope(){
    return ScopesList.back();
}

int SymbolTable::GetNCurrIndexInMem(){
    int numOfVars = 0;
    std::list<std::list<Symbol*> >::iterator it = ScopesList.begin();
    int tmp = (*it).size();
    for(it = ScopesList.begin();
    it != ScopesList.end() ; it++){
        numOfVars += (*it).size();
    }
    numOfVars -= ScopesList.front().size();
    //std::cout<<std::endl << ScopesList.front().size() << std::endl;

}



