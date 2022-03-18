#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Definición de constantes*/
#define BUFFSIZE 1
#define PUERTO		1100
#define ERROR		-1


unsigned int len;
struct sockaddr_in stSockAddr;
struct sockaddr_in clSockAddr;
struct sockaddr_in servaddr, client;
FILE *archivo;
char *direccIP;
int SocketServerFD;
int SocketClientFD;
int clientLen;
int serverLen;
int number;
char buff_rx[100];
char buff_tx[100] = "Hello client, I am the server";

/*Prototipos de función*/
void recibirArchivo(int SocketFD, FILE *file);
void esperarConexion();
void enviarConfirmacion(int SocketFD);
void enviarMD5SUM(int SocketFD);
void getIP(int tipo, char * IP);
/*Recibe la clave de la interfaz que va a manejar:
 * lo : 0
 * wlan: 1
 * eth0: 2
*/
int main(){
    
    direccIP = malloc(20);

    /*Se obtiene la IP de la interfaz solicitada*/
    getIP(0,direccIP);

    /*Se crea el socket*/
    if((SocketServerFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == ERROR){
        perror("No se puede crear el socket");
        exit(EXIT_FAILURE);
    }//End if-SocketFD

    /*Se configura la dirección del socket*/
    memset(&stSockAddr, 0, sizeof stSockAddr);

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(PUERTO);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(SocketServerFD,(struct sockaddr *)&stSockAddr, sizeof stSockAddr) == ERROR){
        perror("Error en bind");
        close(SocketServerFD);
        exit(EXIT_FAILURE);
    }//End if-bind
    inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);
    printf("Socket atado a la dirección %s\n",(char *)inet_ntoa(stSockAddr.sin_addr));
    if(listen(SocketServerFD, 10) == ERROR){
        perror("Error listen");
        close(SocketServerFD);
        exit(EXIT_FAILURE);
    }//End if-listen

    while (1){

        printf("Esperando nueva Conexion\n");
        esperarConexion();

        recibirArchivo(SocketClientFD, archivo);

        printf("Esperando Parametro \n");

        char pixel[3];
        esperarConexion();

        recv(SocketClientFD, pixel, 3, 0);
        printf("Recibi el valor: %s\n", pixel); //Si aca pixel == end, no debe de hacer el algoritmo

        send(SocketClientFD, pixel, 3, 0); //Respuesta para el cliente (aca va el resultado del algoritmo)


    }//End infinity while

    close(SocketClientFD);
    close(SocketServerFD);
    return 0;
}//End main program

void recibirArchivo(int SocketFD, FILE *file){

    char buffer[BUFFSIZE];
    int recibido = -1;
    printf("Esperando archivo\n");

    /*Se abre el archivo para escritura*/
    file = fopen("archivoRecibido","wb");
    
    while((recibido = recv(SocketFD, buffer, BUFFSIZE, 0)) > 0){
        //printf("Recibi del cliente// %s\n",buffer);
        fwrite(buffer,sizeof(char),1,file);
        //printf("Escribi en el archivo \n");
    }//Termina la recepción del archivo

    printf("Archivo recibido\n");
    
    fclose(file);


}//End recibirArchivo procedure

void enviarConfirmacion(int SocketFD){
    char mensaje[80] = "Paquete Recibido";
    int lenMensaje = strlen(mensaje);
    printf("\nConfirmación enviada\n");
    if(write(SocketFD,mensaje,sizeof(mensaje)) == ERROR)
        perror("Error al enviar la confirmación:");


}//End enviarConfirmacion

void esperarConexion(){
    clientLen = sizeof(clSockAddr);
    //Espera por la conexión de un cliente//
    if ((SocketClientFD = accept(SocketServerFD,(struct sockaddr *) &clSockAddr,&clientLen)) < 0){
            perror("Fallo para aceptar la conexión del cliente");
        }

    /*Se configura la dirección del cliente*/
    clSockAddr.sin_family = AF_INET;
    clSockAddr.sin_port = htons(PUERTO);

}

void getIP(int tipo, char * IP){
    FILE *tmpIP;
    char dIP[20];
    char dIP2[20];
    int i,j;
    switch(tipo){
        case 0:
            system("ifconfig lo | grep inet > tmp");
            break;
        case 1:
            system("ifconfig wlan | grep inet > tmp");
            break;
        case 2:
            system("ifconfig eth | grep inet > tmp");
            break;


    }//End switch
    j = 0;
    tmpIP = fopen("tmp","r");
    fscanf(tmpIP,"%s %s",dIP,dIP);
    for(i = 5;i<20;i++){
        IP[j] = dIP[i];
        j++;
    }
}//End getIP
