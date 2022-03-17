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
#define	PUERTO	1100

int main(){
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



    char direccion[100];
    char ip[100];
    char pixel[100];
    char puerto[100];
    printf("**Configurando Conexion **\n");
    printf("Digite la direccion ip: ");
    gets(ip);
    printf("Digite el puerto: ");
    gets(puerto);

    /*Se crea el socket*/
    SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    /*Se verifica la integridad del archivo*/


    /*Se verifica la integridad del socket*/
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

    printf("Se ha conectado con el servidor:%s\n",(char *)inet_ntoa(stSockAddr.sin_addr));
    printf( "Digite la direccion del archivo: ");
    gets( direccion);
    /*Se abre el archivo a enviar*/

    if(strcmp( direccion, "end") == 0){
        close(SocketFD);
    }else {        
        archivo = fopen(direccion, "rb");
        if (!archivo) {
            perror("Error al abrir el archivo:");
            exit(EXIT_FAILURE);
        }

        /*char data[1024] = {0};

        while(fgets(data, 1024, archivo) != NULL) {
            if (send(SocketFD, data, sizeof(data), 0) == -1) {
                perror("[-]Error in sending file.");
                exit(1);
            }
        bzero(data, 1024);
        }*/

        /*Se envia el archivo*/
        fread(buffer, sizeof(char), BUFFSIZE, archivo);
        while (!feof(archivo)) {

            //printf("FEOF => %d\n", feof(archivo));
            
            //printf("Enviando archivo // ");
            
            //printf("%s // ", buffer);
            
            if (send(SocketFD, buffer, BUFFSIZE, 0) == ERROR)
                perror("Error al enviar el archivo:");
            //printf("Le envie datos al servidor\n");
            fread(buffer, sizeof(char), BUFFSIZE, archivo);
        }
        //printf("FEOF => %d\n", feof(archivo));
        printf("\nArchivo enviado\n");
        //char endOfLine[BUFFSIZE] = "'";
        //send(SocketFD, endOfLine, BUFFSIZE, 0);
        //close(SocketFD);

        fclose(archivo);

        

        /*read(SocketFD, mensaje, sizeof(mensaje));
        printf("\nRespuesta: %s\n", mensaje);
        fclose(archivo);
        close(SocketFD);

        printf("Ingrese el valor del pixel: ");
        gets(pixel);
        if (send(SocketFD, pixel, 100, 0) == ERROR)
                perror("Error al enviar el archivo:");

        read(SocketFD, mensaje, sizeof(mensaje));
        printf("\nRespuesta: %s\n", mensaje);*/

        close(SocketFD);
    }

    return 0;
}//End main
