#include<iostream>
#include<windows.h>
#include<conio.h>
#include<math.h>

using namespace std;

void hideConsoleCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT |
        ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState ==

            FROM_LEFT_1ST_BUTTON_PRESSED)

        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void color(int k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}
enum direction {
    Up, Down, Left, Right
};
struct position {
	int rpos, cpos;
};
struct snake {
	position* snakepos;
	int size;
	direction DIR;
	int Ukey, Dkey, Lkey, Rkey;

};

void init(snake& S) {
    // HardCoding the values for single player game because no need for initialization
    S.size = 3;
    S.DIR = Right;
    S.Ukey = 72, S.Dkey = 80, S.Lkey = 75, S.Rkey = 77;
    S.snakepos = new position[S.size];
        S.snakepos[0].rpos = 40, S.snakepos[0].cpos = 39;
        S.snakepos[1].rpos = 40, S.snakepos[1].cpos = 40;
        S.snakepos[2].rpos = 40, S.snakepos[2].cpos = 41;
    
}

void displaySnake(snake S) {
    for (int i = 0; i < S.size; i++) {
        gotoRowCol(S.snakepos[i].rpos, S.snakepos[i].cpos);
        cout << "O";
    }
}
bool validFood(snake S, int rows, int cols, position& Food) {
    if (Food.rpos < 4 || Food.rpos >= rows || Food.cpos < 4 || Food.cpos >= cols)
        return false;
    for (int i = 0; i < S.size; i++) {
        if (S.snakepos[i].rpos == Food.rpos && S.snakepos[i].cpos == Food.cpos)
            return false;
    }
    return true;
}
void foodGenerate(snake S, int rows, int cols, position& Food) {
    do {
        Food.rpos = rand() % rows;
        Food.cpos = rand() % cols;
    } while (!validFood(S, rows, cols, Food));
    color(12);
    gotoRowCol(Food.rpos, Food.cpos);
    cout << "o";
}
void foodGenerate2(snake S1, snake S2, snake S3, int rows, int cols, position& Food) {
    do {
        Food.rpos = rand() % rows;
        Food.cpos = rand() % cols;
    } while (!validFood(S1, rows, cols, Food) || !validFood(S2, rows, cols, Food) || !validFood(S3, rows, cols, Food));
    color(12);
    gotoRowCol(Food.rpos, Food.cpos);
    cout << "o";
}
bool foodEaten(position Food, snake S) {
    if (S.snakepos[S.size - 1].rpos == Food.rpos && S.snakepos[S.size - 1].cpos == Food.cpos)
        return true;
    else
        return false;
}

void growSnake(snake& S, position Food) {

    position* newSnake = new position[S.size + 1];
    newSnake[S.size] = Food;
    for (int i = 0; i < S.size; i++) {
        newSnake[i] = S.snakepos[i];
    }
    delete[] S.snakepos;
    S.snakepos = newSnake;
    S.size++;

}

bool Killed(snake S) {

    for (int i = 0; i < S.size - 1; i++) {
        if (S.snakepos[i].rpos == S.snakepos[S.size - 1].rpos && S.snakepos[i].cpos == S.snakepos[S.size - 1].cpos) {
            return true;

        }
    }
    return false;
}


void moveSnake(snake& S) {
    for (int i = 0; i < S.size - 1; i++) {
        S.snakepos[i] = S.snakepos[i + 1];
    }
    if (S.DIR == Up)
        S.snakepos[S.size - 1].rpos--;
    else if (S.DIR == Down)
        S.snakepos[S.size - 1].rpos++;
    else if (S.DIR == Left)
        S.snakepos[S.size - 1].cpos--;
    else if (S.DIR == Right)
        S.snakepos[S.size - 1].cpos++;


}
void EraseSnake(snake S) {
    for (int i = 0; i < S.size; i++) {
        gotoRowCol(S.snakepos[i].rpos, S.snakepos[i].cpos);
        cout << ' ';
    }
}

void boundaryCheck(snake& S, int rows, int cols) {
    if (S.snakepos[S.size - 1].rpos <=1) {
        S.snakepos[S.size - 1].rpos = rows - 2;
    }
    else if (S.snakepos[S.size - 1].rpos >= rows - 1) {
        S.snakepos[S.size - 1].rpos = 1;
    }

    if (S.snakepos[S.size - 1].cpos <=1) {
        S.snakepos[S.size - 1].cpos = cols - 2;
    }
    else if (S.snakepos[S.size - 1].cpos >= cols - 1) {
        S.snakepos[S.size - 1].cpos = 1;
    }
}
bool boundaryCheck2(snake& S, int rows, int cols) {
    if (S.snakepos[S.size - 1].rpos <= 1) {
        return true;
    }
    else if (S.snakepos[S.size - 1].rpos >= rows - 1) {
        return true;
    }

    if (S.snakepos[S.size - 1].cpos <= 1) {
        return true;
    }
    else if (S.snakepos[S.size - 1].cpos >= cols - 1) {
        return true;
    }
    else
        return false;
}
bool Win(snake S, int winscore) {
    return (S.size-3>=winscore);
}

void grid() {
    char sym = -37;
    for (int i = 0; i <= 80; i++) {
        gotoRowCol(i, 80);
        cout << sym;
        gotoRowCol(80, i);
        cout << sym;
        gotoRowCol(i, 0);
        cout << sym;
        gotoRowCol(0, i);
        cout << sym;
    }
}

int main1() {
    _getch();
    snake S1;
    position Food;
    init(S1);
    foodGenerate(S1, 80, 80, Food);
    grid();
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                key = _getch();
                if (key == S1.Ukey && S1.DIR != Down)
                    S1.DIR = Up;
                else if (key == S1.Dkey && S1.DIR != Up)
                    S1.DIR = Down;
                else if (key == S1.Lkey && S1.DIR != Right)
                    S1.DIR = Left;
                else if (key == S1.Rkey && S1.DIR != Left)
                    S1.DIR = Right;
            }
        }

        if (foodEaten(Food, S1)) {
            growSnake(S1, Food);
            foodGenerate(S1, 75, 75, Food);
        }
        gotoRowCol(4, 83);
        cout << "Score: " << S1.size - 3;
        hideConsoleCursor();
        EraseSnake(S1);
        boundaryCheck(S1, 80, 80);
        moveSnake(S1);
        displaySnake(S1);
        Sleep(100);
        if (Killed(S1)) {
            break;
        }
    }
    delete[] S1.snakepos;
    _getch();
    return 0;
}
int main2() {
    _getch();
    snake S1;
    position Food;
    init(S1);
    foodGenerate(S1, 80, 80, Food);
    grid();
    gotoRowCol(2, 83);
    cout << "LONELY SNAKE 2.O :(";
    gotoRowCol(3, 83);
    cout << "Dont hit yourself or the walls!";
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                key = _getch();
                if (key == S1.Ukey && S1.DIR != Down)
                    S1.DIR = Up;
                else if (key == S1.Dkey && S1.DIR != Up)
                    S1.DIR = Down;
                else if (key == S1.Lkey && S1.DIR != Right)
                    S1.DIR = Left;
                else if (key == S1.Rkey && S1.DIR != Left)
                    S1.DIR = Right;
            }
        }

        if (foodEaten(Food, S1)) {
            growSnake(S1, Food);
            foodGenerate(S1, 75, 75, Food);
        }
        gotoRowCol(5, 83);
        cout << "Score: " << S1.size - 3;
        hideConsoleCursor();
        EraseSnake(S1);
        
        moveSnake(S1);
        displaySnake(S1);
        Sleep(100);
        if (Killed(S1) || boundaryCheck2(S1, 80, 80)) {
            gotoRowCol(7, 83);
            cout << "Game Over!!";
            break;
        }
    }
    delete[] S1.snakepos;
    _getch();
    return 0;
}
int main3() {
    _getch();
    int winscore = 10;
    snake S1, S2, S3;
    position Food;
    init(S1);
    init(S2);
    init(S3);
    S2.Ukey = 119, S2.Lkey = 97, S2.Rkey = 100, S2.Dkey = 115;
    S3.Ukey = 105, S3.Lkey = 106, S3.Rkey = 108, S3.Dkey = 107;

    for (int i = 0; i < 3; i++) {
        S2.snakepos[i].rpos = 38;
        S3.snakepos[i].rpos = 42;
    }
    foodGenerate2(S1, S2, S3, 75, 75, Food);
    grid();
    gotoRowCol(2, 83);
    cout << "3 LONELY SNAKES :(";
    gotoRowCol(3, 83);
    cout << "Dont hit yourself! CONTROLS: WASD  IJKL  ARROW KEYS";
    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                key = _getch();
                if (key == S1.Ukey && S1.DIR != Down)
                    S1.DIR = Up;
                else if (key == S1.Dkey && S1.DIR != Up)
                    S1.DIR = Down;
                else if (key == S1.Lkey && S1.DIR != Right)
                    S1.DIR = Left;
                else if (key == S1.Rkey && S1.DIR != Left)
                    S1.DIR = Right;
            }
            else if (key == S2.Ukey && S2.DIR != Down)
                S2.DIR = Up;
            else if (key == S2.Dkey && S2.DIR != Up)
                S2.DIR = Down;
            else if (key == S2.Lkey && S2.DIR != Right)
                S2.DIR = Left;
            else if (key == S2.Rkey && S2.DIR != Left)
                S2.DIR = Right;

            else if (key == S3.Ukey && S3.DIR != Down)
                S3.DIR = Up;
            else if (key == S3.Dkey && S3.DIR != Up)
                S3.DIR = Down;
            else if (key == S3.Lkey && S3.DIR != Right)
                S3.DIR = Left;
            else if (key == S3.Rkey && S3.DIR != Left)
                S3.DIR = Right;

        }

        if (foodEaten(Food, S1)) {
            growSnake(S1, Food);
            foodGenerate2(S1, S2, S3, 75, 75, Food);
        }
        if (foodEaten(Food, S2)) {
            growSnake(S2, Food);
            foodGenerate2(S1, S2, S3, 75, 75, Food);
        }
        if (foodEaten(Food, S3)) {
            growSnake(S3, Food);
            foodGenerate2(S1, S2, S3, 75, 75, Food);
        }
        gotoRowCol(5, 83);
        cout << "Snake 1: " << S1.size - 3;
        gotoRowCol(7, 83);
        cout << "Snake 2: " << S2.size - 3;
        gotoRowCol(9, 83);
        cout << "Snake 3: " << S3.size - 3;
        hideConsoleCursor();
        EraseSnake(S1);
        moveSnake(S1);
        boundaryCheck(S1, 80, 80);
        displaySnake(S1);

        EraseSnake(S2);
        moveSnake(S2);
        boundaryCheck(S2, 80, 80);
        displaySnake(S2);

        EraseSnake(S3);
        moveSnake(S3);
        boundaryCheck(S3, 80, 80);
        displaySnake(S3);

        Sleep(100);
        if (Win(S1, winscore)) {
            gotoRowCol(13, 83);
            cout << "Snake 1 has won!";
            break;
        }
        if (Win(S2, winscore)) {
            gotoRowCol(13, 83);
            cout << "Snake 2 has won!";
            break;
        }

        if (Win(S3, winscore)) {
            gotoRowCol(13, 83);
            cout << "Snake 3 has won!";
            break;
        }
        if (Killed(S1)) {
            EraseSnake(S1);
            gotoRowCol(15, 83);
            cout << "Snake 1 Died";
            delete[] S1.snakepos;
            S1.size = 0;
        }

        if (Killed(S2)) {
            EraseSnake(S2);
            gotoRowCol(15, 83);
            cout << "Snake 2 Died";
            delete[] S2.snakepos;
            S2.size = 0;
        }
        if (Killed(S3)) {
            EraseSnake(S3);
            gotoRowCol(15, 83);
            cout << "Snake 3 Died";
            delete[] S3.snakepos;
            S3.size = 0;
        }


    }
    delete[] S1.snakepos;
    delete[] S2.snakepos;
    delete[] S3.snakepos;
    _getch();
    return 0;
}

void menu() {
    cout << "WELCOME TO LONELY SNAKE GAME. CHOOSE LEVEL: ";
    cout << "\n1. Lonely Snake Open World";
    cout << "\n2. Lonely Snake No Way Home (Closed boundaries)";
    cout << "\n3. Three Lonely snakes\n";
    cout << "0. Quit\n";
    cout << "GETCH IS ON SO YOU MIGHT NEED TO CLICK TO START THE GAME.\n";
}

int main()
{
    int choice;
    while (true)
    {

        menu();
        cin >> choice;
        cout << "\n";
        system("cls");
        switch (choice)
        {
        case 1:
            main1();
            break;
        case 2:
            main2();
            break;
        case 3:
            main3();
            break;
        case 0:
            cout << "Thank You";
            return 0;
        default:
            cout << "Wrong Input\n";
            break;
        }
        cout << endl;
        system("pause");
        system("cls");
    }
    return 0;
}