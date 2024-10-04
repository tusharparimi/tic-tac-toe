#include <iostream>

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

    const int ROWS = 3;
    const int COLUMNS = 3;
    char board[ROWS][COLUMNS] = {{'_', '_', '_'},
                                 {'_', '_', '_'},
                                 {'_', '_', '_'}};

    std::cout << "Starting game..." << std::endl;
    while(g < 9)
    {
        std::cout << "Board status: " << std::endl;
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
        turn = !turn;
        g++;

    }
    
    std::cout << "Game finished !!!" << std::endl;
    std::cout << "Final Board status: " << std::endl;
    for(int i=0; i<ROWS; i++)
        {
            for(int j=0; j<COLUMNS; j++)
            {
                std::cout << board[i][j] << "  ";
            }
            std::cout << std::endl << std::endl;
        }

    return 0;
}