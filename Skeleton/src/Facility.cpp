#include "Facility.h"
#include <iostream>

using namespace std;

class FacilityType {
    protected:
        const string name;
        const FacilityCategory category;
        const int price;
        const int lifeQuality_score;
        const int economy_score;
        const int environment_score;

    public:
        FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score){};
        const string &getName() const
        {
            return this->name;     
        }
        int getCost() const
        {
            return this->price;     
        }
        int getLifeQualityScore() const
        {
            return this->lifeQuality_score;     
        }
        int getEnvironmentScore() const
        {
            return this->environment_score;
        }
        int getEconomyScore() const
        {
            return this->economy_score;
        }
        FacilityCategory getCategory() const
        {
            return this->category;
        }
};

class Facility:public FacilityType
{
private:
    const string settlementName;
    FacilityStatus status;
    int timeLeft;
public:
        Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){};
        Facility(const FacilityType &type, const string &settlementName):FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()){};
        const string &getSettlementName()const
        {
            return this->settlementName;
        }
        const int getTimeLeft() const
        {
            return this->timeLeft;
        }
        FacilityStatus step()
        {
            if(timeLeft > 0)
            {
                timeLeft--;
                if (timeLeft == 0)
                    this->setStatus(FacilityStatus::OPERATIONAL);
            }
        }
        void setStatus(FacilityStatus status)
        {
            this->status = status;
        }
        const FacilityStatus& getStatus() const
        {
            return this->status;
        }
        const string toString() const;
};
