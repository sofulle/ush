// vector.c

#include "libmx.h"
#include "vector.h"

vector_t *vector_init(void *data) {
    vector_t *head = malloc(sizeof(vector_t));
    if(head == NULL) return NULL;

    head->id = 0;
    head->data = data;
    head->prev = NULL;
    head->next = NULL;
    head->head = head;
    head->tail = head;

    return head;
}

vector_t *vector_get(vector_t **head, uint32_t id) {
    vector_t *node = NULL;

    if(head == NULL) return NULL;
    if(*head == NULL) return NULL;

    if(id > (*head)->tail->id || id < 0) return NULL;

    for(vector_t *nd = *head; nd != NULL; nd = nd->next) {
        if(nd->id == id) {
            node = nd;
            break;
        }
    }

    return node;
}

void vector_push_front(vector_t **head, void *data) {
    vector_t *node = NULL;
    uint32_t id = 0;

    if(head == NULL) return;
    if(*head == NULL) {
        *head = vector_init(data);
        return;
    }

    node = malloc(sizeof(vector_t));
    if(node == NULL) return;

    node->data = data;
    node->prev = NULL;
    node->next = *head;
    node->tail = (*head)->tail;

    (*head)->prev = node;

    for(vector_t *nd = node; nd != NULL; nd = nd->next) {
        nd->id = id;
        nd->head = node;
        id++;
    }

    *head = node;
}

void vector_push_back(vector_t **head, void *data) {
    vector_t *node = NULL;
    uint32_t id = 0;

    if(head == NULL) return;
    if(*head == NULL) {
        *head = vector_init(data);
        return;
    }

    node = malloc(sizeof(vector_t));
    if(node == NULL) return;

    node->data = data;
    node->prev = (*head)->tail;
    node->next = NULL;
    node->head = *head;

    (*head)->tail->next = node;

    for(vector_t *nd = *head; nd != NULL; nd = nd->next) {
        nd->id = id;
        nd->tail = node;
        id++;
    }
}

void vector_insert(vector_t **head, void *data, uint32_t id) {
    vector_t *node = NULL;
    vector_t *before = NULL;
    vector_t *after = NULL;
    uint32_t _id = 0;

    if(head == NULL) return;
    if(*head == NULL) return;

    if(id > (*head)->tail->id || id < 1) return;

    node = malloc(sizeof(vector_t));
    if(node == NULL) return;

    for(vector_t *nd = *head; nd != NULL; nd = nd->next) {
        if(nd->id == id) {
            after = nd;
            break;
        }
    }
    if(after != NULL) before = after->prev;

    node->data = data;
    node->prev = before;
    node->next = after;
    node->tail = (*head)->tail;
    node->head = *head;

    if(before != NULL) before->next = node;
    if(after != NULL) after->prev = node;

    for(vector_t *nd = *head; nd != NULL; nd = nd->next) {
        nd->id = _id;
        _id++;
    }
}

void vector_pop_front(vector_t **head) {
    vector_t *before = NULL;
    uint32_t id = 0;
    bool is_last = false;

    if(head == NULL) return;
    if(*head == NULL) return;

    if(*head == (*head)->tail) is_last = true;

    before = *head;
    if(before->next != NULL) *head = before->next;
    (*head)->prev = NULL;

    for(vector_t *nd = *head; nd != NULL; nd = nd->next) {
        nd->id = id;
        nd->head = *head;
        id++;
    }

    if(before->data != NULL) free(before->data);
    free(before);

    if(is_last) *head = NULL;
}

void vector_pop_back(vector_t **head) {
    vector_t *after = NULL;
    uint32_t id = 0;
    bool is_last = false;

    if(head == NULL) return;
    if(*head == NULL) return;

    if(*head == (*head)->tail) is_last = true;

    after = (*head)->tail;
    if(after->prev != NULL) (*head)->tail = after->prev;
    (*head)->tail->next = NULL;

    if((*head)->tail == NULL) *head = NULL;

    for(vector_t *nd = *head; nd != NULL; nd = nd->next) {
        nd->id = id;
        nd->tail = (*head)->tail;
        id++;
    }

    if(after->data != NULL) free(after->data);
    free(after);

    if(is_last) *head = NULL;
}

void vector_delete(vector_t **head, uint32_t id) {
    vector_t *before = NULL;
    vector_t *current = NULL;
    vector_t *after = NULL;
    uint32_t _id = 0;

    if(head == NULL) return;
    if(*head == NULL) return;

    if(id > (*head)->tail->id || id < 0) return;

    if(id == 0) {
        vector_pop_front(head);
        return;
    }
    if(id == vector_size(head) - 1) {
        vector_pop_back(head);
        return;
    }

    for(vector_t *nd = *head; nd != NULL; nd = nd->next) {
        if(nd->id == id) {
            current = nd;
            break;
        }
    }
    before = current->prev;
    after = current->next;

    before->next = after;
    after->prev = before;

    for(vector_t *nd = *head; nd != NULL; nd = nd->next) {
        nd->id = _id;
        _id++;
    }

    if(current->data != NULL) free(current->data);
    free(current);
}

void vector_free(vector_t **head) {

    for(uint32_t i = 0; i < vector_size(head); i++) {
        vector_pop_back(head);
    }

    *head = NULL;
}

uint32_t vector_size(vector_t **head) {
    uint32_t count = 0;

    for(vector_t *node = *head; node != NULL; node = node->next) {
        count++;
    }

    return count;
}
