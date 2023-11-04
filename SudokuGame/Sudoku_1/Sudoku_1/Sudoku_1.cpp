#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include "windows.h"

using namespace std;

const int MAX_ROWS = 9;
const int MAX_COLS = 9;
const int EASY_ZEROS = 1; //quantity of numbers that will be exchanged depending on game mode
const int MEDIUM_ZEROS = 45;
const int HARD_ZEROS = 60;


class Sudoku {
public:

    int wrongCounter = 0;
    int boardOrig[MAX_ROWS][MAX_COLS] = { 0 };
    int board[MAX_ROWS][MAX_COLS] = { 0 };
    time_t timestart;

    void startTimer()
    {
        time_t start = time(nullptr); 
        timestart = start;
    }
    
    
    //select random board for game
    string selectBoard()
    {
        srand(time(0));
        int boardFileNum = (rand() % 4) + 1;
        string boardNumStr = to_string(boardFileNum);
        return ("board"+boardNumStr + ".txt");

    }

    //gets board for player from text file into array
    void boardFromFile(string fileName) 
    {
        ifstream boardFile(fileName);
        if (!boardFile) 
        {
            cout << "Error!" << endl;
            return;
        }

        for (int i = 0; i < MAX_ROWS; i++) 
        {
            for (int j = 0; j < MAX_COLS; j++) 
            {
                boardFile >> boardOrig[i][j];
            }
        }
        for (int i = 0; i < MAX_ROWS; i++) {
            for (int j = 0; j < MAX_COLS; j++) {
                board[i][j] = boardOrig[i][j];
            }
        }
        boardFile.close();
       
    }

    //exchanges random elements of array to zero; quantity of exchanged zeros depends on game mode 
    void randZeros(int zeroQuantity) 
    {
        srand(time(0));

        for (int i = 0; i < zeroQuantity; i++) {
            int nullRow = rand() % MAX_ROWS;
            int nullCol = rand() % MAX_COLS;
            board[nullRow][nullCol] = 0;
        }
    }
    //shows board for playing that updates
    void showBoard() 
    {
        //shows rows grid from 1 to 9
        cout << " ";
        for (int i = 0; i < MAX_ROWS; i++) 
        {
            if (i == 0 || i == 3 || i == 6) 
            {
                cout << "  ";
            }

            cout << i << " ";
        }
        
        cout << endl << endl;
        //shows column grid from 1 to 9
        for (int i = 0; i < MAX_ROWS; i++) 
        {
            cout << i << "  ";

            //shows numbers and shows " " instead of 0
            for (int j = 0; j < MAX_COLS; j++) 
            {
                if (board[i][j] == 0) 
                {
                    cout << "  ";
                }

                else 
                {
                    cout << board[i][j] << " ";
                }

                if (j == 2 || j == 5) 
                {
                    cout << "| ";
                }
            }

            cout << endl;

            if (i == 2 || i == 5) 
            {
                cout << "   ----- + ----- + -----" << endl;
            }
        }

        cout << endl;
    }

    void welcomeScr()
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        cout << " WELCOME TO SUDOKU GAME" << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "Levels: " << endl;
        cout << "1. Easy Level" << endl;
        cout << "2. Medium Level" << endl;
        cout << "3. Hard Level" << endl << endl;
        cout << "Enter number 1-3 to choose mode" << endl;
    }

    //to run game
    void playSudoku() 
    {
        int game_level = 3;
        cin >> game_level;
        while (cin.fail() || game_level < 1 || game_level>3) {
            cin.clear();
            cin.ignore(10000, '\n'); // ignore 100 characters of input until the newline character
            cout << "Please enter number between 1 and 3: ";
            cin >> game_level;
        }
        string currentBoard = selectBoard();
        system("CLS");
        switch (game_level)
        {
        case 1:
            cout << "Easy mode" << endl;
            boardFromFile(currentBoard);
            randZeros(EASY_ZEROS);
            showBoard();            
            guessNum();
            break;
        case 2:
            cout << "Medium mode" << endl;
            boardFromFile(currentBoard);
            randZeros(MEDIUM_ZEROS);
            showBoard();
            guessNum();
            break;
        case 3:
            cout << "Hard mode" << endl;
            boardFromFile(currentBoard);
            randZeros(HARD_ZEROS);
            showBoard();
            guessNum();

            break;
        default:
            break;
        }
    }
    bool ifGameFinished()
    {
        bool hasZeros = false;
        for (int i = 0; i < MAX_ROWS; i++)
        {
            for (int j = 0; j < MAX_COLS; j++)
            {
                if (board[i][j] == 0)
                {
                    hasZeros = true;
                    break;
                }
            }
            if (hasZeros)
            {
                break;
            }
        }

        if (!hasZeros)
        {
            cout << "Congratulations! You have solved the Sudoku puzzle!" << endl;
            time_t end = time(nullptr);
            time_t timeTaken = end - timestart;
            Sleep(1000);
            cout << "Time taken: " << timeTaken / 60 << " m " << timeTaken % 60 << " s " << endl;
            int answer;
            cout << "Do you want to play again? [1] yes, [0] no ";
            cin >> answer;
            while (cin.fail() || answer < 0 || answer > 1)
            {
                cin.clear();
                cin.ignore(10000, '\n'); // ignore 100 characters of input until the newline character
                cout << "Invalid input. Please enter [1] yes, [0] no ";
                cin >> answer;
            }
            if (answer == 1)
            {
                wrongCounter = 0;
                system("cls");
                welcomeScr();
                playSudoku();

            }
            else if (answer == 0) 
            {
                cout << "Thank you for game :)";
                Sleep(1000);
                system("cls");
                exit(0);

            }
            

            
        }
        return false;
    }
    //checks if guess matches with element on original board
    void guessNum()
    {
        bool continueLoop = true;
        startTimer();

        while (continueLoop)
        {
            int row;
            int col;
            int guess;

            cout << "Type row of number you want to add: ";
            cin >> row;
            while (cin.fail() || row < 0 || row > 8)
            {
                cin.clear();
                cin.ignore(10000, '\n'); // ignore 100 characters of input until the newline character
                cout << "Invalid input. Please enter a row number between 0 and 8: ";
                cin >> row;
            }

            cout << "Type column of number you want to add: ";
            cin >> col;
            while (cin.fail() || col < 0 || col > 8)
            {
                cin.clear();
                cin.ignore(10000, '\n'); // ignore 100 characters of input until the newline character
                cout << "Invalid column number. Please enter numbers between 0 and 8." << endl;
                cin >> col;
            }
            cout << "What's number on " << row << " row and " << col << " column? ";
            cin >> guess;
            while (cin.fail() || guess < 1 || guess > 9)
            {
                cin.clear();
                cin.ignore(10000, '\n'); // ignore 100 characters of input until the newline character
                cout << "Invalid number. Please enter a number between 1 and 9." << endl;
                cin >> guess;
            }
            cout << endl;

            //if user's guess matches with original board and this element isn't filled already - it updates players board
            if (boardOrig[row][col] == guess && boardOrig[row][col] != board[row][col])
            {
                system("Color 0A");
                cout << "You are correct!!!";
                Sleep(1000);
                board[row][col] = guess;
                system("CLS");
                system("Color 07");
                // Print the updated array
                showBoard();
                continueLoop = !ifGameFinished();
            }
            //if this element is alreasy filled - asks again
            else if (boardOrig[row][col] == board[row][col])
            {
                system("Color 04");
                cout << "The cell is already filled! Try again" << endl;
                Sleep(500);
                system("Color 07");
                guessNum();
            }
            else {
                system("Color 04");
                cout << "Sorry, you are wrong :( You have " << 2 - wrongCounter << " attempts left." << endl;
                wrongCounter++;
                Sleep(500);
                system("Color 07");
                if (wrongCounter == 3) {
                    loserScr();
                    Sleep(1000);
                    playSudoku();
                    break;
                }
                else {
                    guessNum();
                }
            }
        }
    }
    void loserScr()
    {

        system("CLS");

        cout << "You've lost the game!!!" << endl;
        int answer;
        cout << "Do you want to play again? [1] yes, [0] no";
        cin >> answer;
        while (cin.fail() || answer < 0 || answer > 1)
        {
            cin.clear();
            cin.ignore(10000, '\n'); // ignore 100 characters of input until the newline character
            cout << "Invalid input. Please enter [1] yes, [0] no ";
            cin >> answer;
        }
        if (answer == 1)
        {
            wrongCounter = 0;
            system("cls");
            welcomeScr();
            playSudoku();

        }
        else if (answer == 0)
        {
            cout << "Thank you for game :)";
            Sleep(1000);
            system("cls");
            exit(0);

        }
    }

};




int main()
{
    Sudoku ch;

    ch.welcomeScr();
    
    ch.playSudoku();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file