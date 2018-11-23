#include<iostream>
#include<iomanip>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<string>

using namespace std;
int turnplayer = 0, numberofplayer = 2, numberofhuman = 0, savenumberofplayer, savenumberofhuman, roll[3], iplayerchoise, nextstage, set52 = 0;
bool quit[4], movementcheak[4], set6 = 0;
char mapstage[58], cplayerchoise[3];
void doquitgame(char &input, int z) {//function to quit for the normal game, demo menu, setting menu and the whole progame
	cout << "\nInput \"Y\" or \"y\" to quit, or input \"N\" or \"n\" to return.\nYour choise: ";
	cin >> input;
	switch (input) {
	case('Y'): {
		quit[z] = 1;
		break;
	}
	case('y'): {
		input = 'Y';
		quit[z] = 1;
		break;
	}
	case('N'): {
		break;
	}
	case('n'): {
		input = 'N';
		break;
	}
	default: {//for the cheak of wrong input and redo this function
		cout << "\nYou input a wrong value.";
		doquitgame(input, z);
	}
	}
}
void doendgame(int player) {//while someone has win the game, show the winning massage
	cout << "Player" << player + 1 << " win the game. Congratulation.";
	system("pause");
	system("CLS");
	quit[0] = 1;
}
class player {
public:
	void movingcheak(int z) {//to cheak if any chess will be eatten
		for (int x = 0; 4 > x; x++) if (chesss[x] + 10 * player == z) chesss[x] = 0;
	}
	void movecheak(int roll, int times) {
		nextstage = 0;
		for (int z = 0; 4 > z; z++) {// cheak if each turn player's chess can move
			movementcheak[z] = 1;
			if ((chesss[z] > 60)||((chesss[z] == 0) && (roll != 6))) movementcheak[z] = 0;//the condition when the chess are at base or home
			for (int x = 0; 4 > x; x++) {
				if ((z != x) && ((chesss[z] + roll == chesss[x]) || ((chesss[z] + roll > 45) && (45 - (chesss[z] + roll - 45) == chesss[x])))) movementcheak[z] = 0;//the condition if the chess will overlep
			}
		}
		if ((((movementcheak[0]==1)||(movementcheak[1]==1))||(movementcheak[2]==1))||(movementcheak[3]==1)) {//showing the choises
			cout << "You can choose ";
			if (movementcheak[0] == 1) cout << (char)(97 + 0 + player * 4) << " ";
			if (movementcheak[1] == 1) cout << (char)(97 + 1 + player * 4) << " ";
			if (movementcheak[2] == 1) cout << (char)(97 + 2 + player * 4) << " ";
			if (movementcheak[3] == 1) cout << (char)(97 + 3 + player * 4) << " ";//showing the choises
			cout << "\nYour choise: ";
			iplayerchoise = 5;
			do {
				if (status == "COMP") {//comp's logic
					do {
						iplayerchoise = 5; //for cheaking the fail input (debug
						for (int z = 0; 4 > z; z++) {//first prioraty is the detroy others' chess
							if (((mapstage[(chesss[z] + roll + player * 10) % 40] != '*') && (chesss[z] + roll < 41)) && (movementcheak[z] == 1)) {
								iplayerchoise = z;
								cplayerchoise[times] = (char)(97 + z + player * 4);
								break;
							}
						}
						if (iplayerchoise != 5)break;
						for (int z = 0; 4 > z; z++) {//start new chess feom base
							if (((chesss[z] == 0) && (roll == 6)) && (movementcheak[z] == 1)) {
								iplayerchoise = z;
								cplayerchoise[times] = (char)(97 + z + player * 4);
								break;
							}
						}
						if (iplayerchoise != 5)break;
						for (int z = 0; 4 > z; z++) {// move away the chess at stage 6 so that the new chess can start when you have roll a 6
							if ((chesss[z] == 6) && (movementcheak[z] == 1)) {
								iplayerchoise = z;
								cplayerchoise[times] = (char)(97 + z + player * 4);
								break;
							}
						}
						if (iplayerchoise != 5)break;
						for (int z = 0; 3 > z; z++) {//move the chess to save zone which is last 4 stage
							if (((chesss[z] < 41) && (chesss[z] > chesss[z + 1])) && (movementcheak[z] == 1)) {
								iplayerchoise = z;
								cplayerchoise[times] = (char)(97 + z + player * 4);
								break;
							}
						}
						if ((chesss[3] > chesss[iplayerchoise]) && (movementcheak[3] == 1)) {
							iplayerchoise = 3;
							cplayerchoise[times] = (char)(100 + player * 4);
						}
						else{// for debug
							for (int z = 0; 4 > z; z++) {
								if (movementcheak[z] == 1) {
									iplayerchoise = z;
									cplayerchoise[times] = (char)(97 + z + player * 4);
									break;
								}
							}
						}
					} while (iplayerchoise == 10);//make it to be a loop to use the break function
				}
				else cin >> cplayerchoise[times];
				if (cplayerchoise[times] == 'q') {
					doquitgame(cplayerchoise[times], 0);
					return;
				}
				for (int z = 0; 4 > z; z++) if ((cplayerchoise[times] == (char)(97 + z + player * 4))&&(movementcheak[z])) iplayerchoise = z;// for fail input cheak
				if (iplayerchoise == 5) cout << "You have input a wrong value. Please choise again.\nYour choise: ";
			} while (iplayerchoise == 5);
			if(chesss[iplayerchoise] + roll < 41) nextstage = (chesss[iplayerchoise] + roll + player * 10) % 40;//find the stage that the chess will move to for doing moving cheak
			else nextstage = 50;//conclude movable chess have correctly chozen and no need the use of moving cheak

		}
		else cout << "You have no moveable chess.\n";
	}
	void showplayerinfo() {
		cout << "\nPlayer" << player + 1 << ": " << status << "\nBase: ";
		for (int z = 0; 4 > z; z++) if (chesss[z] == 0)cout << (char)(97 + z + player * 4) << " ";
		cout << "\nHome: ";
		for (int z = 0; 4 > z; z++) if (chesss[z] == 100)cout << (char)(97 + z + player * 4) << " ";
		cout << endl;
	}
	void setp(int e) {//for reseting
		player = e;
		for (int z = 0; 4 > z;z++)chesss[z] = 0;
	}
	void demoset(int z, int x, int y, int v) {//for demo to setup
		chesss[0] = z;
		chesss[1] = x;
		chesss[2] = y;
		chesss[3] = v;
	}
	void chessposition() {//tell the position of each while building the map
		for (int z = 0;4 > z;z++) {
			if ((chesss[z] > 0) && (chesss[z] < 41))mapstage[(chesss[z] + 10 * player) % 40] = 97 + z + player * 4;
			else if ((chesss[z] != 0) && (chesss[z] != 100)) mapstage[chesss[z] + 4 * player] = 97 + z + 4 * player;
		}
	}
	void moving(int roll) {//move the chess
		chesss[iplayerchoise] += roll;
		if (chesss[iplayerchoise] == 45) chesss[iplayerchoise] = 100;//if chess stsge reach home, change it to 100 for debug
		if ((chesss[iplayerchoise] > 45)&&(chesss[iplayerchoise]!=100)) chesss[iplayerchoise] = 90 - chesss[iplayerchoise];//cheak if the chess stage exsist 45 which is home. if yes then return
	}
	void wincheak() {
		if ((((chesss[0] == 100) && (chesss[1] == 100)) && (chesss[2] == 100)) && (chesss[3] == 100)) doendgame(player);
	}
	void resettocomp() {//for seting that play to COMP
		status = "COMP";
	}
	void settohuman() {//for seting that play to HUMAN
		status = "HUMAN";
	}
private:
	int player, chesss[4];
	string status = "COMP";
};
player p1, p2, p3, p4;

void showinfo() {
	for (int z = 1; 57 > z; z++) mapstage[z] = '.';//showing the map
	p1.chessposition();
	p2.chessposition();
	p3.chessposition();
	p4.chessposition();
	cout << left;
	for (int z = 0; 11 > z; z++) {
		for (int x = 0; 11 > x; x++) {
			if (z == 0)cout << setw(2) << mapstage[x + 1];
			else if (z == 10)cout << setw(2) << mapstage[31 - x];
			else if (x == 0)cout << setw(2) << mapstage[41 - z];
			else if (x == 10) cout << setw(2) << mapstage[11 + z];
			else if ((z < 5) && (x == z))cout << setw(2) << mapstage[40 + z];
			else if ((z < 5) && (10 - x == z))cout << setw(2) << mapstage[44 + z];
			else if ((z > 5) && (x == z))cout << setw(2) << mapstage[58 - z];
			else if ((z > 5) && (10 - x == z))cout << setw(2) << mapstage[62 - z];
			else if ((z == 5) && (x == 5))cout << setw(2) << mapstage[57];
			else cout << setw(2) << " ";
		}
		cout << "\n";//showing the map
	}
	cout << "\nIt's player" << turnplayer + 1 << " turn.\n";
	p1.showplayerinfo();
	if (numberofplayer > 1)p2.showplayerinfo();
	if (numberofplayer > 2)p3.showplayerinfo();
	if (numberofplayer > 3)p4.showplayerinfo();
}
void gamesetting() {
	mapstage[57] = 'X';
	p1.setp(0);
	p2.setp(1);
	p3.setp(2);
	p4.setp(3);
	quit[0] = 0;
	turnplayer = 0;
}
void rolladie(int& z) {
	if (set6 == 1) z = 6;//for demo use of the roll number
	else if (set52 == 1) {//for demo use of the roll number
		set52++;
		z = 5;
	}
	else if (set52 == 2) z = 2;//for demo use of the roll number
	else z = rand() % 6 + 1;//randomly output a number from 1 to 6
}
void callmovingcheak() {
	if (nextstage != 0) {
		p1.movingcheak(nextstage);
		p2.movingcheak(nextstage);
		p3.movingcheak(nextstage);
		p4.movingcheak(nextstage);
	}
}
void callwincheak() {
	p1.wincheak();
	p2.wincheak();
	p3.wincheak();
	p4.wincheak();
}
void showpastmainturninfo(int roll, char times) {//since updating the map have clear the screen
	if (times == 0) cout << "\nYour 1st";
	else if (times == 1) cout << "\nYour 2nd";
	else if (times == 2) cout << "\nYour 3rd";
	cout << " roll: " << roll << "\nYour choise: " << cplayerchoise[times] << endl;
}
void mainturninfoshow(int r, int times) {
	if (cplayerchoise[times] == 'N') {
		system("CLS");
		showinfo();
		for (int z = 0; times > z; z++) showpastmainturninfo(roll[z], z);
	}
	cout << "\nYou have rolled: " << r << endl;
	switch (turnplayer) {
	case 0: {
		p1.movecheak(r, times);
		if (cplayerchoise[times] == 'N') break;
		if (cplayerchoise[times] == 'Y') return;
		callmovingcheak();
		if (nextstage != 0) p1.moving(r);
		break;
	}
	case 1: {
		p2.movecheak(r, times);
		if (cplayerchoise[times] == 'N') break;
		if (cplayerchoise[times] == 'Y') return;
		callmovingcheak();
		if (nextstage != 0) p2.moving(r);
		break;
	}
	case 2: {
		p3.movecheak(r, times);
		if (cplayerchoise[times] == 'N') break;
		if (cplayerchoise[times] == 'Y') return;
		callmovingcheak();
		if (nextstage != 0) p3.moving(r);
		break;
	}
	case 3: {
		p4.movecheak(r, times);
		if (cplayerchoise[times] == 'N') break;
		if (cplayerchoise[times] == 'Y') return;
		callmovingcheak();
		if (nextstage != 0) p4.moving(r);
		break;
	}
	}

	if (cplayerchoise[times] == 'N')mainturninfoshow(r, times);
}
void extramove(int z) {//move control
	if (z > 0) {//massage show you have an extra move
		cout << "\nYou have rolled a \"6\". You can gain a extra move.";
		system("pause");
	}
	rolladie(roll[z]);
	mainturninfoshow(roll[z], z);
	if (cplayerchoise[z] == 'Y') return;
	if (nextstage != 0) {
		system("CLS");
		showinfo();
		for (int x = 0; z >= x; x++) showpastmainturninfo(roll[x], x);
		callwincheak();
	}
}
void turnflow() {
	system("CLS");
	showinfo();
	extramove(0);
	if (quit[0] == 1) return;
	if (roll[0] == 6) {
		extramove(1);
		if (quit[0] == 1) return;
		if (roll[1] == 6) {
			extramove(2);
			if (quit[0] == 1) return;
			if (roll[2] == 6) cout << "You have already reach the third move. You cannot gain any extra move.\n";
		}
	}
	cout << "Your turn ended. Click any buttom to start the next turn.\n";
	system("pause");
	turnplayer = (turnplayer + 1) % numberofplayer;
}
void callresettocomp() {
	p1.resettocomp();
	p2.resettocomp();
	p3.resettocomp();
	p4.resettocomp();
	if (numberofhuman > 0) p1.settohuman();
	if (numberofhuman > 1) p2.settohuman();
	if (numberofhuman > 2) p3.settohuman();
	if (numberofhuman > 3) p4.settohuman();
}
void menu1() {
	gamesetting();
	do {
		turnflow();
	} while (quit[0] != 1);
}
void menu2() {
	system("CLS");
	cout << "* Settings Menu \n[1] Number of players\n[2] Number of human players\n[3] Return to Game Menu\n****\nOption(1 - 3) : ";
	cin >> cplayerchoise[0];
	switch (cplayerchoise[0]) {
	case '1': {//setting up the total number of player
		cout << "\n The number of player: ";
		cin >> numberofplayer;
		cout << "\n The number of player have set to " << numberofplayer << ".\n";
		system("pause");
		break;
	}
	case '2': {//setting the number of player
		cout << "\n The number of human player: ";
		cin >> numberofhuman;
		callresettocomp();
		cout << "\n The number of human player have set to " << numberofhuman << ".\n";
		system("pause");
		break;
	}
	case '3': {
		doquitgame(cplayerchoise[0], 1);
		break;
	}
	default: {
		cout << "You have input a wrong value.\n ";
		system("pause");
	}
	}
	system("CLS");
}
void menu3() {
	system("CLS");
	cout << "* Demo Menu *\n[1] Three consecutive '6'\n[2] Chess touches home and reverses, reaches home and wins\n[3] Chess does not self - overlap, and sends back opponent chess\n[4] Return to Game Menu\n****\nOption(1 - 4) :";
	savenumberofplayer = numberofplayer;//save the previous value
	savenumberofhuman = numberofhuman;//save the previous value
		cin >> cplayerchoise[0];
		switch (cplayerchoise[0]) {
		case '1': {
			numberofplayer = 2;//set up to run the demo
			numberofhuman = 0;
			callresettocomp();
			set6 = 1;//setting, only roll 6
			gamesetting();
			turnflow();
			set6 = 0;//reset
			break;
		}
		case '2': {
			numberofplayer = 2;//set up to run the demo
			numberofhuman = 0;
			callresettocomp();
			gamesetting();
			set52 = 1;//set to roll 5 then 2 then 2
			p1.demoset(42, 100, 100, 100);//setting
			while (quit[0] != 1) turnflow();
			set52 = 0;//reset
			break;
		}
		case '3': {
			numberofplayer = 2;//set up to run the demo
			numberofhuman = 0;
			callresettocomp();
			gamesetting();
			set6 = 1;//setting
			p1.demoset(0, 6, 100, 100);
			p2.demoset(0, 8, 100, 100);
			turnflow();
			set6 = 0;//reset
			break;
		}
		case '4': {
			doquitgame(cplayerchoise[0], 3);
			break;
		}
		default: {
			cout << "You have input a wrong value.\n ";
			system("pause");
		}
		}
	numberofplayer = savenumberofplayer;
	numberofhuman = savenumberofhuman;
	system("CLS");
}

void menu4() {
}
void menu5() {
}
void menu6() {
	doquitgame(cplayerchoise[0], 2);
	system("CLS");
}
void gamemenu() {
	cout << "//" << " Game Menu \n[1] Start Game\n[2] Setting\n[3] Game Demo\n[4] Instructions\n[5] Creditss\n[6] Exit\n***\nOption (1 - 6): ";
	cin >> iplayerchoise;
	switch (iplayerchoise) {
	case 1: {
		menu1();
		break;
	}
	case 2: {
		quit[1] = 0;
		while (quit[1]!=1)menu2();
		break;
	}
	case 3: {
		quit[3] = 0;
		while (quit[3]!= 1)menu3();
		break;
	}
	case 4: {
		menu4();
		break;
	}
	case 5: {
		menu5();
		break;
	}
	case 6: {
		menu6();
		break;
	}
	default: cout << "You have input a wrong value.";
	}
	system("CLS");
}
int main() {
	srand(time(0));
	while (quit[2] != 1) gamemenu();
	return 0;
}
