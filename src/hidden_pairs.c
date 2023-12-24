#include "hidden_pairs.h"

int find_hidden_pairs_values(Cell **p_cells, IntPair *hidden_pairs_values)
{
    int num_hidden_pairs_value = 0;
    for (int value1 = 1; value1 < BOARD_SIZE; value1++)
    {
        for (int value2 = value1 + 1; value2 <= BOARD_SIZE; value2++)
        {
            int counter = 0;
            int check_naked_pairs = 0;
            for (int i = 0; i < BOARD_SIZE; i++)  {
                if (is_candidate(p_cells[i], value1) + is_candidate(p_cells[i], value2) == 1) {
                    counter = 0;
                    break;
                }
                if (is_candidate(p_cells[i], value1) && is_candidate(p_cells[i], value2)) {
                    counter++;
                    check_naked_pairs += (p_cells[i]->num_candidates > 2);
                }
            }
            if (counter == 2 && check_naked_pairs > 0) hidden_pairs_values[num_hidden_pairs_value++] = (IntPair) {value1, value2};
        }
    }
    return num_hidden_pairs_value;
}

void find_hidden_pairs(Cell **p_cells, IntPair **p_hidden_pairs_check, int *p_counter) {
    IntPair *hidden_pairs_values = (IntPair*) malloc(BOARD_SIZE * sizeof(IntPair));
    int num_hidden_pairs_value = find_hidden_pairs_values(p_cells, hidden_pairs_values);
    for (int i = 0; i < num_hidden_pairs_value; i++) {
        IntPair hidden_pair = {-1, -1};
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (is_candidate(p_cells[j], hidden_pairs_values[i].first) && is_candidate(p_cells[j], hidden_pairs_values[i].second)) {
                if (hidden_pair.first == -1) hidden_pair.first = p_cells[j]->board_index;
                else hidden_pair.second = p_cells[j]->board_index;
            }
            
        }
        if (hidden_pair.second != -1 && p_hidden_pairs_check[hidden_pair.first][hidden_pair.second].first == -1) {
            p_hidden_pairs_check[hidden_pair.first][hidden_pair.second] = hidden_pairs_values[i];
            p_hidden_pairs_check[hidden_pair.second][hidden_pair.first] = hidden_pairs_values[i];
            (*p_counter)++;        
        }
    }
    free(hidden_pairs_values);   
}

int hidden_pairs(SudokuBoard *p_board)
{
    IntPair **p_hidden_pairs_check = (IntPair**) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntPair*));
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        p_hidden_pairs_check[i] = (IntPair*) malloc(BOARD_SIZE * BOARD_SIZE * sizeof(IntPair));
        for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++) 
            p_hidden_pairs_check[i][j] = (IntPair) {-1, -1};
    }
    int num_hidden_pairs = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        find_hidden_pairs(p_board->p_rows[i], p_hidden_pairs_check, &num_hidden_pairs);
        find_hidden_pairs(p_board->p_cols[i], p_hidden_pairs_check, &num_hidden_pairs);
        find_hidden_pairs(p_board->p_boxes[i], p_hidden_pairs_check, &num_hidden_pairs);
    }
    for (int id1 = 0; id1 < BOARD_SIZE * BOARD_SIZE; id1++)
        for (int id2 = id1 + 1; id2 < BOARD_SIZE * BOARD_SIZE; id2++) {
            if (p_hidden_pairs_check[id1][id2].first == -1) continue;
            Cell *cell1 = get_cell(p_board, id1);
            Cell *cell2 = get_cell(p_board, id2);
            for (int k = 1; k <= BOARD_SIZE; k++) {
                if (k == p_hidden_pairs_check[id1][id2].first || k == p_hidden_pairs_check[id1][id2].second) continue;
                if (is_candidate(cell1, k)) unset_candidate(cell1, k);
                if (is_candidate(cell2, k)) unset_candidate(cell2, k);
            }
        }
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) 
        free(p_hidden_pairs_check[i]);
    free(p_hidden_pairs_check);
    return num_hidden_pairs;
}
