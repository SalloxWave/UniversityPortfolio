#ifndef ELECTRIC_H
#define ELECTRIC_H

#include <string>

class Component
{
public:
    Component(std::string _name, Connection &pos_con, Connection &neg_con);
    virtual void doBehaviour(double time_interval) = 0;
    virtual double getCurrent();
    std::string getName();
    virtual double getVoltage();
protected:  
    Connection& get_most_positive_side();
    Connection& get_least_positive_side();

    std::string name;
    double current{0};
    Connection &positive_connection;
    Connection &negative_connection;
};

class Battery: public Component
{
public:
    Battery() = default;
    Battery (std::string _name, double _voltage, Connection &pos_con, Connection &neg_con); 
    void doBehaviour(double time_interval) override;
    double getVoltage() override;
private:
    double voltage;
};

class Resistor: public Component
{
public:
    Resistor (std::string _name, double _resistance, Connection &pos_con, Connection &neg_con);
    void doBehaviour(double time_interval) override;
    double getCurrent() override;
private:    
    double resistance;
};

class Condenser: public Component
{
public:
    Condenser (std::string _name, double _capacitance, Connection &pos_con, Connection &neg_con);
    void doBehaviour(double time_interval) override;
    double getCurrent() override;
private:    
    double capacitance;
    double stored_charge;
};


#endif
