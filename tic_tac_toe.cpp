#include <iostream>
#include <Windows.h>

const int ROWS = 3;
const int COLUMNS = 3;

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

void display_board(char board[ROWS][COLUMNS], bool clean=false)
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

int check_winner(char board[ROWS][COLUMNS], char p1_mark, char p2_mark)
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

int main()
{
    char p1_mark, p2_mark, mark;
    std::cout << "Type Player 1 mark: ";
    std::cin >> p1_mark;
    std::cout << "Type Player 2 mark: ";
    std::cin >> p2_mark;
    int posx, posy;
    bool turn = false;
    int g = 0;
    COORD origin;
    origin.X = 0;
    origin.Y = 0;
    COORD curr;
    int status;

    char board[ROWS][COLUMNS] = {{'_', '_', '_'},
                                 {'_', '_', '_'},
                                 {'_', '_', '_'}};

    std::cout << "Game Started..." << std::endl;
    std::cout << "Board status: " << std::endl;

    while(g < 9)
    {
        status = check_winner(board, p1_mark, p2_mark);
        display_board(board);
        if (status != 0)
        {
            break;
        }

        if (turn)
        {
            mark = p2_mark;
        }
        else
        {
            mark = p1_mark;
        }
        
        std::cout << "Player " << static_cast<int>(turn) + 1 << "'s (" << mark << ") turn..." << std::endl;
        std::cout << "Type desired position: ";
        std::cin >> posx >> posy;
        board[posx][posy] = mark;

        curr = get_cursor_position();
        curr.Y = curr.Y - 8;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curr);

        turn = !turn;
        g++;

    }

    if (status != 0)
    {
        curr = get_cursor_position();
        curr.Y = curr.Y - 6;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curr);
        display_board(board);
    }
    else
    {
        display_board(board, true);
        curr = get_cursor_position();
        curr.Y = curr.Y - 2;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curr);    
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