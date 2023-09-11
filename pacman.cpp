#include <iostream>
#include <conio.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>

using namespace std;

const int BoardSize = 17, ObCounter = BoardSize + 16;
char BOARD[BoardSize][BoardSize];
int EatenFood[2][BoardSize * BoardSize], Obstacle[ObCounter][2];
int XPosPacMan = 0, YPosPacMan = 0;
int vector = 2;
int nf = 0;
int XPosGhost = 6, YPosGhost = 6;
int XPosGhost1 = 4, YPosGhost1 = 2;
int EatenFoodCounter = 0;
enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT
};
Direction dirp = UP, dirg;

void InitializeBoard()
{
    int i, j;
    for (i = 0; i < BoardSize; i++)
    {
        for (j = 0; j < BoardSize; j++)
        {
            BOARD[i][j] = '.';
        }
    }
    for (i = 0; i < BoardSize * BoardSize; i++)
    {
        BOARD[EatenFood[0][i]][EatenFood[1][i]] = ' ';
    }
    BOARD[XPosPacMan][YPosPacMan] = '@';
    BOARD[XPosGhost][YPosGhost] = 'T';
    BOARD[XPosGhost1][YPosGhost1] = 'T';
}

void RemindObstacle()
{
    int k;
    for (k = 0; k < ObCounter; k++)
    {
        BOARD[Obstacle[k][0]][Obstacle[k][1]] = '#';
    }
}

void set_cursor(int x, int y)
{
    HANDLE handle;
    COORD coordinates;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(handle, coordinates);
}

void TrueCoordinate(int x, int y)
{
    if (x < 0)
    {
        x = BoardSize - 1;
    }
    if (x == BoardSize)
    {
        x = 0;
    }
    if (y < 0)
    {
        y = BoardSize - 1;
    }
    if (y == BoardSize)
    {
        y = 0;
    }
}

void ObstacleRandomizer()
{
    srand(time(NULL));
    for (int i = 0; i < ObCounter; i++)
    {
        Obstacle[i][0] = (rand() % (BoardSize - 1) / 2) * 2 + 1;
        Obstacle[i][1] = (rand() % (BoardSize - 1) / 2) * 2 + 1;
    }
}

bool IsWiner()
{
    int counter = 0;
    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            if (BOARD[i][j] == '.')
            {
                counter++;
            }
        }
    }
    if (counter == 0)
    {
        return true;
    }
    return false;
}

void InitializeEatenFood()
{
    int i, j;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < BoardSize; j++)
        {
            EatenFood[i][j] = 0;
        }
    }
}

void EatenFoodF()
{
    int i, j, k;
    for (i = 0; i < BoardSize; i++)
    {
        for (j = 0; j < BoardSize; j++)
        {
            if (BOARD[i][j] == '@')
            {
                for (k = 0; k < nf; k++)
                {
                    if (EatenFood[0][k] == i && EatenFood[1][k] == j)
                    {
                        break;
                    }
                }
                if (k == nf)
                {
                    EatenFood[0][k] = i;
                    EatenFood[1][k] = j;
                    nf++;
                    EatenFoodCounter++;
                }
            }
        }
    }
}

void ObstaclePacman()
{
    switch (dirp)
    {
    case UP:
        for (int k = 0; k < ObCounter; k++)
        {
            if (Obstacle[k][0] == XPosPacMan && Obstacle[k][1] == YPosPacMan)
            {
                XPosPacMan++;
            }
        }
        break;
    case DOWN:
        for (int k = 0; k < ObCounter; k++)
        {
            if (Obstacle[k][0] == XPosPacMan && Obstacle[k][1] == YPosPacMan)
            {
                XPosPacMan--;
            }
        }
        break;
    case RIGHT:
        for (int k = 0; k < ObCounter; k++)
        {
            if (Obstacle[k][0] == XPosPacMan && Obstacle[k][1] == YPosPacMan)
            {
                YPosPacMan--;
            }
        }
        break;
    case LEFT:
        for (int k = 0; k < ObCounter; k++)
        {
            if (Obstacle[k][0] == XPosPacMan && Obstacle[k][1] == YPosPacMan)
            {
                YPosPacMan++;
            }
        }
        break;
    }
}

void ObstacleGhost(int &x, int &y, Direction &direct)
{
    int k;
    for (k = 0; k < ObCounter; k++)
    {
        if (Obstacle[k][0] == x && Obstacle[k][1] == y)
        {
            if (direct == UP)
            {
                x++;
                break;
            }
            else if (direct == RIGHT)
            {
                y--;
                break;
            }
            else if (direct == DOWN)
            {
                x--;
                break;
            }
            else if (direct == LEFT)
            {
                y++;
                break;
            }
        }
    }
}

void BasicFollower(int &xp, int &yp, int &xg, int &yg)
{
    srand(time(NULL));
    int a= rand()%4;
    switch(a){
        case 1:
            xg--;
            dirg=UP;
            ObstacleGhost(xg, yg, dirg);
            TrueCoordinate(xg,yg);
            break;
        case 2:
            yg--;
            dirg=LEFT;
            ObstacleGhost(xg, yg, dirg);
            TrueCoordinate(xg,yg);
            break;
        case 3:
            yg++;
            dirg=RIGHT;
            ObstacleGhost(xg, yg, dirg);
            TrueCoordinate(xg,yg);
            break;
        default:
            xg++;
            dirg=DOWN;
            ObstacleGhost(xg, yg, dirg);
            TrueCoordinate(xg,yg);
            break;
    }
}

// Ghost rider
void Follower(int &xp, int &yp, int &xg, int &yg)
{
    int vect;
    // pos of P from G: top-left.
    if (xg - xp > 0 && yg - yp > 0)
    {
        srand(time(NULL));
        int a = rand() % 2;
        switch (a)
        {
        case 1:
            xg--;
            dirg = UP;
            ObstacleGhost(xg, yg, dirg);
            break;
        default:
            yg--;
            dirg = LEFT;
            ObstacleGhost(xg, yg, dirg);
            break;
        }
    }
    // pos of P from G: top-right.
    else if (xg - xp > 0 && yg - yp < 0)
    {
        srand(time(NULL));
        int a = rand() % 2;
        switch (a)
        {
        case 1:
            xg--;
            dirg = UP;
            ObstacleGhost(xg, yg, dirg);
            break;
        default:
            yg++;
            dirg = RIGHT;
            ObstacleGhost(xg, yg, dirg);
            break;
        }
    }
    // pos of P from G: bottom-right.
    else if (xg - xp < 0 && yg - yp < 0)
    {
        srand(time(NULL));
        int a = rand() % 2;
        switch (a)
        {
        case 1:
            xg++;
            dirg = DOWN;
            ObstacleGhost(xg, yg, dirg);
            break;
        default:
            yg++;
            dirg = RIGHT;
            ObstacleGhost(xg, yg, dirg);
            break;
        }
    }
    // pos of P from G: bottom-left.
    else if (xg - xp < 0 && yg - yp > 0)
    {
        srand(time(NULL));
        int a = rand() % 2;
        switch (a)
        {
        case 1:
            xg++;
            dirg = DOWN;
            ObstacleGhost(xg, yg, dirg);
            break;
        default:
            yg--;
            dirg = LEFT;
            ObstacleGhost(xg, yg, dirg);
            break;
        }
    }
    // pos of P from G: row-mate.
    else if (xg == xp)
    {
        if (yg > yp)
        {
            if (BOARD[xg][yg - 1] == '#')
            {
                switch (rand() % 2)
                {
                case 1:
                    xg++;
                    break;
                default:
                    xg--;
                    break;
                }
            }
            else
            {
                yg--;
            }
        }
        else if (yg < yp)
        {
            if (BOARD[xg][yg + 1] == '#')
            {
                switch (rand() % 2)
                {
                case 1:
                    xg++;
                    break;
                default:
                    xg--;
                    break;
                }
            }
            else
            {
                yg++;
            }
        }
    }
    // pos of P from G: column-mate.
    else if (yg == yp)
    {
        if (xg > xp)
        {
            if (BOARD[xg - 1][yg] == '#')
            {
                switch (rand() % 2)
                {
                case 1:
                    yg++;
                    break;
                default:
                    yg--;
                    break;
                }
            }
            else
            {
                xg--;
            }
        }
        if (xg < xp)
        {
            if (BOARD[xg + 1][yg] == '#')
            {
                switch (rand() % 2)
                {
                case 1:
                    yg++;
                    break;
                default:
                    yg--;
                    break;
                }
            }
            else
            {
                xg++;
            }
        }
    }
}

bool IsLoser(int xg, int yg, int xp, int yp)
{
    if (xp == xg && yg == yp)
    {
        return true;
    }
    return false;
}

void Mover(char move)
{
    if (kbhit())
    {
        move = getch();
        switch (move)
        {
        case 'w':
            XPosPacMan--;
            dirp = UP;
            break;
        case 's':
            XPosPacMan++;
            dirp = DOWN;
            break;
        case 'd':
            YPosPacMan++;
            dirp = RIGHT;
            break;
        case 'a':
            YPosPacMan--;
            dirp = LEFT;
            break;
        }
    }
    else
    {
        if (dirp == UP)
        {
            XPosPacMan--;
        }
        if (dirp == DOWN)
        {
            XPosPacMan++;
        }
        if (dirp == RIGHT)
        {
            YPosPacMan++;
        }
        if (dirp == LEFT)
        {
            YPosPacMan--;
        }
    }
    ObstaclePacman();
    if (XPosPacMan < 0)
    {
        XPosPacMan = BoardSize - 1;
    }
    if (YPosPacMan < 0)
    {
        YPosPacMan = BoardSize - 1;
    }
    if (XPosPacMan == BoardSize)
    {
        XPosPacMan = 0;
    }
    if (YPosPacMan == BoardSize)
    {
        YPosPacMan = 0;
    }
    usleep(0.3 * 1000000);
}

void SetColor(int color)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, color);
}

void Printer()
{
    int i, j;
    for (i = 0; i < BoardSize; i++)
    {
        for (j = 0; j < BoardSize; j++)
        {
            set_cursor(j * 2, i);
            if (BOARD[i][j] == '.')
            {
                SetColor(0x01);
            }
            if (BOARD[i][j] == '@')
            {
                SetColor(0x02);
            }
            if (BOARD[i][j] == '#')
            {
                SetColor(0x04);
            }
            if (BOARD[i][j] == 'T')
            {
                SetColor(0x05);
            }
            cout << BOARD[i][j];
        }
    }
}

int main()
{
    char a;
    int n;
    InitializeEatenFood();
    ObstacleRandomizer();
    system("cls");
    cout<< "Choose the level:\n 1.easy \n 2.medium \n 3.hard \n";
    cin>>n;
    system("cls");
    if(n==1){
        while (true)
        {
            InitializeBoard();
            RemindObstacle();
            EatenFoodF();
            BasicFollower(XPosPacMan, YPosPacMan, XPosGhost, YPosGhost);
            BasicFollower(XPosPacMan, YPosPacMan, XPosGhost1, YPosGhost1);
            if (IsWiner() || IsLoser(XPosGhost, YPosGhost, XPosPacMan, YPosPacMan) || IsLoser(XPosGhost1, YPosGhost1, XPosPacMan, YPosPacMan))
            {
                break;
            }
            Printer();
            Mover(a);
            if (IsWiner() || IsLoser(XPosGhost, YPosGhost, XPosPacMan, YPosPacMan) || IsLoser(XPosGhost1, YPosGhost1, XPosPacMan, YPosPacMan))
            {
                break;
            }
        }
        SetColor(0x0f);
        system("cls");
        if (IsWiner())
            cout << "You Win!";
        else
            cout << "You Lose!" << endl
                << "Your score:" << EatenFoodCounter;
    }
    if(n==2){
        while (true)
        {
            InitializeBoard();
            RemindObstacle();
            EatenFoodF();
            BasicFollower(XPosPacMan, YPosPacMan, XPosGhost, YPosGhost);
            Follower(XPosPacMan, YPosPacMan, XPosGhost1, YPosGhost1);
            if (IsWiner() || IsLoser(XPosGhost, YPosGhost, XPosPacMan, YPosPacMan) || IsLoser(XPosGhost1, YPosGhost1, XPosPacMan, YPosPacMan))
            {
                break;
            }
            Printer();
            Mover(a);
            if (IsWiner() || IsLoser(XPosGhost, YPosGhost, XPosPacMan, YPosPacMan) || IsLoser(XPosGhost1, YPosGhost1, XPosPacMan, YPosPacMan))
            {
                break;
            }
        }
        SetColor(0x0f);
        system("cls");
        if (IsWiner())
            cout << "You Win!";
        else
            cout << "You Lose!" << endl
                << "Your score:" << EatenFoodCounter;
    }
    if(n==3){
        while (true)
        {
            InitializeBoard();
            RemindObstacle();
            EatenFoodF();
            Follower(XPosPacMan, YPosPacMan, XPosGhost, YPosGhost);
            Follower(XPosPacMan, YPosPacMan, XPosGhost1, YPosGhost1);
            if (IsWiner() || IsLoser(XPosGhost, YPosGhost, XPosPacMan, YPosPacMan) || IsLoser(XPosGhost1, YPosGhost1, XPosPacMan, YPosPacMan))
            {
                break;
            }
            Printer();
            Mover(a);
            if (IsWiner() || IsLoser(XPosGhost, YPosGhost, XPosPacMan, YPosPacMan) || IsLoser(XPosGhost1, YPosGhost1, XPosPacMan, YPosPacMan))
            {
                break;
            }
        }
        SetColor(0x0f);
        system("cls");
        if (IsWiner())
            cout << "You Win!";
        else
            cout << "You Lose!" << endl
                << "Your score:" << EatenFoodCounter;
    }
}