#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 1
#define	ERROR	-1


struct sockaddr_in stSockAddr;
int sockLen;
int Res;
int SocketFD;
int recibido;
char buffer[BUFFSIZE];
char mensaje[80];
char  buf_tx[80];
int totalBytesRcvd;
int bytesRcvd;
FILE *archivo;
char ip[100];
char puerto[100];

void abrirSocket();

int main(){

    char direccion[100];
    char pixel[100];
    printf("**Configurando Conexion **\n");
    printf("Digite la direccion ip: ");
    gets(ip);
    printf("Digite el puerto: ");
    gets(puerto);
    int control = 1;

    while(control == 1){

        abrirSocket();        

        printf( "Digite la direccion del archivo: ");
        gets( direccion);
        /*Se abre el archivo a enviar*/

        if(strcmp( direccion, "end") == 0){
            close(SocketFD);
            abrirSocket(SocketFD);
            send(SocketFD, "end", 3, 0);
            char resp[3];
            recv(SocketFD, resp, 3, 0);
            control = 0;
            close(SocketFD);
        }else {        
            archivo = fopen(direccion, "rb");
            if (!archivo) {
                perror("Error al abrir el archivo:");
                exit(EXIT_FAILURE);
            }
            /*Se envia el archivo*/
            fread(buffer, sizeof(char), BUFFSIZE, archivo);
            while (!feof(archivo)) {
                
                if (send(SocketFD, buffer, BUFFSIZE, 0) == ERROR)
                    perror("Error al enviar el archivo:");
                //printf("Le envie datos al servidor\n");
                fread(buffer, sizeof(char), BUFFSIZE, archivo);
            }
            close(SocketFD);
            printf("Archivo enviado\n");

            printf("Digite el valor del pixel: ");
            gets(pixel);

            abrirSocket();

            send(SocketFD, pixel, 3, 0);
            recv(SocketFD, pixel, 3, 0);
            printf("Respuesta del Server: %s\n", pixel);
            close(SocketFD);                        

            fclose(archivo);
            
        }
    }

    return 0;
}

void abrirSocket(){
    SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        
    if (SocketFD == ERROR){
            perror("cannot create socket");
            exit(EXIT_FAILURE);
    }

    /*Se configura la dirección del socket del cliente*/
    memset(&stSockAddr, 0, sizeof stSockAddr);

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(atoi(puerto));
    Res = inet_pton(AF_INET, ip, &stSockAddr.sin_addr);

    sockLen = sizeof(stSockAddr);

    if (connect(SocketFD, (struct sockaddr *)&stSockAddr, sizeof stSockAddr) == ERROR){
        perror("Error a la hora de conectarse con el cliente");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

}
