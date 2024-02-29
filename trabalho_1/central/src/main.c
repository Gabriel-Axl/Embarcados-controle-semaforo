#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/time.h>
#define SERVER_PORT 11061

struct timeval currentTime;
int sockfd1, sockfd2;

int dados[2][16] = {0};
int COMANDOS[] = {0,0}; 
int porta1 = 11061;
int porta2 = 11062;
char ip1[9];
char ip2[9];

void send_vector(int sockfd) {
    
    send(sockfd, COMANDOS  , sizeof(COMANDOS), 0);
}

void *receive_data_cruz1() {
    int new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    int buffer[16]; 

    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(porta1);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd1, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao fazer o bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd1, 10) == 0) {
    } else {
        printf("Erro ao escutar\n");
    }

    while (1) { 
        addr_size = sizeof(new_addr);
        new_sock = accept(sockfd1, (struct sockaddr*)&new_addr, &addr_size);

        recv(new_sock, buffer, sizeof(buffer), 0);

        for (int i = 0; i < 16; i++) {
            dados[0][i] = buffer[i];
        }

        send_vector(new_sock);

        close(new_sock);
    }

    close(sockfd1);
}

void * receive_data_cruz2() {
    int new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    int buffer[16]; 

    sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd2 < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(porta2);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd2, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao fazer o bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd2, 10) == 0) {
    } else {
        printf("Erro ao escutar\n");
    }

    while (1) { 
        addr_size = sizeof(new_addr);
        new_sock = accept(sockfd2, (struct sockaddr*)&new_addr, &addr_size);

        recv(new_sock, buffer, sizeof(buffer), 0);

        for (int i = 0; i < 16; i++) {
            dados[1][i] = buffer[i];
        }

        send_vector(new_sock);
        close(new_sock);
    }

    close(sockfd2);
}

pthread_t thread_cruz1;
pthread_t thread_cruz2;

void menu(){
    printf("\n----------------------------------------\n");
    printf("\tServidor central\n");
    printf("\t\tMENU\n");
    printf("[1] - relatorio de cruzamentos\n");
    printf("[2] - modo noturno\n");
    printf("[3] - liberar rodovia central\n");
    printf("[4] - retornar menu principal\n");
        
}

int main(int argc, char const *argv[]) {

    if(argc != 5){
        printf("defina as duas portas e ambos ips\n");
        exit(0);
    }

    int opcao = 0; 
    int opcaoMenu = 0; 

    gettimeofday(&currentTime, NULL);

    porta1 = atoi(argv[1]);
    strcpy(ip1, argv[2]);
    porta2 = atoi(argv[3]);
    strcpy(ip2, argv[4]);

    pthread_create(&thread_cruz1, NULL, &receive_data_cruz1, NULL);
    pthread_create(&thread_cruz2, NULL, &receive_data_cruz2, NULL);
    
    while(1){
        opcaoMenu = 0; 
        printf("\n----------------------------------------\n");
        printf("\tServidor central\n");
        printf("\t\tMENU\n");
        printf("[1] cruzamento 1\n");
        printf("[2] cruzamento 2\n");
                
        printf("\nescreva a opção de cruzamento: ");
        scanf(" %d", &opcao);
        if (opcao == 1)
        {
            opcao = 0;
        }else if(opcao == 2){
            opcao = 1;
        }
        while (opcaoMenu!=4){
            menu();
            scanf("%d", &opcaoMenu);
            switch (opcaoMenu){
                case 1:
                    system("clear");
                    struct timeval endTime;
                    gettimeofday(&endTime, NULL);

                    long int elapsedTime = (endTime.tv_sec - currentTime.tv_sec) * 1000000 + (endTime.tv_usec - currentTime.tv_usec);
                    double seconds = (double)elapsedTime / 1000000.0;
                    double minutes = seconds / 60.0; 

                    int totalCarsPrincipal = dados[opcao][0] + dados[opcao][3]; 
                    double cpmPrincipal = (double)totalCarsPrincipal / minutes; 

                    int totalCarsAuxiliar1 = dados[opcao][6]; 
                    double cpmPAuxiliar1 = (double)totalCarsAuxiliar1 / minutes; 

                    int totalCarsAuxiliar2 = dados[opcao][9]; 
                    double cpmPAuxiliar2 = (double)totalCarsAuxiliar2 / minutes; 

                    printf("\nTempo decorrido: %.2f segundos\n", seconds);
                    printf("Tempo decorrido: %.2f minutos\n", minutes);
                    printf("\tCarros por minuto\n\n");
                    printf("carros por minuto rodovia principal = %.2f\n", cpmPrincipal);
                    printf("carros por minuto rodovia auxiliar 1 = %.2f\n", cpmPAuxiliar1);
                    printf("carros por minuto rodovia auxiliar 2 = %.2f\n\n", cpmPAuxiliar2);
                    printf("\tVelocidade media das vias\n\n");
                    if(dados[opcao][13] + dados[opcao][12] == 0){
                        printf("velocidade media via principal1 = 0km/h\n");
                    }else{
                        printf("velocidade media via principal1 = %dkm/h\n",(dados[opcao][12]+dados[opcao][13])/totalCarsPrincipal);
                    }
                    if (dados[opcao][14]==0)
                    {
                        printf("velocidade media via auxiliar 1 = 0km/h\n");
                    }else{
                        printf("velocidade media via auxiliar 1 = %dkm/h\n",dados[opcao][14]/totalCarsAuxiliar1);
                    }
                    if (dados[opcao][15] == 0)
                    {
                        printf("velocidade media via auxiliar 2 = 0km/h\n");
                    }else{
                        printf("velocidade media via auxiliar 2 = %dkm/h\n",dados[opcao][15]/totalCarsAuxiliar2);
                    }
                    printf("\n\tInfrações por rodovia\n\n");
                    printf("infrações total do cruzamento: \n\n\tavanço de sinal vermelho = %d\n\tvelocidade acima do permitido %d\n\n", dados[opcao][2] + dados[opcao][5] + dados[opcao][8] + dados[opcao][11], dados[opcao][1]+dados[opcao][4]+dados[opcao][7]+dados[opcao][10]);
                    printf("infrações total da rodovia principal: \n\n\tavanço de sinal vermelho = %d\n\tvelocidade acima do permitido %d\n\n", dados[opcao][2]+dados[opcao][5], dados[opcao][1]+dados[opcao][4]);
                    printf("infrações total da rodovia auxiliar 1: \n\n\tavanço de sinal vermelho = %d\n\tvelocidade acima do permitido %d\n\n", dados[opcao][8], dados[opcao][7]);
                    printf("infrações total da rodovia auxiliar 2: \n\n\tavanço de sinal vermelho = %d\n\tvelocidade acima do permitido %d\n\n", dados[opcao][11], dados[opcao][10]);
                    break;

                case 2:
                    printf("\t\tmodo noturno\n");
                    COMANDOS[0] = 1;
                    int opt;
                    while (COMANDOS[0]!=0)
                    {
                        printf("[0] para finalizar modo noturno\ndigite: ");
                        scanf("%d", &opt);
                        if (opt ==0)  
                        {
                            COMANDOS[0]=0;
                        }else{
                            printf("\nvalor invalido\n");
                        }

                    }
                    break;
                case 3:
                    printf("\t\tLiberando via principal\n");
                    COMANDOS[1] = 1;
                    while (COMANDOS[1]!=0)
                    {
                        printf("[0] para finalizar modo de emergencia\ndigite: ");
                        scanf("%d", &opt);
                        if (opt ==0)  
                        {
                            COMANDOS[1]=0;
                        }else{
                            printf("\nvalor invalido\n");
                        }

                    }
                    break;
                case 4:
                    break;
            }
        }
    }
    return 0;
}
