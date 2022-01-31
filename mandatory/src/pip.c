// Example2.c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>   // getpid(), getppid(),fork()
#include <sys/wait.h> // wait()

void child_a(int fd[],char *string){
    close(fd[0]);//Child process does not need this end of the pipe
    /* Send "string" through the output side of pipe */
    write(fd[1], string, (strlen(string)+1));
    exit(EXIT_SUCCESS);

}

void child_b(int fd[], int nbytes, char *readbuffer){
                
    close(fd[1]);//Parent process does not need this end of the pipe

    /* Read in a string from the pipe */
    nbytes = read(fd[0], readbuffer, sizeof(readbuffer+1));
    printf("Read string: %s", readbuffer);

    printf("\nCHILD B <%ld> Goodbye!\n", (long) getpid());
    exit(EXIT_SUCCESS);
}

void parent(int pidIDs[]) {

  wait(NULL);
  wait(NULL);
   printf("PARENT <%ld> Goodbye!\n",
         (long) getpid());
  exit(EXIT_SUCCESS);
}



int main()
{
        int fd[2], nbytes;
        pid_t c1;
        pid_t c2;
        pid_t par;
        int pidIDs[3];
        char string[] = "Hello,world!\n";
        char readbuffer[80];

        pipe(fd);
       
        if((c1 = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

        if(c1 == 0)
        {   
            pidIDs[0] = c1;
            child_a(fd, string);
        }
        else
        {
            if(c2 = fork()== 0){
                pidIDs[1] = c2;
                child_b(fd, nbytes, readbuffer);
            }
            else{
                pidIDs[2] = par;
                parent(pidIDs);
            }
        }
       
        return(0);
}