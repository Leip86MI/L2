#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"

/* Fonction in order to create a base */
base_t* base_create()
{
    base_t* base;
    base=malloc(sizeof(base_t));
    if(base== NULL)
    {
        printf("Allocation échouée\n");
        return NULL;
    }
    base->country=NULL;
    base->day=0;
    base->facilities=NULL;
    base->month=0;
    base->name=NULL;
    base->nfacilities=0;
    base->year=0;
    return base;
}
/* Append a facility to the structure */
int base_add_facility(base_t *base,facility_t *facility){
    base->facilities=realloc(base->facilities,(base->nfacilities+1)*sizeof(facility_t));
    if(base->facilities==NULL)
    {
        return -1;
    }
    base->facilities[base->nfacilities]=facility;     
    base->nfacilities++;
    return 0;
}
/* Print the country of the base */
void base_handle_c(base_t base){
    printf("%s",base.country);
}
/* Print the base */
void base_handle_d(base_t base){
    printf("%d-",base.year);
    printf("0%d-",base.month);
    printf("0%d",base.day);
}
/* Print the name of the base */
void base_handle_n(base_t base){
    printf("%s",base.name);
}
/* Print the name, the base and the country */
void base_handle_b(base_t base){
    base_handle_n(base);
    printf("  (");
    base_handle_d(base);
    printf(", ");
    base_handle_c(base);
    printf(")");
}
/* f: Prints the base facilities */
void base_handle_f(base_t base){
    int i;
    for(i=0;i<=base.nfacilities-1;i++){
        facility_handle_f(*base.facilities[i]);
    }
}
/* fc COST: Prints the base facilities with the cost equal to COST */
void base_handle_fc(base_t base, double cost){
    int i;
    for(i=0;i<=base.nfacilities-1;i++){
        facility_handle_fc(*base.facilities[i], cost);
    }
}
/* fcge COST: Prints the base facilities with the cost greater than or equal to COST */
void base_handle_fcge(base_t base, double cost){
    int i;
    for(i=0;i<=base.nfacilities-1;i++){
        facility_handle_fcge(*base.facilities[i], cost);
    }
} 
/* fcgt COST: Prints the base facilities with the cost greater than COST */
void base_handle_fcgt(base_t base, double cost){
    int i;
    for(i=0;i<=base.nfacilities-1;i++){
        facility_handle_fcgt(*base.facilities[i],  cost);
    }
}
/* fcle COST: Prints the base facilities with the cost less than or equal to COST */
void base_handle_fcle(base_t base, double cost){
    int i;
    for(i=0;i<=base.nfacilities-1;i++){
        facility_handle_fcle(*base.facilities[i],cost);
    }
}
/* fclt COST: Prints the base facilities with the cost less than COST */
void base_handle_fclt(base_t base, double cost){
    int i;
    for(i=0;i<=base.nfacilities-1;i++){
        facility_handle_fclt(*base.facilities[i], cost);
    }
}
/* fn NAME: Prints the base facilities with the name containing NAME */
void base_handle_fn(base_t base, const char *name){
    int i;
    for(i=0;i<=base.nfacilities-1;i++){
        facility_handle_fn(*(base.facilities[i]),name);
    }
}
/* Print the cost of facility */
void base_handle_t(base_t base){
    int i;
    double somme=0;
    for(i=0;i<=base.nfacilities-1;i++){
        somme =somme + base.facilities[i]->cost;
    }
    printf("%.2f USD\n",somme);
}
/* Free the memory allocated to the base */
void base_free(base_t *base){
    int libere; 
    free(base->name);
    free(base->country);  
    for(libere=0;libere<base->nfacilities;libere++){
      facility_free(base->facilities[libere]); 
    }
    free(base->facilities);

    free(base); 
}
 