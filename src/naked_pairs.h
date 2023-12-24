#pragma once

#include "sudoku.h"
    
void unset_candidate_in_unit(Cell **p_cells, Cell* cell1, Cell* cell2, IntPair naked_pairs_values);

void find_naked_pairs(SudokuBoard *Board, Cell **p_cells, IntPair **p_naked_pairs_check, int *p_counter);

int naked_pairs(SudokuBoard *p_board);