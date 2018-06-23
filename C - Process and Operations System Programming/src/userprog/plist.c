#include <stddef.h>
#include <stdio.h>
#include "plist.h"
#include "threads/synch.h"
#include "threads/malloc.h"

struct semaphore binary_semaphore;

void plist_init(process** plist)
{
    for (int i=0; i < PLSIZE; ++i)
    {
        plist[i] = NULL;
    }
    sema_init(&binary_semaphore, 1);
}

int plist_insert(process** plist, char* name, int parent_pid)
{
    printf("BEGINEBIGNJBEBIG UBIG\n");
    sema_down(&binary_semaphore);    
    int rval = -1;
    for (int i=0; i < PLSIZE; ++i)
    {
        if (plist[i] == NULL)
        {
            process* proc = (process*) malloc(sizeof(process));

            int namelen = strlen(name) + 1;
            proc->name = (char*)malloc(namelen);
            strlcpy(proc->name, name, namelen);

            proc->pid = i+3;
            proc->parent_pid = parent_pid;
            proc->alive = 1;
            proc->parent_alive = 1;
            proc->exit_status = -1;
            plist[i] = proc;
            rval = proc->pid;
            break;
        }
    };
    sema_up(&binary_semaphore);
    printf("ENDENDNENDNDNDNENDN\n");
    return rval;
}

process* plist_find(process** plist, int pid)
{
    printf("FINDFINDIFNFINFDINIFNIDNID: %d\n", pid);
    return plist[pid-3];
}

void plist_remove(process** plist, int pid)
{
    printf("Removing process %d\n", pid);
    sema_down(&binary_semaphore);
    pid = pid-3;
    if (pid != -1 && plist[pid] != NULL)
    {
        for (int i=0; i < PLSIZE; ++i)
        {
            if (plist[i] != NULL && plist[i]->parent_pid == pid)
            {
                if (plist[i]->alive)
                {
                    plist[i]->parent_alive = 0;                    
                }
                else                
                {
                    free(plist[i]->name);
                    free(plist[i]);
                    plist[i] = NULL;
                }
            }
        }
        if (plist[pid]->parent_alive)
        {
            plist[pid]->alive = 0;
        }
        else
        {
            free(plist[pid]->name);
            free(plist[pid]);
            plist[pid] = NULL;
        }
    }
    sema_up(&binary_semaphore);
}

void plist_print(process** plist)
{
    sema_down(&binary_semaphore);
    printf("%15s%15s%15s%15s%15s%15s\n", "name", "pid", "parent_pid", "alive", "parent_alive", "exit_status");
    for (int i=0; i < PLSIZE; ++i)
    {
        if (plist[i] != NULL)
        {
            printf("%15s%15d%15d%15d%15d%15d\n",
                plist[i]->name,
                plist[i]->pid,
                plist[i]->parent_pid,
                plist[i]->alive,
                plist[i]->parent_alive,
                plist[i]->exit_status
            );
        }
    }
    sema_up(&binary_semaphore);
}