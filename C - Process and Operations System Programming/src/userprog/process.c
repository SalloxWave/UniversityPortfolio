#include <debug.h>
#include <stdio.h>
#include <string.h>

#include "userprog/gdt.h"      /* SEL_* constants */
#include "userprog/process.h"
#include "userprog/load.h"
#include "userprog/pagedir.h"  /* pagedir_activate etc. */
#include "userprog/tss.h"      /* tss_update */
#include "filesys/file.h"
#include "threads/flags.h"     /* FLAG_* constants */
#include "threads/thread.h"
#include "threads/vaddr.h"     /* PHYS_BASE */
#include "threads/interrupt.h" /* if_ */
#include "threads/init.h"      /* power_off() */

/* Headers not yet used that you may need for various reasons. */
#include "threads/synch.h"
#include "threads/malloc.h"
#include "lib/kernel/list.h"

#include "userprog/flist.h"
#include "userprog/plist.h"

#include "lib/string.h"

/* HACK defines code you must remove and implement in a proper way */
#define HACK

// DECL_PLIST(plist);
process* plist[PLSIZE];

/* This function is called at boot time (threads/init.c) to initialize
 * the process subsystem. */
void process_init(void)
{
  plist_init(plist);
}

/* This function is currently never called. As thread_exit does not
 * have an exit status parameter, this could be used to handle that
 * instead. Note however that all cleanup after a process must be done
 * in process_cleanup, and that process_cleanup are already called
 * from thread_exit - do not call cleanup twice! */
void process_exit(int status)
{
  plist_find(plist, thread_current()->tid)->exit_status = status;
}

/* Print a list of all running processes. The list shall include all
 * relevant debug information in a clean, readable format. */
void process_print_list()
{
  plist_print(plist);
}


struct parameters_to_start_process
{
  char* command_line;
  //pointer instead?
  struct semaphore binary_semaphore;
  bool success;
  int pid;
  int parent_pid;
};

static void
start_process(struct parameters_to_start_process* parameters) NO_RETURN;

/* Starts a new proccess by creating a new thread to run it. The
   process is loaded from the file specified in the COMMAND_LINE and
   started with the arguments on the COMMAND_LINE. The new thread may
   be scheduled (and may even exit) before process_execute() returns.
   Returns the new process's thread id, or TID_ERROR if the thread
   cannot be created. */
int
process_execute (const char *command_line)
{
  printf("\n****************************************************\n\n");

  char debug_name[64];
  int command_line_size = strlen(command_line) + 1;
  tid_t thread_id = -1;
  int  process_id = -1;

  /* LOCAL variable will cease existence when function return! */
  struct parameters_to_start_process arguments;

  debug("%s#%d: process_execute(\"%s\") ENTERED\n",
        thread_current()->name,
        thread_current()->tid,
        command_line);

  /* COPY command line out of parent process memory */
  arguments.command_line = malloc(command_line_size);
  strlcpy(arguments.command_line, command_line, command_line_size);


  strlcpy_first_word (debug_name, command_line, 64);

  // Use semafor to wait
  // Init binary semaphore
  // TODO/IDEA: Don't use global semaphore. Instead finish plist to make each process has its own sempahore
  // Problem: can't associate process with semaphore before thread with specific process is created
  sema_init(&(arguments.binary_semaphore), 0);

  arguments.success = true;
  arguments.parent_pid = thread_current()->tid;

  /* SCHEDULES function `start_process' to run (LATER) */
  thread_id = thread_create (debug_name, PRI_DEFAULT,
                             (thread_func*)start_process, &arguments);

  printf("AFTER THREAD CREATE!!!!!!!! 1\n");

  // Here we should wait for thread_create to get a result and finish
  if (thread_id != -1)
    sema_down(&(arguments.binary_semaphore));

  printf("AFTER THREAD CREATE!!!!!!!! 2\n");

  //process_id = plist_find(plist, thread_current()->tid);
  process_id = thread_id != -1 ? arguments.pid : thread_id;
  if (!arguments.success)
    process_id = -1;

  printf("AFTER THREAD CREATE!!!!!!!! 3\n");

  /* AVOID bad stuff by turning off. YOU will fix this! */
  //power_off();

  /* WHICH thread may still be using this right now? */
  free(arguments.command_line);

  printf("AFTER THREAD CREATE!!!!!!!! 4\n");

  debug("%s#%d: process_execute(\"%s\") RETURNS %d\n",
        thread_current()->name,
        thread_current()->tid,
        command_line, process_id);

  /* MUST be -1 if `load' in `start_process' return false */
  return process_id;
}

/* A thread function that loads a user process and starts it
   running. */
static void
start_process (struct parameters_to_start_process* parameters)
{
  /* The last argument passed to thread_create is received here... */
  struct intr_frame if_;
  bool success;

  char file_name[64];
  strlcpy_first_word (file_name, parameters->command_line, 64);

  debug("%s#%d: start_process(\"%s\") ENTERED\n",
        thread_current()->name,
        thread_current()->tid,
        parameters->command_line);

  /* Initialize interrupt frame and load executable. */
  memset (&if_, 0, sizeof if_);
  if_.gs = if_.fs = if_.es = if_.ds = if_.ss = SEL_UDSEG;
  if_.cs = SEL_UCSEG;
  if_.eflags = FLAG_IF | FLAG_MBS;

  //printf("file name (start_process): %s", file_name);
  success = load (file_name, &if_.eip, &if_.esp);

  debug("%s#%d: start_process(...): load returned %d\n",
        thread_current()->name,
        thread_current()->tid,
        success);

  if (success)
  {
    /* We managed to load the new program to a process, and have
       allocated memory for a process stack. The stack top is in
       if_.esp, now we must prepare and place the arguments to main on
       the stack. */
       if_.esp = setup_main_stack(parameters->command_line, if_.esp);

    /* A temporary solution is to modify the stack pointer to
       "pretend" the arguments are present on the stack. A normal
       C-function expects the stack to contain, in order, the return
       address, the first argument, the second argument etc. */

    // HACK if_.esp -= 12; /* Unacceptable solution. */

    /* The stack and stack pointer should be setup correct just before
       the process start, so this is the place to dump stack content
       for debug purposes. Disable the dump when it works. */

      // dump_stack ( PHYS_BASE + 15, PHYS_BASE - if_.esp + 16 );

    /*Maybe add process to process list here (if succeded to start ) since the
      thread_current is the newly created thread in this time*/

      //process_list_get_next_free(&plist, &proc)

      // process* proc = (process*) malloc(sizeof(process*));
      // proc.free = 0;
      //proc.pid = thread_current()->tid;
      // proc->parent_pid = parameters->parent_pid;
      // printf("*****JIOGYUFVYGUFVYGVYG***************************\n");
      // printf("thread name: %s\n", thread_current()->name);
      // printf("*****JIOGYUFVYGUFVYGVYG***************************\n");
      // proc.name = thread_current()->name;
      // proc.alive = 1;
      // proc.parent_alive = 1;
      // proc.exit_status = -2;

      //int tid = plist_insert(plist, parameters->parent_pid);
      //thread_current()->tid = 
      int pid = plist_insert(plist, thread_current()->name, parameters->parent_pid);
      parameters->pid = pid;
      thread_current()->tid = pid;
      if (pid == -1)
        success = false;
      
  }
  // thread_current()->tid = plist_insert(plist, parameters->parent_pid);
  

  debug("%s#%d: start_process(\"%s\") DONE\n",
        thread_current()->name,
        thread_current()->tid,
        parameters->command_line);


  /* If load fail, quit. Load may fail for several reasons.
     Some simple examples:
     - File doeas not exist
     - File do not contain a valid program
     - Not enough memory
  */

  parameters->success = success;

  // Use semafor to signal that start_process is finished and has return value
  sema_up(&(parameters->binary_semaphore));
  if ( ! success)
    thread_exit ();  // Leads to process_cleanup


  /* Start the user process by simulating a return from an interrupt,
     implemented by intr_exit (in threads/intr-stubs.S). Because
     intr_exit takes all of its arguments on the stack in the form of
     a `struct intr_frame', we just point the stack pointer (%esp) to
     our stack frame and jump to it. */
  asm volatile ("movl %0, %%esp; jmp intr_exit" : : "g" (&if_) : "memory");
  NOT_REACHED ();
}

/* Wait for process `child_id' to die and then return its exit
   status. If it was terminated by the kernel (i.e. killed due to an
   exception), return -1. If `child_id' is invalid or if it was not a
   child of the calling process, or if process_wait() has already been
   successfully called for the given `child_id', return -1
   immediately, without waiting.

   This function will be implemented last, after a communication
   mechanism between parent and child is established. */
int
process_wait (int child_id)
{
  int status = -1;
  struct thread *cur = thread_current ();

  debug("%s#%d: process_wait(%d) ENTERED\n",
        cur->name, cur->tid, child_id);
  /* Yes! You need to do something good here ! */
  debug("%s#%d: process_wait(%d) RETURNS %d\n",
        cur->name, cur->tid, child_id, status);

  return status;
}

/* Free the current process's resources. This function is called
   automatically from thread_exit() to make sure cleanup of any
   process resources is always done. That is correct behaviour. But
   know that thread_exit() is called at many places inside the kernel,
   mostly in case of some unrecoverable error in a thread.

   In such case it may happen that some data is not yet available, or
   initialized. You must make sure that nay data needed IS available
   or initialized to something sane, or else that any such situation
   is detected.
*/


void
process_cleanup (void)
{
  struct thread  *cur = thread_current ();
  uint32_t       *pd  = cur->pagedir;
  int status = -1;

  //Make sure all files are closed when process is killed
  map_free(&cur->open_files);
  plist_remove(plist, cur->tid);

  debug("%s#%d: process_cleanup() ENTERED\n", cur->name, cur->tid);

  /* Later tests DEPEND on this output to work correct. You will have
   * to find the actual exit status in your process list. It is
   * important to do this printf BEFORE you tell the parent process
   * that you exit.  (Since the parent may be the main() function,
   * that may sometimes poweroff as soon as process_wait() returns,
   * possibly before the printf is completed.)
   */
  printf("%s: exit(%d)\n", thread_name(), status);

  /* Destroy the current process's page directory and switch back
     to the kernel-only page directory. */
  if (pd != NULL)
    {
      /* Correct ordering here is crucial.  We must set
         cur->pagedir to NULL before switching page directories,
         so that a timer interrupt can't switch back to the
         process page directory.  We must activate the base page
         directory before destroying the process's page
         directory, or our active page directory will be one
         that's been freed (and cleared). */
      cur->pagedir = NULL;
      pagedir_activate (NULL);
      pagedir_destroy (pd);
    }
  debug("%s#%d: process_cleanup() DONE with status %d\n",
        cur->name, cur->tid, status);
}

/* Sets up the CPU for running user code in the current
   thread.
   This function is called on every context switch. */
void
process_activate (void)
{
  struct thread *t = thread_current ();

  /* Activate thread's page tables. */
  pagedir_activate (t->pagedir);

  /* Set thread's kernel stack for use in processing
     interrupts. */
  tss_update ();
}

/*****************************************
 ****** OUR CODE *************************
 *****************************************/

#define STACK_DEBUG(...) printf(__VA_ARGS__)

struct main_args
{
  /* Hint: When try to interpret C-declarations, read from right to
   * left! It is often easier to get the correct interpretation,
   * altough it does not always work. */

  /* Variable "ret" that stores address (*ret) to a function taking no
   * parameters (void) and returning nothing. */
  void (*ret)(void);

  /* Just a normal integer. */
  int argc;

  /* Variable "argv" that stores address to an address storing char.
   * That is: argv is a pointer to char*
   */
  char** argv;
};

/* Return true if 'c' is fount in the c-string 'd'
 * NOTE: 'd' must be a '\0'-terminated c-string
 */
bool exists_in(char c, const char* d)
{
  int i = 0;
  while (d[i] != '\0' && d[i] != c)
    ++i;
  return (d[i] == c);
}

/* Return the number of words in 'buf'. A word is defined as a
 * sequence of characters not containing any of the characters in
 * 'delimeters'.
 * NOTE: arguments must be '\0'-terminated c-strings
 */
int count_args(const char* buf, const char* delimeters)
{
  int i = 0;
  bool prev_was_delim;
  bool cur_is_delim = true;
  int argc = 0;

  while (buf[i] != '\0')
  {
    prev_was_delim = cur_is_delim;
    cur_is_delim = exists_in(buf[i], delimeters);
    argc += (prev_was_delim && !cur_is_delim);
    ++i;
  }
  return argc;
}

void* setup_main_stack(const char* command_line, void* stack_top)
{
  /* Variable "esp" stores an address, and at the memory loaction
   * pointed out by that address a "struct main_args" is found.
   * That is: "esp" is a pointer to "struct main_args" */
  struct main_args* esp;
  int argc;
  int total_size;
  int line_size;
  // int cmdl_size;

  /* "cmd_line_on_stack" and "ptr_save" are variables that each store
   * one address, and at that address (the first) char (of a possible
   * sequence) can be found. */
  char* cmd_line_on_stack;
  char* ptr_save;
  int i = 0;

  /* calculate the bytes needed to store the command_line */
  line_size = strlen(command_line) + 1 ; // +1 for end of string
  // STACK_DEBUG("# line_size = %d\n", line_size);

  /* round up to make it even divisible by 4 */
  if (line_size % 4 != 0)
    line_size += 4 - (line_size % 4) ;
  // STACK_DEBUG("# line_size (aligned) = %d\n", line_size);

  /* calculate how many words the command_line contain */
  argc = count_args(command_line, " ") ;
  // STACK_DEBUG("# argc = %d\n", argc);

  /* calculate the size needed on our simulated stack */
  // total_size = line_size + ((argc + 1) * sizeof(char*)) + sizeof(struct main_args) ;
  // total_size = line_size + ((argc + 2) * 4) + 4 + 4 ;
  total_size = line_size + (argc + 1) * sizeof(char*) + sizeof(struct main_args);
            // command_line + argv + argc + return
  // STACK_DEBUG("# total_size = %d\n", total_size);


  /* calculate where the final stack top will be located */
  // stack_top - 4096 == stack_start
  // stack_start + total_size == final_stack_stop
  esp = (struct main_args*) (stack_top - total_size);

  /* setup return address and argument count */
  esp->ret = 0 ;
  esp->argc = argc ;
  /* calculate where in the memory the argv array starts */
  //esp->argv = (char**) (stack_top - total_size) + sizeof(struct main_args) - sizeof(char**);
  //esp->argv = (char**) (stack_top - total_size) + 4 + 4 + 4;
  esp->argv = (char**) (&(esp->argv) + 1);  //+1 adds 4 bytes = 32 bit pointer

  /* calculate where in the memory the words are stored */
  //09a58ea0, 09a58eb4
  cmd_line_on_stack = (char*) (&(esp->argv) + (argc + 2));
  // cmd_line_on_stack = (char*) 0x09a58eb4;
  //STACK_DEBUG("# cmd_line_on_stack address = %c\n", cmd_line_on_stack);

  /* copy the command_line to where it should be in the stack */
  strlcpy(cmd_line_on_stack, command_line, line_size);

  /* build argv array and insert null-characters after each word */
  for (char* token = strtok_r(cmd_line_on_stack, " ", &ptr_save);
       token != NULL;
       token = strtok_r(NULL, " ", &ptr_save),
       ++i) {
         esp->argv[i] = token;
  }

  return esp; /* the new stack top */
}
