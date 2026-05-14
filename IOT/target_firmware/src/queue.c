#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct queue
{
    void **data;
    int front;
    int rear;
    int size;
    int capacity;
} queue;

queue_t queue_create_queue(int capacity)
{
    queue_t q = (queue_t)malloc(sizeof(queue));
    q->data = (void **)malloc(sizeof(void *) * capacity);
    q->capacity = capacity;
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    return q;
}

int queue_isFull(queue_t q)
{
    return q->size == q->capacity;
}

int queue_isEmpty(queue_t q)
{
    return q->size == 0;
}

void queue_enqueue(queue_t q, void *item)
{
    if (queue_isFull(q))
    {
        printf("Queue is full. Cannot enqueue item.\n");
        return;
    }
    q->data[q->rear] = item;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
}

void *queue_dequeue(queue_t q)
{
    if (queue_isEmpty(q))
    {
        printf("Queue is empty. Cannot dequeue.\n");
        return NULL;
    }
    void *item = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return item;
}

void *queue_peek(queue_t q)
{
    if (queue_isEmpty(q))
    {
        printf("Queue is empty. Cannot peek.\n");
        return NULL;
    }
    return q->data[q->front];
}