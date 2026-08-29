/**
 * 10. 循環バッファ (Circular Buffer / Ring Buffer)
 * 
 * 学習ポイント:
 * - 固定サイズのバッファ管理
 * - 読み書きポインタの循環
 * - オーバーフロー処理の選択
 * - 実用例: ログバッファ、ストリーミングデータ
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int *data;
    size_t head;        // 次に読む位置
    size_t tail;        // 次に書く位置
    size_t capacity;
    size_t count;
    bool overwrite;     // 満杯時に上書きするか
} CircularBuffer;

// バッファの作成
CircularBuffer* cbuf_create(size_t capacity, bool overwrite) {
    CircularBuffer *buf = (CircularBuffer*)malloc(sizeof(CircularBuffer));
    if (!buf) return NULL;
    
    buf->data = (int*)malloc(sizeof(int) * capacity);
    if (!buf->data) {
        free(buf);
        return NULL;
    }
    
    buf->head = 0;
    buf->tail = 0;
    buf->capacity = capacity;
    buf->count = 0;
    buf->overwrite = overwrite;
    return buf;
}

bool cbuf_is_empty(CircularBuffer *buf) {
    return buf->count == 0;
}

bool cbuf_is_full(CircularBuffer *buf) {
    return buf->count >= buf->capacity;
}

size_t cbuf_size(CircularBuffer *buf) {
    return buf->count;
}

// データを書き込む
int cbuf_write(CircularBuffer *buf, int value) {
    if (cbuf_is_full(buf)) {
        if (buf->overwrite) {
            // 上書きモード: 最古のデータを破棄
            buf->head = (buf->head + 1) % buf->capacity;
            buf->count--;
        } else {
            // 上書きしないモード: エラーを返す
            return -1;
        }
    }
    
    buf->data[buf->tail] = value;
    buf->tail = (buf->tail + 1) % buf->capacity;
    buf->count++;
    return 0;
}

// データを読み込む
int cbuf_read(CircularBuffer *buf, int *out_value) {
    if (cbuf_is_empty(buf)) return -1;
    
    *out_value = buf->data[buf->head];
    buf->head = (buf->head + 1) % buf->capacity;
    buf->count--;
    return 0;
}

// データを読み込まずに確認する
int cbuf_peek(CircularBuffer *buf, int *out_value) {
    if (cbuf_is_empty(buf)) return -1;
    *out_value = buf->data[buf->head];
    return 0;
}

// バッファをクリア
void cbuf_clear(CircularBuffer *buf) {
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;
}

// バッファを解放
void cbuf_destroy(CircularBuffer *buf) {
    if (buf) {
        free(buf->data);
        free(buf);
    }
}

// バッファの状態を表示
void cbuf_print(CircularBuffer *buf) {
    printf("CircularBuffer (size=%zu/%zu, head=%zu, tail=%zu, overwrite=%s)\n",
           buf->count, buf->capacity, buf->head, buf->tail,
           buf->overwrite ? "yes" : "no");
    
    printf("  Data: [");
    for (size_t i = 0; i < buf->capacity; i++) {
        if (i > 0) printf(", ");
        
        // この位置にデータがあるかチェック
        bool has_data = false;
        if (buf->count > 0) {
            if (buf->head <= buf->tail) {
                has_data = (i >= buf->head && i < buf->tail);
            } else {
                has_data = (i >= buf->head || i < buf->tail);
            }
        }
        
        if (has_data) {
            printf("%3d", buf->data[i]);
        } else {
            printf("  _");
        }
    }
    printf("]\n");
    
    // head/tail のマーカー表示
    printf("        ");
    for (size_t i = 0; i < buf->capacity; i++) {
        if (i == buf->head && i == buf->tail) {
            printf(" H/T");
        } else if (i == buf->head) {
            printf("  H ");
        } else if (i == buf->tail) {
            printf("  T ");
        } else {
            printf("    ");
        }
        if (i < buf->capacity - 1) printf(" ");
    }
    printf("\n");
}

// 全データを順番に読み出す（破壊的）
void cbuf_drain(CircularBuffer *buf) {
    printf("Reading all data: ");
    int value;
    while (cbuf_read(buf, &value) == 0) {
        printf("%d ", value);
    }
    printf("\n");
}

int main() {
    printf("=== 循環バッファのデモ ===\n\n");
    
    // 上書きしないモード
    printf("--- 上書きしないモード ---\n");
    CircularBuffer *buf1 = cbuf_create(5, false);
    
    printf("5つの値を書き込み:\n");
    for (int i = 1; i <= 5; i++) {
        int result = cbuf_write(buf1, i * 10);
        printf("Write %d: %s\n", i * 10, result == 0 ? "成功" : "失敗");
    }
    cbuf_print(buf1);
    
    printf("\n満杯状態で書き込み試行:\n");
    printf("Write 60: %s\n", cbuf_write(buf1, 60) == 0 ? "成功" : "失敗（満杯）");
    
    printf("\n2つ読み込み:\n");
    int value;
    cbuf_read(buf1, &value);
    printf("Read: %d\n", value);
    cbuf_read(buf1, &value);
    printf("Read: %d\n", value);
    cbuf_print(buf1);
    
    printf("\n新しい値を書き込み:\n");
    cbuf_write(buf1, 60);
    cbuf_write(buf1, 70);
    cbuf_print(buf1);
    
    cbuf_drain(buf1);
    cbuf_print(buf1);
    
    cbuf_destroy(buf1);
    
    // 上書きモード
    printf("\n--- 上書きモード ---\n");
    CircularBuffer *buf2 = cbuf_create(5, true);
    
    printf("7つの値を書き込み (容量は5):\n");
    for (int i = 1; i <= 7; i++) {
        cbuf_write(buf2, i * 10);
        printf("Write %d:\n", i * 10);
        cbuf_print(buf2);
        printf("\n");
    }
    
    printf("バッファの内容 (最新の5つ):\n");
    cbuf_drain(buf2);
    
    cbuf_destroy(buf2);
    
    // 実用例: ログバッファ
    printf("\n--- 実用例: ログバッファ ---\n");
    printf("最新の3件のログを保持するバッファ:\n\n");
    CircularBuffer *log_buf = cbuf_create(3, true);
    
    const char *logs[] = {
        "システム起動",
        "ユーザーログイン",
        "ファイルアクセス",
        "データ更新",
        "ユーザーログアウト"
    };
    
    for (int i = 0; i < 5; i++) {
        cbuf_write(log_buf, i);
        printf("ログ追加: %s\n", logs[i]);
        printf("現在保持しているログ: ");
        
        // 現在のバッファ内容を非破壊的に表示
        size_t temp_head = log_buf->head;
        size_t temp_count = log_buf->count;
        for (size_t j = 0; j < temp_count; j++) {
            size_t idx = (temp_head + j) % log_buf->capacity;
            printf("[%s] ", logs[log_buf->data[idx]]);
        }
        printf("\n\n");
    }
    
    cbuf_destroy(log_buf);
    
    return 0;
}
