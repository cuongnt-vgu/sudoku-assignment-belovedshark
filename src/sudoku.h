#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define BOARD_SIZE 9



struct Cell_impl
{
    int row_index;
    int col_index;
    int box_index;
    int board_index;
    int num_candidates;
    int candidates[BOARD_SIZE];
    int value; // solved or 0
    bool fixed;
};

typedef struct Cell_impl Cell;

#define PAIR(T1, T2) \
    struct {          \
        T1 first;      \
        T2 second;     \
    }

typedef PAIR(int, int) IntPair;
typedef PAIR(Cell*, Cell*) CellPair;

#define TRIPLE(T1, T2, T3) \
    struct {          \
        T1 first;      \
        T2 second;     \
        T3 third;       \
    }

typedef TRIPLE(int, int, int) IntTriple;
typedef TRIPLE(Cell*, Cell*, Cell*) CellTriple;

int findMax(int a, int b);

int findMin(int a, int b);

int count_candidates(Cell *cell, int value, int *cnt_candidate, int *unique_candidate);


struct SudokuBoard_impl
{
    int solved_counter;
    Cell **data; // 9x9 cell board

    Cell **p_rows[BOARD_SIZE];  // rows pointers
    Cell **p_cols[BOARD_SIZE];  // cols pointers
    Cell **p_boxes[BOARD_SIZE]; // boxes pointers
    Cell *solved_cells[BOARD_SIZE *
                       BOARD_SIZE]; // solved cell pointers (maximum)
};

typedef struct SudokuBoard_impl SudokuBoard;

void init_sudoku(SudokuBoard *p_board);
void load_sudoku(SudokuBoard *p_board, char *input_text);
bool apply_constraint(Cell **p_cells, int value);
bool is_in_list(Cell **p_array, int size, Cell *p);
void print_candidate_num(SudokuBoard *p_board);
void print_solution(SudokuBoard *p_board);

void set_candidate(Cell *cell, int value);
void unset_candidate(Cell *cell, int value);
void set_candidates(Cell *cell, int *candidates, int size);
int *get_candidates(Cell *cell);
Cell *get_cell(SudokuBoard *p_board, int index);
bool is_candidate(Cell *cell, int value);

int check_solved_cells(SudokuBoard *p_board, Cell ***p_solved_cells);
bool show_possible(SudokuBoard *p_board, Cell **p_solved_cells, int counter);
void free_sudoku(SudokuBoard *p_board);
