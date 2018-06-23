#include <iostream>
#include <iomanip>
#include <vector>
#include <exception>
#include <string>

#include "component.h"
#include "connection.h"
#include "battery.h"
#include "resistor.h"
#include "condenser.h"



// Comments from TA:
//    You shouldn't require to use 3 different vectors between each of
//    the tests, why not re-use the old vector? (change so the same
//    vector is used for all tests)
//
//    It is unnecessary to use a template for "clear_pointer_vector",
//    and if you were suppose to use a template, it is better to have:
//    void clear_pointer_vector(vector<T*> p_vector) rather than:
//    void clear_pointer_vector(T p_vector) although don't use
//    template here, you have full knowledge of what to pass, and it
//    is Only one type of instance that is passed. Templates are used
//    to allow multiple different instances to be passed to the same
//    function, each new object type will create a new instance of
//    that function. (correct this)
//
//    All non-primitive parameters should be constant references.
//    primitives are: char, int, double, float, bool (and other
//    similar types (unsigned, long and combinations of these,
//    unsigned int, long int, unsigned long etc.))
//

// Alexander: Thanks for feedback! Everything should be fixed.
using namespace std;

void print_running_instructions(string const & program_name)
{
    /*Print message for user how to run the program.*/

    cout << "How to run program:\n\""
         << program_name << "\" <iterations to simulate> "
         << "<amount of rows to print> \n"
         << "<time interval to simulate each step> "
         << "<voltage of the battery>" << endl;
}

void print_negative_error(string const & obj)
{
    /*Print error message that the specified object
    can not be a negative number.*/

    cerr << obj << " can not be negative" << endl;
}

void print_invalid_error(string const & obj, string const & wanted_object)
{
    /*Print error message that the specified object
    is not a valid wanted object'.*/

    cerr << obj << " is not a valid " << wanted_object << endl;
}

bool arguments_is_valid(vector<string> const & args)
{
    /*Decides if the arguments for this program is valid.
    This is done by first checking if there is a valid amount of arguments.
    If not, print how to user how to run the program and return false.
    Secondly, check if there is a problem with any argument.
    If a problem was found print an informative error message about the problem
    and return false.*/

    int iterations;
    int row_count;
    double time_interval;
    double battery_voltage;

    if (args.size() != 5)
    {
        print_running_instructions(args[0]);
        return false;
    }

    //Check iterations
    try
    {
        iterations = stoi(args[1]);
        if (iterations < 0)
        {
            //Iterations can't be negative
            print_negative_error("Iteration");
            return false;
        }
    }
    catch(invalid_argument)
    {
        print_invalid_error(args[1], "iteration");
        return false;
    }

    //Check row count
    try
    {
        row_count = stoi(args[2]);
        if (row_count < 0)
        {
            //Row count can't be negative
            print_negative_error("Row count");
            return false;
        }
    }
    catch(invalid_argument)
    {
        print_invalid_error(args[2], "row count");
        return false;
    }

    //Check time interval
    try
    {
        time_interval = stod(args[3]);
        if (time_interval < 0)
        {
            //Time interval can't be negative
            print_negative_error("Time interval");
            return false;
        }
    }
    catch(invalid_argument)
    {
        print_invalid_error(args[3], "time interval");
        return false;
    }

    //Check battery voltage
    try
    {
        battery_voltage = stod(args[4]);
        if (battery_voltage < 0)
        {
            //Voltage can't be negative
            print_negative_error("Battery voltage");
            return false;
        }
    }
    catch(invalid_argument)
    {
        print_invalid_error(args[4], "battery voltage");
        return false;
    }
    return true;
}

void simulate(vector<Component*> const & circuit, int iterations,
              int row_count, double time_interval)
{
    /*Performs a simulation of a circuit board
      Parameters:
      circuit: list of components on circuit board
      iterations: Amount of loops/iterations in circuit board
      row_count: Amount of rows to print in the table. Row is printed after x amount of iterations.
                 x is calculated by dividing iterations with row_count.
      time_interval: Time interval constant value to use in simulation*/

    //Setup the header of the table
    for (Component* comp: circuit)
    {
        cout << setw(12) << comp->getName();
    }
    cout << endl;
    for (Component* comp: circuit)
    {
        cout << setw(6) << "Volt"
             << setw(6) << "Curr";
    }
    cout << endl;

    //Get amount of iterations for each row
    int row_iterations { iterations / row_count };
    for (int i{0}; i < row_count; ++i)
    {
        //Iterations per row
        for (int j{0}; j < row_iterations; ++j)
        {
            //Do behaviour of all components on circuit board
            for (Component* comp: circuit)
            {
                comp->doBehaviour(time_interval);
            }
        }
        //Print result
        cout << right << fixed << setprecision(2);
        for (Component* comp: circuit)
        {
            cout << setw(6) << comp->getVoltage()
                 << setw(6) << comp->getCurrent();
        }
        cout << endl;
    }
}

void clear_circuit_board(vector<Component*> & circuit_board)
{
    /*Delete all components on the circuit board*/
    for(Component* comp: circuit_board)
    {
        delete comp;
    }
    circuit_board.clear();
}

int main(int argc, char *argv[])
{
    //Convert c-string arguments to string arguments
    vector<string> args {argv, argv+argc};

    //Print errors if arguments are invalid
    if (!arguments_is_valid(args))
    {
        //Exit program
        return 1;
    }
    //Set values from argument list
    int iterations {stoi(args[1])};
    int row_count {stoi(args[2])};
    double time_interval {stod(args[3])};
    double battery_voltage {stod(args[4])};

    //Assign circuit board
    vector<Component*> circuit_board;

    //Start simulation for test case 1
    Connection P;
    Connection N;
    Connection R124;
    Connection R23;
    circuit_board.push_back(new Battery("Bat", battery_voltage, P, N));
    circuit_board.push_back(new Resistor("R1", 6.0, R124, P));
    circuit_board.push_back(new Resistor("R2", 4.0, R23, R124));
    circuit_board.push_back(new Resistor("R3", 8.0, R23, N));
    circuit_board.push_back(new Resistor("R4", 12.0, R124, N));
    simulate(circuit_board, iterations, row_count, time_interval);
    //Clear circuit board from its componets
    clear_circuit_board(circuit_board);

    //Start simulation for test case 2
    Connection P2;
    Connection N2;
    Connection L2;
    Connection R2;
    circuit_board.push_back(new Battery("Bat", battery_voltage, P2, N2));
    circuit_board.push_back(new Resistor("R1", 150, L2, P2));
    circuit_board.push_back(new Resistor("R2", 50, R2, P2));
    circuit_board.push_back(new Resistor("R3", 100, R2, L2));
    circuit_board.push_back(new Resistor("R4", 300, N2, L2));
    circuit_board.push_back(new Resistor("R5", 250, N2, R2));
    simulate(circuit_board, iterations, row_count, time_interval);
    //Clear circuit board from its componets
    clear_circuit_board(circuit_board);

    //Start simulation for test case 3
    Connection P3;
    Connection N3;
    Connection L3;
    Connection R3;
    circuit_board.push_back(new Battery("Bat", battery_voltage, P3, N3));
    circuit_board.push_back(new Resistor("R1", 150, L3, P3));
    circuit_board.push_back(new Resistor("R2", 50, R3, P3));
    circuit_board.push_back(new Condenser("C3", 1.0, R3, L3));
    circuit_board.push_back(new Resistor("R4", 300, N3, L3));
    circuit_board.push_back(new Condenser("C5", 0.75, N3, R3));
    simulate(circuit_board, iterations, row_count, time_interval);
    //Clear circuit board from its componets
    clear_circuit_board(circuit_board);

    return 0;
}
