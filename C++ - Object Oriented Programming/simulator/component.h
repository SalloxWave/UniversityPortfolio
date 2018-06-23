#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include "connection.h"

class Component
{
public:
    Component(std::string _name, Connection &pos_con, 
                                 Connection &neg_con);
    virtual void doBehaviour(double time_interval) = 0;
    virtual double getCurrent() = 0;
    std::string getName();
    virtual double getVoltage();
protected:  
    Connection& get_most_positive_side();
    Connection& get_least_positive_side();

    std::string name;
    Connection &positive_connection;
    Connection &negative_connection;
};

#endif