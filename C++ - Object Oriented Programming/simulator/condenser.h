#ifndef CONDENSER_H
#define CONDENSER_H

#include <string>

#include "component.h"
#include "connection.h"

class Condenser: public Component
{
public:
    Condenser (std::string _name, double _capacitance, 
               Connection &pos_con, Connection &neg_con);
    void doBehaviour(double time_interval) override;
    double getCurrent() override;
private:    
    double capacitance;
    double stored_charge;
};

#endif