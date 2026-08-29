/**
 * 8. ヒープ / 優先度キュー (Heap / Priority Queue)
 * 
 * 学習ポイント:
 * - 完全二分木の配列表現
 * - ヒープの性質の維持 (heapify)
 * - ヒープソートの実装
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
    bool is_min_heap;  // true: 最小ヒープ, false: 最大ヒープ
} Heap;

// ヒープの作成
Heap* heap_create(size_t capacity, bool is_min_heap) {
    Heap *heap = (Heap*)malloc(sizeof(Heap));
    if (!heap) return NULL;
    
    heap->data = (int*)malloc(sizeof(int) * capacity);
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    
    heap->size = 0;
    heap->capacity = capacity;
    heap->is_min_heap = is_min_heap;
    return heap;
}

// 親のインデックス
static size_t parent(size_t i) {
    return (i - 1) / 2;
}

// 左の子のインデックス
static size_t left_child(size_t i) {
    return 2 * i + 1;
}

// 右の子のインデックス
static size_t right_child(size_t i) {
    return 2 * i + 2;
}

// 要素を交換
static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 比較関数
static bool should_swap(Heap *heap, int a, int b) {
    if (heap->is_min_heap) {
        return a < b;  // 最小ヒープ: 親より小さければ交換
    } else {
        return a > b;  // 最大ヒープ: 親より大きければ交換
    }
}

// 上方向にヒープを修正 (挿入時)
static void heapify_up(Heap *heap, size_t index) {
    while (index > 0 && should_swap(heap, heap->data[index], heap->data[parent(index)])) {
        swap(&heap->data[index], &heap->data[parent(index)]);
        index = parent(index);
    }
}

// 下方向にヒープを修正 (削除時)
static void heapify_down(Heap *heap, size_t index) {
    size_t target = index;
    size_t left = left_child(index);
    size_t right = right_child(index);
    
    if (left < heap->size && should_swap(heap, heap->data[left], heap->data[target])) {
        target = left;
    }
    
    if (right < heap->size && should_swap(heap, heap->data[right], heap->data[target])) {
        target = right;
    }
    
    if (target != index) {
        swap(&heap->data[index], &heap->data[target]);
        heapify_down(heap, target);
    }
}

// 挿入
int heap_insert(Heap *heap, int value) {
    if (heap->size >= heap->capacity) return -1;
    
    heap->data[heap->size] = value;
    heapify_up(heap, heap->size);
    heap->size++;
    return 0;
}

// 先頭（最小/最大）を取得
int heap_peek(Heap *heap, int *out_value) {
    if (heap->size == 0) return -1;
    *out_value = heap->data[0];
    return 0;
}

// 先頭（最小/最大）を取り出す
int heap_extract(Heap *heap, int *out_value) {
    if (heap->size == 0) return -1;
    
    *out_value = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapify_down(heap, 0);
    return 0;
}

bool heap_is_empty(Heap *heap) {
    return heap->size == 0;
}

void heap_destroy(Heap *heap) {
    if (heap) {
        free(heap->data);
        free(heap);
    }
}

// ヒープを表示
void heap_print(Heap *heap) {
    printf("Heap (%s, size=%zu): ", 
           heap->is_min_heap ? "min" : "max", heap->size);
    for (size_t i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
}

// ヒープを木形式で表示
void heap_print_tree(Heap *heap) {
    if (heap->size == 0) {
        printf("(empty heap)\n");
        return;
    }
    
    printf("Heap Tree:\n");
    size_t level = 0;
    size_t level_size = 1;
    size_t printed = 0;
    
    while (printed < heap->size) {
        // インデント
        int indent = (1 << (3 - level)) - 1;
        for (int i = 0; i < indent; i++) printf("  ");
        
        // このレベルの要素を出力
        for (size_t i = 0; i < level_size && printed < heap->size; i++) {
            printf("%3d", heap->data[printed++]);
            // 要素間のスペース
            int spacing = (1 << (4 - level)) - 1;
            for (int j = 0; j < spacing; j++) printf("  ");
        }
        printf("\n");
        
        level++;
        level_size *= 2;
    }
}

// ヒープソート
void heap_sort(int arr[], size_t n) {
    // 最大ヒープを使用してソート
    Heap *heap = heap_create(n, false);  // 最大ヒープ
    
    // 全要素を挿入
    for (size_t i = 0; i < n; i++) {
        heap_insert(heap, arr[i]);
    }
    
    // 最大値から順に取り出して配列に格納（逆順）
    for (size_t i = n; i > 0; i--) {
        heap_extract(heap, &arr[i - 1]);
    }
    
    heap_destroy(heap);
}

int main() {
    printf("=== ヒープ / 優先度キューのデモ ===\n\n");
    
    // 最小ヒープ
    printf("--- 最小ヒープ ---\n");
    Heap *min_heap = heap_create(15, true);
    
    int values[] = {35, 33, 42, 10, 14, 19, 27, 44, 26, 31};
    size_t n = sizeof(values) / sizeof(values[0]);
    
    printf("挿入する値: ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", values[i]);
    }
    printf("\n\n");
    
    for (size_t i = 0; i < n; i++) {
        heap_insert(min_heap, values[i]);
    }
    
    heap_print_tree(min_heap);
    heap_print(min_heap);
    
    printf("\n優先度順に取り出し:\n");
    int value;
    while (heap_extract(min_heap, &value) == 0) {
        printf("%d ", value);
    }
    printf("\n");
    
    heap_destroy(min_heap);
    
    // 最大ヒープ
    printf("\n--- 最大ヒープ ---\n");
    Heap *max_heap = heap_create(15, false);
    
    for (size_t i = 0; i < n; i++) {
        heap_insert(max_heap, values[i]);
    }
    
    heap_print_tree(max_heap);
    heap_print(max_heap);
    
    printf("\n優先度順に取り出し:\n");
    while (heap_extract(max_heap, &value) == 0) {
        printf("%d ", value);
    }
    printf("\n");
    
    heap_destroy(max_heap);
    
    // ヒープソート
    printf("\n--- ヒープソート ---\n");
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    
    printf("ソート前: ");
    for (size_t i = 0; i < arr_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    heap_sort(arr, arr_size);
    
    printf("ソート後: ");
    for (size_t i = 0; i < arr_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}
