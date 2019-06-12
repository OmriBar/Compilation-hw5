
#include "list"
#include "typeEnums.h"

using namespace std;

class Quad {
    private:
        //std::list<
        opTypeEnum Op;
        int Arg1;
        int Arg2;
        int Result;
    public:
        Quad(opTypeEnum op , int arg1 , int arg2 , int result) : Op(op) , Arg1(arg1) , Arg2(arg2) , Result(result){}

};