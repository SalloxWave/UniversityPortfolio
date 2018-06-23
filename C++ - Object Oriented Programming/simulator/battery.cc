#include "battery.h"

Battery::Battery(std::string _name, double _voltage, 
                 Connection &pos_con, Connection &neg_con)
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

double Battery::getCurrent()
{
	return 0;
}