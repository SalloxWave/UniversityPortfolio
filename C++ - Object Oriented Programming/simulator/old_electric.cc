#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "electric.h"

class Connection
{
public:
    Connection() = default;
    Connection(double _voltage);
    double voltage{0};
};

Connection::Connection(double _voltage)
	: voltage {_voltage}
{}

Component::Component(std::string _name, Connection &pos_con, Connection &neg_con)
	: name {_name}, positive_connection {pos_con}, negative_connection {neg_con}
{}

double Component::getCurrent()
{
	return current;
}

double Component::getVoltage()
{
	/*Returns voltage which is difference between voltage of componet's connections*/

	return std::abs(positive_connection.voltage - negative_connection.voltage);
}

Connection& Component::get_most_positive_side()
{
	/*Returns the connection side with the most voltage*/

	if (positive_connection.voltage > negative_connection.voltage)
	{
		return positive_connection;
	}
	return negative_connection;
}

Connection& Component::get_least_positive_side()
{
	/*Returns the connection side with the most voltage*/

	if (positive_connection.voltage > negative_connection.voltage)
	{
		return negative_connection;
	}
	return positive_connection;
}

std::string Component::getName()
{
	return name;
}

Battery::Battery(std::string _name, double _voltage, Connection &pos_con, Connection &neg_con)
	: Component(_name, pos_con, neg_con), voltage {_voltage}
{}

void Battery::doBehaviour(double time_interval)
{
	/*Set positive connection to same voltage as the battery and
	  set negative connection to 0 voltage*/

	positive_connection.voltage = getVoltage();
	negative_connection.voltage = 0;
}

double Battery::getVoltage()
{
	return voltage;
}

Resistor::Resistor(std::string _name, double _resistance, Connection &pos_con, Connection &neg_con)
	: Component(_name, pos_con, neg_con), resistance {_resistance}
{}

void Resistor::doBehaviour(double time_interval)
{
	/*Move particles from the most positive side to the least positive side.
	  Amount of particles depends on differences in votlage between the sides, 
	  the resistance and the time passing*/

	double diff;
	double particle_amount;
	//Assign which side of resistor has the most and least voltage
	Connection &most_positive { get_most_positive_side() };
	Connection &least_positive { get_least_positive_side() };

	//Calculate difference between highest voltage and lowest voltage
	diff = most_positive.voltage - least_positive.voltage;

	//Calculate amount of particles
	particle_amount = (diff/resistance) * time_interval;

	//Transfer particles from most positive side to least positive side
	most_positive.voltage -= particle_amount;
	least_positive.voltage += particle_amount;
}

double Resistor::getCurrent()
{
	/*Current of resistor is calculated by dividing voltage of the resistor
      with the resistance of the resistor*/

	current = (getVoltage() / resistance);
	return current;
}


Condenser::Condenser(std::string _name, double _capacitance, Connection &pos_con, Connection &neg_con)
	: Component(_name, pos_con, neg_con), capacitance {_capacitance}
{}

void Condenser::doBehaviour(double time_interval)
{
	/*Stores positive particles interally from its most positive side. 
	  Also transforms particles from its most positive side to its least positive side.
	  Amount of particles depends on differences in voltage between the sides,
	  the capcitance, the previous interal charge and the time passing*/

	double diff;
	double particle_amount;
	//Assign which side of resistor has the most and least voltage
	Connection &most_positive { get_most_positive_side() };
	Connection &least_positive { get_least_positive_side() };
	
	//Calculate difference between highest voltage and lowest voltage
	diff = most_positive.voltage - least_positive.voltage;
	//Calculate how much particles to store
	particle_amount = capacitance * (diff - stored_charge) * time_interval;

	//Add particles to internal charge
	stored_charge += particle_amount;
	//Transfer particles from most positive side to least positive side
	most_positive.voltage -= particle_amount;
	least_positive.voltage += particle_amount;
}

double Condenser::getCurrent()
{
	/*Current of condenser is calculated by multiplying
	  capcitance of condenser with the difference between the 
	  voltage of the condenser and the stored charge of the condenser*/

	current = ( capacitance * (getVoltage() - stored_charge) );
	return current;
}