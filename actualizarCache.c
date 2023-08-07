#include <stdio.h>
#include <stdbool.h>

void actualizarTrasFallo (int[], int,long);
int primeraPosLibre (int[]);
bool estaEnCache (int[], int, int);
bool yaEstaActualizadaTrasAcierto(int[], int, int);
void actualizarTrasAcierto(int [], int, int);

int main()
{
	long numero;
	int cache[5];
	printf("la cache ahora es: ");
	for(int i = 0; i < 5;i++)
	{
		cache[i] = -1;
		printf("%d ", cache[i]);
	}
	printf("\n");
	while(1)
	{
		printf("mete un numero en la cache: ");
		scanf("%ld", &numero);
		if(estaEnCache(cache, numero, 5))
		{
			printf("ya esta en cache\n");
			if(yaEstaActualizadaTrasAcierto(cache,numero,5))
			{
				printf("no la toco que ya está actualizada\n");
			}
			else
			{
				printf("reajusto la cache\n");
				actualizarTrasAcierto(cache,numero,5);
			}
			printf("la cache ahora es: ");
			for(int i = 0; i < 5;i++)
			{
				printf("%d ", cache[i]);
			}
			printf("\n");
		}
		else
		{
			actualizarTrasFallo (cache, 5, numero);
			printf("la cache ahora es: ");
			for(int i = 0; i < 5;i++)
			{
				printf("%d ", cache[i]);
			}
			printf("\n");
		}
	}
}

void actualizarTrasFallo (int cache[], int Nlin,long nuevaDireccion)
{
	if(cache[Nlin-1] == -1)
	{
		int primeraLibre = primeraPosLibre(cache);
		cache[primeraLibre] = nuevaDireccion;
	}
	else
	{
		for(int i = 1;i < Nlin; i++)
			cache[i-1] = cache[i];
		cache[Nlin-1] = nuevaDireccion;
	}
}

int primeraPosLibre (int cache[])
{
	int i = 0;
	while(cache[i] != -1)
	{
		i++;
	}
	return i;
}

bool estaEnCache (int cache[], int direccion, int Nlin)
{
	int i = 0;
	while(i < Nlin-1 && cache[i] != direccion)
		i++;
	if(cache[i] == direccion)
		return true;
	else
		return false;
}

bool yaEstaActualizadaTrasAcierto(int cache[], int direccion, int Nlin)
{
	int i = Nlin-1;
	while (i>-1 && cache[i] == -1)
		i--;
	if(cache[i] == direccion)
		return true;
	else
		return false;
}

void actualizarTrasAcierto(int *cache, int direccion, int Nlin)
{
	int i = 0;
	while(cache[i] != direccion)
		i++;
	int j = Nlin-1;
	while (j>-1 && cache[j] == -1)
		j--;
	while(i < j+1)
	{
		cache[i] = cache[i+1];
		i++;
	}
	cache[j] = direccion;
}
