#ifndef RESISTOR_H
#define RESISTOR_H

#include <string>

#include "component.h"
#include "connection.h"

class Resistor: public Component
{
public:
    Resistor (std::string _name, double _resistance, 
              Connection &pos_con, Connection &neg_con);
    void doBehaviour(double time_interval) override;
    double getCurrent() override;
private:    
    double resistance;
};

#endif