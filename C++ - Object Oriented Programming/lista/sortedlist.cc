// BIG TODO!!
//   Move items that should not, and items that definitely is not
//   allowed to be in the header file, to here.
//
// Other todo's:
//   Throughout the code there are some added comments by me.
//   These comments are initiated with "//#" to find them easier.
//
// Other hints:
//   When writing comments, try writing a descriptive block of text,
//   which aids in understanding the entire concept of the function.
//   I've added some comments myself as some inspiration. :)
//   These comments uses /// instead of //#

//Thanks for the feedback!
//I've added some comments marked with //%
//Also I've addded desciptive blocks with marked with ///

#include <iostream>

#include "sortedlist.h"

using namespace std;

Sorted_List::~Sorted_List()
{
    //Clear the list
    clear();
}

Sorted_List::Sorted_List(Sorted_List const & other)
{
    //Create a deep copy from the other list
    deep_copy(other);
}

void Sorted_List::deep_copy(Sorted_List const & other)
{
    /// Creates a deep copy of a list and store in this object.
    /// Take first node of the other list and make
    /// a new node with that value connected to this list.
    /// If list you want to copy is empty don't copy at all.
    /// That will result in first node in list gaining
    /// its default value nullptr (representing an empty list)

    //To not copy an empty list
    if (other.first != nullptr)
    {
        //Set first value
        first = new Node{other.first->value};
        Node* other_curr {other.first};
        Node* this_curr {first};
        //Shall copy more than one value
        while (other_curr->next != nullptr)
        {
            this_curr->next = new Node{other_curr->next->value};
            this_curr = this_curr->next;
            other_curr = other_curr->next;
        }
    }
}

void Sorted_List::swap(Sorted_List & other)
{
    /// Using standard libary's swap function to make 
    /// two values point to each other, i.e swapping.

    std::swap(first, other.first);
}

Sorted_List & Sorted_List::operator=(Sorted_List const & rhs)
{
    /// Create a temporary copy of the input value using the
    /// copy-constructor, then swap yourself with the temporary list
    /// to make that one handle the cleanup of any old value still in
    /// your own list.

    //Create temporary value from right side,
    Sorted_List tmp{rhs};
    //Swap temporary value with left side ("this")
    swap(tmp);
    //Give currently swaped value to itself
    return *this;
}

//# Can you figure out a quicker solution to the move-constructor?
//# Maybe without copying?
Sorted_List::Sorted_List(Sorted_List && other)
{
    /// Swap values with this list and the right hand side.
    /// When the temporary right hand size gets destroyed,
    /// its destructor is destroyed which means it doesn't
    /// matter what value right hand side gets from this list.
    /// Though, this results in this list getting
    /// the right hand side's value.

    swap(other);
}

Sorted_List & Sorted_List::operator=(Sorted_List && rhs)
{
    /// Swap values with this list and the right hand side.
    /// When the temporary right hand size gets destroyed,
    /// its destructor is destroyed which means it doesn't
    /// matter what value right hand side gets from this list.
    /// Though, this results in this list getting
    /// the right hand side's value.

    //Swap values
    swap(rhs);
    //Give currently swapped value to itself.
    return *this;
    //# good comment
    //% Thanks
}

void Sorted_List::insert (int value)
{
    ///Insert value in correct position of the sorted list.
    ///The value should be inserted in beginning of the list when:
    ///A: The list is empty.
    ///B: First value of list is greater than value.
    //If none of these applies, call a iterating recursive function
    //to find correct position to insert the value.

    if (is_empty() || value < first->value)
    {
        first = new Node{value, first};
    }
    else
    {
        insert_rec(value, first);
    }
}

void Sorted_List::insert_rec(int value, Node* current)
{
    /// Insert a value after the current one if one of two criteria is
    /// fulfilled:
    ///  1) there is no node after the current node
    ///  2) the node to insert is smaller than the node after current
    ///     node (which mean that current->val < value < current->next->val)
    ///
    /// If a new value is not added, recursively call this function
    /// with current->next instead of current

    //Last node of list or value should be inserted behind current node
    if (current->next == nullptr || value < current->next->value)
    {
        current->next = new Node{value, current->next};
    }
    else
    {
        //Check next node of list
        insert_rec(value, current->next);
    }
}

void Sorted_List::remove (int index)
{
    /// Removes a node at a specific index.
    /// This is only done if list is not empty.
    /// If index is 0, delete first node and assign
    /// new first node according to the current second node.
    /// Otherwise, iterate trough list until 
    /// correct position is found, and when found
    /// delete node and change previous node to point
    /// to node deleted value pointed to.

    if (is_empty())
    {
        return;
    }
    if (index == 0)
    {
        Node* tmp {first->next};
        delete first;
        first = tmp;
        //# What happens if you directly do:
        //# first = tmp;
        //#
        //# without any check or anything?

        //% Good point!
        //% first will point to tmp value which
        //% is a node or nullptr
    }
    else
    {
        //To stand on previous node
        index--;

        Node* current {first};
        for (int i {0}; i < index+1; ++i)
        {
            if (index == i)
            {
                Node* tmp {current->next->next};
                delete current->next;
                current->next = nullptr;
                //Node is between nodes
                if (tmp != nullptr)
                {
                    current->next = tmp;
                }
                //delete tmp;
                tmp = nullptr;
            }
            //Go to next node in list
            current = current->next;
        }
    }
}

//# Is it possible to solve this function without first fetching the
//# size?

//% Yes, you can use a while loop which can be seen below
void Sorted_List::clear()
{
    /// Removes all items from list until the list is empty.
    /// This is done by removing first index of list until
    /// first node of list doesn't point to a value anymore.

    //% This is an alternative way
    while (first != nullptr)
    {
        remove(0);
    }
}

void Sorted_List::print_list() const
{
    /// Print all values from the sorted list (sorted order).
    /// This is done by iterating through the list
    /// and sending the value of current node into
    /// console output stream.

    Node* current {first};
    //Iterate until the end of the list
    while (current != nullptr)
    {
        cout << current->value << " ";
        //Go to next list node
        current = current->next;
    }
    cout << endl;
}

int Sorted_List::get(int index) const
{
    /// Gets value from list by specific index.
    /// This is done by going through the list until
    /// the correct index position.
    /// Then return the value
    /// of node on the found position.

    Node* current {first};
    for (int i {0}; i < index; ++i)
    {
        //# Will this ever be fulfilled?

        //% Oh, that's right. Loop will automatically
        //% stop when found correct index.

        //Go to next node in list
        current = current->next;
    }
    return current->value;
}

bool Sorted_List::is_empty() const
{
    /// Decides if the list is empty or not. 
    /// List is empty if first node of index doesn't
    /// point to a value at all.

    return (first == nullptr);
}

int Sorted_List::size() const
{
    /// Decides the size of the list.
    /// That is done by going through the list until
    /// a node without a pointer to the next node is found.
    /// Size is calculated from amount of iterations until
    /// end of the list.

    Node* current {first};
    int count {0};
    while(current != nullptr)
    {
        count++;
        current = current->next;
    }
    return count;
}