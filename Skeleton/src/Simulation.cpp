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

Simulation::Simulation(const string &configFilePath): isRunning(false), planCounter(0), actionsLog(vector<BaseAction*>()), plans(vector<Plan>()), settlements(vector<Settlement*>()), facilitiesOptions(vector<FacilityType>()) 
{
    this->open();
    ifstream configFile(configFilePath);  // Open the config file for reading

    string line;
    while (getline(configFile, line)) {
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
            switch (settlementType) {
                case 0:
                    settlements.push_back(new Settlement(settlementName, SettlementType::VILLAGE));
                    break;
                case 1:
                    settlements.push_back(new Settlement(settlementName, SettlementType::CITY));
                    break;
                case 2:
                    settlements.push_back(new Settlement(settlementName, SettlementType::METROPOLIS));
                    break;
            }
        } else if (type == "facility") {
            // Read facility details: facility <facility_name> <category> <price> <lifeq_impact> <eco_impact> <env_impact>
            string facilityName;
            int category, price, lifeQualityImpact, economyImpact, environmentImpact;
            iss >> facilityName >> category >> price >> lifeQualityImpact >> economyImpact >> environmentImpact;
            // Create FacilityType and add to the facilitiesOptions vector
            switch (category) 
            {
                case 0:
                    facilitiesOptions.push_back(FacilityType(facilityName, FacilityCategory::LIFE_QUALITY, price, lifeQualityImpact, economyImpact, environmentImpact));
                    break;
                case 1:
                    facilitiesOptions.push_back(FacilityType(facilityName, FacilityCategory::ECONOMY, price, lifeQualityImpact, economyImpact, environmentImpact));
                    break;
                case 2:
                    facilitiesOptions.push_back(FacilityType(facilityName, FacilityCategory::ENVIRONMENT, price, lifeQualityImpact, economyImpact, environmentImpact));
                    break;
            }
        } else if (type == "plan") {
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

Simulation::Simulation(const Simulation& sim): isRunning(sim.isRunning), planCounter(sim.planCounter), actionsLog(), plans(), settlements(), facilitiesOptions(vector<FacilityType>(sim.facilitiesOptions))
{
    this -> actionsLog = vector<BaseAction*>();
    this -> plans = vector<Plan>();
    this -> settlements = vector<Settlement*>();
    for(BaseAction* act : sim.actionsLog)
    {
        this->actionsLog.push_back(act -> clone());
    }
    for(Settlement* sett : sim.settlements)
    {
        this->settlements.push_back(new Settlement(*sett));
    }
    for(Plan p : sim.plans)
    {
        SelectionPolicy* policy = nullptr;
            if (p.getSelectionPolicy() == "eco") {
                policy = new EconomySelection();
            } else if (p.getSelectionPolicy() == "bal") {
                policy = new BalancedSelection(0, 0, 0);  // Example default parameters, adjust as needed
            } else if (p.getSelectionPolicy() == "nve") {
                policy = new NaiveSelection();
            } else if (p.getSelectionPolicy() == "env") {
                policy = new SustainabilitySelection();
            }
        Plan newPlan(p.getPlanID(), *(this -> settlements[0]), new NaiveSelection(), this->facilitiesOptions);
        for(Settlement* set : this -> settlements)
        {
            if (set -> getName() == p.getSettlement().getName())
            {
                newPlan = Plan(p.getPlanID(), *set, policy, this->facilitiesOptions);
            }
        }
        this->plans.push_back(newPlan);
    }
}
Simulation::Simulation(Simulation&& sim):isRunning(sim.isRunning), planCounter(sim.planCounter), actionsLog(move(sim.actionsLog)), plans(sim.plans), settlements(move(sim.settlements)), facilitiesOptions(sim.facilitiesOptions)
{
    for(BaseAction*& act : sim.actionsLog)
    {
        act = nullptr;
    }
    for(Settlement*& sett : sim.settlements)
    {
        sett = nullptr;
    }
}
Simulation& Simulation::operator=(const Simulation& sim)
{
    if(this != &sim)
    {
        for(BaseAction* act : actionsLog)
        {
            delete act;
        }
        actionsLog.clear();
        for(Settlement* sett : settlements)
        {
            delete sett;
        }
        settlements.clear();
        facilitiesOptions.clear();
        plans.clear();
        this -> isRunning = sim.isRunning;
        this -> planCounter = sim.planCounter;
        for (BaseAction* act : sim.actionsLog)
        {
            actionsLog.push_back(act -> clone());
        }
        for (Settlement* sett : sim.settlements)
        {
            settlements.push_back(new Settlement(*sett));
        }
        for (const FacilityType fac : sim.facilitiesOptions)
        {
            facilitiesOptions.push_back(fac);
        }
        for (const Plan p : sim.plans)
        {
            SelectionPolicy* policy = nullptr;
                if (p.getSelectionPolicy() == "eco") {
                    policy = new EconomySelection();
                } else if (p.getSelectionPolicy() == "bal") {
                    policy = new BalancedSelection(0, 0, 0);  // Example default parameters, adjust as needed
                } else if (p.getSelectionPolicy() == "nve") {
                    policy = new NaiveSelection();
                } else if (p.getSelectionPolicy() == "env") {
                    policy = new SustainabilitySelection();
                }
            Plan newPlan(p.getPlanID(), *(this -> settlements[0]), new NaiveSelection(), this->facilitiesOptions);
            for(Settlement* set : this -> settlements)
            {
                if (set -> getName() == p.getSettlement().getName())
                {
                    newPlan = Plan(p.getPlanID(), *set, policy, this->facilitiesOptions);
                }
            }
            this->plans.push_back(newPlan);
        }
    }
    return *this;
}
Simulation& Simulation::operator=(Simulation&& sim)
{
    if(this != &sim)
    {
        for(BaseAction* act : this -> actionsLog)
        {
            delete act;
        }
        this -> actionsLog.clear();
        this -> plans.clear();
        for(Settlement* sett : this -> settlements)
        {
            delete sett;
        }
        this -> settlements.clear();
        this ->  facilitiesOptions.clear();
        this -> isRunning = sim.isRunning;
        this -> planCounter = sim.planCounter;
        for (BaseAction* act : sim.actionsLog)
        {
            this -> actionsLog.push_back(act -> clone());
            act = nullptr;
        }
        for (Settlement* sett : sim.settlements)
        {
            this -> settlements.push_back(new Settlement(*sett));
            sett = nullptr;
        }
        for (const FacilityType fac : sim.facilitiesOptions)
        {
            this -> facilitiesOptions.push_back(fac);
        }
        for (const Plan p : sim.plans)
        {
            SelectionPolicy* policy = nullptr;
                if (p.getSelectionPolicy() == "eco") {
                    policy = new EconomySelection();
                } else if (p.getSelectionPolicy() == "bal") {
                    policy = new BalancedSelection(0, 0, 0);  // Example default parameters, adjust as needed
                } else if (p.getSelectionPolicy() == "nve") {
                    policy = new NaiveSelection();
                } else if (p.getSelectionPolicy() == "env") {
                    policy = new SustainabilitySelection();
                }
            Plan newPlan(p.getPlanID(), *(this -> settlements[0]), new NaiveSelection(), this->facilitiesOptions);
            for(Settlement* set : this -> settlements)
            {
                if (set -> getName() == p.getSettlement().getName())
                {
                    newPlan = Plan(p.getPlanID(), *set, policy, this->facilitiesOptions);
                }
            }
            this->plans.push_back(newPlan);
        }
    }
    return *this;
}
void Simulation::start()
{
    this -> open();
    cout<< "The simulation has started" <<endl;
    string data;
    string action;
    while(isRunning)
    {
        getline(cin, data);
        istringstream iss(data);
        iss >> action;
        if(action == "step")
        {
            int steps;
            iss >> steps;
            SimulateStep* act = new SimulateStep(steps);
            act -> act(*this);
        }
        else if(action == "plan")
        {
            string settname, selectPol;
            iss >> settname >> selectPol;
            AddPlan* act = new AddPlan(settname, selectPol);
            act -> act(*this);
        }
        else if(action == "settlement")
        {
            string settname;
            int type;
            iss >> settname >> type;
            AddSettlement* act = new AddSettlement(settname, SettlementType::VILLAGE);
            switch(type)
            {
                case 0:
                    delete act;
                    act = new AddSettlement(settname, SettlementType::VILLAGE);
                    break;
                case 1:
                    delete act;
                    act = new AddSettlement(settname, SettlementType::CITY);
                    break;
                case 2:
                    delete act;
                    act = new AddSettlement(settname, SettlementType::METROPOLIS);
                    break;                             
            }
            act -> act(*this);
        }
        else if(action == "facility")
        {
            string facName;
            int cat, price, life, eco, env;
            iss >> facName >> cat >> price >> life >> eco >> env;
            AddFacility* act =nullptr; 
            switch(cat)
            {
                case 0:
                    act = new AddFacility(facName, FacilityCategory::LIFE_QUALITY, price, life, eco, env);
                    break;
                case 1:
                    act = new AddFacility(facName, FacilityCategory::ECONOMY, price, life, eco, env);
                    break;
                case 2:
                    act = new AddFacility(facName, FacilityCategory::ENVIRONMENT, price, life, eco, env);
                    break;                                   
            }
            act -> act(*this);
        }
        else if(action== "planStatus")
        {
            int id;
            iss>> id; 
            PrintPlanStatus* act = new PrintPlanStatus(id);
            act -> act(*this);
        }
        else if(action== "changePolicy")
        {
            string policy; 
            int id;
            iss>> id>> policy; 
            ChangePlanPolicy* act = new ChangePlanPolicy(id, policy);
            act -> act(*this);
        }
        else if(action == "log")
        {
            PrintActionsLog* act = new PrintActionsLog();
            act -> act(*this);
        }
        else if(action == "backup")
        {
            BackupSimulation* act = new BackupSimulation();
            act -> act(*this);
        }
        else if(action == "restore")
        {
            RestoreSimulation* act = new RestoreSimulation();
            act -> act(*this);
        }
        else if(action == "close")
        {
            Close* act = new Close();
            act -> act(*this);
            delete act; 
        }
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
        if(exist -> getName() == settlementName)
            return true;
    }     
    return false; 
}
Settlement& Simulation::getSettlement(const string &settlementName)
{

    for(Settlement* exist: settlements)
    {
        if(exist -> getName() == settlementName)
        {
            return *exist;
        }
    } 
    Settlement result = Settlement("Error", SettlementType::VILLAGE); // Should not reach this code
    Settlement& ref = result;
    return ref;
}
Plan& Simulation::getPlan(const int planID)
{
    for(Plan& exist: plans) 
    {
        if(exist.getPlanID()== planID)
        {
            return exist;
        }
    }
    Plan result = Plan(plans[0]); //Should not reach this code
    Plan& ref = result;
    return ref;  
}
vector<Plan>& Simulation::getPlans()
{
    return this -> plans;
}
vector<BaseAction*>& Simulation::getActions()
{
    return this -> actionsLog;
}

void Simulation::step()
{
    for(Plan& plan: plans) 
    {
        plan.step(); 
    }   
}
void Simulation::close()
{ 
    for(Plan& plan: plans) 
    {
        plan.printStatus(); 
    }
    plans.clear();
    isRunning = false;
    
}
void Simulation::open() 
{
    isRunning = true; 
}
Simulation::~Simulation()
{
    for(BaseAction* act : this->actionsLog)
    {
        delete act;
    }
    actionsLog.clear();
    for(Settlement* sett : this->settlements)
    {
        delete sett;
    }
    settlements.clear();
    
}