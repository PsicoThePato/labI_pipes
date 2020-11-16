#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int main()
{
    int fd[3][2];
    
    for(int i = 0; i < 3; i++)
    {
        if(pipe(fd[i]) < 0)
            return 1;
    }


    int pid1 = fork();
    if (pid1 < 0)
        return 2;
        
    if (pid1 == 0)
    {
        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[2][0]);
        close(fd[2][1]);
        
        int random_x;
        read(fd[0][0], &random_x, sizeof(int));
        printf("INFO ---- random x: %d\n", random_x);
        
        int n_caracteres;
        read(fd[0][0], &n_caracteres, sizeof(int));
        char msg[n_caracteres];
        read(fd[0][0], msg, sizeof(char) * n_caracteres);
        printf("%s\n", msg);

        close(fd[0][0]);

        int array_len = (random() % 10) +1;
        printf("%d\n", array_len);
        int array[array_len];
        for(int i; i <= array_len; i++)
        {
            array[i] = (random() % random_x) + 1;
            printf("%d\n", array[i]);
        }
        close(fd[1][1]);


    }
    
    close(fd[0][0]);
    close(fd[1][0]);
    close(fd[1][1]);
    close(fd[2][1]);
    
    int x;
    scanf("%d", &x);
    srand(5); // fixed random seed for debug purposes
    int x_random = (random() % x) + 1;
    write(fd[0][1], &x, sizeof(int));

    char msg[] = "Meu filho, crie e envie para o seu irmão um array de números inteiros com valores randômicos entre 1 e o \
                valor enviado anteriormente. O tamanho do array também deve ser randômico, na faixa de 1 a 10";
    int n_caracteres = strlen(msg)+1;
    write(fd[0][1], &n_caracteres, sizeof(int));
    write(fd[0][1], msg, n_caracteres * sizeof(char));
    
    close(fd[0][1]);
    close(fd[2][0]);
    wait(NULL);
}