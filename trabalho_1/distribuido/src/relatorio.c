#include <unistd.h>
#include <wiringPi.h>
#include <estado.h>
#include <sensor.h>
#include <semaforo.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 11061
#define UPDATE_INTERVAL 2 

int received_info[2] = {0, 0}; 

void receive_vector(int sockfd) {
    
    recv(sockfd, received_info, sizeof(received_info), 0);

}

void relatorio(int porta, char* ip) {
    int sockfd;
    struct sockaddr_in server_addr;
    while(1){
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Erro ao criar o socket");
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(porta);
        inet_pton(AF_INET, ip, &server_addr.sin_addr);

        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Erro ao conectar-se ao servidor");
            close(sockfd);
            sleep(2); 
            continue;
        }else{
            printf("\nServidor conectado\n");
        }

        int data[16];
        for (int i = 0; i < 16; i++) {
            data[i] = info[i];
        }
        
        send(sockfd, data, sizeof(data), 0);

        receive_vector(sockfd);

        close(sockfd);
        sleep(2);
    }
}