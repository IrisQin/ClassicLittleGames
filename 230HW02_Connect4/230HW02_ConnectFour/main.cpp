#include <iostream>
#include <string>
using namespace std;

int column, row, piecesWin;
int board[21][59];	// 59 = (maxColumn-1)*2+maxColumn+1
int height[21];	// The height of every colume
int mode; 
// 0: Basic, self-define the number of columns, rows, and number of pieces lined up that are required to win
// 1: Wrap around

int cinCheck(int min, int max) {
	int inputNum = 0;
	cin >> inputNum;
	while (!cin.good() || inputNum < min || inputNum > max)
	{
		cout << "Try again:" << endl;
		cin.clear(); // Clear error flags.
		cin.ignore(1000, '\n'); // Ignore rest.
		cin >> inputNum; // Try again.
	}
	cin.ignore(1000, '\n'); // Ignore rest.
	return inputNum;
}

bool canInsert(int col) {
	if (col < 1 || col > column) {
		return false;
	}
	if (height[col] >= row) {
		return false;
	}
	return true;
}

void insert(int col, int player) {
	int thisRow = 20 - height[col];
	board[thisRow][col + column - 1] = player;
	height[col]++;

	// Copy columns
	if (col != 1) {	// Except the first column, copy to left side
		board[thisRow][col-1] = player;
	}
	if (col != column) {	// Except the last column, copy to right side
		board[thisRow][col + column*2 - 1] = player;
	}
}

int gameOver(int col, int player, int mode) {	
	// End in a draw, return 2
	for (int i = 1; i <= column; i++) {
		if (height[i] != row) { break; }
		if (i == column&&height[i] == row) { return 2; }
	}

	// This player win, return 1
	int x = 21 - height[col];
	int y = col + column - 1;
	int count = 1;
	// Case 1: horizontal line
	// Look at left side, keep counting if the next piece is mine
	int tmpY = y - 1;
	if (mode == 0) {
		while (board[x][tmpY] == player && tmpY >= column) { count++; tmpY--; }
	}
	else {
		while (board[x][tmpY] == player ) { count++; tmpY--; }
	}
	// Then look at right side
	tmpY = y + 1;
	if (mode == 0) {
		while (board[x][tmpY] == player && tmpY <= column*2-1) { count++; tmpY++; }
	}
	else {
		while (board[x][tmpY] == player) { count++; tmpY++; }
	}
	if (count >= piecesWin) { return 1; }
	count = 1;
	// Case 2: vertical line
	// Look down, keep counting if the next piece is mine
	while (board[++x][y] == player &&x<=20&&x>0) { count++; }
	x = 21 - height[col];
	if (count >= piecesWin) { return 1; }
	count = 1;
	// Case 3: \ line
	tmpY = y - 1;
	if (mode == 0) {
		while (board[--x][tmpY] == player && tmpY >= column &&x <= 20 && x>0) { count++; tmpY--; }
	}
	else {
		while (board[--x][tmpY] == player &&x <= 20 && x>0) { count++; tmpY--; }
	}
	x = 21 - height[col];
	tmpY = y + 1;
	if (mode == 0) {
		while (board[++x][tmpY] == player && tmpY <= column * 2 - 1 && x <= 20 && x>0) { count++; tmpY++; }
	}
	else {
		while (board[++x][tmpY] == player &&x <= 20 && x>0) { count++; tmpY++; }
	}
	x = 21 - height[col];
	if (count >= piecesWin) { return 1; }
	count = 1;
	// Case 4: / line
	tmpY = y - 1;
	if (mode == 0) {
		while (board[++x][tmpY] == player && tmpY >= column &&x <= 20 && x>0) { count++; tmpY--; }
	}
	else {
		while (board[++x][tmpY] == player &&x <= 20 && x>0) { count++; tmpY--; }
	}
	x = 21 - height[col];
	tmpY = y + 1;
	if (mode == 0) {
		while (board[--x][tmpY] == player && tmpY <= column * 2 - 1 && x <= 20 && x>0) { count++; tmpY++; }
	}
	else {
		while (board[--x][tmpY] == player &&x <= 20 && x>0) { count++; tmpY++; }
	}
	x = 21 - height[col];
	if (count >= piecesWin) { return 1; }
	count = 1;

	// Game continue, return 0
	return 0;

}

void printBoard() {
	for (int i = 1; i <= column; i++) {
		cout << i;
	}
	cout << endl;
	for (int i = 1; i <= row; i++) {
		for (int j = column; j <= column*2-1; j++) {
			if (board[20 - row + i][j] == 0) { cout << "."; }
			if (board[20 - row + i][j] == 1) { cout << "X"; }
			if (board[20 - row + i][j] == 2) { cout << "O"; }
		}
		cout << endl;
	}
}

int main() {
	while (true) {

		// Initialize the board, 0: no piece, 1: playerX, 2: playerO
		memset(board, 0, sizeof(board));
		memset(height, 0, sizeof(height));
		cout << "Please enter the number of columns(4~20):" << endl;
		column = cinCheck(4, 20);
		cout << "Please enter the number of rows(4~20):" << endl;
		row = cinCheck(4, 20);
		cout << "Please enter the number of pieces required to win(3~20):" << endl;
		piecesWin = cinCheck(3, 20);
		cout << "Please select a mode (0: Basic, 1: Wrap around):" << endl;
		mode = cinCheck(0, 1);


		int player = 1; // PlayerX first
		while (true) {
			printBoard();
			cout << "Player " << player << " choose a column:" << endl;
			int col;
			cin >> col;
			while (!canInsert(col) || !cin.good()) {
				cin.clear(); // Clear error flags.
				cin.ignore(1000, '\n'); // Ignore rest.
				cout << "Try again:" << endl;
				cin >> col;
			}
			insert(col, player);
			if (gameOver(col, player, mode) == 2) { printBoard(); cout << "The game end in a draw." << endl;  break; }
			if (gameOver(col, player, mode) == 1) { printBoard(); cout << "Player " << player << " wins." << endl;  break; }
			player = 3 - player;
		}

		cout << "Do you want to play another game?(yes/no)" << endl;
		string inputStr;
		cin >> inputStr;
		while (!cin.good()||(inputStr != "yes"&&inputStr != "no"))
		{
			cout << "Try again:" << endl;
			cin.clear(); // Clear error flags.
			cin.ignore(1000, '\n'); // Ignore rest.
			cin >> inputStr; // Try again.
		}
		cin.ignore(1000, '\n'); // Ignore rest.
		if (inputStr == "no") { break; }
	
	}
	
	system("pause");

	return 0;
}