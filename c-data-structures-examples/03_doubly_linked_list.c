/**
 * 3. 双方向連結リスト (Doubly Linked List)
 * 
 * 学習ポイント:
 * - 双方向のポインタ管理
 * - head と tail の両端からの操作
 * - より複雑なポインタ操作
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct DNode {
    int data;
    struct DNode *prev;
    struct DNode *next;
} DNode;

typedef struct {
    DNode *head;
    DNode *tail;
    size_t size;
} DoublyLinkedList;

// リストの初期化
DoublyLinkedList* dlist_create() {
    DoublyLinkedList *list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// 新しいノードを作成
static DNode* create_dnode(int data) {
    DNode *node = (DNode*)malloc(sizeof(DNode));
    if (!node) return NULL;
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

// 先頭に追加
int dlist_push_front(DoublyLinkedList *list, int data) {
    DNode *node = create_dnode(data);
    if (!node) return -1;
    
    if (list->head == NULL) {
        list->head = list->tail = node;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    list->size++;
    return 0;
}

// 末尾に追加
int dlist_push_back(DoublyLinkedList *list, int data) {
    DNode *node = create_dnode(data);
    if (!node) return -1;
    
    if (list->tail == NULL) {
        list->head = list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
    return 0;
}

// 先頭を削除
int dlist_pop_front(DoublyLinkedList *list, int *out_data) {
    if (list->head == NULL) return -1;
    
    DNode *temp = list->head;
    *out_data = temp->data;
    
    if (list->head == list->tail) {
        list->head = list->tail = NULL;
    } else {
        list->head = temp->next;
        list->head->prev = NULL;
    }
    
    free(temp);
    list->size--;
    return 0;
}

// 末尾を削除
int dlist_pop_back(DoublyLinkedList *list, int *out_data) {
    if (list->tail == NULL) return -1;
    
    DNode *temp = list->tail;
    *out_data = temp->data;
    
    if (list->head == list->tail) {
        list->head = list->tail = NULL;
    } else {
        list->tail = temp->prev;
        list->tail->next = NULL;
    }
    
    free(temp);
    list->size--;
    return 0;
}

// 指定したノードの後に挿入
int dlist_insert_after(DoublyLinkedList *list, DNode *node, int data) {
    if (node == NULL) return -1;
    
    DNode *new_node = create_dnode(data);
    if (!new_node) return -1;
    
    new_node->prev = node;
    new_node->next = node->next;
    
    if (node->next != NULL) {
        node->next->prev = new_node;
    } else {
        list->tail = new_node;
    }
    node->next = new_node;
    
    list->size++;
    return 0;
}

// 指定したノードを削除
int dlist_remove_node(DoublyLinkedList *list, DNode *node) {
    if (node == NULL) return -1;
    
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    
    free(node);
    list->size--;
    return 0;
}

// 値で検索
DNode* dlist_find(DoublyLinkedList *list, int data) {
    DNode *current = list->head;
    while (current != NULL) {
        if (current->data == data) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// リストを解放
void dlist_destroy(DoublyLinkedList *list) {
    DNode *current = list->head;
    while (current != NULL) {
        DNode *temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

// 前方向に表示
void dlist_print_forward(DoublyLinkedList *list) {
    printf("Forward (size=%zu): NULL <-> ", list->size);
    DNode *current = list->head;
    while (current != NULL) {
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// 後方向に表示
void dlist_print_backward(DoublyLinkedList *list) {
    printf("Backward: NULL <-> ");
    DNode *current = list->tail;
    while (current != NULL) {
        printf("%d <-> ", current->data);
        current = current->prev;
    }
    printf("NULL\n");
}

int main() {
    printf("=== 双方向連結リストのデモ ===\n\n");
    
    DoublyLinkedList *list = dlist_create();
    
    // 要素を追加
    printf("末尾に追加 (1, 2, 3):\n");
    dlist_push_back(list, 1);
    dlist_push_back(list, 2);
    dlist_push_back(list, 3);
    dlist_print_forward(list);
    
    printf("\n先頭に追加 (0):\n");
    dlist_push_front(list, 0);
    dlist_print_forward(list);
    dlist_print_backward(list);
    
    // ノードの後ろに挿入
    printf("\n'2'の後ろに'25'を挿入:\n");
    DNode *node = dlist_find(list, 2);
    if (node) {
        dlist_insert_after(list, node, 25);
    }
    dlist_print_forward(list);
    
    // ノードを削除
    printf("\n'2'を削除:\n");
    node = dlist_find(list, 2);
    if (node) {
        dlist_remove_node(list, node);
    }
    dlist_print_forward(list);
    
    // 両端から削除
    int value;
    printf("\n先頭から削除:\n");
    dlist_pop_front(list, &value);
    printf("削除: %d\n", value);
    dlist_print_forward(list);
    
    printf("\n末尾から削除:\n");
    dlist_pop_back(list, &value);
    printf("削除: %d\n", value);
    dlist_print_forward(list);
    
    dlist_destroy(list);
    return 0;
}
