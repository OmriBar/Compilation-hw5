#include "RegManagment.h"

std::string WorkRegEnumToStr(WorkReg workReg){
    switch(workReg){
        case T8 : return "t8";break;
        case T9 : return "t9";break;
        case S0 : return "s0";break;
        case S1 : return "s1";break;
        case S2 : return "s2";break;
        case S3 : return "s3";break;
        case S4 : return "s4";break;
        case S5 : return "s5";break;
        case S6 : return "s6";break;
        case S7 : return "s7";break;
        case T0 : return "t0";break;
        case T1 : return "t1";break;
        case T2 : return "t2";break;
        case T3 : return "t3";break;
        case T4 : return "t4";break;
        case T5 : return "t5";break;
        case T6 : return "t6";break;
        case T7 : return "t7";break;
        default : return "NONE";break;
    }
    return "NONE";
}

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
        //TakenRegList.pair_insert(reg,reg);
        TakenRegList.insert(pair<WorkReg, WorkReg>(reg, reg));
        return reg;
    }
    else{
        return NONE;
    }
}


void RegManagment::FreeReg(WorkReg reg) {
    FreeRegList.push_back(reg);
    TakenRegList.erase(reg);
}

std::list<WorkReg> RegManagment::GetCurrentTakenRegsList() {
    std::list<WorkReg> returnedList = std::list<WorkReg>();
    for(std::map<WorkReg,WorkReg>::iterator it = TakenRegList.begin() ; it != TakenRegList.end() ; it++){
        returnedList.push_back((*it).first);
    }
    return returnedList;
}

void RegManagment::printTaken() {
    std::cout << "taken : " << std::endl;
    for(std::map<WorkReg,WorkReg>::iterator it = TakenRegList.begin() ; it != TakenRegList.end() ; it++){
        std::cout << WorkRegEnumToStr((*it).first) << std::endl;
    }
     std::cout << "end list" << std::endl;
}
std::stringstream RegManagment::printFree(){
    std::stringstream toStr;
    toStr << "free : " << std::endl;
    for(std::list<WorkReg>::iterator it = FreeRegList.begin() ; it != FreeRegList.end() ; it++){
        toStr << WorkRegEnumToStr(*it) << std::endl;
    }
     toStr << "end list" << std::endl;
     return toStr;
}