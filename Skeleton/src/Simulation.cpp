#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Action.h"
#include "Settlement.h"
#include <iostream>
#include <fstream>
#include <sstream>
using std::string;
using std::vector;
using namespace std;


class BaseAction;
class SelectionPolicy;

Simulation::Simulation(const string &configFilePath): isRunning(false), planCounter(0), actionsLog(vector<BaseAction*>()), plans(vector<Plan>()), settlements (vector<Settlement*>()), facilitiesOptions(vector<FacilityType>())
{
    Simulation::open();
    ifstream configFile(configFilePath);  // Open the config file for reading

    string line;
    while (getline(configFile, line)) 
    {
        istringstream iss(line);
        
        // Skip empty lines or comment lines (lines starting with "#")
        if (line.empty() || line[0] == '#') {
            continue;
        }

        string type;
        iss >> type;

        if (type == "settlement") {
            // Read settlement details: settlement <settlement_name> <settlement_type>
            string settlementName;
            int settlementType;
            iss >> settlementName >> settlementType;
            switch (settlementType)
            {
            case 0:
                settlements.push_back(new Settlement(settlementName, SettlementType::VILLAGE));
            case 1:
                settlements.push_back(new Settlement(settlementName, SettlementType::CITY));
            case 2:
                settlements.push_back(new Settlement(settlementName, SettlementType::METROPOLIS));
            }
        }
        else if (type == "facility") {
            // Read facility details: facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
            string facilityName;
            int category, price, lifeQualityImpact, economyImpact, environmentImpact;
            iss >> facilityName >> category >> price >> lifeQualityImpact >> economyImpact >> environmentImpact;
            
            // Create FacilityType and add to the facilitiesOptions vector
            switch (category)
            {
            case 0:
                facilitiesOptions.push_back(FacilityType(facilityName, FacilityCategory::LIFE_QUALITY, price, lifeQualityImpact, economyImpact, environmentImpact));
            case 1:
                facilitiesOptions.push_back(FacilityType(facilityName, FacilityCategory::ECONOMY, price, lifeQualityImpact, economyImpact, environmentImpact));
            case 2:
                facilitiesOptions.push_back(FacilityType(facilityName, FacilityCategory::ENVIRONMENT, price, lifeQualityImpact, economyImpact, environmentImpact));
            }
        }
        else if (type == "plan") {
            // Read plan details: plan <settlement_name> <selection_policy>
            string settlementName, selectionPolicy;
            iss >> settlementName >> selectionPolicy;
            
            // Find the settlement object from the settlements vector
            Settlement* settlement = nullptr;
            for (Settlement* s : settlements) {
                if (s->getName() == settlementName) {
                    settlement = s;
                    break;
                }
            }

            // Create SelectionPolicy object based on the given selectionPolicy string
            SelectionPolicy* policy = nullptr;
            if (selectionPolicy == "eco") {
                policy = new EconomySelection();
            } else if (selectionPolicy == "bal") {
                policy = new BalancedSelection(0, 0, 0);  // Example default parameters, adjust as needed
            } else if (selectionPolicy == "nve") {
                policy = new NaiveSelection();
            } else if (selectionPolicy == "env") {
                policy = new SustainabilitySelection();
            }
            // Create Plan object and add to the plans vector
            Plan newPlan(planCounter++, *settlement, policy, facilitiesOptions);
            plans.push_back(newPlan);
        }
    }

    configFile.close();  // Close the config file after reading
}
Simulation::Simulation(const Simulation& sim): isRunning(sim.isRunning), planCounter(sim.planCounter), actionsLog(vector<BaseAction*>()), plans(vector<Plan>()), settlements(vector<Settlement*>()), facilitiesOptions(vector<FacilityType>(sim.facilitiesOptions))
{
    for(BaseAction* act : sim.actionsLog)
    {
        this->actionsLog.push_back(act -> clone());
    }
    for(Plan p : sim.plans)
    {
        this->plans.push_back(Plan(p));
    }
    for(Settlement* sett : sim.settlements)
    {
        this->settlements.push_back(new Settlement(*sett));
    }
}
void Simulation::start()
{
    cout<< "The simulation has started" <<endl;
    while(isRunning)
    {
        
    }
}
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    Plan newPlan = Plan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
    planCounter++;
}

void Simulation::addAction(BaseAction *action)
{
    this->actionsLog.push_back(action);
}
bool Simulation::addSettlement(Settlement *settlement)
{
    for( Settlement* exist: settlements)
    {
        if(exist->getName() == settlement->getName())
            return false;
    } 
    settlements.push_back(settlement); 
    return true;   
}
bool Simulation::addFacility(FacilityType facility)
{
    for( FacilityType exist: facilitiesOptions)
    {
        if(exist.getName() == facility.getName())
            return false;
    } 
    facilitiesOptions.push_back(facility); 
    return true;  
}
bool Simulation::isSettlementExists(const string &settlementName)
{ 
    for( Settlement* exist: settlements)
    {
        if(exist->getName() == settlementName)
            return true;
    }     
    return false; 
}
Settlement& Simulation::getSettlement(const string &settlementName)
{
    for(Settlement* exist: settlements)
    {
        if(exist->getName() == settlementName)
        {
            return *exist;
        }
    }     
}
Plan& Simulation::getPlan(const int planID)
{
    for(Plan exist: plans) 
    {
        if(exist.getPlanID()== planID)
        {
            return exist;
        }
    }   
}
vector<Plan>& Simulation::getPlans()
{
    return this->plans;
}
vector<BaseAction*>& Simulation::getActions()
{
    return this->actionsLog;
}

void Simulation::step()
{
    for(Plan plan: plans) 
    {
        plan.step(); 
    }   
}
void Simulation::close()
{ 
    for(Plan plan: plans) 
    {
        plan.printStatus(); 
    }  
}
void Simulation::open() 
{
    isRunning=true; 
}
Simulation::~Simulation()
{
    for(BaseAction* act : this->actionsLog)
    {
        delete act;
    }
    for(Settlement* sett : this->settlements)
    {
        delete sett;
    }
}