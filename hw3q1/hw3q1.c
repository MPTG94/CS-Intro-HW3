/*-------------------------------------------------------------------------
    Include files:
-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>

/*-------------------------------------------------------------------------
    Constants and definitions:
-------------------------------------------------------------------------*/
#define N 11

/*-------------------------------------------------------------------------
    Function declaration
-------------------------------------------------------------------------*/
void print_welcome();
void print_enter_board_size();
void print_board(char board[N][N], int n);
void print_player_turn(int player_index);
void print_error();
void print_winner(int player_index);
void print_tie();
int setup_board(int board_size);
int start_game(char board[N][N], int board_size);
int check_winner_by_row(char board[N][N], int board_size);
int check_winner_by_column(char board[N][N], int board_size);
int check_winner_by_diagonal(char board[N][N], int board_size);
bool check_legal_turn(char board[N][N], int board_size, int row, int column);
bool is_board_full(char board[N][N], int board_size);
int check_winner(char board[N][N], int board_size);


/*-------------------------------------------------------------------------
    Implementation
-------------------------------------------------------------------------*/
int main()
{
    int board_size=0, winner = 0;
    print_welcome();
    print_enter_board_size();
    scanf("%d", &board_size);
    winner = setup_board(board_size);
    if (winner == 0)
    {
        print_tie();
    }
    else if (winner == 1)
    {
        print_winner(1);
    }
    else
    {
        print_winner(2);
    }
    return 0;
}

int setup_board(int board_size)
{
    char board[N][N];
    for(int i = 0; i < board_size; i++)
    {
        for(int j=0; j<board_size; j++)
        {
            board[i][j] = '_';
        }
    }

    print_board(board, board_size);
    print_player_turn(1);

    return start_game(board, board_size);
}

int start_game(char board[N][N], int board_size)
{
    int chosen_row =0, chosen_column=0, curr_player = 1;
    //int maximum_moves = board_size*board_size,
    int move_counter = 0;
    char move_history[N*N];
    scanf("%d", &chosen_row);
    while (!is_board_full(board, board_size))
    {
        if (chosen_row < 0)
        {
            if (-1*chosen_row % 2 == 1)
            {
                if (-1*chosen_row <= move_counter)
                {
                    for (int i =0; i<= move_counter - 1; i++)
                    {
                        //printf("move no. %d was: %d\n", i, move_history[i]);
                    }
                    //printf("number of moves to undo: %d\n", chosen_row*-1);
                    //printf("move counter combined: %d\n", move_counter -1 - (-1*chosen_row));
                    int temp = move_counter - 1;
                    for (int i = temp; i > temp - (-1*chosen_row); i--)
                    {
                        //printf("undoing row: %d, column: %d\n", (move_history[i])/10, (move_history[i])%10);
                        move_counter--;
                        board[(move_history[i])/10][(move_history[i])%10] = '_';
                    }
                    print_board(board, board_size);
                    curr_player = 3 - curr_player;
                    print_player_turn(curr_player);
                    scanf("%d", &chosen_row);
                    continue;
                }
            }
            print_error();
            scanf("%d", &chosen_row);
            continue;
        }
        else
        {
            scanf("%d", &chosen_column);
        }
        if (check_legal_turn(board, board_size, chosen_row, chosen_column))
        {
            if (curr_player == 1)
            {
                board[chosen_row-1][chosen_column-1] = 'X';
                move_history[move_counter] = (chosen_row-1)*10+(chosen_column-1);
                move_counter++;
            }
            else
            {
                board[chosen_row-1][chosen_column-1] = 'O';
                move_history[move_counter] = (chosen_row-1)*10+(chosen_column-1);
                move_counter++;
            }
            curr_player = 3 - curr_player;
            int winner = check_winner(board, board_size);
            print_board(board, board_size);
            if (winner != 0)
            {
                return winner;
            }
            if (is_board_full(board, board_size))
            {
                return 0;
            }
            print_player_turn(curr_player);
        }
        else
        {
            print_error();
        }
        scanf("%d", &chosen_row);
    }

    return 0;
}

/*
This function takes as input the board, the board size a row and column
specified by the user and checks if it is legal to perform a move on the
selected place on the board.
The function will return true if the move is legal, false otherwise.
*/
bool check_legal_turn(char board[N][N], int board_size, int row, int column)
{
    if (row > board_size || column > board_size)
    {
        return 0;
    }
    else if (board[row-1][column-1] != '_')
    {
        return 0;
    }
    return 1;
}

/*
This function checks if the board is full or not
The function will return true if the board is full and false otherwise.
*/
bool is_board_full(char board[N][N], int board_size)
{
    for (int i =0; i< board_size; i++)
    {
        for (int j=0; j<board_size; j++)
        {
            if (board[i][j] == '_')
            {
                return 0;
            }
        }
    }
    return 1;
}

/*
This function checks if a winner has been decided by checking the the board.
Returns the index of the winning player, or 0 if no player has won yet.
*/
int check_winner(char board[N][N], int board_size)
{
    int diag_win = check_winner_by_diagonal(board, board_size);
    int row_win = check_winner_by_row(board, board_size);
    int column_win = check_winner_by_column(board, board_size);

    if (diag_win != 0)
    {
        return diag_win;
    }
    else if (row_win != 0)
    {
        return row_win;
    }
    else if (column_win != 0)
    {
        return column_win;
    }
    return 0;
}

/*
This function checks if a winner has been decided by checking the rows of
the board.
Returns the index of the winning player, or 0 if no player has won yet.
*/
int check_winner_by_row(char board[N][N], int board_size)
{
    int streak_length = 0;
    for (int i = 0; i < board_size; i++)
    {
        for (int j=0; j<board_size - 1; j++)
        {
            if (board[i][j] == board[i][j+1] && board[i][j] != '_')
            {
                streak_length++;
            }
        }
        if (streak_length == board_size -1)
        {
            // There is a streak of X's or O's that fills an entire row
            // Checking who the loser is.
            if (board[i][1] == 'X')
            {
                // The winner is player 2.
                return 2;
            }
            else
            {
                // The winner is player 1.
                return 1;
            }
        }
        else
        {
            // Resetting streak counter.
            streak_length = 0;
        }
    }
    return 0;
}

/*
This function checks if a winner has been decided by checking the columns of
the board.
Returns the index of the winning player, or 0 if no player has won yet.
*/
int check_winner_by_column(char board[N][N], int board_size)
{
    int streak_length = 0;
    for (int i = 0; i < board_size; i++)
    {
        for (int j=0; j<board_size - 1; j++)
        {
            if (board[j][i] == board[j+1][i] && board[j][i] != '_')
            {
                streak_length++;
            }
        }
        if (streak_length == board_size -1)
        {
            // There is a streak of X's or O's that fills an entire column
            // Checking who the loser is.
            if (board[1][i] == 'X')
            {
                // The winner is player 2.
                return 2;
            }
            else
            {
                // The winner is player 1.
                return 1;
            }
        }
        else
        {
            // Resetting streak counter.
            streak_length = 0;
        }
    }
    return 0;
}

/*
This function checks if a winner has been decided by checking the diagonal of
the board.
Returns the index of the winning player, or 0 if no player has won yet.
*/
int check_winner_by_diagonal(char board[N][N], int board_size)
{
    int streak_length = 0;
    for (int i = 0; i < board_size - 1; i++)
    {
        if (board[i][i] == board[i+1][i+1] && board[i][i] != '_')
        {
            streak_length++;
        }
    }
    if (streak_length == board_size -1)
    {
        // There is a streak of X's or O's that fills the entire diagonal
        // Checking who the loser is.
        if (board[1][1] == 'X')
        {
            // The winner is player 2.
            return 2;
        }
        else
        {
            // The winner is player 1.
            return 1;
        }
    }
    return 0;
}

//print welcome message
//1 lines
void print_welcome()
{
    printf("*** Welcome to AVOIDANCE TIC-TAC-TOE game ***\n\n");
}

//print message to enter board size
//1 lines
void print_enter_board_size()
{
    printf("Please enter board size (1 to %d):\n", N);
}

//print the board
//7 lines
void print_board(char board[N][N], int n)
{
    printf("\nCurrent board:\n");
    for (int i = 0; i < n; i++)
    {
        printf("|");
        for (int j = 0; j < n; j++)
        {
            printf("%c|", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//print a request for player with index "player_index" to move
//1 lines
void print_player_turn(int player_index)
{
    printf("\nPlayer ** %d **, enter next move:\n", player_index);
}

//print error message
//1 lines
void print_error()
{
    printf("Illegal move!!!, please try again:\n");
}

//print the winner
//1 lines
void print_winner(int player_index)
{
    printf("Player %d Wins! Hooray!\n", player_index);
}

//print message to announce there is a tie (no one wins)
//1 lines
void print_tie()
{
    printf("It's a tie!\n");
}
