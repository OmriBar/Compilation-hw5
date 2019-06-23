#ifndef REG_MNG_H
#define REG_MNG_H

#include <list>
#include <string>
#include <map>


#include <sstream>
#include <iostream>


using namespace std;

enum WorkReg { NONE , T8 , T9 ,
 S0 , S1 , S2 , S3 , S4 , S5 , S6 , S7 ,
 T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 };

std::string WorkRegEnumToStr(WorkReg workReg);

class RegManagment {
    private:
        std::list<WorkReg> FreeRegList;
    public:
        std::map<WorkReg,WorkReg> TakenRegList;
        RegManagment();
        WorkReg AllocateReg();
        void FreeReg(WorkReg reg);
        std::list<WorkReg> GetCurrentTakenRegsList();
        void printTaken();
        std::stringstream printFree();
};

#endif