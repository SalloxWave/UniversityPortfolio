#ifndef BATTERY_H
#define BATTERY_H

#include <string>

#include "component.h"
#include "connection.h"

class Battery: public Component
{
public:
    Battery() = default;
    Battery (std::string _name, double _voltage, 
             Connection &pos_con, Connection &neg_con); 
    void doBehaviour(double time_interval) override;
    double getVoltage() override;
    double getCurrent() override;
private:
    double voltage;
};

#endif