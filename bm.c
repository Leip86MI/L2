#include <stdio.h>  /* for printf */
#include <stdlib.h>  /* for malloc */
#include <string.h>  /* for strlen  */
#include <errno.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "facility.h"
#include "base.h"
#define tMax 100

/* Prints the different available option for the user */
void printH() {
  fprintf(stdout, "b: Prints the base\n");
  fprintf(stdout, "c: Prints the base country\n");
  fprintf(stdout, "d: Prints the base date\n");
  fprintf(stdout, "f: Prints the base facilities\n");
  fprintf(stdout, "fc COST: Prints the base facilities with the cost");
  fprintf(stdout, " equal to COST\n");
  fprintf(stdout, "fcge COST: Prints the base facilities with the");
  fprintf(stdout, " cost greater than or equal to COST\n");
  fprintf(stdout, "fcgt COST: Prints the base facilities with ");
  fprintf(stdout, "the cost greater than COST\n");
  fprintf(stdout, "fcle COST: Prints the base facilities with ");
  fprintf(stdout, "the cost less than or equal to COST\n");
  fprintf(stdout, "fclt COST: Prints the base facilities with the");
  fprintf(stdout, " cost less than COST\n");
  fprintf(stdout, "fn NAME: Prints the base facilities with the name");
  fprintf(stdout, " containing NAME\n");
  fprintf(stdout, "h: Prints this help\n");
  fprintf(stdout, "n: Prints the base name\n");
  fprintf(stdout, "t: Prints the base total cost\n");
  fprintf(stdout, "v: Prints the BM version\n");
  fprintf(stdout, "q: Quits BM\n");
}
/* Prints the name of the programme, the author and theirs mails */
void printV() {
  fprintf(stdout, "BM (Base Manager) 2\n\n");
  fprintf(stdout, "Copyright (C) 2021 Lucas Martin and Yann Simo.\n\n");
  fprintf(stdout, "Written by Lucas Martin <l.martin-de-fourchamdault@");
  fprintf(stdout, "univ-pau.fr> and Yann Junior Simo Tatchim");
  fprintf(stdout, " <yann-junior.simo-tatchim@etud.univ-pau.fr>.\n");
}
/* Function for recover the name in the document xml */
void getReferencename(xmlDocPtr doc, xmlNodePtr cur, base_t * base) {
    while (cur != NULL) {
    if ((!xmlStrcmp(cur -> name, (const xmlChar * )
        "base"))) {
      base -> name = (char * ) xmlGetProp(cur, (const xmlChar * ) "name");
       
    }
    cur = cur -> next;
  }
}
/* Function for access to the country in the document xml */
void parseCountry(xmlDocPtr doc, xmlNodePtr cur, base_t * base) {
  cur = cur -> xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur -> name, (const xmlChar * )"country"))) {
      base -> country = (char * ) xmlNodeListGetString(doc, cur -> xmlChildrenNode, 1);
      
    }
    cur = cur -> next;
  }
}
/* Take the xml's elements at the date to access it to the base structure */
void parseDay(xmlDocPtr doc, xmlNodePtr cur, base_t * base) {
  char * day, * month, * year;
  char * ptrd, * ptrm, * ptry;
  cur = cur -> xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur -> name, (const xmlChar * )
        "day"))) {
      day = (char * ) xmlNodeListGetString(doc, cur -> xmlChildrenNode, 1);
      base -> day = strtol(day, & ptrd, 10);
      xmlFree(day);
    } else if ((!xmlStrcmp(cur -> name, (const xmlChar * )
        "month"))) {
      month = (char * ) xmlNodeListGetString(doc, cur -> xmlChildrenNode, 1);
      base -> month = strtol(month, & ptrm, 10);
      xmlFree(month);
    } else if ((!xmlStrcmp(cur -> name, (const xmlChar * )
        "year"))) {
      year = (char * ) xmlNodeListGetString(doc, cur -> xmlChildrenNode, 1);
      base -> year = strtol(year, & ptry, 10);
      xmlFree(year);
    }
    cur = cur -> next;
  }
}
/* Withdraw facility of the xml document */
void parseinf(xmlDocPtr doc, xmlNodePtr cur, base_t * base) {
  char * key;
  char * ptr;
  facility_t * facility = facility_create();
  facility -> name = (char * ) xmlGetProp(cur, (const xmlChar * )"name");
   
  cur = cur -> xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur -> name, (const xmlChar * )
        "area"))) {
      key = (char * ) xmlNodeListGetString(doc, cur -> xmlChildrenNode, 1);
      facility -> area = strtol(key, & ptr, 10);
      xmlFree(key);

    } else if ((!xmlStrcmp(cur -> name, (const xmlChar * )
        "cost"))) {
      key = (char * ) xmlNodeListGetString(doc, cur -> xmlChildrenNode, 1);
      facility -> cost = strtod(key, & ptr);
      xmlFree(key);
      }
    cur = cur -> next;
  }
  base_add_facility(base, facility);
}
/* Seek the facility knot */
void parsef(xmlDocPtr doc, xmlNodePtr cur, base_t * base) {
  cur = cur -> xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur -> name, (const xmlChar * )"facility"))) {
      parseinf(doc, cur, base);
    }
    cur = cur -> next;
  }
}
/* Analyse if the document is correct */
int parseDoc(char * filename, base_t * base) {
  xmlDocPtr doc; 
  xmlNodePtr cur; 
  doc = xmlParseFile(filename);
  if (doc == NULL) {
    fprintf(stderr, "./bm.out: Unable to parse the document\n");
    return 1;
  }
  cur = xmlDocGetRootElement(doc);
  if ((xmlStrcmp(cur -> name, (const xmlChar * )
      "base"))) {
    return 0;
  }
  getReferencename(doc, cur, base);
  parseCountry(doc, cur, base);
  cur = cur -> xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur -> name, (const xmlChar * )
        "date"))) {

      parseDay(doc, cur, base);
    } else if ((!xmlStrcmp(cur -> name, (const xmlChar * )
        "facilities"))) {
      parsef(doc, cur, base);
     }
    cur = cur -> next;
  }
  xmlFreeDoc(doc);
  return 0;
}

int main(int argc, char * argv[]) {
  double res;
  char * quit = NULL;
  char * str = NULL;
  char * ptr = NULL;
  int i, j = 0, cpt = 0, sortie = 0;
  char tab2[tMax];
  char tab[tMax];
  base_t * base;
  base = base_create();
  /* Check if there is a file in the arguments */
  if (argc != 2) {
    fprintf(stderr, "./bm.out: Invalid number of arguments\n");
    return 1;
  }
  i = parseDoc(argv[1], base);
  if (i == 1) {
    return 1;
  }
  while (sortie == 0) {
    fprintf(stdout, "BM> ");
    fgets(tab, tMax, stdin);
    quit = & tab[0];
    if (strlen(quit) >= 18) {
      fprintf(stderr, "./bm.out: Too many characters for the command\n");
    }
    else if (strlen(quit) <= 2) {
      switch (tab[0]) {    /* Order with one letter */
      case 'h':
        printH();
        break;
      case 'v':
        printV();
        break;
      case 'q':
        fprintf(stdout, "Goodbye!\n");
        sortie = 1;
        break;
      case 'b':
        base_handle_b( * base);
        fprintf(stdout, "\n");
        break;
      case 'c':
        base_handle_c( * base);
        fprintf(stdout, "\n");
        break;
      case 'd':
        base_handle_d( * base);
        fprintf(stdout, "\n");
        break;
      case 'f':
        base_handle_f( * base);
        break;
      case 'n':
        base_handle_n( * base);
        fprintf(stdout, "\n");
        break;
      case 't':
        base_handle_t( * base);
        break;
      default:
        fprintf(stderr, "./bm.out: Invalid command\n");
        break;
      }
    } /* Order fc */
    else if (tab[1] == 'c' && tab[0] == 'f' && (tab[2] == '\n' || tab[2] == ' ')){
          if (tab[2] == '\n'){
            fprintf(stderr, "./bm.out: Missing parameter for the fc command\n");
          } 
          else{
            ptr = & tab[3];
            res = strtod(ptr, & str);
            if (res == 0.000000){
              fprintf(stderr, "./bm.out: Invalid parameter for the fc command\n");
            } 
            else{
            base_handle_fc( * base, res);
            }
          } /* Order fn */
    }else if (tab[0] == 'f' && tab[1] == 'n'){
            if (tab[2] == '\n') {
              fprintf(stderr, "./bm.out: Missing parameter for the fn command\n");
            }else{
              j = 3;
              while( tab[j] != '\n'){
                tab2[cpt] = tab[j];
                cpt++;
                j++;
              }
              base_handle_fn( *base, tab2);
              cpt = 0;
            } /* Order fcge */
    }else if (tab[2] == 'g' && tab[3] == 'e'){
            if (tab[4] == '\n'){
              fprintf(stderr, "./bm.out: Missing parameter for the fcge command\n");
            }else{
              ptr = & tab[5];
              res = strtod(ptr, & str);
              if (res == 0.000000){
                fprintf(stderr, "./bm.out: Invalid parameter for the fcge command\n");
              }else{
                base_handle_fcge( * base, res);
              }
            } /* Order fcgt */ 
    }else if (tab[2] == 'g' && tab[3] == 't'){
            if (tab[4] == '\n') {
              fprintf(stderr, "./bm.out: Missing parameter for the fcgt command\n");
            }else{
              ptr = & tab[5];
              res = strtod(ptr, & str); 
              if (res == 0.000000) {
                fprintf(stderr, "./bm.out: Invalid parameter for the fcgt command\n");
              }else{
                base_handle_fcgt( * base, res);
              }
            } /* Order fclt */
    }else if (tab[2] == 'l' && tab[3] == 't'){
            if (tab[4] == '\n') {
              fprintf(stderr, "./bm.out: Missing parameter for the fclt command\n");
            }else{
              ptr = & tab[5];
              res = strtod(ptr, & str); 
              if (res == 0.000000) {
                fprintf(stderr, "./bm.out: Invalid parameter for the fclt command\n");
              }else{
                base_handle_fclt( * base, res);
              }
            } /* Order fcle */
    }else if (tab[2] == 'l' && tab[3] == 'e'){
            if (tab[4] == '\n') {
              fprintf(stderr, "./bm.out: Missing parameter for the fcle command\n");
            }else{
              ptr = & tab[5];
              res = strtod(ptr, & str); 
              if (res == 0.000000) {
                fprintf(stderr, "./bm.out: Invalid parameter for the fcle command\n");
              }else{
                base_handle_fcle( * base, res);
              }
            }
    } /* Check if the order is two long */
    else{ if (sortie != 1) {
          fprintf(stderr, "./bm.out: Invalid command\n");
        }
      }
    }
  base_free(base);
  
  xmlCleanupParser();
  return 0;
}