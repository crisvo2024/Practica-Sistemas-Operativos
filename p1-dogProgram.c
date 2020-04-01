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
int n;

int main(int argc, char const *argv[])
{
    int opcion=0;
    myf = fopen("dataDogs.dat","rb+");
    tablaHash = fopen("TablaHash.dat","rb");
    
    
    do
    {
        printf("\nMenu: ","%s\n" );
        printf("\n 1. Ingresar Registro " ,"%s\n");
        printf("\n 2. Ver Registro ","%s\n" );
        printf("\n 3. Borrar Registro ","%s\n");
        printf("\n 4. Buscar Registro ","%s\n ");
        printf("\n 0. Salir" ,"%s\n ");
        scanf("%d", &opcion);
        switch(opcion)
        {
            case 1:;
                char nombre[32];
                char tipo[32];
                int edad;
                char raza[16];
                int estatura;
                float peso;
                char sexo;
                printf("Ingrese el nombre de la mascota","%s\n");
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
                printf("Ingrese el sexo de la mascota M o H","%s\n");
                scanf(" %c",&sexo);
                int x= ingresar(nombre,tipo,edad,raza,estatura,peso,sexo);
                break;

            case 2:;
                
                struct Dog Anterior, Nuevo, *lista;
                int id;
                char desition;
                fseek(myf,-sizeof(struct Dog),SEEK_END);
                fread(&Anterior,sizeof(struct Dog),1,myf);
                printf("Hay %d registros, inrese el id del registro que desea ver\n",Anterior.id+1);
                scanf("%d",&id);
                fseek(myf,sizeof(struct Dog)*id,SEEK_SET);
                fread(&Nuevo,sizeof(struct Dog),1,myf);
                for (size_t i = 0; i <= n; i++)
                    {
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
                    printf("Desea ver la historia clinica de %s (S/N)\n",Nuevo.nombre);
                    scanf(" %c",&desition);
                    if(desition=='S')
                    {
                        char comando[32];
                        sprintf(comando,"nano %d.txt",id);
                        system(comando);
                    }else
                    {
                        if(desition!='N')
                            printf("Opcion no valida");
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
    struct Dog Anterior, Nuevo, *lista;
    int id;
    fseek(myf,-sizeof(struct Dog),SEEK_END);
    fread(&Anterior,sizeof(struct Dog),1,myf);
    id=Anterior.id+1;
    Nuevo.id=id;
    strcpy(Nuevo.nombre,nombre);
    Nuevo.edad=edad;
    strcpy(Nuevo.raza,raza);
    Nuevo.estatura=estatura;
    strcpy(Nuevo.tipo,tipo);
    Nuevo.peso=peso;
    Nuevo.sexo=sexo;
    Nuevo.hash=PolyHash(nombre);
    Nuevo.next=0;
    fwrite(&Nuevo,sizeof(struct Dog),1,myf);
    lista=Listar(Nuevo.hash);
    Anterior=lista[n];
    Anterior.next=(Nuevo.id);
    fseek(myf,sizeof(struct Dog)*Anterior.id,SEEK_SET);
    fwrite(&Anterior,sizeof(struct Dog),1,myf);
    lista=Listar(Nuevo.hash);
    for (size_t i = 0; i <= n; i++)
        {
            printf("\nID: %d\n",lista[i].id);
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
}

struct Dog *Listar(int Hash){
    struct Dog prueba;
    int tabla[1010];
    fread(&tabla,sizeof(tabla),1,tablaHash);
    fseek(myf,tabla[Hash]*sizeof(struct Dog),SEEK_SET);
    fread(&prueba,sizeof(struct Dog),1,myf);
    n=1;
    while(prueba.next!=0)
    {
        n++;
        fseek(myf,prueba.next*sizeof(struct Dog),SEEK_SET);
        fread(&prueba,sizeof(struct Dog),1,myf);
    }
    struct Dog *lista=(struct Dog *)malloc(n*sizeof(struct Dog));
    fseek(myf,tabla[Hash]*sizeof(struct Dog),SEEK_SET);
    fread(&lista[0],sizeof(struct Dog),1,myf);
    n=0;
    while(lista[n].next!=0)
    {
        fseek(myf,lista[n].next*sizeof(struct Dog),SEEK_SET);
        fread(&lista[++n],sizeof(struct Dog),1,myf);
    }
    
    return lista;

}
int PolyHash (char cadena[])
    {
        int Hash=0;
        for (int o = 31; o >= 0; o--)
        {
            if(cadena[o]<0)
            {
                Hash=(Hash*38-(int)cadena[o])% 1009;
            }
            else
            {
                Hash=(Hash*38+(int)cadena[o])% 1009;
            }
            
            
        }
        return Hash;        
    }
