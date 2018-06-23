#include <stdio.h>
#include <syscall-nr.h>
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"

/* header files you probably need, they are not used yet */
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/vaddr.h"
#include "threads/init.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "devices/input.h"

#include "../lib/debug.h"
#include "flist.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");  
}


/* This array defined the number of arguments each syscall expects.
   For example, if you want to find out the number of arguments for
   the read system call you shall write:
   
   int sys_read_arg_count = argc[ SYS_READ ];
   
   All system calls have a name such as SYS_READ defined as an enum
   type, see `lib/syscall-nr.h'. Use them instead of numbers.
 */
const int argc[] = {
  /* basic calls */
  0, 1, 1, 1, 2, 1, 1, 1, 3, 3, 2, 1, 1, 
  /* not implemented */
  2, 1,    1, 1, 2, 1, 1,
  /* extended */
  0
};

static void
syscall_handler (struct intr_frame *f) 
{
  int32_t* esp = (int32_t*)f->esp;

  switch ( esp[0] )
  {
    case SYS_HALT:
    {
      debugv2("Halting!\n");
      power_off();
    }
    break;
    case SYS_EXIT:
    {
      debugv2("Exiting with status code %d", esp[1]);
      process_exit(esp[1]);
      thread_exit();
    }
    break;
    case SYS_READ:
    {
      int fd = esp[1];      
      char* buffer = (char*)esp[2];
      int bsize = esp[3];

      if (fd == STDIN_FILENO)
      {
        for (int i=0; i < bsize; ++i)
        {
          char c = input_getc();
          if (c == '\r') c = '\n';
          printf("%c", c);
          buffer[i] = c;  
        }
        f->eax = bsize;
      }            
      else if (fd == STDOUT_FILENO)
      {
        f->eax = -1;
      }
      //Read from a file
      else
      {
        struct file* file = map_find(&thread_current()->open_files, fd);
        if (file == NULL)
        {
          f->eax = -1;
        }
        else
        {
          f->eax = file_read(file, buffer, bsize);
        }
      }
    }
    break;
    case SYS_WRITE:    
    {
      int fd = esp[1];      
      char* buffer = (char*)esp[2];
      int bsize = esp[3];

      if (fd == STDOUT_FILENO)
      {
        putbuf(buffer, bsize);
        f->eax = bsize;
      }      
      else if(fd == STDIN_FILENO)
      {
        f->eax = -1;
      }
      //Write to a file
      else
      {
        struct file* file = map_find(&thread_current()->open_files, fd);
        if (file == NULL)
        {
          f->eax = -1;
        }
        else
        {
          f->eax = file_write(file, buffer, bsize);
        }        
      }
    }
    break;
    case SYS_CREATE:
    {
      char* filename = (char*)esp[1];
      unsigned initial_size = esp[2];
      f->eax = filesys_create(filename, initial_size);
    }
    break;
    case SYS_OPEN:
    {
      char* filename = (char*)esp[1];
      struct file* result = filesys_open(filename);
      if (result == NULL)
      {
        f->eax = -1;        
      }
      else
      {
        f->eax = map_insert(&thread_current()->open_files, result);
      }
    }
    break;
    case SYS_CLOSE:
    {
      int fd = esp[1];
      struct file* file = map_find(&thread_current()->open_files, fd);
      if (file != NULL)
      {
        map_remove(&thread_current()->open_files, fd);
        filesys_close(file);
      }
    }    
    break;
    case SYS_REMOVE:
    {
      char* filename = (char*)esp[1];
      f->eax = filesys_remove(filename);
    }
    break;
    case SYS_SEEK:
    {
      int fd = esp[1];
      unsigned pos = esp[2];
      struct file* file = map_find(&thread_current()->open_files, fd);
      if (file != NULL)
      {
        file_seek(file, pos);
      }
    }
    break;
    case SYS_TELL:
    {
      int fd = esp[1];      
      struct file* file = map_find(&thread_current()->open_files, fd);
      if (file == NULL)
      {
        f->eax = -1;
      }
      else
      {
        f->eax = file_tell(file); 
      }
    }
    break;
    case SYS_FILESIZE:
    {
      int fd = esp[1];      
      struct file* file = map_find(&thread_current()->open_files, fd);
      if (file == NULL)
      {
        f->eax = -1;
      }
      else
      {
        f->eax = file_length(file);
      }
    }
    break;
    case SYS_EXEC:
    {
      const char *command_line = esp[1];
      int pid = process_execute(command_line);
      // process_wait(pid);
      f->eax = pid;
    }
    break;
    case SYS_SLEEP:
    {
      timer_sleep(esp[1]);
    }
    break;
    case SYS_PLIST:
    {
      process_print_list();
    }
    break;
    default:
    {
      printf ("Executed an unknown system call!\n");
      
      printf ("Stack top + 0: %d\n", esp[0]);
      printf ("Stack top + 1: %d\n", esp[1]);      
      
      thread_exit ();
    }
  }
}
