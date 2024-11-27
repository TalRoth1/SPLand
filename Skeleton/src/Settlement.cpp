#include "Settlement.h"
#include <iostream>

using namespace std;

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

    }
};