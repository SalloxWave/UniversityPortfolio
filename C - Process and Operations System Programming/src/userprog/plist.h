#ifndef _PLIST_H_
#define _PLIST_H_


/* Place functions to handle a running process here (process list).
   
   plist.h : Your function declarations and documentation.
   plist.c : Your implementation.

   The following is strongly recommended:

   - A function that given process inforamtion (up to you to create)
     inserts this in a list of running processes and return an integer
     that can be used to find the information later on.

   - A function that given an integer (obtained from above function)
     FIND the process information in the list. Should return some
     failure code if no process matching the integer is in the list.
     Or, optionally, several functions to access any information of a
     particular process that you currently need.

   - A function that given an integer REMOVE the process information
     from the list. Should only remove the information when no process
     or thread need it anymore, but must guarantee it is always
     removed EVENTUALLY.
     
   - A function that print the entire content of the list in a nice,
     clean, readable format.
     
 */

//Maximum amount of processes in OS
#define PLSIZE 100
// #define DECL_PLIST(Name) process* Name[PLSIZE];

typedef struct
{
    // int free;
    int pid;
    int parent_pid;
    char* name;
    int alive;
    int parent_alive;
    int exit_status;
} process;

void plist_init(process** plist);

int plist_insert(process** plist, char* name, int parent_pid);

process* plist_find(process** plist, int pid);

void plist_remove(process** plist, int pid);

void plist_print(process** plist);


#endif
