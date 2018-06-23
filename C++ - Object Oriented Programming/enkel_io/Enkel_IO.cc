#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;


// 1-3 (använd ej tab)
// 1-4
// 2-3
// 2-11

void print_number(int &user_int)
{
    cout << "Skriv in ett heltal: ";
    cin >> user_int;
    cout << "Du skrev in talet: " << user_int << endl;
}

void print_five_numbers(int &user_int)
{
    cout << "Skriv in fem heltal: ";

    cin >> user_int;
    cout << "Du skrev in talen: " << user_int << ", ";
    cin >> user_int;
    cout << user_int << ", ";
    cin >> user_int;
    cout << user_int << ", ";
    cin >> user_int;
    cout << user_int << ", ";
    cin >> user_int;
    cout << user_int << endl;   
}

void print_float(double &user_double)
{
    cout << fixed << setprecision(3);
    cout << "Skriv in ett flyttal: ";
    cin >> user_double;
    cout << "Du skrev in flyttalet: " << user_double << endl;
}

void print_int_float(int &user_int, double &user_double)
{
    cout << "Skriv in ett heltal och ett flyttal: ";
    cin >> user_int >> user_double;
    cout << "Du skrev in heltalet: " << user_int 
         << "\nDu skrev in flyttalet: " << user_double << endl;
}

void print_char(char &user_char)
{
    cout << "Skriv in ett tecken: ";
    cin >> user_char;
    cout << "Du skrev in tecknet: " << user_char << endl;
}

void print_string_length(string &user_string)
{
    cout << "Skriv in en sträng: ";
    cin >> user_string;
    cout << "Strängen '" << user_string << "' har " 
         << user_string.size() << " tecken." << endl;
}

void print_int_string(int &user_int, string &user_string)
{
    cout << "Skriv in ett heltal och en sträng: ";
    cin >> user_int >> user_string;
    cout << "Du skriv in talet: |" << user_int 
         << "| och strängen |" << user_string << "|." << endl;
}

void print_string_float(string &user_string, double &user_double)
{
    cout << "Skriv in en sträng och ett flyttal: ";
    cin >> user_string >> user_double;
    cout << "Du skrev in talet \"" << user_double << "\" och \"" 
         << user_string << "\"" << endl;
}

void print_string_row(string &user_string)
{
    cout << "Skriv in en hel rad med text: ";
    getline(cin, user_string);
    cout << "Du skrev in textraden: '" << user_string << "'" << endl;
}

void print_string_negative_int(string &user_string, int &user_int)
{
    cout << "Skriv in en textrad som slutar med ett negativt heltal: ";
    getline(cin, user_string, '-');
    cin >> user_int;
    cout << "Du skrev in textraden: '" << user_string 
         << "' och heltalet '-" << user_int << "'" << endl;
}

int main()
{
    int user_int;
    char user_char;
    double user_double;
    string user_string;

    //Get number from user and print the number
    print_number(user_int);
    cout << "\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //Get five numbers from user and print the numbers
    print_five_numbers(user_int);
    cout << "\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //Get float number from user and print the number
    print_float(user_double);
    cout << "\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //Get float and integer number and print them
    print_int_float(user_int, user_double);
    cout << "\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //Get char from user and print
    print_char(user_char);
    cout << "\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //Get string from user and count letters
    print_string_length(user_string);
    cout << "\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //Get int and string from user and print...
    print_int_string(user_int, user_string);
    cout << "\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //Get string and float from user and print...
    print_string_float(user_string, user_double);
    cout << "\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    //Get row string from user and print...
    print_string_row(user_string);
    cout << "\n";

    //Get row string ending with negative int
    print_string_negative_int(user_string, user_int);

    return 0;
}

