#include "Plan.h"
#include <iostream>

using namespace std;

Plan::Plan(const int planId, const Settlement& town, SelectionPolicy *selectionPolicy, const vector<FacilityType>& facilityOptions):plan_id(planId), settlement(town),selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE),facilities(), underConstruction(),facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0){}
Plan::Plan(const Plan& p):plan_id(p.plan_id), settlement(p.settlement),selectionPolicy(p.selectionPolicy->clone()), status(p.status), facilities(), underConstruction(), facilityOptions(p.facilityOptions),life_quality_score(p.life_quality_score),economy_score(p.economy_score),environment_score(p.environment_score)
{
    for(Facility* fac : p.facilities)
    {
        this->facilities.push_back(new Facility(*fac));
    }
    for(Facility* fac : p.underConstruction)
    {
        this->underConstruction.push_back(new Facility(*fac));
    }
}
Plan& Plan::operator=(const Plan& p)
{
    if(this != &p)
    {
        delete selectionPolicy;
        for(Facility* fac : underConstruction)
        {
            delete fac;
        }
        underConstruction.clear();
        for(Facility* fac : facilities)
        {
            delete fac;
        }
        facilities.clear();
        plan_id = p.plan_id;
        selectionPolicy = p.selectionPolicy -> clone();
        status = p.status;
        life_quality_score = p.life_quality_score;
        economy_score = p.economy_score;
        environment_score = p.environment_score;
        Settlement set(p.getSettlement().getName(),p.getSettlement().getType()); 
        //clone settelment and then * make it ref

        for (Facility* fac : p.facilities) 
            facilities.push_back(new Facility(*fac));
        for (Facility* fac : p.underConstruction) 
            underConstruction.push_back(new Facility(*fac));
    }
    return *this;
}
const int Plan::getlifeQualityScore() const
{
    return this->life_quality_score;
}
const int Plan::getEconomyScore() const
{
    return this->economy_score;
}
const int Plan::getEnvironmentScore() const
{
    return this->environment_score;
}
const vector<Facility*> Plan::getUnderConstruction() const
{
    return this->underConstruction;
}
const string Plan::getSelectionPolicy() const
{
    return this->selectionPolicy->toString();
}
const Settlement& Plan::getSettlement() const
{
    return this-> settlement;
}
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
      if (this->selectionPolicy != nullptr && this->selectionPolicy->toString() == selectionPolicy->toString()) 

    {
        delete selectionPolicy;  
        return;
    }
    if (this->selectionPolicy != nullptr) 
    {
        delete this->selectionPolicy;
    }

    this->selectionPolicy = selectionPolicy;
}

void Plan::step()
{
    int avail = 0;
    switch (settlement.getType())
    {
    case SettlementType::VILLAGE:
        avail = 1;
        break;
    case SettlementType::CITY:
        avail = 2;
        break;
    case SettlementType::METROPOLIS:
        avail = 3;
        break;
    }
    if (underConstruction.size() == static_cast<vector<Facility*>::size_type>(avail))
        this->status = PlanStatus::BUSY;
    else 
        this->status = PlanStatus::AVALIABLE;
    while(status == PlanStatus::AVALIABLE)
    {
        const FacilityType& selected = selectionPolicy -> selectFacility(facilityOptions);
        addFacility(new Facility(selected, settlement.getName()));
        if ( underConstruction.size() == static_cast<vector<Facility*>::size_type>(avail))
            this->status = PlanStatus::BUSY;
    }
    for(size_t i = 0; i < underConstruction.size(); i++)
    {
        underConstruction[i]->step();
        if(underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL)
        {
            facilities.push_back(underConstruction[i]);
            this->life_quality_score += underConstruction[i]->getLifeQualityScore();
            this->economy_score += underConstruction[i]->getEconomyScore();
            this->environment_score += underConstruction[i]->getEnvironmentScore();
            underConstruction.erase(underConstruction.begin() + i);
            this->status = PlanStatus::AVALIABLE;
        }
    }
}
void Plan::printStatus()
{
    cout << "PlanID :" << this->plan_id << endl;
    cout << "SettlementName :" << this -> settlement.getName() << endl;
    cout << "PlanStatus :" << (this -> status == PlanStatus::AVALIABLE ? "AVAILABLE" : "BUSY") << endl;
    cout << "SelectionPolicy :" << (this -> selectionPolicy->toString()) << endl;
    cout << "Life EconomyScore :" << life_quality_score << endl;
    cout << "Economy Score :" << economy_score << endl;
    cout << "Environment Score :" << environment_score<< endl; 
}
const vector<Facility*>& Plan::getFacilities() const
{ 
    return facilities; 
}
const int Plan::getPlanID() const
{
    return this -> plan_id; 
}
void Plan::addFacility(Facility* facility)
{
    // Check if underconstruction is full
    underConstruction.push_back(facility);
}
const string Plan::toString() const
{
    return "Plan" + this -> plan_id;
}
Plan::~Plan()
{
    delete this -> selectionPolicy;
    for(Facility* fac : this->facilities)
    {
        delete fac;
    }
    facilities.clear();
    for(Facility* fac : this->underConstruction)
    {
        delete fac;
    }
    underConstruction.clear();
}
