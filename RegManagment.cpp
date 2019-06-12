#include "RegManagment.h"

RegManagment::RegManagment() {
    FreeRegList.push_back(T8);
    FreeRegList.push_back(T9);
    FreeRegList.push_back(S0);
    FreeRegList.push_back(S1);
    FreeRegList.push_back(S2);
    FreeRegList.push_back(S3);
    FreeRegList.push_back(S4);
    FreeRegList.push_back(S5);
    FreeRegList.push_back(S6);
    FreeRegList.push_back(S7);
    FreeRegList.push_back(T0);
    FreeRegList.push_back(T1);
    FreeRegList.push_back(T2);
    FreeRegList.push_back(T3);
    FreeRegList.push_back(T4);
    FreeRegList.push_back(T5);
    FreeRegList.push_back(T6);
    FreeRegList.push_back(T7);
}

WorkReg RegManagment::AllocateReg(){
    if(FreeRegList.size()>0){
        WorkReg reg = FreeRegList.back();
        FreeRegList.pop_back();
        return reg;
    }
    else{
        return NONE;
    }
}

void RegManagment::FreeReg(WorkReg reg) {
    FreeRegList.push_back(reg);
}