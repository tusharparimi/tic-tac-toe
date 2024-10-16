#include <iostream>
#include <Windows.h>
#include <conio.h>

// board shape ROWS, COLUMNS
const int ROWS = 3;
const int COLUMNS = 3;
COORD origin; // cursor postion at start of board

// get current cursor position
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

// get the char at current cursor position
char get_char_at_cursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    TCHAR str_from_console[1];
    DWORD dw_chars;
    COORD pos = get_cursor_position();
    ReadConsoleOutputCharacter(handle, str_from_console, 1, pos, &dw_chars);
    char c = str_from_console[0];
    return c; 
}

// display current board status
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

// win status structure with vars:
// winner: int winner id
// 0(draw), 1(player1), 2(player2)
// line: array with len 2, as a player can win max with 2 win lines
// -1(no win line), 0(row 0), 1(row 1), 2(row 2), 3(col 0), 4(col 1), 5(col 2), 6(left diag), 7(right diag)
struct win_status {
    int winner = 0;
    int line[2] = {-1, -1};
};

// helper func for filling values in win_status.line array
void fill_line(int (&line)[2], int val)
{
    if (line[0] != -1) line[1] = val;
    else line[0] = val;
}

// checks the 2d board array and return a win_status structure
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

// update 2d board array based on current cursor mark
void update_board_var(char (&board)[ROWS][COLUMNS], char mark, COORD curr)
{
    int x = (curr.X - origin.X) / 3;
    int y = (curr.Y - origin.Y) / 2;
    board[y][x] = mark;  
}

// highlight cursor using specified color code
void highlight_cursor(int color_code=112)
{
    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, color_code);
}

// interactive cursor movement and marking by using WASD and 'm' buttons for each player during their turn
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
                char c = get_char_at_cursor();
                if (c == '_')
                {
                    highlight_cursor();
                    std::cout << mark;
                    Sleep(250);
                    update_board_var(board, mark, cursor);
                    highlight_cursor(7);
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), origin);
                    move_selected = true;
                    break;
                }    
        }
        if (move_selected) break;
    }
    
}

// connecting the win lines if any specified in the win_status structure
void connect_win_line(win_status ws)
{
    COORD cursor;
    int i = 0;
    while (i < 2) // connect each win line in win_status.line[2]
    {
        cursor = origin; // setting cursor to start of board
        if (ws.line[i] >= 0 && ws.line[i] <= 2) // row lines
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
        if (ws.line[i] >= 3 && ws.line[i] <= 5) // col lines
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
        if (ws.line[i] == 6) // left diag line
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
        if (ws.line[i] == 7) // right diag line
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
    // setting cursor to end of board
    cursor = origin;
    cursor.Y = cursor.Y + 5;
    cursor.X = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);

}

bool validate_player_mark(char mark)
{
    if (mark == ' ' || mark == '_')
    {
        std::cout << "INVALID: cannot use ' ' or '_' as mark" << std::endl;
        return false;
    } 
    return true;
}

bool validate_player_mark(char p1_mark, char p2_mark)
{
    if (p2_mark == ' ' || p2_mark == '_')
    {
        std::cout << "INVALID: cannot use ' ' or '_' as mark" << std::endl;
        return false;
    }
    if (p1_mark == p2_mark)
    {
        std::cout << "INVALID: cannot use same mark as player 1" << std::endl;
        return false;
    }
    return true;
}

int main()
{
    // read and validate player marks
    char p1_mark, p2_mark, mark;
    std::cout << "Type Player 1 mark: ";
    std::cin >> p1_mark;
    while (!validate_player_mark(p1_mark))
    {
        std::cout << "Type Player 1 mark: ";
        std::cin >> p1_mark;
    }
    std::cout << "Type Player 2 mark: ";
    std::cin >> p2_mark;
    while (!validate_player_mark(p1_mark, p2_mark))
    {
        std::cout << "Type Player 2 mark: ";
        std::cin >> p2_mark;
    }

    // turn: false (player 1 turn), true (player 2 turn)
    bool turn = false;
    int g = 0; // game loop interator, only runs 9 times
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
            std::cout << "Game Finished: '" << p1_mark << "' WINS !!!";
        }
    else if (ws.winner == 2)
    {
        std::cout << "Game Finished: '" << p2_mark << "' WINS !!!";
    }
    else
    {
        std::cout << "Game Finished: " << "DRAW !!!";
    }

    return 0;
}