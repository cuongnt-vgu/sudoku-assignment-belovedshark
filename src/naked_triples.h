#pragma once

#include "sudoku.h"

    
void find_naked_triple(SudokuBoard *Board, Cell **p_cells, IntTriple ***p_naked_triple, int *p_counter);

void unset_candidate_in_unit_triple(Cell **p_cells, Cell* cell1, Cell* cell2, Cell* cell3, IntTriple naked_triple_values);

int naked_triples(SudokuBoard *p_board);