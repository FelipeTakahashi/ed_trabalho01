#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/header.h"

#define MAXCHAR 256
#define MAXSIZE 11777
#define N_DIM 2

typeCity* cityAlloc(char* codigo_ibge, char* nome, double latitude, double longitude, int capital, int codigo_uf, int siafi_id, int ddd, char* fuso_horario) {
    typeCity* city = malloc(sizeof(typeCity));
    strcpy(city->codigo_ibge, codigo_ibge);
    strcpy(city->nome, nome);
    city->latitude = latitude;
    city->longitude = longitude;
    city->capital = capital;
    city->codigo_uf = codigo_uf;
    city->siafi_id = siafi_id;
    city->ddd = ddd;
    strcpy(city->fuso_horario, fuso_horario);

    return city;
}

typeCity* cityAllocSimplified(char* name, double latitude, double longitude) {
	typeCity* city = malloc(sizeof(typeCity));
	strcpy(city->nome, name);
	city->latitude = latitude;
	city->longitude = longitude;
	return city;
}

typeCity* copyCity(typeCity* city) {
    typeCity* newCity = malloc(sizeof(typeCity));

    strcpy(newCity->codigo_ibge, city->codigo_ibge);
    strcpy(newCity->nome, city->nome);
    newCity->latitude = city->latitude;
    newCity->longitude = city->longitude;
    newCity->capital = city->capital;
    newCity->codigo_uf = city->codigo_uf;
    newCity->siafi_id = city->siafi_id;
    newCity->ddd = city->ddd;
    strcpy(newCity->fuso_horario, city->fuso_horario);

    return newCity;
}

void printCity(typeCity* city) {
    printf("\nInformações da cidade:\n");
    printf("Nome: %s\n", city->nome);
    printf("Código IBGE: %s\n", city->codigo_ibge);
    printf("Latitude: %lf\n", city->latitude);
    printf("Longitude: %lf\n", city->longitude);
    printf("Capital: %d\n", city->capital);
    printf("Código UF: %d\n", city->codigo_uf);
    printf("Siafi ID: %d\n", city->siafi_id);
    printf("DDD: %d\n", city->ddd);
    printf("Fuso Horário: %s\n", city->fuso_horario);
}

void printRow(typeCity* city) {
    printf("{%s, %s, %.4lf, %.4lf, %d, %d, %d, %d, %s}\n", city->codigo_ibge, city->nome, city->latitude, city->longitude, city->capital, city->codigo_uf, city->siafi_id, city->ddd, city->fuso_horario);
}

void printRowWithDistance(typeCity* city, double distance) {
    printf("{%s, %s, %.4lf, %.4lf, %d, %d, %d, %d, %s, %.6lf}\n", city->codigo_ibge, city->nome, city->latitude, city->longitude, city->capital, city->codigo_uf, city->siafi_id, city->ddd, city->fuso_horario, sqrt(distance));
}
