#include "Settlement.h"
#include <iostream>

using namespace std;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};
class Settlement
{
private:
    const string name;
    SettlementType type;
public:
    Settlement(const string &name, SettlementType type):name(name), type(type){};
    const string &getName() const
    {
        return this->name;
    }
    SettlementType getType() const
    {
        return this->type;
    }
    const string toString()
    {
        if (type == SettlementType::VILLAGE) 
            return "0";
        else if (type == SettlementType::CITY) 
            return "1"; 
        else if (type == SettlementType::METROPOLIS) 
            return "2";  
    }
};