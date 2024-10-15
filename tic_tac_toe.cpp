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

void display_board(char (&board)[ROWS][COLUMNS])
{
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLUMNS; j++)
        {
            std::cout << board[i][j] << "  ";
        }
        std::cout << std::endl << std::endl;
    }
}

struct win_status {
    int winner = 0;
    int line[2] = {-1, -1};
};

void fill_line(int (&line)[2], int val)
{
    if (line[0] != -1) line[1] = val;
    else line[0] = val;
}

win_status check_winner(char (&board)[ROWS][COLUMNS], char p1_mark, char p2_mark)
{
    win_status ws;                            
    if (board[0][0] == p1_mark && board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        ws.winner = 1;
        fill_line(ws.line, 6);
    }
    if (board[0][2] == p1_mark && board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        ws.winner = 1;
        fill_line(ws.line, 7);
    }
    if (board[0][0] == p2_mark && board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        ws.winner = 2;
        fill_line(ws.line, 6);
    }
    if (board[0][2] == p2_mark && board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        ws.winner = 2;
        fill_line(ws.line, 7);
    }
    for (int i=0; i<ROWS; i++)
    {
        if (board[i][0] == p1_mark && board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            ws.winner = 1;
            fill_line(ws.line, i);
        }
        if (board[0][i] == p1_mark && board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            ws.winner = 1;
            fill_line(ws.line, i + 3);
        }
        if (board[i][0] == p2_mark && board[i][0] == board[i][1] && board[i][1] == board[i][2]) 
        {
            ws.winner = 2;
            fill_line(ws.line, i);
        }
        if (board[0][i] == p2_mark && board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            ws.winner = 2;
            fill_line(ws.line, i + 3);
        }
    }
    return ws;
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
                Sleep(250);
                update_board_var(board, mark, cursor);
                highlight_cursor(7);
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), origin);
                move_selected = true;
                break;   
        }
        if (move_selected) break;
    }
    
}

void connect_win_line(win_status ws)
{
    COORD cursor;
    int i = 0;
    while (i < 2)
    {
        cursor = origin;
        if (ws.line[i] >= 0 && ws.line[i] <= 2)
        {
            cursor.Y = cursor.Y + (ws.line[i] * 2);
            cursor.X = 1;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '-';
            cursor.X = 2;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '-';
            cursor.X = 4;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '-';
            cursor.X = 5;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '-';
        }
        if (ws.line[i] >= 3 && ws.line[i] <= 5)
        {
            cursor.X = cursor.X + ((ws.line[i] - 3) * 3);
            cursor.Y = cursor.Y + 1;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '|';
            cursor.Y = origin.Y;
            cursor.Y = cursor.Y + 3;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '|';
        }
        if (ws.line[i] == 6)
        {
            cursor.X = cursor.X + 2;
            cursor.Y = cursor.Y + 1;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '\\';
            cursor = origin;
            cursor.X = cursor.X + 4;
            cursor.Y = cursor.Y + 3;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '\\';
        }
        if (ws.line[i] == 7)
        {
            cursor.X = cursor.X + 4;
            cursor.Y = cursor.Y + 1;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '/';
            cursor = origin;
            cursor.X = cursor.X + 2;
            cursor.Y = cursor.Y + 3;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
            std::cout << '/';
        }
        i++;
    }
    cursor = origin;
    cursor.Y = cursor.Y + 5;
    cursor.X = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);

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
    win_status ws;

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
        ws = check_winner(board, p1_mark, p2_mark);
        if (ws.winner != 0)
        {
            break;
        }

        turn = !turn;
        g++;

    }

    if (ws.winner != 0)
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), origin);
        display_board(board);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), origin);
        connect_win_line(ws);  
    }
    else
    {
        display_board(board);
    }

    if (ws.winner == 1)
        {
            std::cout << "Game Finished: " << p1_mark << " WINS !!!";
        }
    else if (ws.winner == 2)
    {
        std::cout << "Game Finished: " << p2_mark << " WINS !!!";
    }
    else
    {
        std::cout << "Game Finished: " << "DRAW !!!";
    }

    return 0;
}