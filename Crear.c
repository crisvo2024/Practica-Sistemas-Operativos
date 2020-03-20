#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Dog{
    char nombre[32];
    char tipo[32];
    int edad;
    char raza[16];
    int estatura;
    float peso;
    char sexo;
    int hash;
    };
int PolyHash (char cadena[]);



int main(int argc, char const *argv[])
{
    FILE *myf, *nombres, *razas;
    struct Dog lista[10000000];
    nombres=fopen("Nombres.txt","r");
    razas=fopen("razas.txt","r");
    myf = fopen("dataDogs.dat","wb");
    char arr_nombres[1717][32];
    char arr_razas[242][16];
 
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
    for (size_t i = 0; i < 10000000; i++)
    {
        int num=rand() %1717;
        strcpy(lista[i].nombre,arr_nombres[num]);
        num=rand() %242;
        strcpy(lista[i].raza,arr_razas[num]);
        strcpy(lista[i].tipo,"Perro");
        num=rand() %20;
        lista[i].edad=num;
        num=rand() %70;
        lista[i].estatura=num;
        num=rand() %85;
        lista[i].peso=num;
        num= rand()%2;
        if(num==1){
            lista[i].sexo='M';
        }else{
            lista[i].sexo='H';
        }
        lista[i].hash=PolyHash(lista[i].nombre);

    }
    
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
        
    }
    
    fwrite(&lista,sizeof(struct Dog),10000000,myf);
      
    
    fclose(myf);                 
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