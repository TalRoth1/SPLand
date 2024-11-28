#include <string>
#include <vector>
#include "Simulation.h"
using namespace std; 
#include <iostream>; 
enum class SettlementType;
enum class FacilityCategory;

enum class ActionStatus{
    COMPLETED, ERROR
};

class BaseAction{
    private:
        string errorMsg;
        ActionStatus status;
    protected:
        void complete() 
        {
            status=ActionStatus::COMPLETED; 
        }
        void error(string errorMsg)
        {
            status=ActionStatus::ERROR;
            this->errorMsg = errorMsg;
            cout << "Error:" << errorMsg << endl; 
        }
        const string &getErrorMsg() const 
        {
            return this -> errorMsg; 
        }
    public:
        BaseAction(){}
        ActionStatus getStatus() const
        {
            return this -> status;
        }
        virtual void act(Simulation& simulation)=0;
        virtual const string toString() const=0;
        virtual BaseAction* clone() const = 0;
        virtual ~BaseAction() = default;


};

class SimulateStep : public BaseAction {

    private:
        const int numOfSteps;
    public:
        SimulateStep(const int numOfSteps):BaseAction(), numOfSteps(numOfSteps){}
        void act(Simulation &simulation) override
        {
            for(int i = 0; i < numOfSteps; i++)
            {
                simulation.step();
            }
            this -> complete();
            simulation.addAction(this);
        }
        const string toString() const override
        {
            switch (getStatus())
            {
            case ActionStatus::COMPLETED:
                return "step " + to_string(numOfSteps) + " COMPLETED";
            case ActionStatus::ERROR:
                return "step " + to_string(numOfSteps) + " ERROR";
            }
        }
        SimulateStep *clone() const override
        {
            SimulateStep* clon = new SimulateStep(numOfSteps);
            return clon;
        }
};

class AddPlan : public BaseAction {
    private:
        const string settlementName;
        const string selectionPolicy;
    public:
        AddPlan(const string &settlementName, const string &selectionPolicy):BaseAction(), settlementName(settlementName), selectionPolicy(selectionPolicy){}
        void act(Simulation &simulation) override
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
        const string toString() const override
        {
            switch (getStatus())
            {
            case ActionStatus::COMPLETED:
                return "plan " + settlementName +selectionPolicy+ " COMPLETED";
            case ActionStatus::ERROR:
                return "plan " + settlementName +selectionPolicy+ " ERROR";
            }
            

        }
        AddPlan *clone() const override
        {
            AddPlan* clon = new AddPlan(settlementName, selectionPolicy);
            return clon;
        }
};


class AddSettlement : public BaseAction {
    private:
        const string settlementName;
        const SettlementType settlementType;
    public:
        AddSettlement(const string &settlementName,SettlementType settlementType): BaseAction(),settlementName(settlementName), settlementType(settlementType){}
        void act(Simulation &simulation) override{
            Settlement* s = new Settlement(settlementName,settlementType); 
            if(simulation.addSettlement(s)) 
                this -> complete();
            else
                this->error("Settlement already exists");
            simulation.addAction(this);
        }
        AddSettlement *clone() const override
        {
            AddSettlement* clon = new AddSettlement(settlementName,settlementType);
            return clon;
        }

        const string toString() const override
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
};



class AddFacility : public BaseAction {
    private:
        const string facilityName;
        const FacilityCategory facilityCategory;
        const int price;
        const int lifeQualityScore;
        const int economyScore;
        const int environmentScore;
    public:
        AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):BaseAction(), facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){}
        void act(Simulation &simulation) override
        { 
            FacilityType f = FacilityType(facilityName, facilityCategory ,price, lifeQualityScore, economyScore, environmentScore);
            
            if(simulation.addFacility(f))
                this -> complete();
            else
                this->error("Facility already exists");
            simulation.addAction(this);
        }
        AddFacility *clone() const override
        {
            AddFacility* clon = new AddFacility(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
            return clon;
        }
        const string toString() const override
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
};
class PrintPlanStatus: public BaseAction {
    private:
        const int planId;
    public:
        PrintPlanStatus(int planId):BaseAction(), planId(planId) {}
        void act(Simulation &simulation) override
        {
            Plan p = simulation.getPlan(planId); 
            p.printStatus(); 
            this -> complete();
            simulation.addAction(this);
        }
        PrintPlanStatus *clone() const override
        {
            PrintPlanStatus* clon = new PrintPlanStatus(planId);
            return clon;
        }
        const string toString() const override 
        {
            switch (getStatus())
            {
                case ActionStatus::COMPLETED:
                    return "printPlanStatus COMPLETED";
                case ActionStatus::ERROR:
                    return "printPlanStatus Error";
            }
        }
};


class ChangePlanPolicy : public BaseAction {
    private:
        const int planId;
        const string newPolicy;
    public:
        ChangePlanPolicy(const int planId, const string &newPolicy): planId(planId), newPolicy(newPolicy){}
        void act(Simulation &simulation) override
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
        ChangePlanPolicy *clone() const override;
        const string toString() const override
        {
            switch (getStatus())
            {
                case ActionStatus::COMPLETED:
                    return "changePolicy"+ to_string(planId)+ newPolicy+ "COMPLETED";
                case ActionStatus::ERROR:
                    return "changePolicy"+to_string(planId)+ newPolicy+ "Error";
            }


        }
};


class PrintActionsLog : public BaseAction {
    private:
    public:
        PrintActionsLog(){}
        void act(Simulation &simulation) override
        {
            for(BaseAction* action : simulation.getActions())
                cout << action -> toString() << endl; 
            this->complete();       
            simulation.addAction(this);
        }
        PrintActionsLog *clone() const override;
        const string toString() const override 
        {
            switch (getStatus())
            {
            case ActionStatus::COMPLETED:
                    return "PrintActionsLog COMPLETED";
            case ActionStatus::ERROR:
                    return "PrintActionsLog Error";
            }
        }
};
class Close : public BaseAction {
    private:
    public:
        Close(){}
        void act(Simulation &simulation) override
        {
            for(Plan p : simulation.getPlans())
                p.printStatus(); 
            simulation.close();
            this->complete();
        }
        Close *clone() const override;
        const string toString() const override;
};

class BackupSimulation : public BaseAction {
    public:
        BackupSimulation(){}
        void act(Simulation &simulation) override
        {
        }
        BackupSimulation *clone() const override;
        const string toString() const override;
    private:
};


class RestoreSimulation : public BaseAction {
    public:
        RestoreSimulation();
        void act(Simulation &simulation) override
        {

        }
        RestoreSimulation *clone() const override;
        const string toString() const override;
    private:
};