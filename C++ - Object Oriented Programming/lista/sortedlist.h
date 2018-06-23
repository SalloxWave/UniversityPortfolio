#ifndef SORTED_LIST_H
#define SORTED_LIST_H

class Sorted_List
{
    struct Node;
public:
    //Default constructor
    Sorted_List() = default;
    //Destructor
    ~Sorted_List();
    //Copy constructor
    Sorted_List(Sorted_List const & other);
    //Copy assignment constructor
    Sorted_List & operator=(Sorted_List const & rhs);
    //Move constructor
    Sorted_List(Sorted_List && other);
    //Move assignment constructor
    Sorted_List & operator=(Sorted_List && rhs);
    void insert (int value);
    void remove (int index);
    int get (int index) const;
    void print_list() const;
    bool is_empty() const;
    int size() const;
private:
    struct Node
    {
        int value;
        Node* next{nullptr};     
    };
    Node* first {nullptr};
    void insert_rec(int value, Node* current);
    void deep_copy(Sorted_List const & other);
    void swap(Sorted_List & other);
    void clear();
};

#endif