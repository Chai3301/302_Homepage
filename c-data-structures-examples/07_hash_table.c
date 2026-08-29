/**
 * 7. ハッシュテーブル (Hash Table)
 * 
 * 学習ポイント:
 * - ハッシュ関数の実装
 * - 衝突解決 (チェイン法)
 * - キーと値のペア管理
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 16

// キーと値のペアを格納するノード
typedef struct HashNode {
    char *key;
    int value;
    struct HashNode *next;
} HashNode;

// ハッシュテーブル
typedef struct {
    HashNode *buckets[TABLE_SIZE];
    size_t size;
} HashTable;

// ハッシュ関数 (DJB2アルゴリズム)
static unsigned int hash(const char *key) {
    unsigned int hash_value = 5381;
    int c;
    
    while ((c = *key++)) {
        hash_value = ((hash_value << 5) + hash_value) + c;
    }
    
    return hash_value % TABLE_SIZE;
}

// ハッシュテーブルの作成
HashTable* hashtable_create() {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    table->size = 0;
    return table;
}

// 新しいノードを作成
static HashNode* create_hash_node(const char *key, int value) {
    HashNode *node = (HashNode*)malloc(sizeof(HashNode));
    if (!node) return NULL;
    
    node->key = strdup(key);
    if (!node->key) {
        free(node);
        return NULL;
    }
    
    node->value = value;
    node->next = NULL;
    return node;
}

// 挿入または更新
int hashtable_put(HashTable *table, const char *key, int value) {
    unsigned int index = hash(key);
    HashNode *current = table->buckets[index];
    
    // 既存のキーを探す
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;  // 更新
            return 0;
        }
        current = current->next;
    }
    
    // 新しいノードを先頭に追加
    HashNode *node = create_hash_node(key, value);
    if (!node) return -1;
    
    node->next = table->buckets[index];
    table->buckets[index] = node;
    table->size++;
    return 0;
}

// 値を取得
bool hashtable_get(HashTable *table, const char *key, int *out_value) {
    unsigned int index = hash(key);
    HashNode *current = table->buckets[index];
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            *out_value = current->value;
            return true;
        }
        current = current->next;
    }
    return false;
}

// キーが存在するか確認
bool hashtable_contains(HashTable *table, const char *key) {
    int dummy;
    return hashtable_get(table, key, &dummy);
}

// 削除
bool hashtable_remove(HashTable *table, const char *key) {
    unsigned int index = hash(key);
    HashNode *current = table->buckets[index];
    HashNode *prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                table->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current);
            table->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

// ハッシュテーブルを解放
void hashtable_destroy(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *current = table->buckets[i];
        while (current != NULL) {
            HashNode *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table);
}

// ハッシュテーブルの内容を表示
void hashtable_print(HashTable *table) {
    printf("HashTable (size=%zu):\n", table->size);
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("[%2d]: ", i);
        HashNode *current = table->buckets[i];
        if (current == NULL) {
            printf("(empty)");
        }
        while (current != NULL) {
            printf("(%s: %d)", current->key, current->value);
            if (current->next != NULL) printf(" -> ");
            current = current->next;
        }
        printf("\n");
    }
}

// ハッシュテーブルの統計情報
void hashtable_stats(HashTable *table) {
    int non_empty = 0;
    int max_chain = 0;
    int total_nodes = 0;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        int chain_length = 0;
        HashNode *current = table->buckets[i];
        
        if (current != NULL) non_empty++;
        
        while (current != NULL) {
            chain_length++;
            current = current->next;
        }
        
        total_nodes += chain_length;
        if (chain_length > max_chain) max_chain = chain_length;
    }
    
    printf("\n--- 統計情報 ---\n");
    printf("テーブルサイズ: %d\n", TABLE_SIZE);
    printf("要素数: %d\n", total_nodes);
    printf("使用中のバケット: %d\n", non_empty);
    printf("最長チェイン: %d\n", max_chain);
    printf("負荷係数: %.2f\n", (float)total_nodes / TABLE_SIZE);
}

int main() {
    printf("=== ハッシュテーブルのデモ ===\n\n");
    
    HashTable *table = hashtable_create();
    
    // データを挿入
    printf("--- データを挿入 ---\n");
    hashtable_put(table, "apple", 100);
    hashtable_put(table, "banana", 200);
    hashtable_put(table, "cherry", 300);
    hashtable_put(table, "date", 400);
    hashtable_put(table, "elderberry", 500);
    hashtable_put(table, "fig", 600);
    hashtable_put(table, "grape", 700);
    hashtable_put(table, "honeydew", 800);
    
    hashtable_print(table);
    
    // 検索
    printf("\n--- 検索 ---\n");
    int value;
    const char *keys[] = {"apple", "cherry", "watermelon"};
    for (int i = 0; i < 3; i++) {
        if (hashtable_get(table, keys[i], &value)) {
            printf("\"%s\" = %d\n", keys[i], value);
        } else {
            printf("\"%s\" は見つかりません\n", keys[i]);
        }
    }
    
    // 更新
    printf("\n--- 更新 ---\n");
    printf("\"apple\" の値を 150 に更新\n");
    hashtable_put(table, "apple", 150);
    hashtable_get(table, "apple", &value);
    printf("\"apple\" = %d\n", value);
    
    // 削除
    printf("\n--- 削除 ---\n");
    printf("\"banana\" を削除\n");
    hashtable_remove(table, "banana");
    printf("\"banana\" exists: %s\n", 
           hashtable_contains(table, "banana") ? "true" : "false");
    
    hashtable_stats(table);
    
    printf("\n--- 最終状態 ---\n");
    hashtable_print(table);
    
    hashtable_destroy(table);
    return 0;
}
