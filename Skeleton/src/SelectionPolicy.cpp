#include "SelectionPolicy.h"
#include <iostream>

using namespace std;

NaiveSelection::NaiveSelection():lastSelectedIndex(-1){};
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
{
    lastSelectedIndex++;
    if(lastSelectedIndex == facilitiesOptions.size())
        lastSelectedIndex = 0;
    return facilitiesOptions[lastSelectedIndex];
}
const string NaiveSelection::toString() const
{
    return "nve";
}
NaiveSelection* NaiveSelection::clone() const
{
    NaiveSelection* copy = new NaiveSelection();
    copy->lastSelectedIndex = this->lastSelectedIndex;
    return copy;
}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){};
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
{
    int life = this->LifeQualityScore;
    int eco = this->EconomyScore;
    int env = this->EnvironmentScore;
    int minDiff = INT64_MAX;
    int index = 0;
    int minDiffIndex = 0;
    for(const FacilityType& facility : facilitiesOptions)
    {
        life += facility.getLifeQualityScore();
        eco += facility.getEconomyScore();
        env += facility.getEnvironmentScore();
        int diff = abs(life - eco) + abs(eco - env) + abs(env - life);
        if(diff < minDiff)
        {
            minDiff = diff;
            minDiffIndex = index; 
        }
        index++;
    }
    this -> LifeQualityScore += facilitiesOptions[minDiffIndex].getLifeQualityScore();
    this -> EconomyScore += facilitiesOptions[minDiffIndex].getEconomyScore();
    this -> EnvironmentScore += facilitiesOptions[minDiffIndex].getEnvironmentScore();
    return facilitiesOptions[minDiffIndex];
}
const string BalancedSelection::toString() const
{
    return "bal";
}
BalancedSelection* BalancedSelection::clone() const 
{
    BalancedSelection* copy = new BalancedSelection(this -> LifeQualityScore, this -> EconomyScore, this -> EnvironmentScore);
    return copy;
}

EconomySelection::EconomySelection(): lastSelectedIndex(0){};
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
{
    
    while(facilitiesOptions[lastSelectedIndex].getCategory()!=FacilityCategory::ECONOMY){
        lastSelectedIndex++; 
        if(lastSelectedIndex==facilitiesOptions.size())
            lastSelectedIndex=0; 
    }
    return facilitiesOptions[lastSelectedIndex];

}
const string EconomySelection::toString() const
{
    return "eco";
}
EconomySelection* EconomySelection::clone() const
{
    EconomySelection* copy = new EconomySelection();
    copy->lastSelectedIndex = this->lastSelectedIndex;
    return copy;
}

SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(0){};
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
{
    
    while(facilitiesOptions[lastSelectedIndex].getCategory()!=FacilityCategory::ENVIRONMENT){
        lastSelectedIndex++; 
        if(lastSelectedIndex==facilitiesOptions.size())
            lastSelectedIndex=0; 
    }
    return facilitiesOptions[lastSelectedIndex];
}
const string SustainabilitySelection::toString() const 
{
    return "env";
}
SustainabilitySelection* SustainabilitySelection::clone() const
{
    SustainabilitySelection* copy = new SustainabilitySelection();
    copy->lastSelectedIndex = this->lastSelectedIndex;
    return copy;
}