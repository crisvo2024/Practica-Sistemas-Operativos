#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int PolyHash (char cadena[])
{
    int i = 0;    
    while (cadena[i] != '\0') 
    {
        if (cadena[i] >= 'a' && cadena[i] <= 'z') {
            cadena[i] = cadena[i] - 32;
        }
        i++;
    }
    
    int Hash=0;
    for (int o = 31; o >= 0; o--)
    {
        Hash=(Hash*31+(int)cadena[o])% 1009;
    }
    if(Hash<0){
        Hash=-Hash;
    }
    return Hash;        
    }
int main(int argc, char const *argv[])
{
    char nombre[32];
    while(1)
    {
    memset(nombre,'\0',32);
    printf("Nombre: ");
    scanf("%s",&nombre);
    printf("hash: %d",PolyHash(nombre));
    printf("hash %s: ",nombre);
    }
    return 0;
}
