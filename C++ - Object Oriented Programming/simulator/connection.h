#ifndef CONNECTION_H
#define CONNECTION_H

class Connection
{
public:
    Connection() = default;
    Connection(double _voltage);
    double voltage {0};
};

#endif