#include <iostream>
#include <Windows.h>


int screen_width = 11;
int screen_height = 11;

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

    const int ROWS = 3;
    const int COLUMNS = 3;
    char board[ROWS][COLUMNS] = {{'_', '_', '_'},
                                 {'_', '_', '_'},
                                 {'_', '_', '_'}};

    std::cout << "Starting game..." << std::endl;
    std::cout << "Board status: " << std::endl;

    while(g < 9)
    {

        for(int i=0; i<ROWS; i++)
        {
            for(int j=0; j<COLUMNS; j++)
            {
                std::cout << board[i][j] << "  ";
            }
            std::cout << std::endl << std::endl;
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
    
    curr = get_cursor_position();
    curr.Y = curr.Y + 9;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curr);
    std::cout << "Game Finished !!!";

    return 0;
}