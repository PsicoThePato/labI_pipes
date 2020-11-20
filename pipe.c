#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcnt1.h>

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
        
    else if (pid1 == 0)
    {
        // CÓDIGO DO P1 //
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
        printf("INFO ---- O TAMANHO DO ARRAY É: %d\n", array_len);
        int array[array_len];
        
        for(int i= 0; i < array_len; i++)
        {
            array[i] = (random() % random_x) + 1;
            printf("O ELEMENTO %d DO ARRAY É: %d\n", i,  array[i]);
        }

        write(fd[1][1], &array_len, sizeof(int));
        write(fd[1][1], array, array_len * sizeof(int));
        close(fd[1][1]);

        // FIM DO CÓDIGO DE P1 //
    }

    else
    {  
        int pid2 = fork();
        if (pid2 < 0)
            return 3;
        if (pid2 == 0)
        {
            // CÓDIGO DO P2 //
            close(fd[0][0]);
            close(fd[0][1]);
            close(fd[1][1]);
            close(fd[2][0]);

            int array_len;
            read(fd[1][0], &array_len, sizeof(int));
            int array[array_len];
            read(fd[1][0], array, array_len*sizeof(int));
            close(fd[1][0]);

            int sum = 0;
            for(int i = 0; i < array_len; i++)
            {
                sum += array[i];
            }

            write(fd[2][1], &sum, sizeof(int));
            close(fd[2][1]);

            // FIM DO CÓDIGO DO P2 //
        }

        else
        {
            // CÓDIGO DO PAI //
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
            
            int sum;
            read(fd[2][0], &sum, sizeof(int));
            printf("A soma é: %d\n", sum);
            
            int status = 0;
            pid_t wpid;
            while ((wpid = wait(&status)) > 0);
            close(fd[0][1]);
            close(fd[2][0]);

            char ping_str[] = "ping -c 5 ufes.br";

            pid_t pid_ping = fork();
            if (pid_ping < 0)
                return 4;
            else if (pid_ping == 0)
            {
                int fp = open("PipePing.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR));   
                dup2(fp, 1);
                execl("/bin/ping", "ping", "-c 5", "ufes.br");
                fclose(fp);

            }
        }
    } 
}