#pragma once

typedef struct queue *queue_t;

queue_t queue_create_queue(int capacity);

int queue_isFull(queue_t q);

int queue_isEmpty(queue_t q);

void queue_enqueue(queue_t q, void *item);

void *queue_dequeue(queue_t q);

void *queue_peek(queue_t q);
