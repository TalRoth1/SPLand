#include <string>
#include <vector>
#include "Simulation.h"
#include "Action.h"

using namespace std;

extern Simulation* backup;
#include <iostream>; 
enum class SettlementType;
enum class FacilityCategory;


void BaseAction::complete() 
{
    status=ActionStatus::COMPLETED; 
}
void BaseAction::error(string errorMsg)
{
    status=ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    cout << "Error:" << errorMsg << endl; 
}
const string& BaseAction::getErrorMsg() const 
{
    return this -> errorMsg; 
}
BaseAction::BaseAction(){}
ActionStatus BaseAction::getStatus() const
{
    return this -> status;
}

SimulateStep::SimulateStep(const int numOfSteps):BaseAction(), numOfSteps(numOfSteps){}
void SimulateStep::act(Simulation &simulation)
{
    for(int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    this -> complete();
    simulation.addAction(this);
}
const string SimulateStep::toString() const
{
    switch (getStatus())
    {
    case ActionStatus::COMPLETED:
        return "step " + to_string(numOfSteps) + " COMPLETED";
    case ActionStatus::ERROR:
        return "step " + to_string(numOfSteps) + " ERROR";
    }
}
SimulateStep* SimulateStep::clone() const
{
    SimulateStep* copy = new SimulateStep(numOfSteps);
    return copy;
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):BaseAction(), settlementName(settlementName), selectionPolicy(selectionPolicy){}
void AddPlan::act(Simulation &simulation)
{
    SelectionPolicy* s = new NaiveSelection;
    if(selectionPolicy == "nve")
        s = new NaiveSelection();
    else if (selectionPolicy == "bal")
        s = new BalancedSelection(0,0,0);
    else if (selectionPolicy == "eco")
        s = new EconomySelection();
    else if (selectionPolicy == "env")
        s = new SustainabilitySelection();
    else
        this -> error("cannot create this plan");
    if(!simulation.isSettlementExists(settlementName))
    {
        simulation.addPlan(simulation.getSettlement(settlementName), s);
        this -> complete();
    }
    else
        this -> error("cannot create this plan");
    simulation.addAction(this);
}
const string AddPlan::toString() const
{
    switch (getStatus())
    {
    case ActionStatus::COMPLETED:
        return "plan " + settlementName +selectionPolicy+ " COMPLETED";
    case ActionStatus::ERROR:
        return "plan " + settlementName +selectionPolicy+ " ERROR";
    }
    

}
AddPlan* AddPlan::clone() const
{
    AddPlan* copy = new AddPlan(settlementName, selectionPolicy);
    return copy;
}

AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType): BaseAction(),settlementName(settlementName), settlementType(settlementType){}
void AddSettlement::act(Simulation &simulation)
{
    Settlement* s = new Settlement(settlementName,settlementType); 
    if(simulation.addSettlement(s)) 
        this -> complete();
    else
        this->error("Settlement already exists");
    simulation.addAction(this);
}
AddSettlement* AddSettlement::clone() const
{
    AddSettlement* copy = new AddSettlement(settlementName,settlementType);
    return copy;
}

const string AddSettlement::toString() const
{
    int type= -1; 
    switch (settlementType) 
    {
        case SettlementType::VILLAGE:
            type= 0; 
        case SettlementType::CITY:
            type= 1; 
        case SettlementType::METROPOLIS:
            type= 2; 
    }
    switch (getStatus())
    {
    case ActionStatus::COMPLETED:
        return "Settlement " + settlementName +to_string(type) + " COMPLETED";
    case ActionStatus::ERROR:
        return "Settlement " + settlementName +to_string(type)+ " ERROR";
    }
}

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):BaseAction(), facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){}
void AddFacility::act(Simulation &simulation)
{ 
    FacilityType f = FacilityType(facilityName, facilityCategory ,price, lifeQualityScore, economyScore, environmentScore);
    
    if(simulation.addFacility(f))
        this -> complete();
    else
        this->error("Facility already exists");
    simulation.addAction(this);
}
AddFacility* AddFacility::clone() const
{
    AddFacility* copy = new AddFacility(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
    return copy;
}
const string AddFacility::toString() const
{
    int type= -1; 
    switch (facilityCategory) 
    {
        case FacilityCategory::LIFE_QUALITY:
            type= 0; 
        case FacilityCategory::ECONOMY:
            type= 1; 
        case FacilityCategory::ENVIRONMENT:
            type= 2; 
    }
    switch (getStatus())
    {
    case ActionStatus::COMPLETED:
        return "facility " + facilityName +to_string(type) +to_string(price) +to_string(lifeQualityScore) +to_string(economyScore)+ to_string(environmentScore)+ " COMPLETED";
    case ActionStatus::ERROR:
        return "facility " + facilityName +to_string(type) +to_string(price) +to_string(lifeQualityScore) +to_string(economyScore)+ to_string(environmentScore)+ " ERROR";
    }
}

PrintPlanStatus::PrintPlanStatus(int planId):BaseAction(), planId(planId) {}
void PrintPlanStatus::act(Simulation &simulation)
{
    Plan p = simulation.getPlan(planId); 
    p.printStatus(); 
    this -> complete();
    simulation.addAction(this);
}
PrintPlanStatus* PrintPlanStatus::clone() const
{
    PrintPlanStatus* copy = new PrintPlanStatus(planId);
    return copy;
}
const string PrintPlanStatus::toString() const
{
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
            return "printPlanStatus COMPLETED";
        case ActionStatus::ERROR:
            return "printPlanStatus Error";
    }
}


ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): planId(planId), newPolicy(newPolicy){}
void ChangePlanPolicy::act(Simulation &simulation)
{
    SelectionPolicy* s = new NaiveSelection();
    Plan& currplan = simulation.getPlan(planId);
    if(newPolicy == "nve"){}
    else if (newPolicy == "bal")
    {
        int life = currplan.getlifeQualityScore();
        int eco = currplan.getEconomyScore();
        int env = currplan.getEnvironmentScore();
        for (Facility* fac : currplan.getUnderConstruction())
        {
            life += fac->getLifeQualityScore();
            eco += fac->getEconomyScore();
            env += fac->getEnvironmentScore();
        }
        s = new BalancedSelection(life,eco,env);
    }
    else if (newPolicy == "eco")
    {
        s = new EconomySelection();
    }
    else if (newPolicy == "env")
    {
        s = new SustainabilitySelection();
    }
    else
    {
        this->error("Cannot change selection policy");
        simulation.addAction(this);
        return;
    }
    currplan.setSelectionPolicy(s);
    this->complete();
    simulation.addAction(this);
}
ChangePlanPolicy* ChangePlanPolicy::clone() const
{
    ChangePlanPolicy* copy = new ChangePlanPolicy(planId, newPolicy);
    return copy;
}
const string ChangePlanPolicy::toString() const
{
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
            return "changePolicy"+ to_string(planId)+ newPolicy+ "COMPLETED";
        case ActionStatus::ERROR:
            return "changePolicy"+to_string(planId)+ newPolicy+ "Error";
    }
}


PrintActionsLog::PrintActionsLog(){}
void PrintActionsLog::act(Simulation &simulation)
{
    for(BaseAction* action : simulation.getActions())
        cout << action -> toString() << endl; 
    this->complete();       
    simulation.addAction(this);
}
PrintActionsLog* PrintActionsLog::clone() const
{
    PrintActionsLog* copy = new PrintActionsLog();
    return copy;
}
const string PrintActionsLog::toString() const
{
    switch (getStatus())
    {
    case ActionStatus::COMPLETED:
            return "PrintActionsLog COMPLETED";
    case ActionStatus::ERROR:
            return "PrintActionsLog Error";
    }
}

Close::Close(){}
void Close::act(Simulation &simulation)
{
    simulation.close();
    this->complete();
}
Close* Close::clone() const
{
    Close* copy = new Close();
    return copy;
}
const string Close::toString() const {};

BackupSimulation::BackupSimulation(){}
void BackupSimulation::act(Simulation &simulation)
{
    if(backup!=nullptr)
        delete backup;
    backup = new Simulation(simulation);
    this->complete();
    simulation.addAction(this);
}
BackupSimulation* BackupSimulation::clone() const
{
    BackupSimulation* copy = new BackupSimulation();
    return copy;
}
const string BackupSimulation::toString() const{}


RestoreSimulation::RestoreSimulation(){}
void RestoreSimulation::act(Simulation &simulation)
{
    simulation.open();
    this->complete();
    simulation.addAction(this);
}
RestoreSimulation* RestoreSimulation::clone() const
{
    RestoreSimulation* copy = new RestoreSimulation();
    return copy;
}
const string RestoreSimulation::toString() const{}