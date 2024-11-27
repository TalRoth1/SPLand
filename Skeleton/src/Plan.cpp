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
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):plan_id(plan_id), settlement(settlement),selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE),facilities(vector<Facility*>()), underConstruction(vector<Facility*>()),facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0){};
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
        void setSelectionPolicy(SelectionPolicy *selectionPolicy)
        {
            this->selectionPolicy = selectionPolicy;
        }
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
};
