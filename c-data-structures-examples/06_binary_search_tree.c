/**
 * 6. 二分探索木 (Binary Search Tree)
 * 
 * 学習ポイント:
 * - 再帰的なデータ構造
 * - 木の走査 (前順、中順、後順)
 * - 探索、挿入、削除のアルゴリズム
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    TreeNode *root;
    size_t size;
} BST;

// 木の初期化
BST* bst_create() {
    BST *tree = (BST*)malloc(sizeof(BST));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

// 新しいノードを作成
static TreeNode* create_tree_node(int data) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 再帰的に挿入
static TreeNode* insert_recursive(TreeNode *node, int data) {
    if (node == NULL) {
        return create_tree_node(data);
    }
    
    if (data < node->data) {
        node->left = insert_recursive(node->left, data);
    } else if (data > node->data) {
        node->right = insert_recursive(node->right, data);
    }
    // data == node->data の場合は何もしない（重複を許可しない）
    
    return node;
}

// 挿入
int bst_insert(BST *tree, int data) {
    tree->root = insert_recursive(tree->root, data);
    tree->size++;
    return 0;
}

// 再帰的に検索
static TreeNode* search_recursive(TreeNode *node, int data) {
    if (node == NULL || node->data == data) {
        return node;
    }
    
    if (data < node->data) {
        return search_recursive(node->left, data);
    }
    return search_recursive(node->right, data);
}

// 検索
bool bst_search(BST *tree, int data) {
    return search_recursive(tree->root, data) != NULL;
}

// 最小値を持つノードを見つける
static TreeNode* find_min(TreeNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// 再帰的に削除
static TreeNode* delete_recursive(TreeNode *node, int data) {
    if (node == NULL) return NULL;
    
    if (data < node->data) {
        node->left = delete_recursive(node->left, data);
    } else if (data > node->data) {
        node->right = delete_recursive(node->right, data);
    } else {
        // 削除するノードが見つかった
        
        // ケース1: 子がないまたは1つだけ
        if (node->left == NULL) {
            TreeNode *temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            TreeNode *temp = node->left;
            free(node);
            return temp;
        }
        
        // ケース2: 子が2つある場合
        // 右部分木の最小値（中順後継者）を見つける
        TreeNode *successor = find_min(node->right);
        node->data = successor->data;
        node->right = delete_recursive(node->right, successor->data);
    }
    return node;
}

// 削除
int bst_delete(BST *tree, int data) {
    if (!bst_search(tree, data)) return -1;
    tree->root = delete_recursive(tree->root, data);
    tree->size--;
    return 0;
}

// 中順走査 (In-order): 左 -> 根 -> 右 (昇順に表示)
void inorder_traversal(TreeNode *node) {
    if (node != NULL) {
        inorder_traversal(node->left);
        printf("%d ", node->data);
        inorder_traversal(node->right);
    }
}

// 前順走査 (Pre-order): 根 -> 左 -> 右
void preorder_traversal(TreeNode *node) {
    if (node != NULL) {
        printf("%d ", node->data);
        preorder_traversal(node->left);
        preorder_traversal(node->right);
    }
}

// 後順走査 (Post-order): 左 -> 右 -> 根
void postorder_traversal(TreeNode *node) {
    if (node != NULL) {
        postorder_traversal(node->left);
        postorder_traversal(node->right);
        printf("%d ", node->data);
    }
}

// 木の高さを計算
int tree_height(TreeNode *node) {
    if (node == NULL) return -1;
    
    int left_height = tree_height(node->left);
    int right_height = tree_height(node->right);
    
    return (left_height > right_height ? left_height : right_height) + 1;
}

// 木を解放
static void free_tree(TreeNode *node) {
    if (node != NULL) {
        free_tree(node->left);
        free_tree(node->right);
        free(node);
    }
}

void bst_destroy(BST *tree) {
    free_tree(tree->root);
    free(tree);
}

// 木を視覚的に表示 (簡易版)
void print_tree_helper(TreeNode *node, int level, char prefix) {
    if (node == NULL) return;
    
    for (int i = 0; i < level; i++) printf("    ");
    printf("%c-- %d\n", prefix, node->data);
    
    print_tree_helper(node->left, level + 1, 'L');
    print_tree_helper(node->right, level + 1, 'R');
}

void bst_print(BST *tree) {
    printf("Tree (size=%zu, height=%d):\n", tree->size, tree_height(tree->root));
    print_tree_helper(tree->root, 0, 'R');
}

int main() {
    printf("=== 二分探索木のデモ ===\n\n");
    
    BST *tree = bst_create();
    
    // 要素を挿入
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25};
    size_t n = sizeof(values) / sizeof(values[0]);
    
    printf("挿入する値: ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", values[i]);
        bst_insert(tree, values[i]);
    }
    printf("\n\n");
    
    bst_print(tree);
    
    // 走査
    printf("\n中順走査 (昇順): ");
    inorder_traversal(tree->root);
    printf("\n");
    
    printf("前順走査: ");
    preorder_traversal(tree->root);
    printf("\n");
    
    printf("後順走査: ");
    postorder_traversal(tree->root);
    printf("\n");
    
    // 検索
    printf("\n--- 検索 ---\n");
    int search_values[] = {40, 25, 100};
    for (size_t i = 0; i < 3; i++) {
        printf("%d を検索: %s\n", search_values[i], 
               bst_search(tree, search_values[i]) ? "見つかりました" : "見つかりません");
    }
    
    // 削除
    printf("\n--- 削除 ---\n");
    printf("30を削除 (子が2つあるノード):\n");
    bst_delete(tree, 30);
    bst_print(tree);
    
    printf("\n70を削除:\n");
    bst_delete(tree, 70);
    bst_print(tree);
    
    printf("\n中順走査: ");
    inorder_traversal(tree->root);
    printf("\n");
    
    bst_destroy(tree);
    return 0;
}
