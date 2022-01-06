// vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct vector_s {
    uint32_t id;
    void *data;
    struct vector_s *prev;
    struct vector_s *next;
    struct vector_s *head;
    struct vector_s *tail;
} vector_t;

vector_t *vector_init(void *data);
vector_t *vector_get(vector_t **head, uint32_t id);
void vector_push_front(vector_t **head, void *data);
void vector_push_back(vector_t **head, void *data);
void vector_insert(vector_t **head, void *data, uint32_t id);
void vector_pop_front(vector_t **head);
void vector_pop_back(vector_t **head);
void vector_delete(vector_t **head, uint32_t id);
void vector_free(vector_t **head);
uint32_t vector_size(vector_t **head);

#endif
