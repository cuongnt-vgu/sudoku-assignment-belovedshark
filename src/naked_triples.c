#include "naked_triples.h"

void unset_candidate_in_unit_triple(Cell **p_cells, Cell* cell1, Cell* cell2, Cell* cell3, IntTriple naked_triple_values) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i] == cell1 || p_cells[i] == cell2 || p_cells[i] == cell3) continue;
        if (is_candidate(p_cells[i], naked_triple_values.first)) unset_candidate(p_cells[i], naked_triple_values.first);
        if (is_candidate(p_cells[i], naked_triple_values.second)) unset_candidate(p_cells[i], naked_triple_values.second);
        if (is_candidate(p_cells[i], naked_triple_values.third)) unset_candidate(p_cells[i], naked_triple_values.third);
    }
}



void find_naked_triple(SudokuBoard *Board, Cell **p_cells, IntTriple ***p_naked_triple, int *p_counter) {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = i + 1; j < BOARD_SIZE; j++)
            for (int k = j + 1; k < BOARD_SIZE; k++) {
                Cell *cell1 = p_cells[i];
                Cell *cell2 = p_cells[j];
                Cell *cell3 = p_cells[k];
                IntTriple id = {cell1->board_index, cell2->board_index, cell3->board_index};
                IntTriple tmp;
                tmp.first = findMin(id.first, findMin(id.second, id.third));
                tmp.third = findMax(id.first, findMax(id.second, id.third));
                tmp.second = id.first + id.second + id.third - tmp.first - tmp.third;
                if (p_naked_triple[tmp.first][tmp.second][tmp.third].first != -1) continue;
                if (cell1->num_candidates < 2 || cell1->num_candidates > 3) continue;
                if (cell2->num_candidates < 2 || cell2->num_candidates > 3) continue;
                if (cell3->num_candidates < 2 || cell3->num_candidates > 3) continue;
                int *cnt_candidates = (int*) calloc(BOARD_SIZE + 1, sizeof(int));
                int unique_candidates = 0;
                for (int candidate = 1; candidate <= BOARD_SIZE; candidate++) {
                    count_candidates(cell1, candidate, cnt_candidates, &unique_candidates);
                    count_candidates(cell2, candidate, cnt_candidates, &unique_candidates);
                    count_candidates(cell3, candidate, cnt_candidates, &unique_candidates);
                }
                if (unique_candidates != 3) {
                    free(cnt_candidates);
                    continue;
                }
                bool check = true;
                IntTriple naked_triple_values = {-1, -1, -1};
                for (int candidate = 1; candidate <= BOARD_SIZE; candidate++) {
                    if (cnt_candidates[candidate] == 0) continue;
                    if (cnt_candidates[candidate] == 1 || cnt_candidates[candidate] > 3) {
                        check = false;
                        break;
                    }
                    if (naked_triple_values.first == -1) naked_triple_values.first = candidate;
                    else if (naked_triple_values.second == -1) naked_triple_values.second = candidate;
                    else naked_triple_values.third = candidate;
                }
                if (check) {
                    p_naked_triple[tmp.first][tmp.second][tmp.third] = naked_triple_values;
                    (*p_counter)++;
                }
                free(cnt_candidates);
            }
}

int naked_triples(SudokuBoard *p_board)
{
    IntTriple ***p_naked_triple = (IntTriple***) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntTriple**));
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        p_naked_triple[i] = (IntTriple**) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntTriple*));
        for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++) {
            p_naked_triple[i][j] = (IntTriple*) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntTriple));
            for (int k = 0; k < BOARD_SIZE * BOARD_SIZE; k++) 
                p_naked_triple[i][j][k] = (IntTriple) {-1, -1, -1};
        }
    }
    int num_naked_triples = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        find_naked_triple(p_board, p_board->p_rows[i], p_naked_triple, &num_naked_triples);
        find_naked_triple(p_board, p_board->p_cols[i], p_naked_triple, &num_naked_triples);
        find_naked_triple(p_board, p_board->p_boxes[i], p_naked_triple, &num_naked_triples);
    }

    for (int id1 = 0; id1 < BOARD_SIZE * BOARD_SIZE - 2; id1++) {
        for (int id2 = id1 + 1; id2 < BOARD_SIZE * BOARD_SIZE - 1; id2++) {
            for (int id3 = id2 + 1; id3 < BOARD_SIZE * BOARD_SIZE; id3++) {
                if (p_naked_triple[id1][id2][id3].first != -1) {
                    Cell *cell1 = get_cell(p_board, id1);
                    Cell *cell2 = get_cell(p_board, id2);
                    Cell *cell3 = get_cell(p_board, id3);
                    if (cell1->row_index == cell2->row_index && cell1->row_index == cell2->row_index) 
                        unset_candidate_in_unit_triple(p_board->p_rows[cell1->row_index], cell1, cell2, cell3, p_naked_triple[id1][id2][id3]);
                    if (cell1->col_index == cell2->col_index && cell1->col_index == cell2->col_index) 
                        unset_candidate_in_unit_triple(p_board->p_cols[cell1->col_index], cell1, cell2, cell3, p_naked_triple[id1][id2][id3]);
                    if (cell1->box_index == cell2->box_index && cell1->box_index == cell2->box_index) 
                        unset_candidate_in_unit_triple(p_board->p_boxes[cell1->box_index], cell1, cell2, cell3, p_naked_triple[id1][id2][id3]);
                }
            }
        }
    }

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++) {
            free(p_naked_triple[i][j]);
        }
        free(p_naked_triple[i]);
    }
    free(p_naked_triple);
    return num_naked_triples;
}