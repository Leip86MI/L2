#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "facility.h"

/* Structure facility */
facility_t* facility_create()
{
    facility_t* facility;
    
    facility=malloc(sizeof(facility_t));
    if(facility== NULL)
    {
        printf("Allocution échouée\n");
        return NULL;
    }
    facility->area=0;
    facility->cost=0;
    facility->name=NULL;
    return facility;
}
/* f: Prints the base facilities */ 
void facility_handle_f(facility_t facility){
    printf("%s, %d m2, %.2f USD\n",facility.name,facility.area,facility.cost);
}
/* fc COST: Prints the base facilities with the cost equal to COST */
void facility_handle_fc(facility_t facility, double cost){
    if(cost==facility.cost){
        printf("%s, %d m2, %.2f USD\n",facility.name,facility.area,facility.cost); 
    } 
}
/* fcge COST: Prints the base facilities with the cost greater than or equal to COST */
void facility_handle_fcge(facility_t facility, double cost){
    if(cost<=facility.cost){
        printf("%s, %d m2, %.2f USD\n",facility.name,facility.area,facility.cost); 
    }
}
/* fcgt COST: Prints the base facilities with the cost greater than COST */
void facility_handle_fcgt(facility_t facility, double cost){
    if(cost<facility.cost){
        printf("%s, %d m2, %.2f USD\n",facility.name,facility.area,facility.cost); 
    }
}
/* fcle COST: Prints the base facilities with the cost less than or equal to COST */
void facility_handle_fcle(facility_t facility, double cost){
    if(cost>=facility.cost){
        printf("%s, %d m2, %.2f USD\n",facility.name,facility.area,facility.cost); 
    }
}
/* fclt COST: Prints the base facilities with the cost less than COST */
void facility_handle_fclt(facility_t facility, double cost){
    if(cost>facility.cost){
        printf("%s, %d m2, %.2f USD\n",facility.name,facility.area,facility.cost); 
    }
}
/* fn NAME: Prints the base facilities with the name containing NAME */
void facility_handle_fn(facility_t facility, const char *name){
    if(strstr(facility.name, name)!=NULL){
        printf("%s, %d m2, %.2f USD\n",facility.name,facility.area,facility.cost);
    }
}
/* Free the memory for a facility structure */
void facility_free(facility_t *facility){
    free(facility->name);
    free(facility);
}
  