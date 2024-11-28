#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Action.h"
#include "Settlement.h"
#include <iostream>
using std::string;
using std::vector;
using namespace std;


 class BaseAction;
class SelectionPolicy;

class Simulation {
    private:
        bool isRunning; 
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

    public:
        Simulation(const string &configFilePath): isRunning(false), planCounter(0), actionsLog(vector<BaseAction*>()), plans(vector<Plan>()), settlements (vector<Settlement*>()), facilitiesOptions(vector<FacilityType>()){}; 
        void start()
        {
            cout<< "The simulation has started" <<endl; 
        }
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
        {
            Plan newPlan= Plan(planCounter, settlement, selectionPolicy, facilitiesOptions);
            plans.push_back(newPlan);
            planCounter++;
        }

        void addAction(BaseAction *action)
        {
            this->actionsLog.push_back(action);
        }
        bool addSettlement(Settlement *settlement)
        {
            for( Settlement* exist: settlements)
            {
                if(exist->getName() == settlement->getName())
                    return false;
            } 
            settlements.push_back(settlement); 
            return true;   
        }
        bool addFacility(FacilityType facility)
        {
            for( FacilityType exist: facilitiesOptions)
            {
                if(exist.getName() == facility.getName())
                    return false;
            } 
            facilitiesOptions.push_back(facility); 
            return true;  
        }
        bool isSettlementExists(const string &settlementName)
        { 
            for( Settlement* exist: settlements)
            {
                if(exist->getName() == settlementName)
                    return true;
            }     
            return false; 
        }
        Settlement &getSettlement(const string &settlementName)
        {
            for(Settlement* exist: settlements)
            {
                if(exist->getName() == settlementName)
                {
                    return *exist;
                }
            }     
        }
        Plan &getPlan(const int planID)
        {
           for(Plan exist: plans) 
            {
                if(exist.getPlanID()== planID)
                {
                    return exist;
                }
            }   
        }
        vector<Plan> getPlans()
        {
            return this->plans;
        }
        vector<BaseAction*> &getActions()
        {
            return this->actionsLog;
        }
        
        void step()
        {
            for(Plan plan: plans) 
            {
                plan.step(); 
            }   
        }
        void close()
        { 
            for(Plan plan: plans) 
            {
                plan.printStatus(); 
            }  
        }
        void open() 
        {
            isRunning=true; 
        }
};