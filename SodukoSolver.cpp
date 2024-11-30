#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#define GRID_SIZE 9
#define CELL_SIZE 100

using namespace std;

// Function to draw the Sudoku grid and numbers
void drawGrid(sf::RenderWindow& window, sf::Font& font, const vector<vector<int>>& grid, int selectedRow, int selectedCol, const string& statusMessage) {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cell.setOutlineThickness(2);
    cell.setOutlineColor(sf::Color::Black);

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            cell.setPosition(j * CELL_SIZE, i * CELL_SIZE);
            // Highlight the selected cell
            if (i == selectedRow && j == selectedCol) {
                cell.setFillColor(sf::Color(200, 160, 255)); // Light blue highlight
            }
            else {
                cell.setFillColor(sf::Color::White);
            }
            window.draw(cell);

            // Display the numbers
            if (grid[i][j] != 0) {
                sf::Text number;
                number.setFont(font);
                number.setString(to_string(grid[i][j]));
                number.setCharacterSize(72);
                number.setFillColor(sf::Color::Black);
                number.setPosition(j * CELL_SIZE + 30, i * CELL_SIZE);
                window.draw(number);
            }
        }
    }

    // Display the status message
    sf::Text statusText;
    statusText.setFont(font);
    statusText.setString(statusMessage);
    statusText.setCharacterSize(30);
    statusText.setFillColor(sf::Color::Black);
    statusText.setPosition(10, GRID_SIZE * CELL_SIZE + 10); // Position below the grid
    window.draw(statusText);
}

// Function to check if it's valid to place a number in a cell
bool isSafe(vector<vector<int>>& grid, int row, int col, int num) {
    for (int x = 0; x < GRID_SIZE; x++) {
        if (grid[row][x] == num || grid[x][col] == num) return false;
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[startRow + i][startCol + j] == num) return false;
        }
    }
    return true;
}

// Function to solve Sudoku using backtracking
bool solveSudoku(vector<vector<int>>& grid) {
    int row = -1, col = -1;
    bool isEmpty = true;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == 0) {
                row = i;
                col = j;
                isEmpty = false;
                break;
            }
        }
        if (!isEmpty) break;
    }
    if (isEmpty) return true;

    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;
            if (solveSudoku(grid)) return true;
            grid[row][col] = 0;
        }
    }
    return false;
}

int main() {
    vector<vector<int>> grid(9, vector<int>(9, 0)); // Empty grid
    int selectedRow = 0, selectedCol = 0;
    string statusMessage = "Press Enter to solve.";

    sf::RenderWindow window(sf::VideoMode(900, 950), "Sudoku Solver");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Failed to load font!" << endl;
        return 1;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Mouse click to select a cell
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / CELL_SIZE;
                int y = event.mouseButton.y / CELL_SIZE;
                if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                    selectedRow = y;
                    selectedCol = x;
                }
            }

            // Keyboard input to set numbers in the selected cell
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                    int num = event.key.code - sf::Keyboard::Num1 + 1;
                    grid[selectedRow][selectedCol] = num; // Set the number
                }
                else if (event.key.code == sf::Keyboard::BackSpace || event.key.code == sf::Keyboard::Delete) {
                    grid[selectedRow][selectedCol] = 0; // Clear the cell
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (solveSudoku(grid)) {
                        statusMessage = "Sudoku solved successfully!";
                    }
                    else {
                        statusMessage = "No solution exists!";
                    }
                }
            }
        }

        // Render the grid
        window.clear(sf::Color::White);
        drawGrid(window, font, grid, selectedRow, selectedCol, statusMessage);
        window.display();
    }

    return 0;
}
