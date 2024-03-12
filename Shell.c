#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_LEN 1024
#define MAX_COMMANDS 35
/*
Integrantes:

Isaac Fabian Palma Medina 1 1865 0422
Karla Verónica Quiros Delgado 6 0465 0870
*/
void mostrarShell()
{
    printf("\033[0;31m");
    printf("PalmaQuirosShell>> ");
    printf("\033[0m");
}
//Cuenta los caracteres validos de un arreglo de chars
int contarChars(char str[]) {
    int num = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        num++;
    }
    return num - 1;
}
//Verifica entradas validas(inicada en ! y seguida de un numero, no permite espacios o caracteres, letras)
bool revisarDigitos(char *str) {
    int largo = contarChars(str);
    for (int i = 1; i < largo; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true && str[0] == '!';
}

// viewHistorial: funcion que devuelve los ultimos comandos ingresados en un rango de 1-35(los ultimos 35)

void viewHistorial(int* actual_comando, int * contador,char ** historial)
{
    int numComandos = *contador < MAX_COMMANDS ? *contador : MAX_COMMANDS;
    for (int i = numComandos - 1; i >= 0; i--)
    {
        int indice = (*actual_comando - numComandos + i + MAX_COMMANDS) % MAX_COMMANDS;
        printf("%d: %s", i + 1, historial[indice]);
    }
}

// comandoSolicitado: verifica si el numero de comando ingresado existe en el historial y si existe lo muestra, sino muestra un mensaje

void comandoSolicitado(char *entrada, int *contador, char **historial) {
    int cont = *contador;
    int comandoNum = atoi(entrada + 1); // Convertir la parte numérica de la entrada en un entero.
    
    if(!revisarDigitos(entrada)){
    	printf("La entrada no cumple con el formato requerido.\n");
        return;
    }
    else if (comandoNum < 1 || comandoNum > 35 ) {
        printf("Numero de comando solicitado, fuera del rango del historial(1-35)\n");
        return;
    }
    if (cont == 0){
        printf("El historial esta vacio.\n"); 
        return; 
    } else if (comandoNum > cont) {
        printf("No existe un comando en esa posicion\n");
        return;
    }
    printf("%s", historial[comandoNum - 1]);
}

// ultimoEsAnd: verifica si en la entrada de argumentos el ultimo elemento es &

bool ultimoEsAnd(char **argumentos)
{
    int i = 0;
    while (argumentos[i + 1] != NULL)
    {
        i++;
    }
    return strcmp(argumentos[i], "&") == 0;
}

// tokenizar: separa una cadena de caracteres en tokens, segun un delimitador

void tokenizar(char *str, const char *delim, char **arr1, char **arr2)
{
    char *token;
    int pos = 0;
    bool piped = false;
    token = strtok(str, delim);
    while (token != NULL)
    {
        if (*token == '|'){
            piped = true;
            pos = 0;
        }
        else{
            if (piped == true){
                arr2[pos] = token;
                pos++;
            }
            else{
                arr1[pos] = token;
                pos++;
            }
        }
        token = strtok(NULL, delim);
    }
}

void inicializarHistorial(char** historial) {
    for (int i = 0; i < MAX_COMMANDS; i++) {
        historial[i] = malloc(MAX_LEN * sizeof(char));
    }
}

int main(int argc, char *argv[]) {

    char** historial = malloc(MAX_COMMANDS * sizeof(char*));
    inicializarHistorial(historial);
    
    int* actual_comando = malloc(sizeof(int));
    int* contador = malloc(sizeof(int));
    *actual_comando = 0;
    *contador = 0;
    char entrada[MAX_LEN]; // Entrada del usuario.

    for (;;) 
    {
        mostrarShell();

    verificar:
        if (fgets(entrada, sizeof(entrada), stdin) == NULL || entrada[0] == '\n') // Obtener la entrada del usuario.
        {
            // En caso de que la entrada sea inválida.
            mostrarShell();
            goto verificar;
        }

        if (strcmp(entrada, "exit\n") == 0){break;}
        // Opción que el usuario ingrese !N siendo n el numero de comando que desea obtener del historial.
        else if (entrada[0] == '!')
        {
            if (entrada[2]=='\0'){
                printf("Necesita un numero de comando para buscar, seguido del ! \n");
            }
            else{comandoSolicitado(entrada,contador,historial);}
        }
        else if (strcmp(entrada, "historial\n") == 0 || strcmp(entrada, "historial &\n") == 0)
        {
            viewHistorial(actual_comando,contador,historial);
        }
        else
        {
            if (*contador >= MAX_COMMANDS)
            {
            for (int i = 0; i < MAX_COMMANDS - 1; i++)
                {
                    strcpy(historial[i], historial[i + 1]);
                }
                strcpy(historial[MAX_COMMANDS - 1], entrada);
               
            }
            else{
                strcpy(historial[*actual_comando], entrada);
                *actual_comando = (*actual_comando + 1) % MAX_COMMANDS;
                *contador = *contador + 1;
            }

            char *arg1[MAX_LEN];
            char *arg2[MAX_LEN];

            for (int i = 0; i < MAX_LEN; i++){ arg1[i] = NULL;}

            for (int i = 0; i < MAX_LEN; i++){arg2[i] = NULL;}

            tokenizar(entrada, " \n", arg1, arg2);

            if (arg2[0] == NULL)
            {
                bool segundoPlano = ultimoEsAnd(arg1);
                int pid = fork();
                if (pid < 0)
                {
                    printf("Error al crear el proceso hijo.\n");
                    exit(1);
                }
                else if (pid == 0)
                {
                    // Proceso hijo.
                    if (segundoPlano)
                    {
                        // Se elimina el "&" del arreglo de argumentos.
                        int j = 0;
                        while (arg1[j + 1] != NULL)
                        {
                            j++;
                        }
                        arg1[j] = NULL;
                        sleep(2);
                        printf("\nProceso hijo dice:\n");
                    }

                    execvp(arg1[0], arg1);

                    printf("\tError al ejecutar el comando, comando no válido.\n");
                    exit(0);
                }
                else
                {
                    // Proceso padre.
                    if (!segundoPlano)
                    { 
                        int status;
                        waitpid(pid, &status, 0);
                    }
                    else
                    { // No se espera al proceso hijo.
                        printf("Proceso en segundo plano.\n");
                    }
                }
            }
            else
            {
                int extremos[2];
                int pipeStatus = pipe(extremos);
                if (pipeStatus == -1)
                {
                    exit(1);
                }
                int pid1 = fork();
                if (pid1 == 0)
                {
                    close(STDOUT_FILENO);
                    dup(extremos[1]);
                    close(extremos[0]);
                    close(extremos[1]);

                    execvp(arg1[0], arg1);
                    printf("\tError al ejecutar el comando, comando no válido.\n");
                    exit(0);
                }
                int pid2 = fork();
                if (pid2 == 0)
                {
                    close(STDIN_FILENO);
                    dup(extremos[0]);
                    close(extremos[1]);
                    close(extremos[0]);

                    execvp(arg2[0], arg2);
                       printf("\tError al ejecutar el comando, comando no válido.\n");
                    exit(0);
                }

                close(extremos[0]);
                close(extremos[1]);
                int status1;
                int status2;
                waitpid(pid1, &status1, 0);
                waitpid(pid2, &status2, 0);
            }
        }
    }
	free(contador);
	free(actual_comando);
    	free(historial);
    return 0;
}
/*
Referencias:

https://www.geeksforgeeks.org/piping-in-unix-or-linux/
https://stackoverflow.com/questions/13801175/classic-c-using-pipes-in-execvp-function-stdin-and-stdout-redirection
https://cplusplus.com/reference/cstring/strcmp/
https://www.programacion.com.py/escritorio/c/pipes-en-c-linux
https://stackoverflow.com/questions/8058704/how-to-detect-empty-string-from-fgets
https://www.cs.swarthmore.edu/~newhall/unixhelp/c_codestyle.html
https://linux.die.net/man/3/execlp
https://stackoverflow.com/questions/40605075/how-to-compare-2-character-arrays

*/
