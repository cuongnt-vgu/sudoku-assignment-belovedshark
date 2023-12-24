#pragma once

#include "sudoku.h"
    
int find_hidden_pairs_values(Cell **p_cells, IntPair *hidden_pairs_values);

void find_hidden_pairs(Cell **p_cells, IntPair **p_hidden_pairs_check, int *p_counter);

int hidden_pairs(SudokuBoard *p_board);
