#include <stdio.h>    // puts(), printf(), perror(), getchar()
#include <stdlib.h>   // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h>   // getpid(), getppid(),fork()
#include <sys/wait.h> // wait()

#define READ  0
#define WRITE 1

void child_a(int fd[]) {
  printf(" CHILD A <%ld> I'm alive! My PID is <%ld> and my parent got PID <%ld>.\n",
         (long) getpid(), (long) getpid(), (long) getppid());
        
        // Close the pipe read descriptor.
        close(fd[READ]);
        // Redirect STDOUT to write to the pipe.
        dup2(fd[WRITE], STDOUT_FILENO);
        // Close the dangling pipe write descriptor.
        close(fd[WRITE]);
        // Execute ls -l
        //execlp("ls", "ls", NULL);
  puts("hej");
  printf(" CHILD  A <%ld> Goodbye!\n",
         (long) getpid());
  exit(EXIT_SUCCESS);
}

void child_b(int fd[]) {
   printf(" CHILD B <%ld> I'm alive! My PID is <%ld> and my parent got PID <%ld>.\n",
         (long) getpid(), (long) getpid(), (long) getppid());
    
    // The child process will execute wc.
    // Close the pipe write descriptor.
    close(fd[WRITE]);
    // Redirect STDIN to read from the pipe.
    dup2(fd[READ], STDIN_FILENO);
    // Close the dangling pipe read descriptor.
    //read(fd[READ],"aaaaaaaa",1);
    
    close(fd[READ]);
    // Execute wc
    //execlp("wc", "wc", NULL);  

  printf(" CHILD B <%ld> Goodbye!\n",
         (long) getpid());
  exit(EXIT_SUCCESS);
}

void parent(int pidIDs[]) {
  printf("PARENT <%ld> My PID is <%ld> and I spawned a child with PID <%ld>.\n",
         (long) getpid(), (long) getpid(), (long) pidIDs[0]);
  printf("PARENT <%ld> My PID is <%ld> and I spawned a child with PID <%ld>.\n",
         (long) getpid(), (long) getpid(), (long) pidIDs[1]);
  wait(NULL);
  wait(NULL);

  execlp("ls", "ls", NULL);

  printf("PARENT <%ld> Goodbye!\n",
         (long) getpid());
  exit(EXIT_SUCCESS);
}




int main(void) {
  int fd[2];
  pid_t c1, c2, par;
  int pidIDs[2];
  
  // if (pipe(fd) == -1) {
  //   perror("Creating pipe");
  //   exit(EXIT_FAILURE);
  // }
  pipe(fd);
  c1 = fork();
  pidIDs[0]=c1;
  if (c1 == 0) {
    child_a(fd);

  } else {
      c2 = fork();
      pidIDs[1]=c2;
      
      if (c2 == 0) {
        child_b(fd);
        
      } else {
        parent(pidIDs);
      }
  }

}