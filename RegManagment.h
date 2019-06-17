#ifndef REG_MNG_H
#define REG_MNG_H

#include "list"

enum WorkReg { NONE , T8 , T9 ,
 S0 , S1 , S2 , S3 , S4 , S5 , S6 , S7 ,
 T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 };

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
}

class RegManagment {
    private:
        std::list<WorkReg> FreeRegList;
    public:
        RegManagment();
        WorkReg AllocateReg();
        void FreeReg(WorkReg reg);
};

#endif