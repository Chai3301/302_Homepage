/**
 * 9. グラフ (Graph)
 * 
 * 学習ポイント:
 * - 隣接リストによるグラフ表現
 * - 深さ優先探索 (DFS)
 * - 幅優先探索 (BFS)
 * - 最短経路の探索
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

// 隣接リストのノード
typedef struct AdjNode {
    int vertex;
    int weight;
    struct AdjNode *next;
} AdjNode;

// グラフ
typedef struct {
    AdjNode *adj_list[MAX_VERTICES];
    int num_vertices;
    bool is_directed;
} Graph;

// キュー（BFS用）
typedef struct {
    int data[MAX_VERTICES];
    int front;
    int rear;
} Queue;

Queue* queue_create() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = 0;
    return q;
}

void queue_enqueue(Queue *q, int value) {
    q->data[q->rear++] = value;
}

int queue_dequeue(Queue *q) {
    return q->data[q->front++];
}

bool queue_is_empty(Queue *q) {
    return q->front == q->rear;
}

// グラフの作成
Graph* graph_create(int num_vertices, bool is_directed) {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;
    
    graph->num_vertices = num_vertices;
    graph->is_directed = is_directed;
    
    for (int i = 0; i < num_vertices; i++) {
        graph->adj_list[i] = NULL;
    }
    
    return graph;
}

// 隣接リストにノードを追加
static AdjNode* create_adj_node(int vertex, int weight) {
    AdjNode *node = (AdjNode*)malloc(sizeof(AdjNode));
    if (!node) return NULL;
    node->vertex = vertex;
    node->weight = weight;
    node->next = NULL;
    return node;
}

// エッジを追加
int graph_add_edge(Graph *graph, int src, int dest, int weight) {
    // src -> dest のエッジ
    AdjNode *node = create_adj_node(dest, weight);
    if (!node) return -1;
    node->next = graph->adj_list[src];
    graph->adj_list[src] = node;
    
    // 無向グラフの場合、逆方向も追加
    if (!graph->is_directed) {
        node = create_adj_node(src, weight);
        if (!node) return -1;
        node->next = graph->adj_list[dest];
        graph->adj_list[dest] = node;
    }
    
    return 0;
}

// DFS（再帰版）
static void dfs_recursive(Graph *graph, int vertex, bool visited[]) {
    visited[vertex] = true;
    printf("%d ", vertex);
    
    AdjNode *current = graph->adj_list[vertex];
    while (current != NULL) {
        if (!visited[current->vertex]) {
            dfs_recursive(graph, current->vertex, visited);
        }
        current = current->next;
    }
}

void graph_dfs(Graph *graph, int start_vertex) {
    bool visited[MAX_VERTICES] = {false};
    printf("DFS from %d: ", start_vertex);
    dfs_recursive(graph, start_vertex, visited);
    printf("\n");
}

// BFS
void graph_bfs(Graph *graph, int start_vertex) {
    bool visited[MAX_VERTICES] = {false};
    Queue *queue = queue_create();
    
    printf("BFS from %d: ", start_vertex);
    
    visited[start_vertex] = true;
    queue_enqueue(queue, start_vertex);
    
    while (!queue_is_empty(queue)) {
        int vertex = queue_dequeue(queue);
        printf("%d ", vertex);
        
        AdjNode *current = graph->adj_list[vertex];
        while (current != NULL) {
            if (!visited[current->vertex]) {
                visited[current->vertex] = true;
                queue_enqueue(queue, current->vertex);
            }
            current = current->next;
        }
    }
    printf("\n");
    
    free(queue);
}

// 最短経路（BFS、重みなしグラフ用）
void graph_shortest_path(Graph *graph, int start, int end) {
    int distance[MAX_VERTICES];
    int parent[MAX_VERTICES];
    bool visited[MAX_VERTICES] = {false};
    
    for (int i = 0; i < graph->num_vertices; i++) {
        distance[i] = -1;
        parent[i] = -1;
    }
    
    Queue *queue = queue_create();
    
    visited[start] = true;
    distance[start] = 0;
    queue_enqueue(queue, start);
    
    while (!queue_is_empty(queue)) {
        int vertex = queue_dequeue(queue);
        
        AdjNode *current = graph->adj_list[vertex];
        while (current != NULL) {
            if (!visited[current->vertex]) {
                visited[current->vertex] = true;
                distance[current->vertex] = distance[vertex] + 1;
                parent[current->vertex] = vertex;
                queue_enqueue(queue, current->vertex);
                
                if (current->vertex == end) {
                    break;
                }
            }
            current = current->next;
        }
    }
    
    free(queue);
    
    // 結果を表示
    if (distance[end] == -1) {
        printf("%d から %d への経路はありません\n", start, end);
    } else {
        printf("%d から %d への最短距離: %d\n", start, end, distance[end]);
        
        // 経路を復元
        int path[MAX_VERTICES];
        int path_len = 0;
        int current = end;
        
        while (current != -1) {
            path[path_len++] = current;
            current = parent[current];
        }
        
        printf("経路: ");
        for (int i = path_len - 1; i >= 0; i--) {
            printf("%d", path[i]);
            if (i > 0) printf(" -> ");
        }
        printf("\n");
    }
}

// グラフを解放
void graph_destroy(Graph *graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        AdjNode *current = graph->adj_list[i];
        while (current != NULL) {
            AdjNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}

// グラフを表示
void graph_print(Graph *graph) {
    printf("Graph (vertices=%d, %s):\n", 
           graph->num_vertices,
           graph->is_directed ? "directed" : "undirected");
    
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("[%d]: ", i);
        AdjNode *current = graph->adj_list[i];
        while (current != NULL) {
            printf("-> %d(w:%d) ", current->vertex, current->weight);
            current = current->next;
        }
        printf("\n");
    }
}

int main() {
    printf("=== グラフのデモ ===\n\n");
    
    // 無向グラフの作成
    /*
     *     0
     *    /|\
     *   1-2-3
     *   |   |
     *   4---5
     */
    printf("--- 無向グラフ ---\n");
    Graph *graph = graph_create(6, false);
    
    graph_add_edge(graph, 0, 1, 1);
    graph_add_edge(graph, 0, 2, 1);
    graph_add_edge(graph, 0, 3, 1);
    graph_add_edge(graph, 1, 2, 1);
    graph_add_edge(graph, 1, 4, 1);
    graph_add_edge(graph, 2, 3, 1);
    graph_add_edge(graph, 3, 5, 1);
    graph_add_edge(graph, 4, 5, 1);
    
    graph_print(graph);
    printf("\n");
    
    // 探索
    graph_dfs(graph, 0);
    graph_bfs(graph, 0);
    
    // 最短経路
    printf("\n");
    graph_shortest_path(graph, 0, 5);
    graph_shortest_path(graph, 1, 3);
    
    graph_destroy(graph);
    
    // 有向グラフの作成
    printf("\n--- 有向グラフ ---\n");
    Graph *digraph = graph_create(5, true);
    
    graph_add_edge(digraph, 0, 1, 1);
    graph_add_edge(digraph, 0, 2, 1);
    graph_add_edge(digraph, 1, 2, 1);
    graph_add_edge(digraph, 2, 3, 1);
    graph_add_edge(digraph, 3, 4, 1);
    graph_add_edge(digraph, 4, 0, 1);
    
    graph_print(digraph);
    printf("\n");
    
    graph_dfs(digraph, 0);
    graph_bfs(digraph, 0);
    
    graph_destroy(digraph);
    
    return 0;
}
