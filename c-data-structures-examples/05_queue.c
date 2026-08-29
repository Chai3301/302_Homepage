/**
 * 5. キュー (Queue) - FIFO (First In, First Out)
 * 
 * 学習ポイント:
 * - キューの基本操作 (enqueue, dequeue)
 * - 循環配列による効率的な実装
 * - 連結リストベースの実装
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// === 循環配列ベースのキュー ===
typedef struct {
    int *data;
    size_t front;
    size_t rear;
    size_t size;
    size_t capacity;
} CircularQueue;

CircularQueue* cqueue_create(size_t capacity) {
    CircularQueue *queue = (CircularQueue*)malloc(sizeof(CircularQueue));
    if (!queue) return NULL;
    
    queue->data = (int*)malloc(sizeof(int) * capacity);
    if (!queue->data) {
        free(queue);
        return NULL;
    }
    
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = capacity;
    return queue;
}

bool cqueue_is_empty(CircularQueue *queue) {
    return queue->size == 0;
}

bool cqueue_is_full(CircularQueue *queue) {
    return queue->size >= queue->capacity;
}

int cqueue_enqueue(CircularQueue *queue, int value) {
    if (cqueue_is_full(queue)) return -1;
    
    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;
    return 0;
}

int cqueue_dequeue(CircularQueue *queue, int *out_value) {
    if (cqueue_is_empty(queue)) return -1;
    
    *out_value = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return 0;
}

int cqueue_peek(CircularQueue *queue, int *out_value) {
    if (cqueue_is_empty(queue)) return -1;
    *out_value = queue->data[queue->front];
    return 0;
}

void cqueue_destroy(CircularQueue *queue) {
    if (queue) {
        free(queue->data);
        free(queue);
    }
}

void cqueue_print(CircularQueue *queue) {
    printf("Queue [front -> rear]: ");
    if (cqueue_is_empty(queue)) {
        printf("(empty)");
    } else {
        size_t i = queue->front;
        for (size_t count = 0; count < queue->size; count++) {
            printf("%d ", queue->data[i]);
            i = (i + 1) % queue->capacity;
        }
    }
    printf("(size=%zu)\n", queue->size);
}

// === 連結リストベースのキュー ===
typedef struct QueueNode {
    int data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    size_t size;
} LinkedQueue;

LinkedQueue* lqueue_create() {
    LinkedQueue *queue = (LinkedQueue*)malloc(sizeof(LinkedQueue));
    if (!queue) return NULL;
    
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

bool lqueue_is_empty(LinkedQueue *queue) {
    return queue->front == NULL;
}

int lqueue_enqueue(LinkedQueue *queue, int value) {
    QueueNode *node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!node) return -1;
    
    node->data = value;
    node->next = NULL;
    
    if (queue->rear == NULL) {
        queue->front = queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
    return 0;
}

int lqueue_dequeue(LinkedQueue *queue, int *out_value) {
    if (lqueue_is_empty(queue)) return -1;
    
    QueueNode *temp = queue->front;
    *out_value = temp->data;
    queue->front = temp->next;
    
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    free(temp);
    queue->size--;
    return 0;
}

void lqueue_destroy(LinkedQueue *queue) {
    QueueNode *current = queue->front;
    while (current != NULL) {
        QueueNode *temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
}

void lqueue_print(LinkedQueue *queue) {
    printf("LinkedQueue [front -> rear]: ");
    QueueNode *current = queue->front;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("(size=%zu)\n", queue->size);
}

int main() {
    printf("=== キューのデモ ===\n\n");
    
    // 循環配列ベースのキュー
    printf("--- 循環配列ベースのキュー ---\n");
    CircularQueue *cqueue = cqueue_create(5);
    
    printf("要素を追加:\n");
    for (int i = 1; i <= 5; i++) {
        if (cqueue_enqueue(cqueue, i * 10) == 0) {
            printf("Enqueue %d: ", i * 10);
            cqueue_print(cqueue);
        }
    }
    
    printf("\n2つ取り出し:\n");
    int value;
    for (int i = 0; i < 2; i++) {
        cqueue_dequeue(cqueue, &value);
        printf("Dequeue %d: ", value);
        cqueue_print(cqueue);
    }
    
    printf("\n新しい要素を追加 (循環の確認):\n");
    cqueue_enqueue(cqueue, 60);
    cqueue_enqueue(cqueue, 70);
    cqueue_print(cqueue);
    
    cqueue_destroy(cqueue);
    
    // 連結リストベースのキュー
    printf("\n--- 連結リストベースのキュー ---\n");
    LinkedQueue *lqueue = lqueue_create();
    
    printf("要素を追加:\n");
    for (int i = 1; i <= 3; i++) {
        lqueue_enqueue(lqueue, i * 100);
        printf("Enqueue %d: ", i * 100);
        lqueue_print(lqueue);
    }
    
    printf("\n全要素を取り出し:\n");
    while (lqueue_dequeue(lqueue, &value) == 0) {
        printf("Dequeue %d: ", value);
        lqueue_print(lqueue);
    }
    
    lqueue_destroy(lqueue);
    
    return 0;
}
