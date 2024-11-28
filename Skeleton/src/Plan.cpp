#include "Plan.h"
#include <iostream>

using namespace std;

class Plan {
    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):plan_id(plan_id), settlement(settlement),selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE),facilities(vector<Facility*>()), underConstruction(vector<Facility*>()),facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0){}
        const int getlifeQualityScore() const
        {
            return this->life_quality_score;
        }
        const int getEconomyScore() const
        {
            return this->economy_score;
        }
        const int getEnvironmentScore() const
        {
            return this->environment_score;
        }
        const Settlement& getSettlement() const
        {
            return this-> settlement;
        }
        const void setSelectionPolicy(SelectionPolicy *selectionPolicy)
        {
            if(typeid(this->selectionPolicy) ==  typeid(selectionPolicy))
                throw string("Cannot change selction policy");
            this->selectionPolicy = selectionPolicy;
        }
        void step()
        {
            int avail = 0;
            switch (settlement.getType())
            {
            case SettlementType::VILLAGE:
                avail = 1;
            case SettlementType::CITY:
                avail = 2;
            case SettlementType::METROPOLIS:
                avail = 3;
            }
            if ( underConstruction.size() == avail)
                this->status = PlanStatus::BUSY;
            else 
                this->status = PlanStatus::AVALIABLE;
            while(status == PlanStatus::AVALIABLE)
            {
                const FacilityType& selected = selectionPolicy->selectFacility(facilityOptions);
                addFacility(new Facility(selected, settlement.getName()));
                if ( underConstruction.size() == avail)
                    this->status = PlanStatus::BUSY;
            }
            for(int i = 0; i < underConstruction.size(); i++)
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
        void printStatus()
        {
            cout << "PlanID :" << this->plan_id << endl;
            cout << "SettlementName :" << this->settlement.getName() << endl;
            cout << "PlanStatus :" << (this->status == PlanStatus::AVALIABLE ? "AVAILABLE" : "BUSY") << endl;
            cout << "SelectionPolicy :" << (this->selectionPolicy->toString()) << endl;
            cout << "Life EconomyScore :" << life_quality_score << endl;
            cout << "Economy Score :" << economy_score << endl;
            cout << "Environment Score :" << environment_score<< endl; 
            for (Facility* facility  : facilities)
            {
                cout << "Facility Name :"  << facility->getName() << endl;
                cout << "Facility Name :"  << (facility->getStatus()  == FacilityStatus::OPERATIONAL ? "OPERATOPNAL" : "UNDERCONSTRUCTION")<< endl;
                
            } 

        }
        const vector<Facility*> &getFacilities() const
        { 
            return facilities; 
        }
        void addFacility(Facility* facility)
        {
            underConstruction.push_back(facility);
        }
        const string toString() const;

        const int getPlanID() const
        {
            return this-> plan_id; 

        }
};
