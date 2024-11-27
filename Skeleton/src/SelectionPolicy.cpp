#include "SelectionPolicy.h"
#include <iostream>

using namespace std;

class NaiveSelection: public SelectionPolicy {
    private:
        int lastSelectedIndex;
    public:
        NaiveSelection():lastSelectedIndex(-1){};
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override
        {
            lastSelectedIndex++;
            if(lastSelectedIndex == facilitiesOptions.size())
                lastSelectedIndex = 0;
            return facilitiesOptions[lastSelectedIndex];
        }
        const string toString() const override;
    NaiveSelection* clone() const override;
        ~NaiveSelection() override = default;
};

class BalancedSelection: public SelectionPolicy {
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){};
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override
        {
            int life = this->LifeQualityScore;
            int eco = this->EconomyScore;
            int env = this->EnvironmentScore;
            int minDiff = INFINITY;
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
            return facilitiesOptions[minDiffIndex];
        }
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;
};

class EconomySelection: public SelectionPolicy {
     private:
        int lastSelectedIndex;
    public:
        EconomySelection(): lastSelectedIndex(0){};
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override
        {
            
            while(facilitiesOptions[lastSelectedIndex].getCategory()!=FacilityCategory::ECONOMY){
                lastSelectedIndex++; 
                if(lastSelectedIndex==facilitiesOptions.size())
                    lastSelectedIndex=0; 
            }
            return facilitiesOptions[lastSelectedIndex];

        }
        const string toString() const override;
        EconomySelection *clone() const override;
        ~EconomySelection() override = default;
   
};

class SustainabilitySelection: public SelectionPolicy {
    private:
        int lastSelectedIndex;
    public: 
        SustainabilitySelection():lastSelectedIndex(0){};
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override
        {
            
            while(facilitiesOptions[lastSelectedIndex].getCategory()!=FacilityCategory::ENVIRONMENT){
                lastSelectedIndex++; 
                if(lastSelectedIndex==facilitiesOptions.size())
                    lastSelectedIndex=0; 
            }
            return facilitiesOptions[lastSelectedIndex];
        }
        const string toString() const override;
        SustainabilitySelection *clone() const override;
        ~SustainabilitySelection() override = default;
    
};