#include <iostream>
#include <Windows.h>
#include <conio.h>

const int ROWS = 3;
const int COLUMNS = 3;
COORD origin;

COORD get_cursor_position()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(handle, &csbi))
        {
            return csbi.dwCursorPosition;
        }
        else
        {
            return {-1, -1};
        }
}

void display_board(char (&board)[ROWS][COLUMNS], bool clean=false)
{
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLUMNS; j++)
        {
            std::cout << board[i][j] << "  ";
        }
        std::cout << std::endl << std::endl;
    }
    if (clean)
    {
        std::cout << "                                    " << std::endl;
        std::cout << "                                    " << std::endl;
    }
}

int check_winner(char (&board)[ROWS][COLUMNS], char p1_mark, char p2_mark)
{                            
    if ((board[0][0] == p1_mark && board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
        (board[0][2] == p1_mark && board[0][2] == board[1][1] && board[1][1] == board[2][0]))
    {
        return 1;
    }
    if (board[0][0] == p2_mark && board[0][0] == board[1][1] && board[1][1] == board[2][2] ||
        (board[0][2] == p2_mark && board[0][2] == board[1][1] && board[1][1] == board[2][0]))
    {
        return 2;
    }
    for (int i=0; i<ROWS; i++)
    {
        if ((board[i][0] == p1_mark && board[i][0] == board[i][1] && board[i][1] == board[i][2]) ||
            (board[0][i] == p1_mark && board[0][i] == board[1][i] && board[1][i] == board[2][i]))
        {
            return 1;
        }
        if ((board[i][0] == p2_mark && board[i][0] == board[i][1] && board[i][1] == board[i][2]) ||
            (board[0][i] == p2_mark && board[0][i] == board[1][i] && board[1][i] == board[2][i]))
        {
            return 2;
        }
    }
    return 0;
}

void update_board_var(char (&board)[ROWS][COLUMNS], char mark, COORD curr)
{
    int x = (curr.X - origin.X) / 3;
    int y = (curr.Y - origin.Y) / 2;
    board[y][x] = mark;  
}

void highlight_cursor(int color_code=112)
{
    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, color_code);
}

void player_move(char (&board)[ROWS][COLUMNS], char mark)
{
    COORD cursor;
    cursor = get_cursor_position();
    bool move_selected = false;

    while (true)
    {
        int key = _getch();
        switch (key)
        {
            case 119: // up arrow
                if (cursor.Y - 2 >= origin.Y)
                {
                    cursor.Y = cursor.Y - 2;
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
                    break; 
                }
            case 115: // down arrow
                if (cursor.Y + 2 < origin.Y + 5)
                {
                    cursor.Y = cursor.Y + 2;
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
                    break; 
                }
            case 97: // left arrow
                if (cursor.X - 3 >= origin.X)
                {
                    cursor.X = cursor.X - 3;
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
                    break; 
                }
            case 100: // right arrow
                if (cursor.X + 3 < origin.X + 7)
                {
                    cursor.X = cursor.X + 3;
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
                    break; 
                }
            case 109: // 'm' buttom (mark)
                highlight_cursor();
                std::cout << mark;
                update_board_var(board, mark, cursor);
                highlight_cursor(7);
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), origin);
                move_selected = true;
                break;   
        }
        if (move_selected) break;
    }
    
}

int main()
{
    char p1_mark, p2_mark, mark;
    std::cout << "Type Player 1 mark: ";
    std::cin >> p1_mark;
    std::cout << "Type Player 2 mark: ";
    std::cin >> p2_mark;
    bool turn = false;
    int g = 0;
    COORD curr;
    int status;

    char board[ROWS][COLUMNS] = {{'_', '_', '_'},
                                 {'_', '_', '_'},
                                 {'_', '_', '_'}};

    std::cout << "Game Started..." << std::endl;
    std::cout << "Board status: " << std::endl;
    origin = get_cursor_position();

    while(g < 9)
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), origin);
        display_board(board);
        
        if (turn)
        {
            mark = p2_mark;
        }
        else
        {
            mark = p1_mark;
        }
        
        player_move(board, mark);
        status = check_winner(board, p1_mark, p2_mark);
        if (status != 0)
        {
            break;
        }

        turn = !turn;
        g++;

    }

    if (status != 0)
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), origin);
        display_board(board);
    }
    else
    {
        display_board(board);    
    }
    if (status == 1)
        {
            std::cout << "Game Finished: " << p1_mark << " WINS !!!";
        }
    else if (status == 2)
    {
        std::cout << "Game Finished: " << p2_mark << " WINS !!!";
    }
    else
    {
        std::cout << "Game Finished: " << "DRAW !!!";
    }

    return 0;
}