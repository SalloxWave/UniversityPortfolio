#include "condenser.h"

Condenser::Condenser(std::string _name, double _capacitance, 
                     Connection &pos_con, Connection &neg_con)
    : Component(_name, pos_con, neg_con), capacitance {_capacitance}
{}

void Condenser::doBehaviour(double time_interval)
{
    /*Stores positive particles interally from its most positive side. 
      Also transforms particles from its most positive side to its 
      least positive side.
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

    return ( capacitance * (getVoltage() - stored_charge) );
}