# C言語データ構造学習用 Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

SOURCES = $(wildcard *.c)
PROGRAMS = $(SOURCES:.c=)

.PHONY: all clean run

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMS)

# 個別実行用ターゲット
run-01: 01_dynamic_array
	./01_dynamic_array

run-02: 02_singly_linked_list
	./02_singly_linked_list

run-03: 03_doubly_linked_list
	./03_doubly_linked_list

run-04: 04_stack
	./04_stack

run-05: 05_queue
	./05_queue

run-06: 06_binary_search_tree
	./06_binary_search_tree

run-07: 07_hash_table
	./07_hash_table

run-08: 08_heap
	./08_heap

run-09: 09_graph
	./09_graph

run-10: 10_circular_buffer
	./10_circular_buffer

# 全てのプログラムを実行
run-all: all
	@echo "=========================================="
	@echo "01: 動的配列"
	@echo "=========================================="
	@./01_dynamic_array
	@echo ""
	@echo "=========================================="
	@echo "02: 単方向連結リスト"
	@echo "=========================================="
	@./02_singly_linked_list
	@echo ""
	@echo "=========================================="
	@echo "03: 双方向連結リスト"
	@echo "=========================================="
	@./03_doubly_linked_list
	@echo ""
	@echo "=========================================="
	@echo "04: スタック"
	@echo "=========================================="
	@./04_stack
	@echo ""
	@echo "=========================================="
	@echo "05: キュー"
	@echo "=========================================="
	@./05_queue
	@echo ""
	@echo "=========================================="
	@echo "06: 二分探索木"
	@echo "=========================================="
	@./06_binary_search_tree
	@echo ""
	@echo "=========================================="
	@echo "07: ハッシュテーブル"
	@echo "=========================================="
	@./07_hash_table
	@echo ""
	@echo "=========================================="
	@echo "08: ヒープ"
	@echo "=========================================="
	@./08_heap
	@echo ""
	@echo "=========================================="
	@echo "09: グラフ"
	@echo "=========================================="
	@./09_graph
	@echo ""
	@echo "=========================================="
	@echo "10: 循環バッファ"
	@echo "=========================================="
	@./10_circular_buffer
