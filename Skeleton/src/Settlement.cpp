#include "Settlement.h"
#include <iostream>

using namespace std;

Settlement::Settlement(const string &name, SettlementType type):name(name), type(type){};
const string& Settlement::getName() const
{
    return this->name;
}
SettlementType  Settlement::getType() const
{
    return this->type;
}
const string  Settlement::toString() const
{
    if (type == SettlementType::VILLAGE) 
        return "0";
    else if (type == SettlementType::CITY) 
        return "1"; 
    else if (type == SettlementType::METROPOLIS) 
        return "2";  
}