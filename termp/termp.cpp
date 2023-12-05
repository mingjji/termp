#include <iostream>
#include <string>
#include <thread>
#include <random>
#include <Windows.h>

// �ܾ�� �е��� ������ ����ü
struct WordLine {
    std::string content;
    int padding;
};

// 25���� �ܾ� ������ ��� �迭
struct WordLine wordLines[25];
int wordType = 1;        // �ܾ� ������ �����ϴ� ���� (1: ���ĺ�, 2: ����, ..., 7: ���ĺ� + ���� + Ư������)
int difficulty = 0;      // ���̵��� ��Ÿ���� ����
int health = 100;        // �÷��̾��� ü��
int playerScore = 0;     // �÷��̾��� ����
std::string displayText; // ȭ�鿡 ǥ�õǴ� �ؽ�Ʈ

// �Լ� ����
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

    // ���� �޴� ����
    while (1) {
        int menuOption = 0;
        printf("\n\n                 <�꼺�� Ÿ�� ����>\n           \n\n");
        printf("                    1.���� ����\n                    2.���� ����\n");
        printf("\n��ȣ ���� : ");
        std::cin >> menuOption;

        if (menuOption == 1) {
            system("cls");

            // ���� ��� ���� ����
            while (1) {
                int gameMode;
                printf("\n\n                 <��� ����>\n           \n\n");
                printf("                1. ���ĺ�\n                2. ����\n                3. Ư������\n                4. ���ĺ� + ����\n\
                5. ���ĺ� + Ư������\n                6. ���� + Ư������\n                7. ���ĺ� + ���� + Ư������\n");
                printf("\n��� ���� : ");
                std::cin >> gameMode;

                if (gameMode > 0 && gameMode < 8) {
                    wordType = gameMode;
                    break;
                }
                else {
                    printf("�ٽ� �Է����ּ���.\n");
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
            printf("�ٽ� �Է����ּ���.\n");
        }
    }
    return 0;
}

// ���� ���� �Լ�
int getRandomNumber(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(a, b);
    return dis(gen);
}

// ȭ�� ��ǥ �̵� �Լ�
void moveToCoordinates(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// ȭ�� ǥ�� �Լ�
void displayScreen() {
    initializeWordLines();
    addNewWordLine();
    moveToCoordinates(7, 28);

    // ���� ����
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
        displayText += "\n�Է� :  ";

        printf("\x1b[s");
        moveToCoordinates(0, 0);
        std::cout << displayText;

        printf("\x1b[u");

        // difficulty�� ���� Sleep �ð� ���� ����
        Sleep(1200 - difficulty * 20);
        addNewWordLine();
    }
}

// ����� �Է��� �޴� �Լ�
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

// ���� �ܾ� ���� �Լ�
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

// ���� ���ĺ� ���� �Լ�
char generateRandomAlphabet() {
    return static_cast<char>(getRandomNumber(97, 122));
}

// ���� ���� ���� �Լ�
char generateRandomNumber() {
    return static_cast<char>(getRandomNumber(48, 57));
}

// ���� Ư������ ���� �Լ�
char generateRandomSpecialCharacter() {
    return static_cast<char>(getRandomNumber(33, 47));
}

// �ܾ� ���� ���� �Լ�
struct WordLine generateWordLine() {
    struct WordLine tempWordLine;
    int ranPadding = getRandomNumber(0, 51);
    int ranLength = getRandomNumber(1, 9);
    tempWordLine.content = generateRandomWord(ranLength);
    tempWordLine.padding = ranPadding;
    return tempWordLine;
}

// �ܾ� ���� �迭 �ʱ�ȭ �Լ�
void initializeWordLines() {
    for (int i = 0; i < 10; i++) {
        wordLines[i].padding = 0;
        wordLines[i].content = "";
    }
}

// ���ο� �ܾ� ���� �߰� �Լ�
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

// ����� �Է� Ȯ�� �Լ�
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

// �ܼ� Ŀ�� ����� �Լ�
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
