/**
 * 1. 動的配列 (Dynamic Array)
 * 
 * 学習ポイント:
 * - メモリの動的確保 (malloc, realloc, free)
 * - ポインタの基本操作
 * - 構造体の定義と使用
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;      // 現在の要素数
    size_t capacity;  // 確保済みの容量
} DynamicArray;

// 初期化
DynamicArray* array_create(size_t initial_capacity) {
    DynamicArray *arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (!arr) return NULL;
    
    arr->data = (int*)malloc(sizeof(int) * initial_capacity);
    if (!arr->data) {
        free(arr);
        return NULL;
    }
    
    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

// 容量を拡張
static int array_resize(DynamicArray *arr, size_t new_capacity) {
    int *new_data = (int*)realloc(arr->data, sizeof(int) * new_capacity);
    if (!new_data) return -1;
    
    arr->data = new_data;
    arr->capacity = new_capacity;
    return 0;
}

// 末尾に要素を追加
int array_push(DynamicArray *arr, int value) {
    if (arr->size >= arr->capacity) {
        // 容量が足りない場合、2倍に拡張
        if (array_resize(arr, arr->capacity * 2) != 0) {
            return -1;
        }
    }
    arr->data[arr->size++] = value;
    return 0;
}

// 末尾の要素を取り出す
int array_pop(DynamicArray *arr, int *out_value) {
    if (arr->size == 0) return -1;
    *out_value = arr->data[--arr->size];
    return 0;
}

// 指定インデックスの要素を取得
int array_get(DynamicArray *arr, size_t index, int *out_value) {
    if (index >= arr->size) return -1;
    *out_value = arr->data[index];
    return 0;
}

// 指定インデックスに値を設定
int array_set(DynamicArray *arr, size_t index, int value) {
    if (index >= arr->size) return -1;
    arr->data[index] = value;
    return 0;
}

// 配列を解放
void array_destroy(DynamicArray *arr) {
    if (arr) {
        free(arr->data);
        free(arr);
    }
}

// 配列を表示
void array_print(DynamicArray *arr) {
    printf("[");
    for (size_t i = 0; i < arr->size; i++) {
        printf("%d", arr->data[i]);
        if (i < arr->size - 1) printf(", ");
    }
    printf("] (size=%zu, capacity=%zu)\n", arr->size, arr->capacity);
}

int main() {
    printf("=== 動的配列のデモ ===\n\n");
    
    DynamicArray *arr = array_create(4);
    
    // 要素を追加
    for (int i = 1; i <= 10; i++) {
        array_push(arr, i * 10);
        printf("Push %d: ", i * 10);
        array_print(arr);
    }
    
    // 要素を取り出す
    int value;
    printf("\n");
    while (array_pop(arr, &value) == 0) {
        printf("Pop %d: ", value);
        array_print(arr);
    }
    
    array_destroy(arr);
    return 0;
}
