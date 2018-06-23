#include "resistor.h"

Resistor::Resistor(std::string _name, double _resistance, 
                   Connection &pos_con, Connection &neg_con)
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

    return (getVoltage() / resistance);
}