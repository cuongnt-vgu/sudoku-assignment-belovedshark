#pragma once

#include "sudoku.h"

int count_candidates(Cell *cell, int value, int *cnt_candidate, int *unique_candidate);

int find_hidden_triple_values(Cell **p_cells, IntTriple *hidden_pairs_values);

void find_hidden_triple(SudokuBoard *Board, Cell **p_cells, IntTriple ***p_hidden_triple, int *p_counter);

int hidden_triples(SudokuBoard *p_board);