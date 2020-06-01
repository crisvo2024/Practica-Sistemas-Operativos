#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 3535


struct dogType
{
    int id;
    char nombre[32];
    char tipo[32];
    int edad;
    char raza[16];
    int estatura;
    float peso;
    char sexo;
    int hash;
    int next;
};

int clientfd, r;
struct sockaddr_in client;
socklen_t tamano;
struct hostent *he;
int *buffer, *deleted;

struct dogType *lista;
int PolyHash (char cadena[]);
int ingresar(char nombre[32],char tipo[32],int edad,char raza[16],int estatura,float peso, char sexo);
int verRegistro();
int eliminar();
int Buscar();
int mygetch ( void );

FILE *myf,*tablaHash,*eliminados;
int n, tabla[1010],offset,total;

int maxid;


int main(int argc, char const *argv[])
{
    //abrimos el stream para el paso de mensajes por sockets
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0){
        perror("\n-->Error en socket():");
        exit(-1);
    }
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    inet_aton("127.0.0.1", &client.sin_addr);
    //se incicia la conexion a clientfd
    r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr));
    if(r < 0){
        perror("\n-->Error en connect(): ");
        exit(-1);
    }
    r=0;
    //inicializacion de variables 
    int opcion=0;
    int id;
    int salir=0;
    char continuar;
    char nombre[32];
    struct dogType Anterior;
    char tipo[32];
    int edad;
    char raza[16];
    int estatura;
    float peso;
    char sexo;
    //informacion general
    buffer=malloc(sizeof(int)*4);
    r = recv(clientfd, buffer, sizeof(int)*2, 0);
    if(r<sizeof(int)*2){
        perror("\n-->Error en recv() inicial");
        exit(-1);
    }
    maxid=buffer[0];
    total=buffer[1];
    printf("ID maximo: %d Registros en Total: %d ",maxid,total);
    
    //menu
    do
    {
        printf("\nMenu: ","%s\n" );
        printf("\n 1. Ingresar Registro " ,"%s\n");
        printf("\n 2. Ver Registro ","%s\n" );
        printf("\n 3. Borrar Registro ","%s\n");
        printf("\n 4. Buscar Registro ","%s\n ");
        printf("\n 0. Salir" ,"%s\n ");
        printf("\nIngrese el numero de la opcion: ","%s\n");
        scanf("%d", &opcion);
        if(opcion<=4){
            //envio al servidor de la opcion del menu
            r=send(clientfd,&opcion,sizeof(int),0);
            if(r<0){
                perror("error en send opción menu");
            }
        }
        switch(opcion)
        {
            case 0:break;
            case 1:;
                
                printf("Ingrese el nombre de la mascota: ","%s\n");
                //limpieza de cadena nombre
                memset(nombre,'\0',32);
                scanf("%s",&nombre);
                strcat(nombre,"\n");
                printf("Ingrese el tipo de la mascota: ","%s\n");
                scanf("%s",&tipo);
                printf("Ingrese la edad de la mascota: ","%s\n");
                scanf("%d",&edad);
                printf("Ingrese la raza de la mascota: ","%s\n");
                scanf("%s",&raza);
                printf("Ingrese la estatura de la mascota: ","%s\n");
                scanf("%d",&estatura);
                printf("Ingrese el peso de la mascota: ","%s\n");
                scanf("%f",&peso);
                printf("Ingrese el sexo de la mascota M o H: ","%s\n");
                scanf(" %c",&sexo);
                //llamamos funcion ingresar
                int x= ingresar(nombre,tipo,edad,raza,estatura,peso,sexo);
                printf("Oprima cualquier tecla para continuar");
                mygetch();
                break;

            case 2:;
                //recibimos de servidor el id maximo y el total de estructuras
                buffer=malloc(sizeof(int)*4);
                r = recv(clientfd, buffer, sizeof(int)*2, 0);
                if(r<sizeof(int)*2){
                    perror("\n-->Error en recv() inicial");
                    exit(-1);
                }
                maxid=buffer[0];
                total=buffer[1];
                salir=0;
                id=-1;
                struct dogType Nuevo;
                char desition;
                do
                {
                    //informe del total de registroa
                    salir=0;
                    printf("Hay %d registros, ingrese el id del registro que desea ver\n",total);
                    scanf("%d",&id);
                    //envio de el id al servidor para su busqueda
                    r=send(clientfd,&id,sizeof(int),0);
                    if (r<0)
                    {
                        perror("\nError send() caso 2");
                        exit(EXIT_FAILURE);
                    }
                    //se recibe confirmacion del servidor sobre la existencia del id enviado
                    r = recv(clientfd, &salir, sizeof(int), 0);
                    if(r<sizeof(int)){
                        perror("\n-->Error en recv() inicial");
                        exit(-1);
                    }
                    //informe de validez
                    if(salir==1){
                        printf("Ingrese un valor valido \n");
                    }
                    //el ciclo continu hasta que se ingrese un id valido
                } while (salir==1);
                //llamada a funcion verRegistro
                verRegistro();   
                
                printf("\nOprima cualquier tecla para continuar");
                mygetch();  
                break;

            case 3:;
                //recibimos de servidor el id maximo y el total de estructuras
                buffer=malloc(sizeof(int)*4);
                r = recv(clientfd, buffer, sizeof(int)*2, 0);
                if(r<sizeof(int)*2){
                    perror("\n-->Error en recv() inicial");
                    exit(-1);
                }
                maxid=buffer[0];
                total=buffer[1];
                struct dogType eliminado;
                do{
                    salir=0;
                    printf("\n Hay %d registros, ingrese el id del registro que desea eliminar: \n",total);
                    scanf("%d",&id);
                    //envio del id al servidor para su busqueda
                    r=send(clientfd,&id,sizeof(int),0);
                    if (r<0)
                    {
                        perror("\nError send() caso 2");
                        exit(EXIT_FAILURE);
                    }
                    //se recibe confirmacion del servidor sobre la existencia del id enviado
                    r = recv(clientfd, &salir, sizeof(int), 0);
                    if(r<sizeof(int)){
                        perror("\n-->Error en recv() eliminado");
                        exit(-1);
                    }
                    if(r==1)printf("\nIngrese un valor valido\n");
                    //el ciclo continu hasta que se ingrese un id valido
                }while (salir==1);
                //llamado a la funcion eliminar
                eliminar();
                printf("\nOprima cualquier tecla para continuar");
                mygetch();
                break;
            case 4:;
                printf("Ingrese el nombre a buscar: ");
                //limpieza de cadena nombre
                memset(nombre,'\0',32);
                scanf("%s",&nombre);
                strcat(nombre,"\n");
                //envio de la cadena nombre a buscar al servidor
                r=send(clientfd,&nombre,32,0);
                if (r<0){
                    perror("\nError send() caso 4");
                    exit(EXIT_FAILURE);
                }
                //llamada a la funcion buscar
                Buscar();
                printf("\nOprima cualquier tecla para continuar");
                mygetch();
                break;
            default:
                printf("opcion no valida");
        }
    }while(opcion!=0);
    //cerramos el stream
    close(clientfd);
    return 0;
}
int ingresar(char nombre[32],char tipo[32],int edad,char raza[16],int estatura,float peso, char sexo)
{
    //se crea una nueva estructura dogType con la informacion ingresada
    //utilizando memset para limpiar las variables
    struct dogType  Nuevo;
    Nuevo.id=0;
    memset(Nuevo.nombre,'\0',32);
    strcpy(Nuevo.nombre,nombre);
    Nuevo.edad=edad;
    memset(Nuevo.raza,'\0',32);
    strcpy(Nuevo.raza,raza);
    Nuevo.estatura=estatura;
    memset(Nuevo.tipo,'\0',32);
    strcpy(Nuevo.tipo,tipo);
    Nuevo.peso=peso;
    Nuevo.sexo=sexo;
    Nuevo.hash=0;
    Nuevo.next=0;
    //se envia la esctructura al servidor para su ingreso
    send(clientfd,&Nuevo,sizeof(struct dogType),0);
    //se recibe la estructura escrita para confirmar que se ingreso correctamente
    recv(clientfd,&Nuevo,sizeof(struct dogType),0);
    //imprimimos el nuevo dogType ingresado
    printf("\nID: %d\n",Nuevo.id);
    printf("Nombre: %s\n",Nuevo.nombre);
    printf("Tipo: %s \n",Nuevo.tipo);
    printf("Edad: %d \n", Nuevo.edad);
    printf("Raza: %s \n", Nuevo.raza);
    printf("Estatura: %d \n", Nuevo.estatura);
    printf("Peso: %.2f \n", Nuevo.peso);
    printf("Sexo: %c \n", Nuevo.sexo);
    printf("Hash: %d \n", Nuevo.hash);
    printf("Next: %d \n", Nuevo.next);
}

int verRegistro(){
    struct dogType Nuevo;
    char desition=0;
    //se recibe la estructura correspondiente al id enviado
    recv(clientfd,&Nuevo,sizeof(struct dogType),0);
    //se imprime la informacion de la estructura
    printf("\nID: %d\n",Nuevo.id);
    printf("Nombre: %s\n",Nuevo.nombre);
    printf("Tipo: %s \n",Nuevo.tipo);
    printf("Edad: %d \n", Nuevo.edad);
    printf("Raza: %s \n", Nuevo.raza);
    printf("Estatura: %d \n", Nuevo.estatura);
    printf("Peso: %.2f \n", Nuevo.peso);
    printf("Sexo: %c \n", Nuevo.sexo);
    printf("Hash: %d \n", Nuevo.hash);
    printf("Next: %d \n", Nuevo.next);
    //confirmacion de apertura de historia clinica
    int c=0;
    do
    {                 
        printf("Desea ver la historia clinica de %s (S/N)\n",Nuevo.nombre);
        scanf(" %c",&desition);
        if(desition=='S'||desition=='s')
        {
            c=1;
            break;
        }
        if(desition!='N'&&desition!='n')printf("Opcion no valida");
    }while(desition!='n' && desition!='N');
    //enviar a servidor si se quiere o no abrir la historia
    r=send(clientfd,&c,sizeof(int),0);
    if(r<0){
        perror("\n-->Error en send historia clinica");
        exit(-1);
    }
    int e;
    if(c==1){
        //recibede servidor informacion sobre la existencia de la historia clinica
        r= recv(clientfd,&e,sizeof(int),0);
        if(r<0){
            perror("\n-->Error en recv historia clinica");
            exit(-1);
        }
        FILE* historia;
        char comando[32];
        if(e==0){
            char info[1024];
            //abrimos un archivo para guardar la historia del servidor
            sprintf(comando,"%d.txt",Nuevo.id);
            historia = fopen(comando,"w");
            int sz=0;
            r= recv(clientfd,&sz,sizeof(int),0);
            if(r<0){
                perror("\n-->Error en recv info historia clinica");
                exit(-1);
            }
            do{                
                memset(info,'\0',1024);
                //recibe todos los caracteres de la historia clinica desde servidor
                r= recv(clientfd,&info,sizeof(char)*sz,MSG_WAITALL);
                if(r<0){
                    perror("\n-->Error en recv info historia clinica");
                    exit(-1);
                }
                fprintf(historia,"%s",info);
                r= recv(clientfd,&sz,sizeof(int),0);
                if(r<0){
                    perror("\n-->Error en recv info historia clinica");
                    exit(-1);
                }
            }while(sz>0);
            fclose(historia);
        }
        //abrimos la historia clinica para que el usuario pueda editarla
        sprintf(comando,"nano %d.txt",Nuevo.id);
        system(comando);
        sprintf(comando,"%d.txt",Nuevo.id);
        historia = fopen(comando,"r");
        if(historia==NULL){
            //le decimos a servidor que no se va a enviar nueva informacion para la historia
            e = 1;
            r=send(clientfd,&e,sizeof(int),0);
            if(r<0){
                perror("error envio");
                exit(-1);
            }
            return 0;
        }
        //le decimos a servidor que si se va a enviar nueva informacion para la historia
        e = 0;
        r=send(clientfd,&e,sizeof(int),0);
        if(r<0){
            perror("error envio");
            exit(-1);
        }
        fseek(historia, 0L, SEEK_SET);
        char filedata[1024];
        int sz = fread(&filedata,sizeof(char),1024,historia);
        while(sz>0){
            //envio a servidor de todos los caracteres en la historia clinica
            r=send(clientfd,&sz,sizeof(int),0);
            if(r<0){
                perror("error envio");
                exit(-1);
            }
            r=send(clientfd,&filedata,sizeof(char)*sz,0);
            if(r<0){
                perror("error envio");
                exit(-1);
            }
            sz =fread(&filedata,sizeof(char),1024,historia);
        }
        sz=0;
        r=send(clientfd,&sz,sizeof(int),0);
        if(r<0){
            perror("error envio");
            exit(-1);
        }
        fclose(historia);
        //eliminacion del archivo creado para interactuar con el cliente     
        sprintf(comando,"rm %d.txt",Nuevo.id);
        system(comando);   
    }

}
int eliminar ()
{
    struct dogType eliminado;
    char desition;
    //recibe la estrutura correspondiente al id a eliminar
    r=recv(clientfd,&eliminado,sizeof(struct dogType),0);
    if(r<0){
        perror("\n-->Error en recv() estructura");
        exit(-1);
    }
    //se imprime la informacion de la estructura
    printf("\n Datos del registro que desea eliminar: \n");
    printf("\nID: %d\n",eliminado.id);
    printf("Nombre: %s\n",eliminado.nombre);
    printf("Tipo: %s \n",eliminado.tipo);
    printf("Edad: %d \n", eliminado.edad);
    printf("Raza: %s \n", eliminado.raza);
    printf("Estatura: %d \n", eliminado.estatura);
    printf("Peso: %.2f \n", eliminado.peso);
    printf("Sexo: %c \n", eliminado.sexo);
    printf("Hash: %d \n", eliminado.hash);
    printf("Next: %d \n", eliminado.next);
    //confirmacion de eliminacion
    do
    {
        printf("Desea eliminar el registro de %s (S/N): \n",eliminado.nombre);
        scanf(" %c",&desition);
        int conf=1;
        if(desition=='S'||desition=='s'){
            //envio de la confimarcion positiva al servidor
            send(clientfd,&conf,sizeof(int),0);
            if (r<0){
                perror("\nError send() caso 2");
                exit(EXIT_FAILURE);
            }
            break; 
        }
        if(desition=='n' && desition=='N'){
            conf=-1;
            //envio de la confimarcion negativa al servidor
            send(clientfd,&conf,sizeof(int),0);
            printf("Operacion abortada \n");
        }
    }while(desition!='n' && desition!='N');
    //recibe confirmacion del fin de la eliminacion
    r=recv(clientfd,buffer,sizeof(int),0);
    if(r<0){
        perror("\n-->Error en recv() inicial");
        exit(-1);
    }
    
}

int Buscar(){
    int count;
    //recibe la cantidad de regitros que corresponden con el nombre
    r=recv(clientfd,&count,sizeof(int),0);
    if(r<0){
        perror("Error recv() count");
        exit(-1);
    }
    //ciclo para recibir las estruturas
    struct dogType Element;
    if(count>0){                    
        for (size_t i = 0; i < count; i++)
        {
            //recibimos una estructura a la vez y se imprime la informacion
            r=recv(clientfd,&Element,sizeof(struct dogType),MSG_WAITALL);
            if(r<0){
                perror("Error recv() lista");
                exit(-1);
            }
            printf("\nID: %d\n",Element.id);
            printf("Nombre: %s\n",Element.nombre);
            printf("Tipo: %s \n",Element.tipo);
            printf("Edad: %d \n", Element.edad);
            printf("Raza: %s \n", Element.raza);
            printf("Estatura: %d \n", Element.estatura);
            printf("Peso: %.2f \n", Element.peso);
            printf("Sexo: %c \n", Element.sexo);
            printf("Hash: %d \n", Element.hash);
            printf("Next: %d \n", Element.next);
        }
    }else{
        printf("No hay registros que coincidan");
    }
}
//funcion tomada de https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?id=1043284385&answer=1042856625
int mygetch ( void ) 
{
  int ch;
  struct termios oldt, newt;
  
  tcgetattr ( STDIN_FILENO, &oldt );//leer los atributos de entrada de la consola
  newt = oldt;//copiarlos en newt
  newt.c_lflag &= ~( ICANON | ECHO );//descativa el modo canonico de la entrada
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );//fijar atributos de newt
  ch = getchar();//lee el caracter anterior que deja scanf sin leer
  ch = getchar();//espera cualquier entrada
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );//resetea los atributos de entrada de la consola
  
  return ch;
}