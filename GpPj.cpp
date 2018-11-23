#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <Windows.h>

using namespace std;

#define BOARD_SIZE 11

#define PAUSE_TIME 2000
#define DEMO_PAUSE_TIME 3000

enum dice {
	ONE = 1,
	TWP = 2,
	THREE = 3,
	FOUR = 4,
	FIVE = 5,
	SIX = 6
};

class Location {
public:
	Location(int x_, int y_);

	int x;
	int y;
};

Location::Location(int x_, int y_) : x(x_), y(y_) {
}

class Player;

class Chess {
public:
	Chess(char c_, int whichPlayer_);
	int locate(int x, int y);
	// check chess location
	bool at_path();
	bool at_base();
	bool at_home();
	void return_to_base();
	void go_to_home();
	void jump_to_path();

	char get_symbol();
	int get_current_idx();

	int move(int idx);

private:
	char c;
	int currentIdx;
	bool inBase;
	bool inHome;

	int whichPlayer;
};

class GameBoard {
public:
	GameBoard();
	void display(const vector<Player*>&);

private:
	char arrs[BOARD_SIZE][BOARD_SIZE];
	const char space;
	const char X;
	const char dot;
};

class Setting {
public:
	Setting();
	void start_to_set();
	void display_info();

	int human_Count();
	int computer_Count();

	void demoMode();

private:
	// R2
	int settingMenu();
	// R2.1
	void setPlayers();
	// R2.2
	void setHumanPlayers();

private:
	int humanCount;
	int computerCount;
	int totalCount;
};

class LudoGame {
public:
	LudoGame();
	~LudoGame();
	void start_to_play();

	// R1.1
	void printGameInfo();

private:
	void printWelcomeMsg();
	int gameMenu();
	int demoMenu();

	void play();

	// R3
	void showGameDemo();

	void initPlayers();
	bool player_move();

	// R5
	void display_credits();

	// R6
	bool exit();

	// R4
	void display_instructions();

	bool winner();

	void demo_three_consecutive_6();

	void demo_touches_home_and_reverse_then_reaches_home_and_wins();

	void demo_does_not_self_overlap_and_sends_back_opponent_chess();

private:
	GameBoard gameBoard;
	vector<Player*> players;

	vector<string> names;
	vector<vector<Location> > pathes;

	Setting setting;
};

class Player {
public:
	Player(const string& names, const vector<Location>& vec_, int idx_, LudoGame* ludoGame);
	char locate(int x, int y);
	void display();
	virtual string type() = 0;
	bool move(vector<Player* >& players);
	bool move_by_value(int value, vector<Player* >& players);

	vector<char> find_movable_chess(int value);
	int find_chess_idx(char c);
	virtual char make_valid_decision(const vector<char>& char_vec) = 0;

	bool is_winner();
	bool chessOnBoard();
	int get_idx();
	void send_back_others(const Location& pos, vector<Player* >& players);

	void set_in_demo_2();
	void set_in_demo_3_1();
	void set_in_demo_3_2();

protected:
	bool exit();

	// Generate random number between [min, max]
	int random_numbers(int min, int max) {
		return (rand() % (max - min + 1)) + min;
	}

protected:
	void display_base_home();

protected:
	// Path
	vector<Location> vec;
	vector<Chess> chesses;

	int idx;
	LudoGame* ludoGame;
};

Player::Player(const string& names, const vector<Location>& vec_, int idx_, LudoGame* ludoGame_) :
	vec(vec_), idx(idx_), ludoGame(ludoGame_) {
	for (int i = 0; i < 4; ++i) {
		chesses.push_back(Chess(names[i], idx));
	}
}

char Player::locate(int x, int y) {
	for (int i = 0; i < chesses.size(); ++i) {
		int loc = chesses[i].locate(x, y);
		if (loc != -1) {
			Location pos = vec[loc];
			if (pos.x == x && pos.y == y) {
				return chesses[i].get_symbol();
			}
		}
	}

	return ' ';
}

void Player::display_base_home() {
	cout << "Base: ";
	for (int i = 0; i < chesses.size(); ++i) {
		if (chesses[i].at_base()) {
			cout << chesses[i].get_symbol() << " ";
		}
	}
	cout << endl;

	cout << "Home: ";
	for (int i = 0; i < chesses.size(); ++i) {
		if (chesses[i].at_home()) {
			cout << chesses[i].get_symbol() << " ";
		}
	}
	cout << endl;
}

bool Player::chessOnBoard() {
	for (int i = 0; i < 4; ++i) {
		if (chesses[i].at_path()) {
			return true;
		}
	}
	return false;
}

bool Player::is_winner() {
	bool flag = true;
	for (int i = 0; i < 4; ++i) {
		if (!chesses[i].at_home()) {
			flag = false;
		}
	}

	return flag;
}

int Player::get_idx() {
	return idx;
}

void Player::display() {
	cout << "Player " << (idx + 1) << ": " << type() << endl;
	display_base_home();
}

bool Player::move(vector<Player* >& players) {
	cout << "Turns to Player " << (idx + 1) << ": " << type() << endl;
	int value = random_numbers(dice::ONE, dice::SIX);
	if (move_by_value(value, players)) {
		return true;
	}
	if (value == dice::SIX) {
		cout << "\nYou got another chance!\n";
		value = random_numbers(dice::ONE, dice::SIX);
		if (move_by_value(value, players)) {
			return true;
		}
		if (value == dice::SIX) {
			cout << "\nYou got another chance!\n";
			value = random_numbers(dice::ONE, dice::SIX);
			if (move_by_value(value, players)) {
				return true;
			}
		}
	}

	return false;
}

bool Player::move_by_value(int value, vector<Player* >& players) {
	cout << "You roll " << value << endl;
	vector<char> vecs = find_movable_chess(value);
	if (vecs.empty()) {
		cout << "You can't move.\n";
		Sleep(PAUSE_TIME);
	}
	else {
		cout << "Chess move available: ";
		for (int i = 0; i < vecs.size(); ++i) {
			cout << vecs[i] << " ";
		}
		cout << endl;
		char decision = make_valid_decision(vecs);
		if (decision == 'q') {
			return true;
		}
		else {
			int chess_idx = find_chess_idx(decision);
			int nowIdx = chesses[chess_idx].move(value);
			Location pos = vec[nowIdx];
			cout << "Chess " << chesses[chess_idx].get_symbol() << " move to (" << pos.x << ", " << pos.y << ")" << endl;
			if (!chesses[chess_idx].at_home()) {
				send_back_others(pos, players);
			}
			Sleep(PAUSE_TIME);
			ludoGame->printGameInfo();
		}
	}

	return false;
}

vector<char> Player::find_movable_chess(int value) {
	vector<char> vec;
	for (int i = 0; i < chesses.size(); ++i) {
		if (chesses[i].at_path()) {
			bool canMove = true;
			int cIdx = chesses[i].get_current_idx();
			for (int j = 0; j < chesses.size(); ++j) {
				if (i != j) {
					int index = cIdx + value;
					if (index >= 45) {
						index = 45 - (index - 44) - 1;
					}
					if (!chesses[j].at_home() && index == chesses[j].get_current_idx()) {
						canMove = false;
						break;
					}
				}
			}
			if (canMove) {
				vec.push_back(chesses[i].get_symbol());
			}
		}
		else if (value == dice::SIX && chesses[i].at_base()) {
			bool canMove = true;
			for (int j = 0; j < chesses.size(); ++j) {
				if (i != j) {
					if (chesses[j].at_path() && chesses[j].get_current_idx() == 0) {
						canMove = false;
						break;
					}
				}
			}
			if (canMove) {
				vec.push_back(chesses[i].get_symbol());
			}
		}
	}

	return vec;
}

int Player::find_chess_idx(char c) {
	for (int i = 0; i < chesses.size(); ++i) {
		if (chesses[i].get_symbol() == c) {
			return i;
		}
	}
	return -1;
}

bool Player::exit() {
	cout << "Enter Y/y to confirm, N/n returns to Game Menu: ";
	char c;
	while (true) {
		cin >> c;
		if (c == 'Y' || c == 'y') {
			return true;
		}
		else if (c == 'N' || c == 'n') {
			return false;
		}
		else {
			cout << "Invalid input! Please re-enter: ";
		}
	}

	return false;
}

void Player::send_back_others(const Location& pos, vector<Player* >& players) {
	int size = players.size();
	for (int i = 0; i < size; ++i) {
		if (i != idx) {
			for (int j = 0; j < players[i]->chesses.size(); ++j) {
				int cIdx = players[i]->chesses[j].get_current_idx();
				Location otherPos = players[i]->vec[cIdx];
				if (pos.x == otherPos.x && pos.y == otherPos.y) {
					players[i]->chesses[j].return_to_base();
					return;
				}
			}
		}
	}
}

void Player::set_in_demo_2() {
	chesses[0].go_to_home();
	chesses[1].go_to_home();
	chesses[2].go_to_home();

	chesses[3].move(6);
	chesses[3].move(43);
}

void Player::set_in_demo_3_1() {
	chesses[0].move(6);
	chesses[0].move(8);

	chesses[1].move(6);
	chesses[1].move(6);
}

void Player::set_in_demo_3_2() {
	chesses[0].move(6);
}

Chess::Chess(char c_, int whichPlayer_) :
	c(c_), currentIdx(0), inBase(true), inHome(false), whichPlayer(whichPlayer_) {
}

int Chess::locate(int x, int y) {
	if (at_path()) {
		return currentIdx;
	}
	else {
		return -1;
	}
}

bool Chess::at_path() {
	return !inBase && !inHome;
}

char Chess::get_symbol() {
	return c;
}

bool Chess::at_base() {
	return inBase;
}

bool Chess::at_home() {
	return inHome;
}

void Chess::return_to_base() {
	inBase = true;
	inHome = false;
	currentIdx = 0;
	cout << c << " returns to base" << endl;
}

void Chess::go_to_home() {
	inBase = false;
	inHome = true;
}

int Chess::move(int idx) {
	if (inBase && idx == dice::SIX) {
		jump_to_path();
		currentIdx = 0;
	}
	else if (at_path()) {
		int index = currentIdx + idx;
		if (index < 45) {
			currentIdx = index;
		}
		else {
			currentIdx = 45 - (index - 44) - 1;
		}
		if (currentIdx == 44) {
			go_to_home();
		}
	}

	return currentIdx;
}

void Chess::jump_to_path() {
	inBase = false;
	inHome = false;
}

int Chess::get_current_idx() {
	return currentIdx;
}

class Human : public Player {
public:
	Human(const string& names, const vector<Location>& vec_, int idx_, LudoGame* ludoGame_);
	string type();
	char make_valid_decision(const vector<char>& char_vec);
};

Human::Human(const string& names, const vector<Location>& vec_, int idx_, LudoGame* ludoGame_) :Player(names, vec_, idx_, ludoGame_) {

}

std::string Human::type() {
	return "Human";
}

char Human::make_valid_decision(const vector<char>& char_vec) {
	int size = char_vec.size();
	char c;
	cout << "Make your choice: ";
	while (true) {
		cin >> c;
		if (c == 'q') {
			if (Player::exit()) {
				return c;
			}
			else {
				cout << "please re-enter: ";
			}
		}
		else {
			for (int i = 0; i < size; ++i) {
				if (char_vec[i] == c) {
					return c;
				}
			}
			cout << "Invalid input, please re-enter: ";
		}
	}
}

class Computer : public Player {
public:
	Computer(const string& names, const vector<Location>& vec_, int idx_, LudoGame* ludoGame_);
	string type();
	char make_valid_decision(const vector<char>& char_vec);
};

Computer::Computer(const string& names, const vector<Location>& vec_, int idx_, LudoGame* ludoGame_) :Player(names, vec_, idx_, ludoGame_) {
}

std::string Computer::type() {
	return "COMP";
}

char Computer::make_valid_decision(const vector<char>& char_vec) {
	int size = char_vec.size();
	cout << "Make your choice: ";

	Sleep(PAUSE_TIME);
	int dice = Player::random_numbers(0, size - 1);

	return char_vec[dice];
}

GameBoard::GameBoard() :space(' '), dot('.'), X('X') {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			arrs[i][j] = space;
		}
	}

	for (int i = 0; i < BOARD_SIZE; ++i) {
		arrs[0][i] = dot;
		arrs[BOARD_SIZE - 1][i] = dot;
		arrs[i][0] = dot;
		arrs[i][BOARD_SIZE - 1] = dot;
		arrs[i][i] = dot;
		arrs[i][BOARD_SIZE - i - 1] = dot;
	}

	int mid = BOARD_SIZE / 2;
	arrs[mid][mid] = X;
}

void GameBoard::display(const vector<Player*>& players) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			bool hasChess = false;
			for (int k = 0; k < players.size(); ++k) {
				char c = players[k]->locate(i, j);
				if (c != space) {
					cout << c << " ";
					hasChess = true;
					break;
				}
			}
			if (!hasChess) {
				cout << arrs[i][j] << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

int Setting::settingMenu() {
	cout << "*** Settings Menu ***\n";
	cout << "[1] Number of players\n";
	cout << "[2] Number of human players\n";
	cout << "[3] Return to Game Menu\n";
	cout << "*****************\n";
	cout << "Option (1 - 3): ";

	int option;
	while (true) {
		cin >> option;
		if (option >= 1 && option <= 3) {
			break;
		}
		else {
			cout << "Invalid input! Please re-enter: ";
		}
	}

	cout << endl;
	system("cls");

	return option;
}

void Setting::start_to_set() {
	bool isSetting = true;
	while (isSetting) {
		int option = settingMenu();
		switch (option) {
		case 1: {
			setPlayers();
			break;
		}
		case 2: {
			setHumanPlayers();
			break;
		}
		case 3: {
			// R2.3
			isSetting = false;
			break;
		}
		}
	}
}

void Setting::setPlayers() {
	display_info();
	cout << "Please enter players(2 - 4): ";

	int number;
	while (true) {
		cin >> number;
		if (number < 2 || number > 4) {
			cout << "It is out of range. Please re-enter: ";
		}
		else if (number < humanCount) {
			cout << "It should not be less than the current number of human players. Please re-enter: ";
		}
		else {
			totalCount = number;
			computerCount = totalCount - humanCount;
			break;
		}
	}

	cout << endl;
	system("cls");
	cout << "The new setting: \n";
	display_info();
}

void Setting::display_info() {
	cout << "There are " << totalCount << " players.\n";
	cout << humanCount << " are human player(s).\n";
	cout << computerCount << " are computer player(s).\n";
	cout << endl;
}

Setting::Setting() : totalCount(2), humanCount(1), computerCount(1) {
}

void Setting::setHumanPlayers() {
	display_info();
	cout << "Please enter human players(0 - " << totalCount << "): ";

	int number;
	while (true) {
		cin >> number;
		if (number >= 0 && number <= totalCount) {
			humanCount = number;
			computerCount = totalCount - humanCount;
			break;
		}
		else {
			cout << "It is out of range. Please re-enter: ";
		}
	}

	cout << endl;
	system("cls");
	cout << "The new setting: \n";
	display_info();
}

int Setting::human_Count() {
	return humanCount;
}

int Setting::computer_Count() {
	return computerCount;
}

void Setting::demoMode() {
	humanCount = 0;
	computerCount = 2;
	totalCount = 2;
}

void LudoGame::start_to_play() {
	printWelcomeMsg();

	bool isRunning = true;
	while (isRunning) {
		int option = gameMenu();
		switch (option) {
		case 1: {
			play();
			break;
		}
		case 2: {
			setting.start_to_set();
			break;
		}
		case 3: {
			// Game demo
			showGameDemo();
			break;
		}
		case 4: {
			/*The system displays the instructions for playing the Ludo game. After displaying the
			instruction, the system returns back to the Game Menu.*/
			display_instructions();
			break;
		}
		case 5: {
			/*The system displays the personal particulars (e.g. student name, student ID, class,
			tutorial group, etc.) of the group members. After displaying the information, the
			system returns back to the Game Menu.*/
			display_credits();
			break;
		}
		case 6: {
			/*When the user inputs this option, the system prompts for user¡¯s confirmation. If the
			user inputs "Y/y", the program terminates. If the user inputs "N/n", the system
			returns to the Game Menu. Other input is not acceptable and the system would ask
			the user to confirm again.*/
			isRunning = exit();
			system("cls");
			break;
		}
		}
	}
}

void LudoGame::printWelcomeMsg() {
	cout << "Welcome to Ludo game world!\n";
}

int LudoGame::gameMenu() {
	cout << "*** Game Menu ***\n";
	cout << "[1] Start Game\n";
	cout << "[2] Settings\n";
	cout << "[3] Game Demo\n";
	cout << "[4] Instructions\n";
	cout << "[5] Credits\n";
	cout << "[6] Exit\n";
	cout << "*****************\n";
	cout << "Option (1 - 6): ";

	int option;
	while (true) {
		cin >> option;
		if (option >= 1 && option <= 6) {
			break;
		}
		else {
			cout << "Invalid input! Please re-enter: ";
		}
	}

	cout << endl;
	system("cls");

	return option;
}

bool LudoGame::exit() {
	cout << "Enter Y/y to confirm, N/n returns to Game Menu: ";
	char c;
	while (true) {
		cin >> c;
		if (c == 'Y' || c == 'y') {
			return false;
		}
		else if (c == 'N' || c == 'n') {
			return true;
		}
		else {
			cout << "Invalid input! Please re-enter: ";
		}
	}

	return false;
}

void LudoGame::display_credits() {

	system("cls");
	cout << "Student name\t\tStudent ID\t\tClass\t\tTutorial group\n";
	cout << "Sarki Joshan\t\t16098910A\t\t104\t\tB\n";
	cout << "HO YIU HIM\t\t17056165A\t\t104\t\tB\n";
	cout << "Luk Ho Fung\t\t17160208A\t\t104\t\tB" << endl;
	cout << "So Chun Lung\t\t17143918A\t\t104\t\tB\n";
	cout << "Kwok Chi Hang\t\t17158840A\t\t104\t\tC\n";
	


	cout << endl;
	system("pause");
	

	
}

LudoGame::LudoGame() {
	vector<Location> path1;
	vector<Location> path2;
	vector<Location> path3;
	vector<Location> path4;

	// Path for player1
	for (int i = 0; i < BOARD_SIZE; ++i) {
		path1.push_back(Location(0, i));
	}
	for (int i = 1; i < BOARD_SIZE; ++i) {
		path1.push_back(Location(i, BOARD_SIZE - 1));
	}
	for (int i = BOARD_SIZE - 2; i >= 0; --i) {
		path1.push_back(Location(BOARD_SIZE - 1, i));
	}
	for (int i = BOARD_SIZE - 2; i >= 1; --i) {
		path1.push_back(Location(i, 0));
	}
	for (int i = 1; i <= BOARD_SIZE / 2; ++i) {
		path1.push_back(Location(i, i));
	}

	// Path for player2
	for (int i = 0; i < BOARD_SIZE; ++i) {
		path2.push_back(Location(i, BOARD_SIZE - 1));
	}
	for (int i = BOARD_SIZE - 2; i >= 0; --i) {
		path2.push_back(Location(BOARD_SIZE - 1, i));
	}
	for (int i = BOARD_SIZE - 2; i >= 0; --i) {
		path2.push_back(Location(i, 0));
	}
	for (int i = 1; i < BOARD_SIZE - 1; ++i) {
		path2.push_back(Location(0, i));
	}
	for (int i = 1; i <= BOARD_SIZE / 2; ++i) {
		path2.push_back(Location(i, BOARD_SIZE - i - 1));
	}

	// Path for player3
	for (int i = BOARD_SIZE - 1; i >= 0; --i) {
		path3.push_back(Location(BOARD_SIZE - 1, i));
	}
	for (int i = BOARD_SIZE - 2; i >= 0; --i) {
		path3.push_back(Location(i, 0));
	}
	for (int i = 1; i < BOARD_SIZE; ++i) {
		path3.push_back(Location(0, i));
	}
	for (int i = 1; i < BOARD_SIZE - 1; ++i) {
		path3.push_back(Location(i, BOARD_SIZE - 1));
	}
	for (int i = 1; i <= BOARD_SIZE / 2; ++i) {
		path3.push_back(Location(BOARD_SIZE - i - 1, BOARD_SIZE - i - 1));
	}

	// Path for player4
	for (int i = BOARD_SIZE - 1; i >= 0; --i) {
		path4.push_back(Location(i, 0));
	}
	for (int i = 1; i < BOARD_SIZE; ++i) {
		path4.push_back(Location(0, i));
	}
	for (int i = 1; i < BOARD_SIZE; ++i) {
		path4.push_back(Location(i, BOARD_SIZE - 1));
	}
	for (int i = BOARD_SIZE - 2; i >= 1; --i) {
		path4.push_back(Location(BOARD_SIZE - 1, i));
	}
	for (int i = 1; i <= BOARD_SIZE / 2; ++i) {
		path4.push_back(Location(BOARD_SIZE - i - 1, i));
	}

	pathes.push_back(path1);
	pathes.push_back(path2);
	pathes.push_back(path3);
	pathes.push_back(path4);

	names.push_back("abcd");
	names.push_back("efgh");
	names.push_back("ijkl");
	names.push_back("mnop");
}

LudoGame::~LudoGame() {
}

void LudoGame::printGameInfo() {
	system("cls");
	gameBoard.display(players);
	for (int i = 0; i < players.size(); ++i) {
		players[i]->display();
		cout << endl;
	}
}

void LudoGame::display_instructions() {
	cout << "====================================================================================================\n";
	cout << "GAME RULE\n";
	cout << "A. To have a chess leaving ¡°Base¡± and entering the game board, a player must roll\n";
	cout << "¡®6¡¯. If the player has no chess on the game board and fails to roll ¡®6¡¯, the turn\n";
	cout << "passes to the next player.\n\n";
	cout << "B. If a player has one or more chess to move, he chooses a movable chess from the\n";
	cout << "given list and moves the chess along the path by the number of Locations\n";
	cout << "according to the rolling result.\n\n";
	cout << "C. If a player rolls a ¡®6¡¯, he gets a bonus turn. If the bonus turn rolls a ¡®6¡¯ again, he\n";
	cout << "gets one more bonus turn. If he rolls a ¡®6¡¯ once again, there is no more bonus turn;\n";
	cout << "after his move the turn passes to the next player.\n\n";
	cout << "D. A player cannot move a chess into a location that is occupied by other chesses of that player\n\n";
	cout << "E. If a chess ends on a Location occupied by an opponent¡¯s chess, the opponent chess\n";
	cout << "is sent back to the opponent¡¯s ¡°Base¡±. Any chess that has reached ¡°Home¡± should\n";
	cout << "be ¡°safe¡± from being sent back.\n\n";
	cout << "F. A chess can reaches ¡°Home¡± only if it exactly steps on ¡®X¡¯. If the rolling value\n";
	cout << "exceeds the number of steps required, the chess should move backward for the\n";
	cout << "remaing steps.\n";
	cout << "====================================================================================================\n";
	cout << endl;
}

void LudoGame::play() {
	initPlayers();

	bool isPlaying = true;
	while (isPlaying) {
		printGameInfo();
		if (player_move()) {
			isPlaying = false;
		}
		if (winner()) {
			isPlaying = false;
		}
	}
	system("cls");
}

void LudoGame::initPlayers() {
	for (int i = 0; i < players.size(); ++i) {
		delete players[i];
	}
	players.clear();

	int humans = setting.human_Count();
	int computers = setting.computer_Count();
	for (int i = 0; i < humans; ++i) {
		players.push_back(new Human(names[i], pathes[i], i, this));
	}
	for (int i = 0; i < computers; ++i) {
		players.push_back(new Computer(names[i + humans], pathes[i + humans], i + humans, this));
	}
}

bool LudoGame::player_move() {
	for (int i = 0; i < players.size(); ++i) {
		if (players[i]->move(players)) {
			return true;
		}
		cout << endl;
	}
	return false;
}

bool LudoGame::winner() {
	for (int i = 0; i < players.size(); ++i) {
		if (players[i]->is_winner()) {
			cout << "Player " << (players[i]->get_idx() + 1) << " wins!\n";
			Sleep(PAUSE_TIME * 3);
			system("pause");
			return true;
		}
	}

	return false;
}

void LudoGame::showGameDemo() {
	Setting backupSetting = setting;

	bool isShowing = true;
	while (isShowing) {
		int option = demoMenu();
		switch (option) {
		case 1: {
			// R3.1
			demo_three_consecutive_6();
			break;
		}
		case 2: {
			// R3.2
			demo_touches_home_and_reverse_then_reaches_home_and_wins();
			break;
		}
		case 3: {
			// R3.3
			demo_does_not_self_overlap_and_sends_back_opponent_chess();
			break;
		}
		case 4: {
			isShowing = false;
			// restore previous game setting
			setting = backupSetting;
			system("cls");
			break;
		}
		}
	}
}

int LudoGame::demoMenu() {
	cout << "*** Demo Menu ***\n";
	cout << "[1] Three consecutive '6'\n";
	cout << "[2] Chess touches home and reverses, reaches home and wins\n";
	cout << "[3] Chess does not self-overlap, and sends back opponent chess\n";
	cout << "[4] Return to Game Menu\n";
	cout << "*****************\n";
	cout << "Option (1 - 4): ";

	int option;
	while (true) {
		cin >> option;
		if (option >= 1 && option <= 4) {
			break;
		}
		else {
			cout << "Invalid input! Please re-enter: ";
		}
	}

	cout << endl;
	system("cls");

	return option;
}

void LudoGame::demo_three_consecutive_6() {
	cin.ignore();
	cin.clear();

	setting.demoMode();
	initPlayers();
	printGameInfo();

	cout << "[When you roll first 6, press any key to continue...]\n";
	getchar();
	int idx = 0;
	cout << "Turns to Player " << idx + 1 << ": " << players[idx]->type() << endl;
	int value = dice::SIX;
	Sleep(DEMO_PAUSE_TIME);
	players[idx]->move_by_value(value, players);
	cout << "[You will get another chance, press any key to continue...]\n";
	getchar();

	if (value == dice::SIX) {
		cout << "\nYou got another chance!\n";
		value = dice::SIX;
		Sleep(DEMO_PAUSE_TIME);
		players[idx]->move_by_value(value, players);
		cout << "[You will get one more chance, press any key to continue...]\n";
		getchar();

		if (value == dice::SIX) {
			cout << "\nYou got another chance!\n";
			value = dice::SIX;
			Sleep(DEMO_PAUSE_TIME);
			players[idx]->move_by_value(value, players);
			cout << "[You can have three times maximum chance, press any key to continue...]\n";
			getchar();
		}
	}

	idx = 1;
	cout << "Turns to Player " << idx + 1 << ": " << players[idx]->type() << endl;
	cout << "[Now it turns to next Player, press any key to return...]\n";
	getchar();
	system("cls");
}

void LudoGame::demo_touches_home_and_reverse_then_reaches_home_and_wins() {
	cin.ignore();
	cin.clear();

	setting.demoMode();
	initPlayers();

	players[0]->set_in_demo_2();

	printGameInfo();
	cout << "[Player 1 has chess a, b, c at home, while chess d on the path, press any key to continue...]\n";
	getchar();

	players[0]->move_by_value(5, players);
	cout << "[Player 1 rolls 5, chess d touches home and reverses, press any key to continue...]\n";
	getchar();

	players[1]->move_by_value(6, players);
	cout << "[It turns to Player 2, press any key to continue...]\n";
	getchar();

	players[0]->move_by_value(4, players);
	cout << "[Player 1 rolls 4, chess d reaches home, press any key to continue...]\n";
	getchar();

	if (winner()) {
		system("cls");
		return;
	}
}

void LudoGame::demo_does_not_self_overlap_and_sends_back_opponent_chess() {
	cin.ignore();
	cin.clear();

	setting.demoMode();
	initPlayers();

	players[0]->set_in_demo_3_1();
	players[1]->set_in_demo_3_2();
	printGameInfo();
	cout << "[Player 1 has chess a, b on path; Player 2 has chess e on path, press any key to continue...]\n";
	getchar();

	players[0]->move_by_value(2, players);
	cout << "[Player 1 rolls dice 2, then chess b can't move since it will overlap with chess a, press any key to continue...]\n";
	getchar();

	cout << "[Chess e was sent back to Player 2's base.]\n";
	cout << "[It is because that Player 1's chess a move to the same Location, press any key to continue...]";
	getchar();

	system("cls");
}

int main() {
	srand((unsigned int)time(NULL));

	LudoGame ludoGame;
	ludoGame.start_to_play();

	system("pause");
	return 0;
}
