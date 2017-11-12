#include<iostream>
#include<fstream>
#include<SFML\Graphics.hpp>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#define ESC "\x1b["

using namespace std;
ifstream inFile;

//Variables, constants and parameters//
char charIn; //For storing the characters of the external file//
bool doneReading; //Determines if the program is done with reading//
bool gameOver = false;
const int gridsize = 1000; //Size of the grid in the matrix//
int height = 25; //Screen height//
int width = 50; //Screen width//
bool curGenActive; //Determines the state of the current generation of a cell//
bool nextGenActive; //Determines the state of the following generation of a cell//
bool curGen[gridsize][gridsize] = { false }; //Two-state matrix for the current generation of cells//
bool nextGen[gridsize][gridsize] = { false }; //Two-state matrix for the next generation of cells//
char cell; //Parameter for the character of a cell//
string inFileName;
string semiCool = ";";
bool reset = true;

struct cursor {
	int i = 12;
	int j = 25;
	char sign = '+';
};
cursor Corsair;

char readFile() { //Reads a file//
	charIn = inFile.get();
	if (!doneReading) {
		if (inFile.eof()) {
			inFile.close();
			doneReading = true;
			return 'e';
		}
		else {
			return charIn;
		}
		charIn = inFile.get();
	}
	return 'e';
}

void loadFile() {
	cout << "What kind of pattern would you like to open?\n[s]till lifes\n[o]scillators\ns[p]aceships\no[t]hers" << endl;
	char selectFile;
	char selectStill;
	cin >> selectFile;
	switch (selectFile) {
	case 's': {
		cout << "Which still life would you like to open?\n[b]lock\nb[e]ehive\n[l]oaf\nb[o]at\n[t]ub " << endl;
		cin >> selectStill;
		switch (selectStill) {

		}
		break;
	}
	case 'o': {
		cout << "Which oscillator would you like to open?\n[b]linker\n[t]oad\nb[e]acon\n[p]ulsar\npe[n]tadecathlon" << endl;
		cin >> selectStill;
		switch (selectStill) {
		}
		break;
	}
	case 'p': {
		cout << "Which spaceship would you like to open?\n[g]lider\n[l]ightweight spaceship" << endl;
		cin >> selectStill;
		switch (selectStill) {
		}
		break;
	}
	case 't': {
		cout << "Which other file would you like to open?\n[r]-pentomino\n[d]iehard\n[a]corn\n[g]osper glider gun\n[o]ther" << endl;
		cin >> selectStill;
		switch (selectStill) {
		case 'o': {
			cout << "Enter the file name of the file you want to open: " << endl;
			cin >> inFileName;
			inFile.open(inFileName);
			if (inFile.fail()) {
				cout << "File not found" << endl;
				loadFile();
			}
			return;
		}
		break;
		}
	}
	}
}

void purgeMat() { //Purging the matrix//
	for (int i = 0; i < gridsize; i++) {
		for (int j = 0; j < gridsize; j++) {
			curGen[i][j] = false;
		}
	}
}

void setPos() { //Creates the matrix for the first generation//
	for (int i = 0; i < gridsize; i++) {
		for (int j = 0; j < gridsize; j++) {
			char inFileChar = readFile();
			if (inFileChar != 'e') {
				if (inFileChar == ' ') {
					curGenActive = false;
				}
				else if (inFileChar == '\n') {
					i++; j = -1;
					curGenActive = false;
				}
				else {
					curGenActive = true;
				}
			curGen[i][j] = curGenActive;
			}
		}
	}
}


int countNCells(int i, int j) { //Counts the neghbouring cells for every cell//
	int nCells = 0;
	bool nCell;
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			if (i + x < 0 || j + y < 0 || i + x > gridsize || j + y > gridsize) {
				nCell = false;
			}
			else {
				nCell = curGen[i + x][j + y];
			}
			if (nCell) {
				nCells++;
			}
		}
	}
	if (curGen[i][j]) {
		nCells--;
	}
	return nCells;
}
bool percentToBool(int r) { //Changes a percentage into a boolean//
	int a = rand() % 101;
	if (r >= a) {
		return true;
	}
	return false;
}
void fillRand(int a) { //Function to fill cells randomly//
	for (int i = 0; i < gridsize + 1; i++) {
		for (int j = 0; j < gridsize + 1; j++) {
			curGen[i][j] = percentToBool(a);
		}
	}
}
void overwrite() { //Overwrite function//
	for (int i = 0; i < gridsize + 1; i++) {
		for (int j = 0; j < gridsize + 1; j++) {
			curGen[i][j] = nextGen[i][j];
		}
	}
	nextGen[gridsize][gridsize] = { false };
}
void setNextPos() { //Calculates the next generation of cells and then overwrites them//
	for (int i = 0; i < gridsize; i++) {
		for (int j = 0; j < gridsize; j++) {
			if (countNCells(i, j) == 3) {
				nextGenActive = true;
			}
			else if ((countNCells(i, j) == 2) && (curGen[i][j])) {
				nextGenActive = true;
			}
			else {
				nextGenActive = false;
			}
			nextGen[i][j] = nextGenActive;
		}
	}
	overwrite();
}
char randCell() { //Randomizes the characters shown for active cells in ascii from 55 to 142//
	char cell = char(rand() % 87 + 55);
	return cell;
}
char cellRepresentation(bool curGenActive) { //Create te representation of the cell on the screen//
	if (curGenActive) {
		cell = randCell();
	}
	else {
		cell = ' ';
	}
	return cell;
}
int randomRGB(int a) { //Generates a random RGB value//
	return rand() % a + 48;
}
string setR() { //Sets the red shade of a RGB value//
	char c = randomRGB(3);
	char d = randomRGB(6);
	char e = randomRGB(7);
	string G = semiCool;
	G.append(1u, c);
	G.append(1u, d);
	G.append(1u, e);
	return G;
}
string setG() { //Sets the green shade of a RGB value//
	char c = randomRGB(2);
	char d = randomRGB(6);
	char e = randomRGB(7);
	string G = semiCool;
	G.append(1u, c);
	G.append(1u, d);
	G.append(1u, e);
	return G;
}
string setB() { //Sets the blue shade of a RGB value//
	char c = randomRGB(2);
	char d = randomRGB(6);
	char e = randomRGB(7);
	string G = semiCool;
	G.append(1u, c);
	G.append(1u, d);
	G.append(1u, e);
	return G;
}
string RGBString(string r, string g, string b) { //Translates the RGB value into a readable string//
	string str = "\x1b[";
	str.append("38;2");
	str.append(r);
	str.append(g);
	str.append(b);
	str.append("m");
	return str;
}
void SetColor(int value) { //Not important//
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
}
char aInColour() { //Not important//
	SetColor(randomRGB(1)-48+8);
	char c = randCell();
	return c;
}

void draw() { //Draws a generation of cells//
	printf(ESC "H");
	printf(ESC "?25l");
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			string r = setR();
			string g = setG();
			string b = setB();
			string code = RGBString(r, g, b);

			if ((x == Corsair.i) && (y == Corsair.j)) {

				cout << '+';
			}
			else if (curGen[x][y]) {
//				cout << code << deadcell() << "\033[0;00m";
//				cout << code;
				cout << cellRepresentation(curGen[x][y]);
			}
			else {
				cout << ' ';
				
			}
		}
		cout << endl;
	}
	cout << endl;
}

int vt()
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwOriginalOutMode = 0;
	DWORD dwOriginalInMode = 0;
	if (!GetConsoleMode(hOut, &dwOriginalOutMode))
	{
		return false;
	}
	if (!GetConsoleMode(hIn, &dwOriginalInMode))
	{
		return false;
	}

	DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

	DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
	if (!SetConsoleMode(hOut, dwOutMode))
	{
		// we failed to set both modes, try to step down mode gracefully.
		dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
		if (!SetConsoleMode(hOut, dwOutMode))
		{
			// Failed to set any VT mode, can't do anything here.
			return -1;
		}
	}

	DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
	if (!SetConsoleMode(hIn, dwInMode))
	{
		// Failed to set VT input mode, can't do anything here.
		return -1;
	}

	return 0;
}

void polyGen(int p) { //For running multiple generations//
	for (int i = 0; i < p; i++) {
		setNextPos();
		draw();
	}
}

void menu() { //Function to choose what you want to do//
	cout << " | [g]enerations | e[x]it | [r]andom | [m]onitor size | load [f]ile |\n |                        |                           |             |\n |   [w][a][s][d]-cursor  |     crea[t]e/des[t]roy    |   r[e]set   | ";
	char input = cin.get();
	int c;
	switch (input) {
	case 'g': {
		int p;
		cout << "How many generations do you want to run?\n";
		cin >> p;
		polyGen(p);
		break;
	}
	case 'w':
		Corsair.i--;
		break;
	case 's':
		Corsair.i++;
		break;
	case 'a':
		Corsair.j--;
		break;
	case 'd':
		Corsair.j++;
		break;
	case 't': {
		if (curGen[Corsair.i][Corsair.j]) {
			curGen[Corsair.i][Corsair.j] = false;
		}
		else {
			curGen[Corsair.i][Corsair.j] = true;
		}
		break;
	}
	case 'r': {
		reset = false;
		cout << "Enter the percentage of random cells: ";
		cin >> c;
		fillRand(c);
		break;
	}
	case 'm': {
		cout << "Enter the character width of the screen: ";
		cin >> width;
		cout << "Enter the character height of the screen: ";
		cin >> height;
		break;
	}
	case 'e': {
		purgeMat();
		doneReading = false;
		inFile.open("inFile.txt");
		setPos();
		break;
	}
	case 'f': {
		purgeMat();
		doneReading = false;
		loadFile();
		setPos();
		break;
	}
	case 'x':
		gameOver = true;
	} 
}

void runGame() {
	while (!gameOver) {
		printf(ESC "2J");
		draw();
		menu();
	}
}

//Initializing program//
int main(){
	vt();
	printf(ESC "92m");
	inFile.open("inFile.txt");
	setPos();
	runGame();
	return 0;
}