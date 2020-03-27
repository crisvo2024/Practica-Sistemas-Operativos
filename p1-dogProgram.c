#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Dog{
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

int PolyHash (char cadena[]);
int ingresar(char nombre[32],char tipo[32],int edad,char raza[16],int estatura,float peso, char sexo);
struct Dog *Listar(int Hash);
FILE *myf,*tablaHash;

int main(int argc, char const *argv[])
{
    int opcion=0;
    myf = fopen("dataDogs.dat","rb+");
    tablaHash = fopen("TablaHash.dat","ab");
    
    
    do
    {
        printf("Menu: ","%s\n" );
        printf("1. Ingresar Registro" ,"%s\n");
        printf("2. Ver Registro ","%s\n" );
        printf("3. Borrar Registro ","%s\n");
        printf("4. Buscar Registro ","%s\n ");
        printf("0. Salir" ,"%s\n ");
        scanf("%d", &opcion);
        switch(opcion)
        {
            case 1:
                printf("Ingrese el nombre de la mascota","%s\n");
                char nombre[32];
                char tipo[32];
                int edad;
                char raza[16];
                int estatura;
                float peso;
                char sexo;
                scanf("%s",&nombre);
                printf("Ingrese el tipo de la mascota","%s\n");
                scanf("%s",&tipo);
                printf("Ingrese la edad de la mascota","%s\n");
                scanf("%d",&edad);
                printf("Ingrese la raza de la mascota","%s\n");
                scanf("%s",&raza);
                printf("Ingrese la estatura de la mascota","%s\n");
                scanf("%d",&estatura);
                printf("Ingrese el peso de la mascota","%s\n");
                scanf("%f",&peso);
                printf("Ingrese el sexo de la mascota F o H","%s\n");
                scanf("%c",&sexo);
                int x= ingresar(nombre,tipo,edad,raza,estatura,peso,sexo);
                break;

            case 2:
                printf("hola");
                char nombreS[32];
                memset(nombreS,'\0',32);
                strcpy(nombreS,"Abel");
                struct Dog *lista=Listar(PolyHash(nombreS));
                int i=sizeof(lista)/sizeof(struct Dog)+1;
                for (size_t i = 0; i < i; i++)
                    {
                        printf("%d",i);
                        printf("Nombre: %s\n",lista[i].nombre);
                        printf("Tipo: %s \n",lista[i].tipo);
                        printf("Edad: %d \n", lista[i].edad);
                        printf("Raza: %s \n", lista[i].raza);
                        printf("Estatura: %d \n", lista[i].estatura);
                        printf("Peso: %.2f \n", lista[i].peso);
                        printf("Sexo: %c \n", lista[i].sexo);
                        printf("Hash: %d \n", lista[i].hash);
                        printf("Next: %d \n", lista[i].next);
                    }
                break;

            case 3:
                printf("%.1lf * %.1lf = %.1lf");
                break;

            case 4:
                printf("%.1lf / %.1lf = %.1lf");
                break;

            default:
                printf("opcion no valida");
        }
    }while(opcion!=0);
    return 0;
}
int ingresar(char nombre[32],char tipo[32],int edad,char raza[16],int estatura,float peso, char sexo)
{
    struct Dog Anterior, Nuevo;
    int id;
    fseek(myf,-sizeof(struct Dog),SEEK_END);
    fread(&Anterior,sizeof(struct Dog),1,myf);
    id=Anterior.id+1;
    Nuevo.id=id;
    strcpy(Nuevo.nombre,nombre);
    Nuevo.edad=edad;
    strcpy(Nuevo.raza,raza);
    Nuevo.estatura=estatura;
    Nuevo.peso=peso;
    Nuevo.sexo=sexo;
    Nuevo.hash=PolyHash(nombre);
    fwrite(&Nuevo,sizeof(struct Dog),1,myf);
}

struct Dog *Listar(int Hash){
    struct Dog prueba;
    int tabla[1010];
    fread(&tabla,sizeof(int),1009,myf);
    fseek(myf,tabla[Hash]*sizeof(struct Dog),SEEK_SET);
    fread(&prueba,sizeof(struct Dog),1,myf);
    int i=1;
    while(prueba.next!=0)
    {
        i++;
        fseek(myf,prueba.next*sizeof(struct Dog),SEEK_SET);
        fread(&prueba,sizeof(struct Dog),1,myf);
    }
    struct Dog *lista=(struct Dog *)malloc(i*sizeof(struct Dog));
    fseek(myf,tabla[Hash]*sizeof(struct Dog),SEEK_SET);
    fread(&lista[0],sizeof(struct Dog),1,myf);
    i=0;
    while(lista[i].next!=0)
    {
        fseek(myf,lista[i].next*sizeof(struct Dog),SEEK_SET);
        fread(&lista[++i],sizeof(struct Dog),1,myf);
    }
    return lista;
}
int PolyHash (char cadena[])
    {
        int Hash=0;
        for (size_t o = 31; o > 0; o--)
        {
            Hash=(Hash*38+(int)cadena[o])% 1009;
        }
        Hash=(Hash*38+(int)cadena[0])% 1009;
        return Hash;        
    }