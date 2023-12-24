#include "naked_pairs.h"

void unset_candidate_in_unit(Cell **p_cells, Cell* cell1, Cell* cell2, IntPair naked_pairs_values) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i] == cell1 || p_cells[i] == cell2) continue;
        if (is_candidate(p_cells[i], naked_pairs_values.first)) unset_candidate(p_cells[i], naked_pairs_values.first);
        if (is_candidate(p_cells[i], naked_pairs_values.second)) unset_candidate(p_cells[i], naked_pairs_values.second);
    }
}

void find_naked_pairs(SudokuBoard *Board, Cell **p_cells, IntPair **p_naked_pairs_check, int *p_counter) {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = i + 1; j < BOARD_SIZE; j++) {
            if (p_cells[i]->num_candidates != 2 || p_cells[j]->num_candidates != 2) continue;
            if (p_naked_pairs_check[p_cells[i]->board_index][p_cells[j]->board_index].first != -1) continue;
            int *list_candidate1 = get_candidates(p_cells[i]);
            int *list_candidate2 = get_candidates(p_cells[j]);
            if (list_candidate1[0] == list_candidate2[0] && list_candidate1[1] == list_candidate2[1]) {
                p_naked_pairs_check[p_cells[i]->board_index][p_cells[j]->board_index] = (IntPair) {list_candidate1[0], list_candidate1[1]};
                p_naked_pairs_check[p_cells[j]->board_index][p_cells[i]->board_index] = (IntPair) {list_candidate1[0], list_candidate1[1]};
                (*p_counter)++;
            }
            free(list_candidate1);
            free(list_candidate2);
            
        }
}

int naked_pairs(SudokuBoard *p_board)
{
    IntPair **p_naked_pairs_check = (IntPair**) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntPair*));
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        p_naked_pairs_check[i] = (IntPair*) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntPair));
        for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++) 
            p_naked_pairs_check[i][j] = (IntPair) {-1, -1};
    }
    int num_naked_pairs = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        find_naked_pairs(p_board, p_board->p_rows[i], p_naked_pairs_check, &num_naked_pairs);
        find_naked_pairs(p_board, p_board->p_cols[i], p_naked_pairs_check, &num_naked_pairs);
        find_naked_pairs(p_board, p_board->p_boxes[i], p_naked_pairs_check, &num_naked_pairs);
    }
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE - 1; i++)
        for (int j = i + 1; j < BOARD_SIZE * BOARD_SIZE; j++) {
            if (p_naked_pairs_check[i][j].first != -1) {
               Cell* cell1 = get_cell(p_board, i);
                Cell* cell2 = get_cell(p_board, j);
                if (cell1->row_index == cell2->row_index) unset_candidate_in_unit(p_board->p_rows[cell1->row_index], cell1, cell2, p_naked_pairs_check[i][j]);
                if (cell1->col_index == cell2->col_index) unset_candidate_in_unit(p_board->p_cols[cell1->col_index], cell1, cell2, p_naked_pairs_check[i][j]);
                if (cell1->box_index == cell2->box_index) unset_candidate_in_unit(p_board->p_boxes[cell1->box_index], cell1, cell2, p_naked_pairs_check[i][j]);
            }
        }
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        free(p_naked_pairs_check[i]);
    }
    free(p_naked_pairs_check);
    return num_naked_pairs;
}