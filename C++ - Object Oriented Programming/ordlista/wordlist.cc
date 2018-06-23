#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

using word_list = vector<pair<string, int>>;
using ordered_word_list = vector<string>;

enum class Sort_Type
{
    Alphabetical,
    Frequence,
    Ordered
};

bool is_valid_char(char c)
{
    /*Decides if a character is valid.
      This is decided by checking if character is a letter or a hypgen*/

    //hyphen(-) = 45 in ASCII table
    int ascii_num = static_cast<int>(c);
    return isalpha(c) || c == '-';
}

bool valid_word(string const &word)
{
    /*Decides if word is valid
      This is done by checking if word has at least 3 characters
      and if there is any invalid characters in the word*/

    //Word must contain at least three letters
    //All-of
    if (word.length() < 3)
    {
        return false;
    }
    for (int i {0}; i < word.length(); ++i)
    {
        //Letter is not valid
        if (!is_valid_char(word.at(i)))
        {
            return false;
        }
    }

    if (word.find("--") != string::npos)
    {
        return false;
    }

    return true;
}

void clean_word(string &word)
{
    /*Clean a word from junk character's.
      This is done by splitting the word where a junk character is not found.
      Initial junk: " ' (
      Terminative junk: ? ; , : ! . " ' )
      Other junk: genitives ('s)*/

    //Characters that should be cleaned before the word
    char initial_junk[] {'"', '\'', '('};
    //Character that should be cleaned after the word
    char terminative_junk[] {'?', ';', ',', ':', '!', '.', '\"', '\'', ')'};

    /*Find where the word begins and ends by finding position 
      where a junk character is NOT found*/
    size_t word_begin = word.find_first_not_of(initial_junk);
    size_t word_end = word.find_last_not_of(terminative_junk);
    //Word contained only junk
    if (word_begin == string::npos || word_end == string::npos)
    {
        //Don't clean word
        word_begin = 0;
        word_end = word.size();
    }
    else
    {
        //Get length of word
        word_end = (word_end - word_begin) + 1;    
    }

    //Assign new word to word without junk characters
    word = word.substr(word_begin, word_end);
    //Check if there's a possibility word can contain a genitive
    if (word.size() >= 2)
    {
        //If word contains genetive
        if (word.back() == 's' && word.at(word.size() - 2) == '\'')
        {
            //Erase apostrophe and coming 's' letter
            word.erase(word.size() - 2, 2);
        }
    }    
}

void sort_word_list(word_list &word_lst, Sort_Type sort_type)
{ 
    /*Sort a word list by the specified sort type.
      Sort type alphabetical and frequence both leads to sorting
      by frequence a word was found.
      Sort type ordered means nothing is done because a word list
      is already sorted in order words was found in file*/

    if (sort_type == Sort_Type::Alphabetical)
    {
        //Sort list by amount of words found
        sort(begin(word_lst), end(word_lst),
                [](pair<string, int> p1, pair<string, int> p2)
                {
                    return p1.second > p2.second;
                });
        //Sort alphabetically
        sort(begin(word_lst), end(word_lst));
        
    }
    else if (sort_type == Sort_Type::Frequence)
    {
        //Sort alphabetically
        sort(begin(word_lst), end(word_lst));

        //Sort list by amount of words found
        sort(begin(word_lst), end(word_lst),
                [](pair<string, int> p1, pair<string, int> p2)
                {
                    return p1.second > p2.second;
                });        
    }
}

void print_restricted_word(string const & word,
                           int row_restriction, 
                           int &current_length)
{
    /*Print specified word either on current row or on next row 
      depending on specified restriction on the row.
      current length is to keep track of how long the row
      currently is.*/

    //Check if word fits on row
    if (current_length + word.size() >= row_restriction)
    {
        //Move to next row before printing word
        cout << "\n";
        cout << word;
        //Reset row length
        current_length = 0;
    }
    else
    {
        cout << word;
    }
    //Print space between words
    cout << " ";
    //Change current row length to length of word included with space
    current_length += word.size() + 1;
}

void print_ordered_word_list(ordered_word_list const &ordered_word_lst,
                             int row_restriction = 0)
{
    /*Prints an ordered word list where each the words on a line 
      is restrictied by the specified row restriction.
      If the word goes out of the restriction the word will be printed 
      on the next line.*/

    //To keep track of current row length
    int current_length {0};
    //Print word for each word in word list
    for_each(begin(ordered_word_lst), end(ordered_word_lst),
        [row_restriction, &current_length](string word)
        {
            //Print word either on this row or on next row depending on restrictions
            print_restricted_word(word, row_restriction, current_length);    
                           
        });
    cout << endl;
}

void print_word_list(word_list const &word_lst, Sort_Type sort_type)
{
    /*Prints a word list based on the sort type of the word list.
      Sort type alphabetical means table should be left aligned while
      sort type Frequence means it should be right aligned.*/

    //Decide length of column in table based on longest word
    int col_length = (max_element
    (
        cbegin(word_lst), cend(word_lst),
        [](pair<string, int> const &p1, pair<string, int> const &p2)
        { return p1.first.size() < p2.first.size(); }
    )->first).length();

    if (sort_type == Sort_Type::Frequence)
    {
        //Right align words
        cout << right << fixed;
    }
    else
    {
        //Left align words and create space between word and count
        cout << left << fixed;
        col_length = col_length + 2;
    }

    //Print header
    cout << setw(col_length) << "Word"
         << setw(8) << "Count" << endl;
    //Go through word list
    for_each(begin(word_lst), end(word_lst),
        [col_length](pair<string, int> word_pair)
        {
            //Print word
            cout << setw(col_length) << word_pair.first;
            //Print word count
            cout << setw(8) << word_pair.second << "\n";
        });
    /*
    for ( word_list::const_iterator it = cbegin(word_lst); 
          it != cend(word_lst); ++it )
    {
        //Print word
        cout << setw(col_length) << it->first;
        //Print word count
        cout << setw(8) << it->second << "\n";
    }*/
    cout << flush;
}

void print_running_instructions(string prog_name = "<program name>")
{
    /*Print for user how to run the specified program*/
    cout << "How to run program:\n"
         << "\"" << prog_name << "\" <filename> <sorting flag>\n"
         << "Sorting flags:\n"
         << "-a: Alphabetical order\n"
         << "-f: Frequence order\n"
         << "-o <max characters on row>: Order words was found in file with a maximum row length\n"
         << endl;
}

bool valid_sort_argument(string sort_arg)
{
    /*Returns if sorting argument is any of the 
    possible sorting arguments*/

    return (sort_arg == "-a" 
         || sort_arg == "-f" 
         || sort_arg == "-o");
}

bool valid_arguments(vector<string> arguments)
{
    /*Checks if all arguments to the program are valid.
      If not, print the error message and return false*/    

    /*Check if arguments are inputted in a wrong way
      This is done by checking if there is a correct amount of
      arguments are if sort flag is a valid flag*/
    if ( arguments.size() < 2
      || !valid_sort_argument(arguments[1]) 
      || (arguments[1] == "-o" && arguments.size() != 3) )
    {
        print_running_instructions();
        return false;
    }
    //Open file from filename from argument
    ifstream file { arguments[0] };
    //File could not be opened
    if (!file)
    {
        cerr << "File: " << arguments[0] << " could not be found" << endl;
        return false;
    }
    //When sort option is ordered
    if (arguments[1] == "-o")
    {
        //Check if row restriction is a number
        int row_restriction;
        try
        {
            row_restriction = stoi(arguments[2]);
        }
        catch(invalid_argument)
        {
            cerr << arguments[2] << " must be a number" << endl;
            return false;
        }
        //Check if row restriction is negative
        if (row_restriction <= 0)
        {
            cerr << arguments[2] << " must be a positive number above zero" << endl;
            return false;
        }        
    }
    return true;
}

Sort_Type convert_flag_to_sort_type(string str)
{
    /*Check what sort-type command flag represents 
      and return that sort type*/

    if (str == "-a")
    {
        return Sort_Type::Alphabetical;
    }
    else if (str == "-f")
    {
        return Sort_Type::Frequence;
    }
    else if (str == "-o")
    {
        return Sort_Type::Ordered;
    }
}

int main(int argc, char *argv[])
{
    //Convert arguments to string
    vector<string> args { argv+1, argv+argc };

    //Check if arguments are valid
    if (!valid_arguments(args))
    {
        return 1;
    }

    //Open file with words
    string filename { args[0] };
    fstream file { filename };

    //Get sort type from specfied sort flag
    Sort_Type sort_type = convert_flag_to_sort_type(args[1]);

    //Get row restriction if sort type was ordered
    int row_restriction;
    if (sort_type == Sort_Type::Ordered)
    {
        row_restriction = stoi(args[2]);
    } 

    //Declare the two types of word lists
    word_list word_lst;
    ordered_word_list ordered_word_lst;

    string word;
    //Go through every word in file separated with whitespace or new line
    for (; file >> word;)
    {        
        //Clean words of none-wanted characters
        clean_word(word);
        //Check if word is a valid word
        if( valid_word(word) )
        {
            //Make word lowercase
            transform(begin(word), end(word), begin(word), ::tolower);
            //Should use an ordered word list
            if (sort_type == Sort_Type::Ordered)
            {
                ordered_word_lst.push_back(word);
            }
            //Should use a word list
            else
            {
                //Check where word is in word list (if there is a word)
                auto it = find_if(begin(word_lst), end(word_lst),
                    [word](pair<string, int> const & p)
                    {
                        return p.first == word;
                    });
                //Word exist in word list
                if (it != end(word_lst))
                {
                    //Increase count of that word
                    it->second++;
                }
                //Word didn't exist in word list           
                else
                {
                    //Add new word to word list
                    word_lst.push_back(make_pair(word, 1));
                }
            }
        }
    }

    //Sort word list by specified sort order
    sort_word_list(word_lst, sort_type);    

    //Check which type of word list to use
    if (sort_type == Sort_Type::Ordered)
    {
        //Print word from ordered word list
        print_ordered_word_list(ordered_word_lst, row_restriction);
    }
    else
    {
        //Print word from word list
        print_word_list(word_lst, sort_type);
    }

    return 0;
}