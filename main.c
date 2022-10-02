#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define TRUE 1

void type_prompt(){
    static int first_time = 1;
    if(first_time){
        const char *CLEAR_SCREEN_ANSI;
        CLEAR_SCREEN_ANSI = " \e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        first_time = 0;
    }
    printf("#"); //Display do prompt
}

void read_command (char command[], char* parameters[]) {
    char line[1024];
    int count = 0, i=0, j=0;
    char *array[100], *pch;

    //Leitura da linha do prompt
    for( ; ; ){
        int c = fgetc(stdin);
        line[count++] = (char) c;
        if (c=='\n') break;
    }
    if (count == 1) return;
    pch = strtok(line, "\n");

    //analisar a linha em palavras
    while(pch != NULL){
        array[i++] = strdup(pch);
        pch = strtok(NULL, "\n");
    }
    //primeira palavra do command
    strcpy(command, array[0]);

    //As próximas são parâmetros
    for (int j =0; j<i; j++){
        parameters[j] = array[j];
        parameters[i] = NULL; //NULL é o final da lista de parâmetros
    }
}

int main () {
    char cmd[100], command[100], *parameters[20];
    char *envp[] = { (char*) "Path=/bin", 0};

    while(TRUE){ //Repete sempre
        type_prompt(); //Abre um prompt na tela
        read_command(command, parameters); //Ler o input do terminal
        if (fork() != 0) wait(NULL); //Pai esperando pelo filho
        else {
            strcpy(cmd, "/bin/");
            strcat(cmd, command);
            execve(cmd, parameters, envp); //Executa o comando
        }

        if (strcmp(command, "exit") == 0) break; //Se o comando for exit, fecha o shell.
    }
}
