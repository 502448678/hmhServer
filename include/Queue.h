#ifndef _QUEUE_H
#define _QUEUE_H
#include "Packdef.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct NODE
{
    void *nValue;
    struct NODE* pNext;
} Myqueue;

typedef struct NODE1
{
    int nCount;
    Myqueue* pHead;
    Myqueue* pTail;
} Queue;

void q_Init(Queue **ppQueue);
void q_Push(Queue *pQueue,void* nValue);
void q_Clear(Queue **ppQueue);
void q_Destroy(Queue **ppQueue);
int q_isEmpty(Queue *pQueue);
void* q_Pop(Queue *pQueue);
void q_DeleteNode(Queue *pQueue, void* nValue);
int q_GetNum(Queue *pQueue);





#endif
