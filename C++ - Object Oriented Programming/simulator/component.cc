#include <cmath>

#include "component.h"


Component::Component(std::string _name, Connection &pos_con, 
                                        Connection &neg_con)
    : name {_name}, positive_connection {pos_con}, negative_connection {neg_con}
{}

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