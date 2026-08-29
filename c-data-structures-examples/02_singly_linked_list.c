/**
 * 2. 単方向連結リスト (Singly Linked List)
 * 
 * 学習ポイント:
 * - ノードベースのデータ構造
 * - ポインタを使った要素の連結
 * - リストの走査とメモリ管理
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t size;
} LinkedList;

// リストの初期化
LinkedList* list_create() {
    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    if (!list) return NULL;
    list->head = NULL;
    list->size = 0;
    return list;
}

// 新しいノードを作成
static Node* create_node(int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

// 先頭に追加
int list_push_front(LinkedList *list, int data) {
    Node *node = create_node(data);
    if (!node) return -1;
    
    node->next = list->head;
    list->head = node;
    list->size++;
    return 0;
}

// 末尾に追加
int list_push_back(LinkedList *list, int data) {
    Node *node = create_node(data);
    if (!node) return -1;
    
    if (list->head == NULL) {
        list->head = node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
    list->size++;
    return 0;
}

// 先頭を削除
int list_pop_front(LinkedList *list, int *out_data) {
    if (list->head == NULL) return -1;
    
    Node *temp = list->head;
    *out_data = temp->data;
    list->head = temp->next;
    free(temp);
    list->size--;
    return 0;
}

// 指定した値を検索
Node* list_find(LinkedList *list, int data) {
    Node *current = list->head;
    while (current != NULL) {
        if (current->data == data) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 指定した値を削除
int list_remove(LinkedList *list, int data) {
    if (list->head == NULL) return -1;
    
    // 先頭の場合
    if (list->head->data == data) {
        Node *temp = list->head;
        list->head = temp->next;
        free(temp);
        list->size--;
        return 0;
    }
    
    // それ以外の場合
    Node *current = list->head;
    while (current->next != NULL) {
        if (current->next->data == data) {
            Node *temp = current->next;
            current->next = temp->next;
            free(temp);
            list->size--;
            return 0;
        }
        current = current->next;
    }
    return -1;
}

// リストを逆順にする
void list_reverse(LinkedList *list) {
    Node *prev = NULL;
    Node *current = list->head;
    Node *next = NULL;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list->head = prev;
}

// リストを解放
void list_destroy(LinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

// リストを表示
void list_print(LinkedList *list) {
    printf("List (size=%zu): ", list->size);
    Node *current = list->head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    printf("=== 単方向連結リストのデモ ===\n\n");
    
    LinkedList *list = list_create();
    
    // 要素を追加
    printf("末尾に追加:\n");
    for (int i = 1; i <= 5; i++) {
        list_push_back(list, i * 10);
        list_print(list);
    }
    
    printf("\n先頭に追加 (100):\n");
    list_push_front(list, 100);
    list_print(list);
    
    // 検索
    printf("\n30を検索: %s\n", list_find(list, 30) ? "見つかりました" : "見つかりません");
    printf("99を検索: %s\n", list_find(list, 99) ? "見つかりました" : "見つかりません");
    
    // 削除
    printf("\n30を削除:\n");
    list_remove(list, 30);
    list_print(list);
    
    // 逆順
    printf("\nリストを逆順に:\n");
    list_reverse(list);
    list_print(list);
    
    // 先頭から削除
    int value;
    printf("\n先頭から削除:\n");
    while (list_pop_front(list, &value) == 0) {
        printf("削除: %d, ", value);
        list_print(list);
    }
    
    list_destroy(list);
    return 0;
}
