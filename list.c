#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct Node Node;



typedef List List;

void freeList(List* lista) {
    Node* temp;
    while (lista->head != NULL) {
        temp = lista->head;
        lista->head = lista->head->next;
        free(temp);
    }
    free(lista);
}


Node * createNode(void * data) {
    Node * new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

List * createList() {
    List* lista = (List*) malloc(sizeof(List));
    lista->head = NULL;
    lista->tail = NULL;
    lista->current = NULL;
    return lista;
}

void * firstList(List * list) {
    if (list->head == NULL){
        return NULL;
    }
    list->current = list->head;
    return list->head->data;
}

void * nextList(List * list) {
    if (list->current == NULL){
        return NULL;
    }
    Node * proximoNodo = list->current->next;
    list->current = proximoNodo;

    if (proximoNodo == NULL){
        return NULL;
    }

    return proximoNodo->data;
}

void * lastList(List * list) {
    if (list->tail == NULL){
        return NULL;
    }
    list->current = list->tail;
    return list->tail->data;
}

void * prevList(List * list) {
    if (list->current == NULL){
        return NULL;
    }
    Node * nodoAnterior = list->current->prev;
    list->current = nodoAnterior;

    if (nodoAnterior == NULL){
        return NULL;
    }

    return nodoAnterior->data;
}

void pushFront(List * list, void * data) {
    Node * newNode = createNode(data);

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        list->current = newNode;
        return;
    }

    Node * nodo_b = list->head;
    Node * nodo_a = NULL;

    newNode->next = nodo_b;
    newNode->prev = nodo_a;

    nodo_b->prev = newNode;

    list->head = newNode;

    list->current = newNode;
}

void pushBack(List * list, void * data) {
    list->current = list->tail;
    pushCurrent(list,data);
}

void pushCurrent(List * list, void * data) {
    if (list->current == NULL) {
        return;
    }
    Node * newNode = createNode(data);
    newNode->next = list->current->next;
    newNode->prev = list->current;

    if (list->current->next != NULL) {
        list->current->next->prev = newNode;
    }

    list->current->next = newNode;


    if (list->current == list->tail) {
        list->tail = newNode;
    }
}

void * popFront(List * list) {
    list->current = list->head;
    return popCurrent(list);
}

void * popBack(List * list) {
    list->current = list->tail;
    return popCurrent(list);
}

void * popCurrent(List * list) {
    if (list->current == NULL) {
        return NULL;
    }

    Node * current = list->current;
    void * data = current->data;

    Node * prevNode = NULL;
    Node * aux = list->head;

    while (aux != NULL && aux != current) {
        prevNode = aux;
        aux = aux->next;
    }

    if (aux == current) {
        if (prevNode != NULL) {
            prevNode->next = current->next;
        } else {
            list->head = current->next;
        }

        if (current->next != NULL) {
            current->next->prev = prevNode;
        } else {
            list->tail = prevNode;
        }

        list->current = current->next;

        free(current);

        return data;
    } else {
        return NULL;
    }
    return NULL;
}

void cleanList(List * list) {
    while (list->head != NULL) {
        popFront(list);
    }
}
