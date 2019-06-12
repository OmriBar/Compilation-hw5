#include "AidFunctions.h"
#include "SymbolTable.h"
#include <math.h>
#include <cmath>

int CharPointerToInt(char * str){
    int i=0;
    int val=0;
    while(str[i] != '\0'){
        val *=10;
        val += (int)(str[i]-'0');
        i++;
    }
    return val;
}