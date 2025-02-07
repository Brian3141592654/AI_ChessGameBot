#include <iostream>
#include <conio.h> // For getch()
#include <ctime>   // For srand(time(NULL))
#include <windows.h> // For Sleep() and system("cls")
#include <cstdlib> // For exit()
using namespace std;

// Structure to represent a point on the board
struct point {
    int y; // Row
    int x; // Column
};

// Structure to represent a direction (change in y and x)
struct direction {
    int dy; // Change in row
    int dx; // Change in column
};

// Define constants for directions (right, down, down-left, down-right)
const direction d1 = {0, 1};  // Right
const direction d2 = {1, 0};  // Down
const direction d3 = {1, -1}; // Down-Left
const direction d4 = {1, 1};  // Down-Right

class a {
private:
    int record_position[15][15]; // Stores the game board state (0: empty, 1: player, 2: computer)
    int player;                 // Player's mark (1 or 2)
    int computer;               // Computer's mark (1 or 2)
    int Y, X;                   // Current cursor position on the board
    point position;             // The last move made by either player or computer
public:
    void play();                // Starts the game
    int mode();                 // Sets the game mode (exit, player vs computer, computer vs player)
    void playerT();             // Handles the player's turn
    void computerT();           // Handles the computer's turn
    bool WinOrNot();            // Checks if there is a winner
protected:
    bool inBoard(point p);      // Checks if a point is within the board boundaries
    void display();             // Displays the game board
    int score(point p, int who); // Calculates the score for a given position for a player (or computer)
    point newPoint(point p, direction d, int lenth); // Calculates a new point given a starting point, direction, and length
};

// Sets the game mode and initializes the board
int a::mode() {
    cout << "0: Exit" << endl;
    cout << "1: Computer vs Player" << endl;
    cout << "2: Player vs Computer" << endl;
    while (true) {
        int i;
        cin >> i;
        if (i == 0) {
            exit(1); // Exit the game
        } else if (i == 1 || i == 2) { // Valid mode selection
            X = 9; // Initialize cursor position
            Y = 9;
            // Initialize the game board to empty
            for (int j = 0; j < 15; j++) {
                for (int k = 0; k < 15; k++) {
                    record_position[j][k] = 0;
                }
            }
            display(); // Display the initial board
            return i; // Return the chosen mode
        } else {
            cout << "Invalid Input" << endl;
        }
    }
}

// Main game loop
void a::play() {
    int i = mode(); // Get the game mode
    if (i == 1) { // Computer vs Player
        computer = 1;
        player = 2;
    } else { // Player vs Computer
        player = 1;
        computer = 2;
    }
    int cur = 1; // Current player (1 or 2)

    while (true) {
        if (cur == player) {
            playerT(); // Player's turn
        } else {
            computerT(); // Computer's turn
        }
        if (WinOrNot()) { // Check for a winner
            if (cur == player) {
                cout << "Player wins!";
                system("pause"); // Pause the console
                break;
            } else {
                cout << "Computer wins!";
                system("pause");
                break;
            }
        } else {
            cur = (cur == 1) ? 2 : 1; // Switch players
        }
    }
}

// Handles the player's turn
void a::playerT() {
    cout << "Player's turn" << endl;
    while (true) {
        char c = getch(); // Get character input without echoing
        if (c == 'w') { // Move cursor up
            if (Y != 0) {
                Y--;
                display();
            }
        } else if (c == 's') { // Move cursor down
            if (Y != 14) {
                Y++;
                display();
            }
        } else if (c == 'a') { // Move cursor left
            if (X != 0) {
                X--;
                display();
            }
        } else if (c == 'd') { // Move cursor right
            if (X != 14) {
                X++;
                display();
            }
        } else if (c == 'j' && record_position[Y][X] == 0) { // Place mark
            position.x = X;
            position.y = Y;
            record_position[Y][X] = player;
            display();
            break; // End player's turn
        }
    }
}

// Handles the computer's turn
void a::computerT() {
    cout << "Computer's turn" << endl;
    point best1, best2;

    // Find a random empty position for initial best points
    do {
        srand(time(NULL)); // Seed the random number generator
        best1.y = best2.y = rand() % 15;
        best1.x = best2.x = rand() % 15;
    } while (record_position[best1.y][best1.x] != 0);

    int a1 = score(best1, computer), b1 = score(best1, player); // Score for computer's move
    // Iterate through the board to find the best move for computer
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (record_position[i][j] != 0) {
                continue; // Skip occupied cells
            }
            point cur = {i, j};
            int m1 = score(cur, computer); // Score for computer
            int m2 = score(cur, player);   // Score for player (to block)
            if (m1 > a1) {
                best1 = cur;
                a1 = m1;
                b1 = m2;
            } else if (m1 == a1) {
                if (m2 > b1) {
                    best1 = cur;
                    b1 = m2;
                }
            }
        }
    }

    int a2 = score(best2, player), b2 = score(best2, computer); // Score for player's move
    // Iterate through the board to find the best move to block the player
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (record_position[i][j] != 0) {
                continue;
            }
            point cur = {i, j};
            int m1 = score(cur, player);   // Score for player
            int m2 = score(cur, computer); // Score for computer (to avoid bad moves)
            if (m1 > a2) {
                best2 = cur;
                a2 = m1;
                b2 = m2;
            } else if (m1 == a2) {
                if (m2 > b2) {
                    best2 = cur;
                    b2 = m2;
                }
            }
        }
    }

    if (a1 >= a2) {
        position = best1; // Choose the best move for computer
    } else {
        position = best2; // Choose the best move to block the player
    }
    record_position[position.y][position.x] = computer;
    cout << position.y << "," << position.x << endl; // Output computer move
    Sleep(2000); // Wait 2 seconds
    display();
}

// 評估函數: 判斷當前點的分數 (Evaluates the score of a position)
int a::score(point p, int who) {
    // 初始化計分變數 (Initialize score variables)
    int win5 = 0, // 5連子 (5 in a row)
        alive4 = 0, // 活4 (Live 4)
        die4 = 0, // 死4 (Dead 4)
        ddie4 = 0, // 雙死4 (Double dead 4)
        alive3 = 0, // 活3 (Live 3)
        dalive3 = 0, // 雙活3 (Double live 3)
        die3 = 0, // 死3 (Dead 3)
        alive2 = 0, // 活2 (Live 2)
        dalive2 = 0, // 雙活2 (Double live 2)
        die2 = 0, // 死2 (Dead 2)
        nothing = 0; // 無 (None)

    // 設置對手棋子 (Set opponent's piece value)
    int opp = (who == 1) ? 2 : 1;

    // 檢查四個方向 (Check four directions)
    for (int i = 1; i <= 4; i++) {
        direction d;
        switch (i) {
            case 1: d = d1; break;
            case 2: d = d2; break;
            case 3: d = d3; break;
            case 4: d = d4; break;
        }

        // 初始化變數 (Initialize variables)
        int l = 1; // 連續棋子數 (Number of consecutive pieces)
        point le, ri, p1; // 左邊界, 右邊界, 臨時點 (Left boundary, right boundary, temporary point)
        int left[5], right[5]; // 左邊和右邊的棋子狀態 (Left and right pieces status)

        // 檢查左邊 (Check left side)
        p1 = newPoint(p, d, -1);
        le = p;
        while (inBoard(p1) && record_position[p1.y][p1.x] == who) {
            le = p1;
            p1 = newPoint(p1, d, -1);
            l++;
        }

        // 檢查右邊 (Check right side)
        p1 = newPoint(p, d, 1);
        ri = p;
        while (inBoard(p1) && record_position[p1.y][p1.x] == who) {
            ri = p1;
            p1 = newPoint(p1, d, 1);
            l++;
        }

        // 紀錄左右邊的狀態 (Record left and right status)
        for (int j = 1; j <= 4; j++) {
            p1 = newPoint(le, d, -j);
            left[j] = inBoard(p1) ? record_position[p1.y][p1.x] : opp;
            p1 = newPoint(ri, d, j);
            right[j] = inBoard(p1) ? record_position[p1.y][p1.x] : opp;
        }

        // 根據不同連續數判斷分數 (Evaluate score based on piece sequences)
        if (l == 5) {
            win5++;
        } else if (l == 4) {
            if (left[1] == 0 && right[1] == 0) alive4++;
            else if (left[1] == 0 || right[1] == 0) die4++;
            else nothing++;
        } else if (l == 3) {
            if ((left[1] == 0 && left[2] == who) || (right[1] == 0 && right[2] == who)) ddie4++;
            else if (left[1] == 0 && right[1] == 0 && (left[2] == 0 || right[2] == 0)) alive3++;
            else if ((left[1] == 0 && left[2] == 0) || (right[1] == 0 && right[2] == 0)) die3++;
            else nothing++;
        }
    }
    // 回傳分數 (Return score)
    if (win5 >= 1) return 14;
    if (alive4 >= 1 || die4 >= 2 || (die4 >= 1 && alive3 >= 1)) return 13;
    if (alive3 >= 2) return 12;
    if (die3 >= 1 && alive3 >= 1) return 11;
    if (die4 >= 1) return 10;
    if (ddie4 >= 1) return 9;
    if (alive3 >= 1) return 8;
    if (dalive3 >= 1) return 7;
    if (alive2 >= 2) return 6;
    if (alive2 >= 1) return 5;
    if (dalive2 >= 1) return 4;
    if (die3 >= 1) return 3;
    if (die2 >= 1) return 2;
    return 1;
}

// 判斷是否勝利 (Check if win condition is met)
bool a::WinOrNot() {
    for (int i = 1; i <= 4; i++) {
        direction d;
        int count = 0;
        switch (i) {
            case 1: d = d1; break;
            case 2: d = d2; break;
            case 3: d = d3; break;
            case 4: d = d4; break;
        }
        for (int j = -4; j <= 4; j++) {
            point p1 = newPoint(position, d, j);
            if (inBoard(p1) && record_position[p1.y][p1.x] == record_position[position.y][position.x]) {
                count++;
            } else {
                count = 0;
            }
            if (count == 5) return true;
        }
    }
    return false;
}

// 判斷座標是否在棋盤內 (Checks if a coordinate is within the board)
bool a::inBoard(point p){
    if(p.y >= 0 && p.y < 15 && p.x >= 0 && p.x < 15){
        return true;
    }
    else{
        return false;
    }
}

// 根據方向和長度創建新座標 (Creates a new coordinate based on direction and length)
point a::newPoint (point p, direction d, int lenth){
    point p1 = {p.y + d.dy * lenth, p.x + d.dx * lenth};
    return p1;
}

// 顯示棋盤 (Displays the game board)
void a::display(){
    system("cls"); // 清屏 (Clears the screen)
    for(int i = 0; i < 15; i ++){
        for(int j = 0; j < 15; j ++){
            if(i == Y && j == X){
                cout << "��"; // 當前游標位置 (Current cursor position)
            }
            else if(record_position[i][j] == 1){
                cout << "��"; // 玩家1的棋子 (Player 1's piece)
            }
            else if(record_position[i][j] == 2){
                cout << "��"; // 玩家2的棋子 (Player 2's piece)
            }
            else{
                cout << ". "; // 空位 (Empty space)
            }
            
        }
        cout << endl;
    }
}

int main(){
    a game; // 創建遊戲物件 (Creates a game object)
    game.play(); // 開始遊戲 (Starts the game)
    return 0;
}
