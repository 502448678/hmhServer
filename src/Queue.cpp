#include "Queue.h"



void q_Init(Queue **ppQueue)
{
    *ppQueue = (Queue*)malloc(sizeof(Queue));
    (*ppQueue)->nCount = 0;
    (*ppQueue)->pHead = NULL;
    (*ppQueue)->pTail = NULL;
}

void q_Push(Queue *pQueue,void* nValue)
{
    if(pQueue == NULL)return;
    Myqueue* pMyqueue = (Myqueue*)malloc(sizeof(Myqueue));
    pMyqueue->nValue = nValue;
    pMyqueue->pNext = NULL;
    if(pQueue->pHead == NULL)
    {
        pQueue->pHead = pMyqueue;
    }
    else
    {
        pQueue->pTail->pNext = pMyqueue;
    }
    pQueue->pTail = pMyqueue;
    ++pQueue->nCount;
}

void q_Clear(Queue **ppQueue)
{
    if(*ppQueue == NULL)exit(1);
    Myqueue *pDel = NULL;
    while((*ppQueue)->pHead != NULL)
    {
        pDel = (*ppQueue)->pHead;
        (*ppQueue)->pHead = (*ppQueue)->pHead->pNext;
        free(pDel);
        pDel = NULL;
    }
}


void q_Destroy(Queue **ppQueue)
{
    q_Clear(ppQueue);
    free(*ppQueue);
    *ppQueue = NULL;
}

int q_isEmpty(Queue *pQueue)
{
    if(pQueue == NULL)exit(1);
    return pQueue->nCount = 0 ? 1 : 0;
}


void* q_Pop(Queue *pQueue)
{
    if(pQueue == NULL || pQueue->nCount == 0)return NULL;
    void* nValue = pQueue->pHead->nValue;
    Myqueue *pDel = NULL;
    pDel = pQueue->pHead;
    pQueue->pHead = pQueue->pHead->pNext;
    free(pDel);
    pDel = NULL;
    pQueue->nCount--;
    if(pQueue->pHead == NULL)
    {
        pQueue->pTail = NULL;
    }
    return nValue;
}



int q_GetNum(Queue *pQueue)
{
    if(pQueue == NULL)return -1;
    return pQueue->nCount;
}

void q_DeleteNode(Queue *pQueue, void* nValue)
{
    if(pQueue == NULL || pQueue->nCount == 0 || nValue == 0)return ;

    Myqueue* tmp = pQueue->pHead;
    Myqueue* pDel = NULL;

    if( tmp->nValue == nValue)
    {
        q_Pop(pQueue);
    }
    else
    {
        while( tmp && tmp->pNext )
        {
            if( tmp->pNext->nValue == nValue )
            {
                pDel = tmp->pNext;
                tmp->pNext = pDel->pNext;
                if( pDel == pQueue->pTail )
                {
                    pQueue->pTail = tmp;
                }
                pQueue->nCount--;
                free(pDel);
                pDel = NULL;
                break;
            }
            tmp = tmp->pNext;
        }
    }
}
