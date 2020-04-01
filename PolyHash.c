#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[])
{
    char nombre[32];
    while(1)
    {
    memset(nombre,'\0',32);
    printf("Nombre: ");
    scanf("%s",&nombre);
    printf("hash: %d",PolyHash(nombre));
    printf("hash: ");
    }
    return 0;
}
