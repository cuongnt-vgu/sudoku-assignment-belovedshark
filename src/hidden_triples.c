#include "hidden_triples.h"


int find_hidden_triple_values(Cell **p_cells, IntTriple *hidden_pairs_values) {
    int num_hidden_triples_values = 0;
    for (int value1 = 1; value1 < BOARD_SIZE; value1++) {
        for (int value2 = value1 + 1; value2 < BOARD_SIZE; value2++) {
            for (int value3 = value2 + 1; value3 <= BOARD_SIZE; value3++) {
                int counter = 0;
                int check_naked_triples = 0;
                int unique_candidates = 0;
                int *cnt_candidate = (int*) calloc(BOARD_SIZE + 1, sizeof(int));
                for (int i = 0; i < BOARD_SIZE; i++) {
                    if (is_candidate(p_cells[i], value1) || is_candidate(p_cells[i], value2) || is_candidate(p_cells[i], value3)) {
                        count_candidates(p_cells[i], value1, cnt_candidate, &unique_candidates);
                        count_candidates(p_cells[i], value2, cnt_candidate, &unique_candidates);
                        count_candidates(p_cells[i], value3, cnt_candidate, &unique_candidates);
                        counter++;
                        check_naked_triples += (p_cells[i]->num_candidates > 3);
                    }
                }
                if (unique_candidates != 3 || counter != 3 || check_naked_triples == 0) {
                    free(cnt_candidate);
                    continue;
                }
                if (cnt_candidate[value1] < 2 || cnt_candidate[value2] < 2 || cnt_candidate[value3] < 2) {
                    free(cnt_candidate);
                    continue;
                }
                hidden_pairs_values[num_hidden_triples_values++] = (IntTriple) {value1, value2, value3};
                free(cnt_candidate);
            }
        }
    }
    return num_hidden_triples_values;
}

void find_hidden_triple(SudokuBoard *Board, Cell **p_cells, IntTriple ***p_hidden_triple, int *p_counter) {
    IntTriple *hidden_triple_values = (IntTriple*) malloc(BOARD_SIZE * sizeof(IntTriple));
    int num_hidden_triple_values = find_hidden_triple_values(p_cells, hidden_triple_values);
    for (int i = 0; i < num_hidden_triple_values; i++) {
        IntTriple hidden_triple = {-1, -1, -1};
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_candidate(p_cells[j], hidden_triple_values[i].first) || is_candidate(p_cells[j], hidden_triple_values[i].second) || is_candidate(p_cells[j], hidden_triple_values[i].third)) {
                if (hidden_triple.first == -1) hidden_triple.first = p_cells[j]->board_index;
                else if (hidden_triple.second == -1) hidden_triple.second = p_cells[j]->board_index;
                else hidden_triple.third = p_cells[j]->board_index;
            }
        }
        IntTriple id = hidden_triple;
        
        IntTriple tmp;
        tmp.first = findMin(id.first, findMin(id.second, id.third));
        tmp.third = findMax(id.first, findMax(id.second, id.third));
        tmp.second = id.first + id.second + id.third - tmp.first - tmp.third;
        if (hidden_triple.third != -1 && p_hidden_triple[tmp.first][tmp.second][hidden_triple.third].first == -1) {
            p_hidden_triple[hidden_triple.first][hidden_triple.second][hidden_triple.third] = hidden_triple_values[i];
            (*p_counter)++;
        }
    }
    free(hidden_triple_values);
}

int hidden_triples(SudokuBoard *p_board) {
    IntTriple ***p_hidden_triple = (IntTriple***) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntTriple**));
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        p_hidden_triple[i] = (IntTriple**) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntTriple*));
        for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++) {
            p_hidden_triple[i][j] = (IntTriple*) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntTriple));
            for (int k = 0; k < BOARD_SIZE * BOARD_SIZE; k++) 
                p_hidden_triple[i][j][k] = (IntTriple) {-1, -1, -1};
        }
    }
    int num_hidden_triples = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        find_hidden_triple(p_board, p_board->p_rows[i], p_hidden_triple, &num_hidden_triples);
        find_hidden_triple(p_board, p_board->p_cols[i], p_hidden_triple, &num_hidden_triples);
        find_hidden_triple(p_board, p_board->p_boxes[i], p_hidden_triple, &num_hidden_triples);
    }

    for (int id1 = 0; id1 < BOARD_SIZE * BOARD_SIZE; id1++)
        for (int id2 = id1 + 1; id2 < BOARD_SIZE * BOARD_SIZE; id2++)
            for (int id3 = id2 + 1; id3 < BOARD_SIZE * BOARD_SIZE; id3++) {
                Cell *cell1 = get_cell(p_board, id1);
                Cell *cell2 = get_cell(p_board, id2);
                Cell *cell3 = get_cell(p_board, id3);
                IntTriple candidates = p_hidden_triple[id1][id2][id3];
                if (candidates.first == -1) continue;
                for (int i = 1; i <= BOARD_SIZE; i++) {
                    if (i == candidates.first || i == candidates.second || i == candidates.third) continue;
                    if (is_candidate(cell1, i)) unset_candidate(cell1, i);
                    if (is_candidate(cell2, i)) unset_candidate(cell2, i);
                    if (is_candidate(cell3, i)) unset_candidate(cell3, i);
                }
            }
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++)
            free(p_hidden_triple[i][j]);
        free(p_hidden_triple[i]);
    }
    free(p_hidden_triple);
    return num_hidden_triples;
}