
#include "list"
#include "typeEnums.h"
#include "RegManagment.h"

using namespace std;

class Quad {
    private:
        //std::list<
        opTypeEnum Op;
        WorkReg Arg1;
        WorkReg Arg2;
        WorkReg Result;
    public:
        Quad(opTypeEnum op , WorkReg arg1 , WorkReg arg2 , WorkReg result) : Op(op) , Arg1(arg1) , Arg2(arg2) , Result(result){}

};