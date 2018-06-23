#include <iostream>
#include <iomanip>

using namespace std;

struct vattable
{
    float start_price {};
    float end_price {};
    float step_length {};
    float vat_percent {};
};

void fill_table_from_user_input(vattable & vtable);
float calc_vat(float const price, float const vat_percent);
float calc_vat_price(float const price, float const vat_percent);
void print_vat_table(vattable const vtable);

int main()
{
     //Create table
    vattable my_vattable {};

    //Fill table with user values
    fill_table_from_user_input(my_vattable);

    //Print table to screen
    print_vat_table(my_vattable);
    return 0;
}

void fill_table_from_user_input(vattable & vtable)
{
    // 8-2
    float user_input {};
    bool valid_input {};

    //Get start price
    do
    {
        valid_input = false;
        cout << "Mata in första pris: ";
        cin >> user_input;

        if (user_input >= 0)
        {
            valid_input = true;
            vtable.start_price = user_input;
        }
        else
        {
            cout << "FEL: Första pris måste vara minst 0 (noll) kronor\n";
        }
    }
    while(!valid_input);

    //Get end price
    do
    {
        valid_input = false;
        cout << "Mata in sista pris: ";
        cin >> user_input;

        if (user_input <= vtable.start_price)
        {
            cout << "FEL: Du måste ha större sista pris än första pris\n";
        }
        else
        {
            valid_input = true;
            vtable.end_price = user_input;
        }
    }
    while(!valid_input);

    //Get step length
    do
    {
        valid_input = false;
        cout << "Mata in steglängd: ";
        cin >> user_input;

        if (user_input < 0.01)
        {
            cout << "FEL: Steglängden måste vara minst 0.01\n";
        }
        else if (user_input > (vtable.end_price - vtable.start_price))
        {
            cout << "FEL: Du har för stor steglängd\n";
        }
        else
        {
            valid_input = true;
            vtable.step_length = user_input;
        }
    }
    while(!valid_input);

    do
    {
        valid_input = false;
        cout << "Mata in momsprocent: ";
        cin >> user_input;

        if (user_input > 0)
        {
            valid_input = true;
            vtable.vat_percent = user_input;
        }
        else
        {
            cout << "FEL: Moms måste vara minst 0 procent\n";
        }
    }
    while(!valid_input);    
}

float calc_vat(float const price, float const vat_percent)
{
    return price * (vat_percent/100);
}

float calc_vat_price(float const price, float const vat_percent)
{
    return price * ((vat_percent/100) + 1);
}

void print_vat_table(vattable const vtable)
{
    //Get amount of rows in table (+1 to include first row)
        static_cast<int>
        (
            (vtable.end_price - vtable.start_price) / vtable.step_length
        ) + 1;

    cout << right << fixed << setprecision(2) 
         << setw(15) << "Pris" 
         << setw(15) << "Moms" 
         << setw(20) << "Pris med moms" << endl 
         << string(50, '-') << "\n";

    for (int i {0}; i < table_rows; ++i)
    {
        float price = vtable.start_price + (i * vtable.step_length);
        float vat = calc_vat(price, vtable.vat_percent);
        float vat_price = calc_vat_price(price, vtable.vat_percent);
        cout << setw(15) << price
             << setw(15) << vat 
             << setw(20) << vat_price    
             << endl;
    }
}
