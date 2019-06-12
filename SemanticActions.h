#ifndef SEMANTIC_ACTIONS_H
#define SEMANTIC_ACTIONS_H

#include "attributes.h"
#include "SymbolTable.h"
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
// Statment -> Type ID SC
void StatmentAction1(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3);
// Statment -> Type ID ASSIGN Exp SC
void StatmentAction2(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3, Node* node4, Node* node5);
//Statment -> ID ASSIGN Exp SC
void StatmentAction3(SymbolTable& symTable , Node* node1 , Node* node2, Node* node3, Node* node4);
//Statment -> BREAK SC
void StatmentAction4(int in_while_flag);
//Statment -> CONTINUE SC
void StatmentAction5(int in_while_flag);
//Statment -> RETURN SC
void StatmentAction6(SymbolTable& symTable);
//Statment -> RETURN Exp SC
void StatmentAction7(SymbolTable& symTable , Node * node1 , Node * node2);
// Call -> ID LPAREN ExpList RPAREN
Node* CallAction1(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 , Node* node4);
// Call -> ID LPAREN RPAREN
Node* CallAction2(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3);
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
Node* ExpAction1(Node* node1 , Node* node2 , Node* node3);
// Exp -> Exp BINOP Exp
Node* ExpAction2(Node* node1 , Node* node2 , Node* node3);
// Exp -> ID
Node* ExpAction3(SymbolTable& symTable , Node* node1 );
// Exp -> Call
Node* ExpAction4(Node* node);
// Exp -> NUM
Node* ExpAction5(Node* node);
// Exp -> NUM B
Node* ExpAction6(Node* node1 , Node* node2);
// Exp -> STRING
Node* ExpAction7();
// Exp -> TRUE
Node* ExpAction8();
// Exp -> FALSE
Node* ExpAction9();
// Exp -> Exp AND Exp
Node* ExpAction10(Node* node1 , Node* node2 , Node* node3);
// Exp -> Exp OR Exp
Node* ExpAction11(Node* node1 , Node* node2 , Node* node3);
// Exp -> Exp RELOP Exp
Node* ExpAction12(Node* node1 , Node* node2 , Node* node3);
// Exp -> NOT Exp
Node* ExpAction13(Node* node1 , Node* node2);

void CallToEnterGlobalScope(SymbolTable& symTable);

void CallToExitGlobalScope(SymbolTable& symTable);

void CallToEnterFunctionScope(SymbolTable& symTable);

void CallToExitFunctionScope(SymbolTable& symTable);

void CallToEnterInnerScope(SymbolTable& symTable);

void CallToExitInnerScope(SymbolTable& symTable);

void EnterWhile(int &in_while_flag);

void ExitWhile(int &in_while_flag);

// Assoiated with : FuncDecl -> RetType ID LPAREN Formals RPAREN PreConditions LBRACE Statements RBRACE
void CallToExitScopeWithPreConds(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 ,
        Node* node4 , Node* node5 , Node* node6 , Node* node7);

void addFunction(SymbolTable& symTable , Node* node1 , Node* node2 , Node* node3 ,
        Node* node4 , Node* node5 , Node* node6 , Node* node7);
        
void checkIfBoolInWhileIf(Node* node);

void printIDsInGlobalScope(SymbolTable& symTable);

void printIDsInFunctionScope(SymbolTable& symTable);

void printIDsInInnerScope(SymbolTable& symTable);

void mainCheck(SymbolTable& symTable);

#endif