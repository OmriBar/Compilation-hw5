#ifndef SEMANTIC_ACTIONS_H
#define SEMANTIC_ACTIONS_H

#include "attributes.h"
#include "SymbolTable.h"
#include "RegManagment.h"
#include <sstream>
#include <iostream>
#include <string>
#include "bp.hpp"
#include <stdlib.h> 

extern int yylineno;
extern Node* yylval;

// RetType -> TYPE 
Node * RetTypeAction1(Node * node1);
// RetType -> VOID 
Node * RetTypeAction2();
// Formals -> epsilon
Node * FormalsAction1();
// Formals -> FormalsList
Node * FormalsAction2(Node * node1);
//FormalsList -> FormalDecl
Node * FormalsListAction1(Node * node1);
//FormalsList ->  FormalDecl COMMA FormalsList
Node * FormalsListAction2(Node * node1 , Node * node2 , Node * node3);
//FormalDecl -> Type ID
Node * FormalDeclAction1(SymbolTable& symTable , Node * node1 , Node * node2);
// PreConditions -> epsilon
Node* PreConditionsAction1();
// PreConditions -> PreConditions PreCondition
Node* PreConditionsAction2(Node* node1 , Node* node2);
// PreCondition -> PRECOND LPAREN Exp RPAREN
void PreConditionAction1(Node* node1 , Node* node2 , Node* node3 , Node* node4);
// Statment -> LBRACE <MARKER> Statements <MARKER> RBRACE
Node* StatmentAction1(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3
 , Node* node4 , Node* node5 , RegManagment& regManagment , CodeBuffer& codeBuffer);
// Statment -> Type ID SC
Node* StatmentAction2(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3
 , RegManagment& regManagment , CodeBuffer& codeBuffer);
// Statment -> Type ID ASSIGN Exp SC
Node* StatmentAction3(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3
, Node* node4, Node* node5 , RegManagment& regManagment , CodeBuffer& codeBuffer);
//Statment -> ID ASSIGN Exp SC
Node* StatmentAction4(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3
, Node* node4 , RegManagment& regManagment , CodeBuffer& codeBuffer);
//Statment -> IF_SUFFIX
Node* StatmentAction5(Node* node1 , RegManagment& regManagment , CodeBuffer& codeBuffer);
//Statment -> IF_SUFFIX ELSE <Marker> M Statement
Node* StatmentAction6(Node* node1 , Node* node2, Node* node3, Node* node4 , Node* node5 , RegManagment& regManagment , CodeBuffer& codeBuffer);
// Statment -> WHILE LPAREN <MARKER> M Exp <MARKER> RPAREN M Statement
Node* StatmentAction7(Node* node1 , Node* node2, Node* node3, Node* node4 , Node* node5 
, Node* node6 , Node* node7 , Node* node8 , Node* node9 , RegManagment& regManagment , CodeBuffer& codeBuffer);
//Statment -> BREAK SC
Node* StatmentAction8(int in_while_flag , CodeBuffer& codeBuffer);
//Statment -> CONTINUE SC
Node* StatmentAction9(int in_while_flag , CodeBuffer& codeBuffer);
//Statment -> RETURN SC
Node* StatmentAction10(SymbolTable& symTable, CodeBuffer& codeBuffer);
//Statment -> RETURN Exp SC
Node* StatmentAction11(SymbolTable& symTable , Node * node1 , Node * node2, RegManagment& regManagment , CodeBuffer& codeBuffer);
//Statement -> Call SC
Node* StatmentAction12(SymbolTable& symTable , Node * node1);
//IF_SUFFIX -> IF LPAREN <Marker> Exp <Marker> RPAREN M Statement
Node* IfActionAction(Node * node1);
// Call -> ID LPAREN ExpList RPAREN
Node* CallAction1(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 , Node* node4
 , RegManagment& regManagment , CodeBuffer& codeBuffer);
// Call -> ID LPAREN RPAREN
Node* CallAction2(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3, RegManagment& regManagment, CodeBuffer& codeBuffer);
//ExpList -> Exp COMMA ExpList 
Node* ExpListAction1(Node* node1 , Node* node2 , Node* node3);
//ExpList -> Exp
Node* ExpListAction2(Node* node1);
// Type -> INT
Node* TypeAction1();
// Type -> BYTE
Node* TypeAction2();
// Type -> BOOL
Node* TypeAction3();
// Exp -> LPAREN Exp RPAREN
Node* ExpAction1(Node* node1 , Node* node2 , Node* node3 , RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> Exp BINOP Exp
Node* ExpAction2(Node* node1 , Node* node2 , Node* node3 , RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> ID
Node* ExpAction3(SymbolTable& symTable , Node* node1  , RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> Call
Node* ExpAction4(Node* node , RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> NUM
Node* ExpAction5(Node* node , RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> NUM B
Node* ExpAction6(Node* node1 , Node* node2 , RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> STRING
Node* ExpAction7(Node* node1 , RegManagment& regManagment, CodeBuffer& codeBuffer);
// Exp -> TRUE
Node* ExpAction8(RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> FALSE
Node* ExpAction9(RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> Exp AND M Exp
Node* ExpAction10(Node* node1 , Node* node2 , Node* node3 , Node* node4 , RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> Exp OR M Exp
Node* ExpAction11(Node* node1 , Node* node2 , Node* node3 , Node* node4 , RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> Exp RELOP Exp
Node* ExpAction12(Node* node1 , Node* node2 , Node* node3 , RegManagment& RegManagment , CodeBuffer& codeBuffer);
// Exp -> NOT Exp
Node* ExpAction13(Node* node1 , Node* node2 , RegManagment& RegManagment , CodeBuffer& codeBuffer);

void CallToEnterGlobalScope(SymbolTable& symTable);

void CallToExitGlobalScope(SymbolTable& symTable);

void CallToEnterFunctionScope(SymbolTable& symTable);

void CallToExitFunctionScope(SymbolTable& symTable, CodeBuffer& codeBuffer);

void CallToEnterInnerScope(SymbolTable& symTable);

void CallToExitInnerScope(SymbolTable& symTable);

void EnterWhile(int &in_while_flag);

void ExitWhile(int &in_while_flag);

// Assoiated with : FuncDecl -> RetType ID LPAREN Formals RPAREN PreConditions LBRACE Statements RBRACE
void CallToExitScopeWithPreConds(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 ,
        Node* node4 , Node* node5 , Node* node6 , Node* node7);

void addFunction(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 ,
        Node* node4 , Node* node5 , Node* node6 , Node* node7 , CodeBuffer& codeBuffer);
        
void checkIfBoolInWhileIf(Node* node);

void printIDsInGlobalScope(SymbolTable& symTable);

void printIDsInFunctionScope(SymbolTable& symTable);

void printIDsInInnerScope(SymbolTable& symTable);

void mainCheck(SymbolTable& symTable);

Node * MMarkerAction(CodeBuffer& codeBuffer);

//================================================= Buffer Related Functions ============================================================

void BinopCmdToBuffer(WorkReg left , opTypeEnum op , WorkReg right , WorkReg res , RegManagment& regManagment , CodeBuffer& codeBuffer);

//====== Function decleration Related Functions ==============

void FuncDeclToBuffer(std::string funcName , CodeBuffer& codeBuffer);

void AddFuncPrintPrintiToBuffer(CodeBuffer& codeBuffer);

std::string SaveStringToData(std::string text  , RegManagment& regManagment , CodeBuffer& codeBuffer);

WorkReg callPrintToBuffer(std::string label , RegManagment& regManagment , CodeBuffer& codeBuffer);

void callPrintiToBuffer(WorkReg workReg , RegManagment& regManagment , CodeBuffer& codeBuffer);

//=========== Variables Related Functions ====================

void AddAndAssignNonBoolVarToBuffer(WorkReg reg  , RegManagment& regManagment , CodeBuffer& codeBuffer);

void AddNonBoolVarToBuffer(RegManagment& regManagment , CodeBuffer& codeBuffer);

void AssignNonBoolVarToBuffer(WorkReg reg , int varOffset , RegManagment& regManagment , CodeBuffer& codeBuffer);

void AddAndAssignBoolVarToBuffer(WorkReg reg  , RegManagment& regManagment , CodeBuffer& codeBuffer);

void AddBoolVarToBuffer(WorkReg reg  , RegManagment& regManagment , CodeBuffer& codeBuffer);

void AssignBoolVarToBuffer(WorkReg reg  , RegManagment& regManagment , CodeBuffer& codeBuffer);

void backUpTakenRegisters(RegManagment& regManagment,CodeBuffer& codeBuffer);

void recoverTakenRegisters(RegManagment& regManagment,CodeBuffer& codeBuffer);

void BinopCmdToBuffer(WorkReg left , opTypeEnum op , WorkReg right , WorkReg res , RegManagment& regManagment , CodeBuffer& codeBuffer);
#endif