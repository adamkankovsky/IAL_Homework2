/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    //pokud strom je neni prazdny tak vracime FALSE
    if(!(*tree)){
        //vyskakujeme z funkce
        return;
    }
    //pokud je strom prazdny nastavime jeho hodnotu na NULL
    (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    //cyklus iteruje dokud nenarazi na prazdny prvek.
    while(tree != NULL){
        //kdyz je klic prvku schodny s klicem zadanym funkci
        if(tree->key == key){
            //zapiseme do promenne value hodnotu daneho prvku
            *value = tree->value; //vratime hodnotu true protoze jsme prvek nasli
            return true;
        }
        //pokud je klic prvku mensi nez klic zadany funkci
        if(tree->key > key){
            //prejdeme do prvku v levo pod nim
            tree = tree->left;
        } else if(tree->key < key){ //pokud je klidc prvku vetsi nez klic zadany funkci
            //prejdeme do prvku napravo pod nim
            tree = tree->right;
        }
    }
    //pokud jsme prvek nenasli nebo je strom prazdny vracime hodnotu false
    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienk
 * u vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if(tree == NULL){
        return;
    }
    //cyklus iteruje dokud nenarazi na prazdny prvek.
    while((*tree) != NULL){
        //kdyz najdeme prvek jehoz klic je stejny jako klic zadany funkci
        if((*tree)->key == key){
            //prehrajeme jeho hodnotu hodnotou predanou funkci
            (*tree)->value = value;
            return;
        }
        //pokud je klic prvku mensi nez klic zadany funkci
        if(((*tree)->key) > key){
            //prejdeme do prvku v levo pod nidm
            tree = &((*tree)->left);
        } else if(((*tree)->key) < key){ //pokud je klidc prvku vetsi nez klic zadany funkci
            //prejdeme do prvku napravo pod nim
            tree = &((*tree)->right);
        }
    }
    //pokud prvek ve strome neni
    (*tree) = (bst_node_t *)malloc(sizeof(bst_node_t)); //naalokujeme prvek
    if((*tree) == NULL) return;
    (*tree)->key = key; //predame prvku klic
    (*tree)->value = value; //predame prvku hodnotu
    (*tree)->left = NULL;   //nastavime jeho leveho potomka na NULL
    (*tree)->right = NULL;  //nastavime jeho praveho potomka na NULL
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *tmp = NULL;
    //postupujeme stromem dokud nedojdeme do nejpravejsiho prvku
    while((*tree)->right != NULL){
        //tree bude nasledujici pravy prvek
        tree = &(*tree)->right;
    }
    target->key = (*tree)->key;//do promenne target zapiseme nejpravejsi prvek
    target->value = (*tree)->value;
    tmp = (*tree)->left;
    free(*tree);
    if(tmp != NULL){
        *tree = tmp;
        return;
    }
    *tree = NULL;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t *next = NULL;
    if(tree == NULL){
        return;
    }
    do {
        if ((*tree)->key == key) {
            if ((*tree)->right != NULL && (*tree)->left != NULL) {
                bst_node_t *target = (bst_node_t *)malloc(sizeof(tree));
                if(target == NULL) return;
                bst_replace_by_rightmost(target, &((*tree)->left));
                (*tree)->key = target->key;
                (*tree)->value = target->value;
                free(target);
                return;
            }
            if ((*tree)->right != NULL) {
                next = (*tree)->right;
                free((*tree));
                (*tree) = next;
                return;
            }
            if ((*tree)->left != NULL) {
                next = (*tree)->left;
                free(*tree);
                (*tree) = next;
                return;
            }
            free(*tree);
            *tree = NULL;
            return;
        }
        //pokud je klic prvku mensi nez klic zadany funkci
        if(((*tree)->key) > key){
            //prejdeme do prvku v levo pod nim
            tree = &((*tree)->left);
        } else if(((*tree)->key) < key){ //pokud je klidc prvku vetsi nez klic zadany funkci
            //prejdeme do prvku napravo pod nim
            tree = &((*tree)->right);
        }
        //prochazime strom dokud nenalezneme prvek ktery hledame
    }while((*tree) != NULL);
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t *stack = (stack_bst_t *)malloc(sizeof(stack_bst_t));
    bool empty = false;
    bst_node_t *next;
    do{
        if (*tree == NULL){
            if (!(empty = stack_bst_empty(stack))) {
                *tree = stack_bst_pop(stack);
            }
        } else {
            if ( (*tree)->right != NULL ) {
                stack_bst_push(stack, (*tree)->right);
            }
            next = (*tree)->left;
            free(*tree);
            *tree = next;
        }
    } while ((*tree != NULL) || !(empty = stack_bst_empty(stack)));
    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        bst_print_node(tree);
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t *stack = (stack_bst_t *)malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);
    bool empty = false;
    while(true) {
        bst_leftmost_preorder(tree, stack);
        if(!(empty = stack_bst_empty(stack))){
            tree = stack_bst_pop(stack);
            if(tree != NULL){
                tree = tree->right;
            }
        }else{
            free(stack);
            return;
        }
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t *stack = (stack_bst_t *)malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);
    bool empty = false;
    while(true) {
        bst_leftmost_inorder(tree, stack);
        if(!(empty = stack_bst_empty(stack))){
            tree = stack_bst_pop(stack);
            bst_print_node(tree);
            if(tree != NULL){
                tree = tree->right;
            }
        }else{
            free(stack);
            return;
        }
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    bool fLeft;
    stack_bst_t *stack = (stack_bst_t *)malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);
    stack_bool_t *bStack = (stack_bool_t *)malloc(sizeof(stack_bool_t));
    stack_bool_init(bStack);
    bst_leftmost_postorder(tree, stack, bStack);
    bool empty = false;
    while (!(empty = stack_bst_empty(stack))){
        tree = stack_bst_top(stack);
        fLeft = stack_bool_pop(bStack);
        if (fLeft){
            stack_bool_push(bStack, false);
            bst_leftmost_postorder(tree->right, stack, bStack);
        } else {
            stack_bst_pop(stack);
            bst_print_node(tree);
        }
    }
    free(stack);
    free(bStack);
}
