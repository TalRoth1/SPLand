#include <string>
#include <vector>
#include "Simulation.h"
#include "Action.h"
#include <iostream>

using namespace std;

extern Simulation* backup;
enum class SettlementType;
enum class FacilityCategory;


void BaseAction::complete() 
{
    status=ActionStatus::COMPLETED; 
}
void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this -> errorMsg = errorMsg;
    cout << "Error:" << errorMsg << endl; 
}
const string& BaseAction::getErrorMsg() const 
{
    return this -> errorMsg; 
}
BaseAction::BaseAction():errorMsg(""), status(ActionStatus::ERROR){}
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
    return ""; // Should never get here
}
SimulateStep* SimulateStep::clone() const
{

    SimulateStep* copy = new SimulateStep(numOfSteps);
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
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
    if(simulation.isSettlementExists(settlementName))
    {
        simulation.addPlan(simulation.getSettlement(settlementName), s);
        this -> complete();
    }
    else
        this -> error("cannot create this plan hbbh");
    simulation.addAction(this);
}
const string AddPlan::toString() const
{
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
            return "plan " + settlementName + " " + selectionPolicy + " COMPLETED";
        case ActionStatus::ERROR:
            return "plan " + settlementName + " " + selectionPolicy + " ERROR";
    }
    return ""; // Should never get here
}
AddPlan* AddPlan::clone() const
{
    AddPlan* copy = new AddPlan(settlementName, selectionPolicy);
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
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
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
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
            return "Settlement " + settlementName+ " " + to_string(type) + " COMPLETED";
        case ActionStatus::ERROR:
            return "Settlement " + settlementName+ " " + to_string(type)+ " ERROR";
    }
    return ""; // Should never get here
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
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
    return copy;
}
const string AddFacility::toString() const
{
    int type= -1; 
    switch (facilityCategory) 
    {
        case FacilityCategory::LIFE_QUALITY:
            type= 0;
            break;
        case FacilityCategory::ECONOMY:
            type= 1;
            break;
        case FacilityCategory::ENVIRONMENT:
            type= 2;
            break;
    }
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
            return "facility " + facilityName + " " + to_string(type) + " " + to_string(price) + " " + to_string(lifeQualityScore) + " " + to_string(economyScore) + " " + to_string(environmentScore) + " " + " COMPLETED";
        case ActionStatus::ERROR:
            return "facility " + facilityName + " " + to_string(type) + " " + to_string(price) + " "+ to_string(lifeQualityScore) + " " + to_string(economyScore) + " " + to_string(environmentScore) + " " + " ERROR";
    }
    return ""; // Should never get here
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
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
    return copy;
}
const string PrintPlanStatus::toString() const
{
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
            return "printPlanStatus COMPLETED ";
        case ActionStatus::ERROR:
            return "printPlanStatus Error";
    }
    return ""; // Should never get here
}
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): planId(planId), newPolicy(newPolicy){}
void ChangePlanPolicy::act(Simulation &simulation)
{
    SelectionPolicy* s = new NaiveSelection();
    Plan& currplan = simulation.getPlan(planId);
    if (newPolicy == currplan.getSelectionPolicy()) 
     {
        delete s;  
        this->error("Cannot change plan policy");
        simulation.addAction(this);
        return;
     }
    cout << "planID: " << planId << endl;
    cout << "previousPolicy: " << currplan.getSelectionPolicy() << endl;
    if(newPolicy == "nve"){
        cout << "newPolicy: Naive " << endl;

    }
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
        cout << "newPolicy: Balanced " << endl;

    }
    else if (newPolicy == "eco")
    {
        s = new EconomySelection();
        cout << "newPolicy: Economy" << endl;

    }
    else if (newPolicy == "env")
    {
        s = new SustainabilitySelection();
        cout << "newPolicy: Sustainability" << endl;

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
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
    return copy;
}
const string ChangePlanPolicy::toString() const
{
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
            return "ChangePlanPolicy "+ to_string(planId)+" "+ newPolicy+ " COMPLETED";
        case ActionStatus::ERROR:
            return "ChangePlanPolicy "+to_string(planId)+" "+ newPolicy+ " ERROR";
    }
    return ""; // Should never get here
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
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
    return copy;
}
const string PrintActionsLog::toString() const
{
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
                return "PrintActionsLog COMPLETED";
        case ActionStatus::ERROR:
                return "PrintActionsLog ERROR";
    }
    return ""; // Should never get here
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
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
    return copy;
}
const string Close::toString() const 
{
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
            return "Close COMPLETED";
        case ActionStatus::ERROR:
            return "Close ERROR";
    }
    return ""; // Should never get here
}

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
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
    return copy;
}
const string BackupSimulation::toString() const
{
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
            return "Backupsimulation COMPLETED";
        case ActionStatus::ERROR:
            return "Backupsimulation ERROR";
    }
    return ""; // Should never get here
}


RestoreSimulation::RestoreSimulation(){}
void RestoreSimulation::act(Simulation &simulation)
{
    if(backup == nullptr)
    {
        this -> error("No backup available");
        return;
    }
    simulation = Simulation(*backup);
    this->complete();
    simulation.addAction(this);
}
RestoreSimulation* RestoreSimulation::clone() const
{
    RestoreSimulation* copy = new RestoreSimulation();
    if(this -> getStatus() == ActionStatus::COMPLETED)
        copy -> complete();
    return copy;
}
const string RestoreSimulation::toString() const
{
    switch (getStatus())
    {
        case ActionStatus::COMPLETED:
            return "Restoresimulation COMPLETED";
        case ActionStatus::ERROR:
            return "Restoresimulation ERROR";
    }
    return ""; // Should never get here
}