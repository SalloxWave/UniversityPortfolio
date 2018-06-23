#include <fstream>
#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <array>
#include <vector>

using namespace std;

class Trimino
{
public:
    Trimino(int n1, int n2, int n3)
    {
        numbers = {n1, n2, n3};
    }
    Trimino(array<int, 3> arr)
        : numbers {arr}
    {}
    bool valid_interval(array<int, 2> interval) const
    {
        /// Check if all trimino numbers are in correct specified interval.
        /// This is done by going through all numbers and check if number
        /// is inside of the interval
        /// If not all are correct, return false.

        for (int i {0}; i < numbers.size(); ++i)
        {
            if (numbers[i] < interval[0] 
             || numbers[i] > interval[1])
            {
                return false;
            }
        }
        return true;
    }
    bool valid_order() const
    {
        /// Check if trimino numbers are rising in clock order.
        /// If not, return false.

        return (numbers[0] <= numbers[1] && numbers[1] <= numbers[2]
             || numbers[0] >= numbers[1] && numbers[1] <= numbers[2]
             || numbers[0] <= numbers[1] && numbers[1] >= numbers[2]);
    }
private:
    array<int, 3> numbers;
};

string get_filename();
array<int, 2> get_interval();
bool valid_args(int argc, char* argv[], bool read_from_file = true);
bool valid_file(string const& filename);
bool validate_interval_arg(string const& low, string const& high);
vector<Trimino> get_triminos(istream & input_stream, 
                             array<int, 2> const& interval, 
                             string const& filename = "");
void print_trimino_error(string const& filename, 
                         int file_row, string const& msg, 
                         bool read_from_file = true);
void print_running_instructions(string const& program_name, 
                                bool read_from_file = true);

int main(int argc, char* argv[])
{
    //Declare wanted variables
    bool read_from_file {true};
    ifstream trimino_file;
    string filename;
    vector<Trimino> trimino_lst;
    array<int, 2> interval;

    //No arguments given->user manually inputs arguments
    if (argc == 1)
    {
        //Get wanted trimino interval
        interval = get_interval();

        if (read_from_file)
        {
            //Get valid filename
            string filename = get_filename();
            //Open file for reading
            trimino_file.open(filename, ifstream::in);        
            //Get triminos
            trimino_lst = get_triminos(trimino_file, interval, filename);    
        }
        else
        {
            cout << "Input trimino numbers and press enter" << endl;
            cout << "Input \"q\" to stop inputting triminos" << endl;
            //Make sure cin is empty before using it
            cin.ignore(10000,'\n');
            cin.clear();
            //Get file from console input
            trimino_lst = get_triminos(cin, interval);
        }        
    }
    //Wrong amount of arguments
    else if( (read_from_file && argc != 4) 
          || (!read_from_file && argc !=3) )
    {
        string prog_name (argv[0]);
        print_running_instructions(prog_name, read_from_file);
        return 1;
    }
    //Arguments is correct
    else if(valid_args(argc, argv, read_from_file))
    {
        //Triminos should be read from file
        if (read_from_file)
        {
            //Open the file with triminos
            filename = string(argv[1]);
            trimino_file.open(filename, ifstream::in);
            //Get interval from arguments
            interval[0] = stoi(argv[2]);
            interval[1] = stoi(argv[3]);
            //Get the triminos from trimino file
            trimino_lst = get_triminos(trimino_file, interval, filename);
        }        
        //Triminos should be read from console input
        else
        {
            cout << "Input trimino numbers and press enter" << endl;
            cout << "Input \"q\" to stop inputting triminos" << endl;
            //Get interval from arguments
            interval[0] = stoi(argv[1]);
            interval[1] = stoi(argv[2]);
            //Get file from console input
            trimino_lst = get_triminos(cin, interval);
        }   
    }
   
    return 0;
}

string get_filename()
{
    /// Asks the user for filename.
    /// If the inputted filename isn't valid
    /// print the error and let user input new filename.
    /// This is repeated until the filename is correct.

    string filename;
    cout << "Input filename to triminos: ";
    cin >> filename;
    //While inputted filename was not valid
    while ( !valid_file(filename) )
    {
        cout << "Could not find file " << filename << endl;
        cout << "Input filename: ";
        //Get filename from console input
        cin >> filename;
    }
    return filename;
}

array<int, 2> get_interval()
{
    /// Asks the user for low and high interval 
    /// for the triminos.
    /// If the interval wasn't correct, print what was wrong
    /// let user input again.
    /// This is repeated until the user is correct.

    //Declare wanted variables
    array<int, 2> interval;
    string low;
    string high;

    cout << "Input high and low interval separated with space: ";
    cin >> low >> high;
    //While inputted interval was not valid
    while ( !validate_interval_arg(low, high) )
    {
        cout << "\nInput high and low interval separated with space: ";
        //Get low and high interval from console input
        cin >> low >> high;
    }
    //Set the correct interval values
    interval[0] = stoi(low);
    interval[1] = stoi(high);
    return interval;
}

bool valid_args(int argc, char* argv[], bool read_from_file)
{
    /// Validate and return if the inputted arguments 
    /// were correct or not.
    /// This is done by validating the filename 
    /// (if filename should be obtained from a file)
    /// and the interval.

    //Position in argument list for high and low interval
    int low_index {1};
    int high_index {2};
    //Trimino should be read from file
    if (read_from_file)
    {
        //Open file by filename from argument
        string filename(argv[1]);
        //Filename wasn't valid
        if (!valid_file(filename))
        {
            cout << "Could not find file " << filename << endl;
            return false;
        }
        //Low and high interval is inputted after the filename
        low_index = 2;
        high_index = 3;
    }

    //Get interval from argument
    string interval_low(argv[low_index]);
    string interval_high(argv[high_index]);

    //Interval was not valid
    return validate_interval_arg(interval_low, interval_high);
}

bool valid_file(string const& filename)
{
    /// Opens file from filename and returns
    /// if the file could be opened or not

    ifstream file;
    file.open(filename, ifstream::in);
    //File could not be opened
    if (!file)
    {
        return false;
    }
    return true;
}

bool validate_interval_arg(string const& low, string const& high)
{
    /// Validates if the interval inputted in string 
    /// is correct or not.
    /// The interval is not correct if:
    /// 1: The interval couldn't be converted to integer
    /// 2: Lower interval is greater or equal than higher interval.
    /// If the interval is not correct, print what wasn't correct
    /// and return false.

    int interval_low;
    int interval_high;
    try
    {
        //Try to convert interval into integer
        interval_low = stoi(low);
        interval_high = stoi(high);
        if (interval_low > interval_high)
        {
            cerr << "Higher interval must be higher than lower interval" << "\n";
            return false;
        }
    }
    //Could not convert interval into integer
    catch(invalid_argument e)
    {
        cerr << "\"" << low << "\" to \"" << high << "\"" 
             << " is not a valid interval!\n"
             << "Interval must be inputted <low interval> <high interval>\n";
        return false;
    }
    return true;
}

vector<Trimino> get_triminos(istream & input_stream, 
                             array<int, 2> const& interval, 
                             string const& filename)
{
    /// Read triminos from either file or console input.
    /// If a trimono was not valid, print the error and
    /// continue reading next trimino.
    /// Then return a list of all valid triminos.

    //Filename has default value means you should not read from file
    bool read_from_file = (filename.size() != 0);

    vector<Trimino> trimino_lst;

    string line;
    int line_count {1};
    // Read line by line from input stream until there isn't 
    // anything more to read
    while ( getline(input_stream, line) )
    {
        //User wanted to quit inputting triminos
        if ( !read_from_file && line == "q")
        {
            break;
        }
        array<int, 3> tri_numbers;
        //Convert string to istringstream
        istringstream ss_line (line);
        string column;
        int i {0};
        try
        {            
            //Read column by column
            for (; ss_line >> column;)
            {       
                //Stop loop when there have been found three trimino numbers
                if (i == 3)
                {                    
                    break;
                }
                //Try to get trimino number from column in file
                tri_numbers[i] = stoi(column);
                ++i;
            }
            //Create a trimino with found numbers
            Trimino tri{tri_numbers};
            //Trimino doesn't have three numbers
            if ( i != 3)
            {
                string msg = "Trimino needs 3 numbers! ";
                msg+=to_string(i) + " was found";
                print_trimino_error(filename, line_count, msg, read_from_file);
            }
            //Trimino numbers are not in correct order (rising clockwise)
            else if (!tri.valid_order())
            {
                string msg = "Trimino numbers are not rising in clock order";
                print_trimino_error(filename, line_count, msg, read_from_file);
            }
            //Trimino numbers are out of range, not in selected interval
            else if(!tri.valid_interval(interval))
            {
                string msg = "One or more trimino numbers are out of range!";
                print_trimino_error(filename, line_count, msg, read_from_file);
            }
            //Fully valid trimino! :D
            else
            {
                cout << "Successfully created a trimino\n" << endl;
                trimino_lst.push_back(tri);
            }
        }
        //Failed to get (convert) trimino number
        catch(invalid_argument e)
        {
            string msg = "\"" + column + "\" is not a valid number";
            print_trimino_error(filename, line_count, msg, read_from_file);
        }
        ++line_count;
    }
    return trimino_lst;
}

void print_running_instructions(string const& program_name, 
                                bool read_from_file)
{
    /// Print how to run the program depending on
    /// if trimino numbers should be read from 
    /// file or console input

    //Triminos wanted to be read from file
    if (read_from_file)
    {
    cerr << "How to run program:\n" 
         << "\"" << program_name << "\""
         << " <filename> <lowest interval> <highest interval>\n"
         << "...or use no arguments to manually enter values\n";    
    }
    //Triminos wanted to be read from console input
    else
    {
    cerr << "How to run program:\n" 
         << "\"" << program_name << "\""
         << " <lowest interval> <highest interval>\n"
         << "...or use no arguments to manually enter values\n";
    }    
}

void print_trimino_error(string const& filename, 
                         int file_row, string const& msg, 
                         bool read_from_file)    
{
    /// Print error saying what was wrong with a trimino.
    /// If the trimino was read from a file, also print
    /// the filename and which line trimino was found.

    //Triminos wanted to be read from file
    if (read_from_file)
    {
        cerr << "Error in file " << filename 
         << " on row " << to_string(file_row)
         << ":\n" << msg << "\n""\n";    
    }
    //Triminos wanted to be read from console input
    else
    {
        cerr << "Error: " << msg << "\n""\n";    
    }
}