#pragma once

#define MAX_QUEUE_SIZE 10

typedef struct string_queue *string_queue_t;

string_queue_t queue_create_string_queue(int capacity);

int queue_isFull(string_queue_t q);

int queue_isEmpty(string_queue_t q);

void queue_enqueue(string_queue_t q, void *item);

void *queue_dequeue(string_queue_t q);

void *queue_peek(string_queue_t q);
