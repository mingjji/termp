#include <iostream>
#include <string>
#include <thread>
#include <random>
#include <Windows.h>

// 단어와 패딩을 가지는 구조체
struct WordLine {
    std::string content;
    int padding;
};

// 25개의 단어 라인을 담는 배열
struct WordLine wordLines[25];
int wordType = 1;        // 단어 유형을 결정하는 변수 (1: 알파벳, 2: 숫자, ..., 7: 알파벳 + 숫자 + 특수문자)
int difficulty = 0;      // 난이도를 나타내는 변수
int health = 100;        // 플레이어의 체력
int playerScore = 0;     // 플레이어의 점수
std::string displayText; // 화면에 표시되는 텍스트

// 함수 선언
int getRandomNumber(int a, int b);
void moveToCoordinates(int x, int y);
void displayScreen();
void getUserInput();
std::string generateRandomWord(int length);
char generateRandomAlphabet();
char generateRandomNumber();
char generateRandomSpecialCharacter();
struct WordLine generateWordLine();
void initializeWordLines();
void addNewWordLine();
void checkUserInput(std::string userInput);
void hideCursor();

int main() {
    hideCursor();

    // 게임 메뉴 루프
    while (1) {
        int menuOption = 0;
        printf("\n\n                 <산성비 타자 게임>\n           \n\n");
        printf("                    1.게임 시작\n                    2.게임 종료\n");
        printf("\n번호 선택 : ");
        std::cin >> menuOption;

        if (menuOption == 1) {
            system("cls");

            // 게임 모드 선택 루프
            while (1) {
                int gameMode;
                printf("\n\n                 <모드 선택>\n           \n\n");
                printf("                1. 알파벳\n                2. 숫자\n                3. 특수문자\n                4. 알파벳 + 숫자\n\
                5. 알파벳 + 특수문자\n                6. 숫자 + 특수문자\n                7. 알파벳 + 숫자 + 특수문자\n");
                printf("\n모드 선택 : ");
                std::cin >> gameMode;

                if (gameMode > 0 && gameMode < 8) {
                    wordType = gameMode;
                    break;
                }
                else {
                    printf("다시 입력해주세요.\n");
                }
            }

            system("cls");
            std::thread t1(displayScreen);
            std::thread t2(getUserInput);
            t1.join();
            t2.join();
            return 0;
        }
        else if (menuOption == 2) {
            return 0;
        }
        else {
            printf("다시 입력해주세요.\n");
        }
    }
    return 0;
}

// 난수 생성 함수
int getRandomNumber(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(a, b);
    return dis(gen);
}

// 화면 좌표 이동 함수
void moveToCoordinates(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 화면 표시 함수
void displayScreen() {
    initializeWordLines();
    addNewWordLine();
    moveToCoordinates(7, 28);

    // 게임 루프
    while (1) {
        displayText = "HP :  " + std::to_string(health) + "\tSCORE :" + std::to_string(playerScore) + "\n";

        displayText += "|";
        for (int k = 0; k < 60; k++) {
            displayText += "-";
        }
        displayText += "|\n";

        if (health <= 0) {
            break;
        }

        for (int j = 0; j < 25; j++) {
            displayText += "|";
            for (int k = 0; k < wordLines[j].padding; k++) {
                displayText += " ";
            }
            displayText += wordLines[j].content;
            for (int k = 0; k + wordLines[j].padding + wordLines[j].content.length() < 60; k++) {
                displayText += " ";
            }

            displayText += "|";
            displayText += "\n";
        }

        for (int k = 0; k < 62; k++) {
            displayText += "~";
        }
        displayText += "\n입력 :  ";

        printf("\x1b[s");
        moveToCoordinates(0, 0);
        std::cout << displayText;

        printf("\x1b[u");

        // difficulty에 따라 Sleep 시간 동적 조정
        Sleep(1200 - difficulty * 20);
        addNewWordLine();
    }
}

// 사용자 입력을 받는 함수
void getUserInput() {
    while (1) {
        if (health <= 0) {
            break;
        }

        std::string userInput;
        std::cin >> userInput;

        checkUserInput(userInput);

        system("cls");
        std::cout << displayText;
        moveToCoordinates(7, 28);
    }
}

// 랜덤 단어 생성 함수
std::string generateRandomWord(int length) {
    std::string word = "";

    if (wordType == 1) {
        for (int i = 0; i < length; i++) {
            word += generateRandomAlphabet();
        }
    }
    else if (wordType == 2) {
        for (int i = 0; i < length; i++) {
            word += generateRandomNumber();
        }
    }
    else if (wordType == 3) {
        for (int i = 0; i < length; i++) {
            word += generateRandomSpecialCharacter();
        }
    }
    else if (wordType == 4) {
        for (int i = 0; i < length / 2; i++) {
            word += generateRandomAlphabet();
            word += generateRandomNumber();
        }
    }
    else if (wordType == 5) {
        for (int i = 0; i < length / 2; i++) {
            word += generateRandomAlphabet();
            word += generateRandomSpecialCharacter();
        }
    }
    else if (wordType == 6) {
        for (int i = 0; i < length / 2; i++) {
            word += generateRandomNumber();
            word += generateRandomSpecialCharacter();
        }
    }
    else if (wordType == 7) {
        for (int i = 0; i < length / 3; i++) {
            word += generateRandomAlphabet();
            word += generateRandomNumber();
            word += generateRandomSpecialCharacter();
        }
    }
    return word;
}

// 랜덤 알파벳 생성 함수
char generateRandomAlphabet() {
    return static_cast<char>(getRandomNumber(97, 122));
}

// 랜덤 숫자 생성 함수
char generateRandomNumber() {
    return static_cast<char>(getRandomNumber(48, 57));
}

// 랜덤 특수문자 생성 함수
char generateRandomSpecialCharacter() {
    return static_cast<char>(getRandomNumber(33, 47));
}

// 단어 라인 생성 함수
struct WordLine generateWordLine() {
    struct WordLine tempWordLine;
    int ranPadding = getRandomNumber(0, 51);
    int ranLength = getRandomNumber(1, 9);
    tempWordLine.content = generateRandomWord(ranLength);
    tempWordLine.padding = ranPadding;
    return tempWordLine;
}

// 단어 라인 배열 초기화 함수
void initializeWordLines() {
    for (int i = 0; i < 10; i++) {
        wordLines[i].padding = 0;
        wordLines[i].content = "";
    }
}

// 새로운 단어 라인 추가 함수
void addNewWordLine() {
    if (!(wordLines[24].content.empty())) {
        health -= 10;
        if (health <= 0) {
            return;
        }
    }
    for (int i = 23; i >= 0; i--) {
        wordLines[i + 1].padding = wordLines[i].padding;
        wordLines[i + 1].content = wordLines[i].content;
    }
    if (getRandomNumber(0, 2) == 0) {
        wordLines[0] = generateWordLine();
    }
    else {
        wordLines[0].padding = 0;
        wordLines[0].content = "";
    }
}

// 사용자 입력 확인 함수
void checkUserInput(std::string userInput) {
    for (int i = 0; i < 25; i++) {
        if (userInput.compare(wordLines[i].content) == 0) {
            playerScore += 10;
            difficulty++;
            wordLines[i].content = "";
            wordLines[i].padding = 0;
        }
    }
}

// 콘솔 커서 숨기기 함수
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
