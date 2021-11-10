/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    for(int i = 0; i < HT_SIZE; i++){
        (*table)[i] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    ht_item_t *searched = (*table)[get_hash(key)];
    while(searched != NULL){
        if(searched->key == key) return searched;
        searched = searched->next;
    }
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    int hash = get_hash(key);
    ht_item_t *item  = ht_search(table, key);
    if(item == NULL){
        ht_item_t *newItem = (ht_item_t *)malloc(sizeof(ht_item_t));
        newItem->key = key;
        newItem->value = value;
        newItem->next = (*table)[hash];
        (*table)[hash] = newItem;
    }else{
        item->value = value;
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    ht_item_t *item  = ht_search(table, key);
    if(item != NULL) return &item->value;
    else return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    int hash = get_hash(key);
    ht_item_t *ptr;
    if((*table)[hash]->key == key){
        ptr = (*table)[hash]->next;
        free((*table)[hash]);
        (*table)[hash] = ptr;
        return;
    }
    while((*table)[hash]->next != NULL){
        if((*table)[hash]->next->key == key){
            ptr = (*table)[hash]->next;
            (*table)[hash]->next = (*table)[hash]->next->next;
            free(ptr);
            return;
        }else {
            (*table)[hash] = (*table)[hash]->next;
        }
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
    for(int i = 0; i < HT_SIZE; i++){
        ht_item_t *tmp = (*table)[i];
        ht_item_t *delete;
        while( tmp != NULL){
            delete = tmp;
            tmp = tmp->next;
            free(delete);
        }
        (*table)[i] = NULL;
    }
}
