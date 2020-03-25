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



int main(int argc, char const *argv[])
{
    FILE *myf, *nombres, *razas, *razasgato, *razasconejo;
    struct Dog *lista=malloc(sizeof(struct Dog));
    nombres=fopen("Nombres.txt","r");
    razas=fopen("razas.txt","r");
    razasgato=fopen("razasgato.txt","r");
    razasconejo=fopen("razasconejo.txt","r");
    
    char arr_nombres[1717][32];
    char arr_razas[242][16];
    char arr_razasgato[56][16];
    char arr_razasconejo[207][16];
 
    //Lee línea a línea y escribe en pantalla hasta el fin de fichero
    for (size_t i = 0; i < 1717; i++)
    {
        memset(arr_nombres[i],0,32);
        fgets(arr_nombres[i], 32, (FILE*) nombres);
    }
    
    fclose(nombres);
    for(size_t i = 0; i < 242; i++)
    {
        fgets(arr_razas[i], 16, (FILE*) razas);
    }
    
    fclose(razas);
    for(size_t i = 0; i < 56; i++)
    {
        fgets(arr_razasgato[i], 16, (FILE*) razasgato);
    }
    
    fclose(razasgato);
    for(size_t i = 0; i < 207; i++)
    {
        fgets(arr_razasconejo[i], 16, (FILE*) arr_razasconejo);
    }
    
    fclose(razasconejo);
    for (size_t i = 0; i < 3333333; i++)
    {
        myf = fopen("dataDogs.dat","ab");
        lista->id=i;
        int num=rand() %1717;
        strcpy(lista->nombre,arr_nombres[num]);
        num=rand() %242;
        strcpy(lista->raza,arr_razas[num]);
        strcpy(lista->tipo,"Perro");
        num=rand() %20;
        lista->edad=num;
        num=rand() %70;
        lista->estatura=num;
        num=rand() %85;
        lista->peso=num;
        num= rand()%2;
        if(num==1){
            lista->sexo='M';
        }else{
            lista->sexo='H';
        }
        lista->hash=PolyHash(lista->nombre);
        fwrite(&lista,sizeof(struct Dog),1,myf);
        fclose(myf);  
    }
    for (size_t i = 3333333; i < 6666666; i++)
    {
        myf = fopen("dataDogs.dat","ab");
        lista->id=i;
        int num=rand() %1717;
        strcpy(lista->nombre,arr_nombres[num]);
        num=rand() %56;
        strcpy(lista->raza,arr_razasgato[num]);
        strcpy(lista->tipo,"Gato");
        num=rand() %20;
        lista->edad=num;
        num=rand() %70;
        lista->estatura=num;
        num=rand() %85;
        lista->peso=num;
        num= rand()%2;
        if(num==1){
            lista->sexo='M';
        }else{
            lista->sexo='H';
        }
        lista->hash=PolyHash(lista->nombre);
        fwrite(&lista,sizeof(struct Dog),1,myf);
        fclose(myf);  
    }
    for (size_t i = 6666666; i < 10000001; i++)
    {
        myf = fopen("dataDogs.dat","ab");
        lista->id=i;
        int num=rand() %1717;
        strcpy(lista->nombre,arr_nombres[num]);
        num=rand() %207;
        strcpy(lista->raza,arr_razasconejo[num]);
        strcpy(lista->tipo,"Conejo");
        num=rand() %20;
        lista->edad=num;
        num=rand() %70;
        lista->estatura=num;
        num=rand() %85;
        lista->peso=num;
        num= rand()%2;
        if(num==1){
            lista->sexo='M';
        }else{
            lista->sexo='H';
        }
        lista->hash=PolyHash(lista->nombre);
        fwrite(&lista,sizeof(struct Dog),1,myf);
        fclose(myf);  
    }
    /*
    for (size_t i = 0; i < 10; i++)
    {
        printf("Nombre: %s\n",lista[i].nombre);
        printf("Tipo: %s \n",lista[i].tipo);
        printf("Edad: %d \n", lista[i].edad);
        printf("Raza: %s \n", lista[i].raza);
        printf("Estatura: %d \n", lista[i].estatura);
        printf("Peso: %.2f \n", lista[i].peso);
        printf("Sexo: %c \n", lista[i].sexo);
        printf("Hash: %d \n", lista[i].hash);
        
    }*/
    
    
      
    
                   
    return 0;
}
int PolyHash (char cadena[])
    {
        int Hash=0;
        for (size_t o = 31; o > 0; o--)
        {
            Hash=(Hash*38+(int)cadena[o])% 991;
        }
        Hash=(Hash*38+(int)cadena[0])% 991;
        return Hash;        
    }