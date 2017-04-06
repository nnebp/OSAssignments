#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 255
#define NUMCMDS 3

//TODO REMEMBER TO FREE MEMORY AT THE END!!!!!!
void addFileToArgs(char *args[][MAX], int size, char* fileName)
{
    int i = 0;

    for (i = 0; i < size; i++)
    {
        int j = 0;
        while(args[i][j] != NULL) j++;

        args[i][j] = malloc(strlen(fileName + 1));
        strcpy(args[i][j], fileName);
    }
}

void freeArgs(char *args[][MAX], int size)
{
    int i = 0;

    for (i = 0; i < size; i++)
    {
        int j = 0;
        while(args[i][j] != NULL) 
        {
            free(args[i][j]);
            j++;
        }
    }
}

int main() 
{
    size_t len = MAX;
    char cmds[NUMCMDS][MAX];
    char fileName[MAX];
    int i = 0;

    char *args[NUMCMDS][MAX]; //we can assume we will have less than 255 chars in an arg

    for (i = 0; i < NUMCMDS; i++)
    {
        char *token;

        printf("mash-%c>", (i + 1) + '0');
        fgets(cmds[i], len, stdin);
        cmds[i][strcspn(cmds[i], "\n")] = 0;//strip off newline


        token = strtok(cmds[i], " ");
             
        int j = 0;

        while( token != NULL ) //build argument list
        {
            args[i][j] = malloc(strlen(token + 1));
            strcpy(args[i][j], token);
            token = strtok(NULL, " ");
            j++;
        }
        args[i][j] = NULL;
    }
    
    /* get the file name and add it to the command
     * argument list. I am assuming I need to ask
     * for the filename after the commands like in
     * the directions */
    printf("file>");
    fgets(fileName, len, stdin); //get the filename
    fileName[strcspn(fileName, "\n")] = 0;
    addFileToArgs(args, NUMCMDS, fileName);

    //run all the processes
    int pid;
    int status = 0;
    pid_t wpid[NUMCMDS];
    int finalpid[NUMCMDS];

    for (i = 0; i < NUMCMDS; i++)
    {
        pid = fork();
        if (pid == 0) //child process
        {
            status = execvp(args[i][0], args[i]);

            if (status <  0) //there was an error
                printf("[SHELL %d] STATUS CODE= %d\n", i + 1, status);
            exit(0);
        }
    }
    
    for(i = 0; i < NUMCMDS; i++)
        wpid[i] = wait(NULL);

    printf("Done waiting on children: %d %d %d\n", wpid[0], wpid[1], wpid[2]);
    freeArgs(args, NUMCMDS);
}
