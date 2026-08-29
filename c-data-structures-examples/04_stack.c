/**
 * 4. スタック (Stack) - LIFO (Last In, First Out)
 * 
 * 学習ポイント:
 * - スタックの基本操作 (push, pop, peek)
 * - 配列ベースと連結リストベースの実装比較
 * - 実用例: 括弧の対応チェック
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// === 配列ベースのスタック ===
typedef struct {
    int *data;
    size_t top;
    size_t capacity;
} ArrayStack;

ArrayStack* astack_create(size_t capacity) {
    ArrayStack *stack = (ArrayStack*)malloc(sizeof(ArrayStack));
    if (!stack) return NULL;
    
    stack->data = (int*)malloc(sizeof(int) * capacity);
    if (!stack->data) {
        free(stack);
        return NULL;
    }
    
    stack->top = 0;
    stack->capacity = capacity;
    return stack;
}

bool astack_is_empty(ArrayStack *stack) {
    return stack->top == 0;
}

bool astack_is_full(ArrayStack *stack) {
    return stack->top >= stack->capacity;
}

int astack_push(ArrayStack *stack, int value) {
    if (astack_is_full(stack)) return -1;
    stack->data[stack->top++] = value;
    return 0;
}

int astack_pop(ArrayStack *stack, int *out_value) {
    if (astack_is_empty(stack)) return -1;
    *out_value = stack->data[--stack->top];
    return 0;
}

int astack_peek(ArrayStack *stack, int *out_value) {
    if (astack_is_empty(stack)) return -1;
    *out_value = stack->data[stack->top - 1];
    return 0;
}

void astack_destroy(ArrayStack *stack) {
    if (stack) {
        free(stack->data);
        free(stack);
    }
}

// === 連結リストベースのスタック ===
typedef struct StackNode {
    int data;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    size_t size;
} LinkedStack;

LinkedStack* lstack_create() {
    LinkedStack *stack = (LinkedStack*)malloc(sizeof(LinkedStack));
    if (!stack) return NULL;
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

bool lstack_is_empty(LinkedStack *stack) {
    return stack->top == NULL;
}

int lstack_push(LinkedStack *stack, int value) {
    StackNode *node = (StackNode*)malloc(sizeof(StackNode));
    if (!node) return -1;
    
    node->data = value;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
    return 0;
}

int lstack_pop(LinkedStack *stack, int *out_value) {
    if (lstack_is_empty(stack)) return -1;
    
    StackNode *temp = stack->top;
    *out_value = temp->data;
    stack->top = temp->next;
    free(temp);
    stack->size--;
    return 0;
}

int lstack_peek(LinkedStack *stack, int *out_value) {
    if (lstack_is_empty(stack)) return -1;
    *out_value = stack->top->data;
    return 0;
}

void lstack_destroy(LinkedStack *stack) {
    StackNode *current = stack->top;
    while (current != NULL) {
        StackNode *temp = current;
        current = current->next;
        free(temp);
    }
    free(stack);
}

// === 実用例: 括弧の対応チェック ===
bool check_brackets(const char *str) {
    LinkedStack *stack = lstack_create();
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        
        if (c == '(' || c == '[' || c == '{') {
            lstack_push(stack, c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (lstack_is_empty(stack)) {
                lstack_destroy(stack);
                return false;
            }
            
            int top;
            lstack_pop(stack, &top);
            
            bool match = (c == ')' && top == '(') ||
                        (c == ']' && top == '[') ||
                        (c == '}' && top == '{');
            
            if (!match) {
                lstack_destroy(stack);
                return false;
            }
        }
    }
    
    bool result = lstack_is_empty(stack);
    lstack_destroy(stack);
    return result;
}

void print_stack(ArrayStack *stack) {
    printf("Stack [bottom -> top]: ");
    for (size_t i = 0; i < stack->top; i++) {
        printf("%d ", stack->data[i]);
    }
    printf("\n");
}

int main() {
    printf("=== スタックのデモ ===\n\n");
    
    // 配列ベースのスタック
    printf("--- 配列ベースのスタック ---\n");
    ArrayStack *astack = astack_create(10);
    
    for (int i = 1; i <= 5; i++) {
        astack_push(astack, i * 10);
        printf("Push %d: ", i * 10);
        print_stack(astack);
    }
    
    int value;
    printf("\n");
    while (astack_pop(astack, &value) == 0) {
        printf("Pop %d: ", value);
        print_stack(astack);
    }
    
    astack_destroy(astack);
    
    // 括弧チェックの実用例
    printf("\n--- 括弧の対応チェック ---\n");
    const char *tests[] = {
        "()",
        "([{}])",
        "{[()]}",
        "((()))",
        "([)]",
        "(((",
        "())",
        "{[}]"
    };
    
    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        printf("\"%s\" -> %s\n", tests[i], 
               check_brackets(tests[i]) ? "有効" : "無効");
    }
    
    return 0;
}
