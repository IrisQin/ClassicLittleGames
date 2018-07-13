#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>






using namespace std;
int tempArray[52];
int money;
struct card {
	int suit;	//1,2,3,4: Diamonds, Clubs, Spades, or Hearts
	int number;     //1~13
	card* next;
};
struct cards {
	card *head;
	card *deck;
};
string cardName(card c) {
	string temp[2];
	if (c.suit == 1) { temp[0] = "Diamonds"; }
	if (c.suit == 2) { temp[0] = "Clubs"; }
	if (c.suit == 3) { temp[0] = "Spades"; }
	if (c.suit == 4) { temp[0] = "Hearts"; }
	if (c.number >= 2 && c.number <= 10) { temp[1] = to_string(c.number); }
	if (c.number == 1) { temp[1] = "Ace"; }
	if (c.number == 11) { temp[1] = "Jack"; }
	if (c.number == 12) { temp[1] = "Queen"; }
	if (c.number == 13) { temp[1] = "King"; }
	return temp[1] + " of " + temp[0];
}
int randint(int i, int j) {
	if (i > j) { swap(i, j); }
	srand((unsigned)time(NULL));
	return i + rand() % (j - i + 1);
}
void shuffle(int *a, int n) {
	for (int i = 0; i < n; ++i) {
		swap(a[i], a[randint(i, n - 1)]);
	}
}
cards *CreateList() {	// Create a random shuffle list
	cards *cardsList = new cards; //
	card *p = NULL;
	card *q = NULL;
	cardsList->head = NULL;
	int index = 0;
	for (int i = 0; i<52; i++) {
		p = new card;
		p->suit = tempArray[index] / 13 + 1;
		p->number = tempArray[index++] % 13 + 1;
		if (cardsList->head == NULL) {
			cardsList->head = p;
		}
		else {
			q->next = p;
		}
		q = p;
	}
	if (cardsList->head != NULL) {
		q->next = NULL;
	}
	cardsList->deck = cardsList->head;
	return cardsList;
}
void addList(cards * cardsList) {
	for (int i = 0; i < 52; i++) {
		tempArray[i] = i;
	}
	shuffle(tempArray, 52);
	card *p = NULL;
	card *q = NULL;
	int index = 0;
	for (int i = 0; i<52; i++) {
		p = new card;
		p->suit = tempArray[index] / 13 + 1;
		p->number = tempArray[index++] % 13 + 1;
		if (cardsList->deck == NULL) {
			cardsList->deck = p;
			// The last node of original list -> (cardsList->deck)
			card *q = cardsList->head;
			if (q == NULL) {
				cardsList->head = cardsList->deck;
			}
			else {
				while (q->next != NULL) {
					q = q->next;
				}
				q->next = cardsList->deck;
			}
		}
		else {
			q->next = p;
		}
		q = p;
	}
	if (cardsList->deck != NULL) {
		q->next = NULL;
	}
}
int listLength(card *head) {
	int count = 0;
	card *p = head;
	while (true) {
		if (p == NULL) { return count; }
		else {
			p = p->next;
			count++;
		}
	}
}
void removeFirstCard(cards *cardsList)
{
	if (cardsList->head == NULL) {
		cout << "Error!" << endl;
		return;
	}
	card *p = cardsList->head;
	cardsList->head = p->next;
	delete p;
}
void removeCard(cards *cardsList, int index)
{
	if (index == 0) {
		removeFirstCard(cardsList);
		return;
	}
	card *p = cardsList->head;
	while (index > 1) {
		p = p->next;
		--index;
	}
	card* q = p->next;
	p->next = q->next;
	delete q;
}
void checkResult(cards* cardsList) {
	int b[5];
	int count = 5;
	card *p = cardsList->head;
	int suit = p->suit;
	bool flush = true;
	while (count--) {
		if (p->suit != suit) { flush = false; }
		b[4 - count] = p->number;
		p = p->next;
	}
	sort(b, b + 5);
	count = 0;
	int pairSuit[5] = { 0,0,0,0,0 };
	for (int i = 0; i < 5; i++) {
		if (b[i] == b[i + 1]) { count++; pairSuit[i] = 1; }
	}
	bool temp = false;
	switch (count) {
	case 0:
		if (flush && b[0] == 1 && b[1] == 10 && b[2] == 11 && b[3] == 12 && b[4] == 13) {
			cout << "Royal Flush! Unbelievable!!!\nYou earned $800!" << endl;
			money += 800;
		}
		else if (b[4] - b[0] == 4 && b[3] - b[0] == 3 && b[2] - b[0] == 2 && b[1] - b[0] == 1) {	// Straight
			if (flush) {
				cout << "Straight Flush! \nYou earned $50!" << endl;
				money += 50;
			}
			else {
				cout << "Straight. \nYou earned $4!" << endl;
				money += 4;
			}
		}
		else if (flush) {
			cout << "Flush. \nYou earned $6!" << endl;
			money += 6;
		}
		else {
			cout << "No Poker hand scored. :(\nYou earned $0" << endl;
		}
		break;
	case 1:
		if (flush) {
			cout << "Flush. \nYou earned $6!" << endl;
			money += 6;
		}
		else {
			for (int i = 0; i < 5; i++) {
				if (pairSuit[i] == 1 && b[i]>10) {
					cout << "One Pair, Jacks or Better. \nYou earned $1!" << endl;
					money += 1;
					break;
				}
				if (i == 4) {
					cout << "No Poker hand scored. :(\nYou earned $0" << endl;
					break;
				}
			}
		}
		break;
	case 2:
		if (flush) {
			cout << "Flush. \nYou earned $6!" << endl;
			money += 6;
		}
		else {
			for (int i = 0; i < 3; i++) {
				if (b[i] == b[i + 2]) {
					cout << "Three of a Kind. \nYou earned $3!" << endl;
					money += 3;
					temp = true;
					break;
				}
			}
			if (!temp) {
				cout << "Two Pairs. \nYou earned $2!" << endl;
				money += 2;
			}
		}
		break;
	case 3:
		if (b[1] == b[3]) {
			cout << "Four of a Kind. \nYou earned $25!" << endl;
			money += 25;
		}
		else {
			cout << "FullHouse. \nYou earned $9!" << endl;
			money += 9;
		}
		break;
	}
	cout << endl;
}
bool checkString(string a) {
	int size = a.size();
	for (int i = 0; i < size; i++) {
		if (a[i] < 65 || a[i]>101 || (a[i]<97 && a[i]>69)) { return false; }
	}
	return true;
}
void deleteEverything(cards * cardsList) {
	card *p = cardsList->head;
	card *q = p->next;
	while (p != NULL) {
		delete p;
		p = q;
		if (p != NULL) {
			q = p->next;
		}
	}
	delete cardsList;

}

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);

	for (int i = 0; i < 52; i++) {
		tempArray[i] = i;
	}
	shuffle(tempArray, 52);
	cards *cardsList = CreateList();
	money = 10;
	cout << "Welcome to Video Poker!\nYou have started with $10\n";
	while (money--) {
		cout << "You pay a $1 ante and now have $" << money << endl << endl;
		cout << "Your hand contains:\n";
		// Draw 5 cards
		int count = 5;
		while (count--) {
			if (cardsList->deck == NULL) {
				addList(cardsList);
			}
			cout << char(69 - count) << ": " << cardName(*cardsList->deck) << endl;
			cardsList->deck = cardsList->deck->next;

		}
		cout << endl << "The deck contains " << listLength(cardsList->deck) << " cards." << endl << endl;
		cout << "OPTIONS...\n- Type the letters for the cards you wish to keep. (i.e., \"acd\")\n- Type \"deck\" to view the cards remaining in the deck.\n- Type \"none\" to discard all cards in your hand.\n- Type \"all\" to keep all cards in your hand.\n- Type \"exit\" to exit the game.\n- Type \"swap\" to CHEAT and swap a card in your hand with one in the deck.\n";
		while (true) {
			cout << endl << "YOUR CHOICE : ";
			string inputStr;
			cin >> inputStr;
			cin.clear(); // Clear error flags.
			cin.ignore(1000, '\n'); // Ignore rest.
			if (inputStr == "deck") {	// Shuffle the deck and print
				const int length = listLength(cardsList->deck);
				int shuffleArray[60];
				for (int i = 0; i < length; i++) { shuffleArray[i] = i; }
				shuffle(shuffleArray, length);
				card cards[60];
				card *p = cardsList->deck;
				for (int i = 0; i < length; i++) {
					//cout << shuffleArray[i]<<" ";
					cards[i] = *p;
					p = p->next;
				}
				for (int i = 0; i < length; i++) {
					cout << cardName(cards[shuffleArray[i]]) << ",";
				}
			}
			else if (inputStr == "none") {	// Discard all
				int count = 5;
				while (count--) {
					removeFirstCard(cardsList);
				}
				cout << "You kept 0 and drew 5 cards.\nYour hand now contains :\n";
				// Draw 5 cards
				count = 5;
				while (count--) {
					if (cardsList->deck == NULL) {
						addList(cardsList);
					}
					cout << char(69 - count) << ": " << cardName(*cardsList->deck) << endl;
					cardsList->deck = cardsList->deck->next;
				}
				checkResult(cardsList);
				cout << "You now have $" << money << endl;
				cout << "Press enter to play another round...";
				cin.get();
				count = 5;
				while (count--) {
					removeFirstCard(cardsList);
				}
				break;
			}
			else if (inputStr == "all") {
				cout << "You kept 5 and drew 0 cards.\nYour hand now contains :\n";
				card *p = cardsList->head;
				int count = 5;
				while (count--) {
					cout << char(69 - count) << ": " << cardName(*p) << "(kept)" << endl;
					p = p->next;
				}
				checkResult(cardsList);
				cout << "You now have $" << money << endl;
				cout << "Press enter to play another round...";
				cin.get();
				count = 5;
				while (count--) {
					removeFirstCard(cardsList);
				}
				break;
			}
			else if (inputStr == "exit") {
				deleteEverything(cardsList);
				system("pause");
				return 0;
				break;
			}
			else if (inputStr == "swap") {
				cout << "Enter the letter of the card in hand:";
				char inputChar;
				cin >> inputChar;
				while (!cin.good() || inputChar < 65 || inputChar>101 || (inputChar<97 && inputChar>69)) {
					cout << "Invalid hand card. Try again:" << endl;
					cin.clear(); // Clear error flags.
					cin.ignore(1000, '\n'); // Ignore rest.
					cin >> inputChar; // Try again.
				}
				cin.ignore(1000, '\n'); // Ignore rest.
				int indexP;
				if (inputChar <= 69) {
					indexP = inputChar - 65;
				}
				else {
					indexP = inputChar - 97;
				}
				cout << "Enter the value of the card in the deck to swap with :";
				int inputInt;
				cin >> inputInt;
				while (!cin.good() || inputInt<1 || inputInt>13) {
					cout << "Invalid hand card. Try again:" << endl;
					cin.clear(); // Clear error flags.
					cin.ignore(1000, '\n'); // Ignore rest.
					cin >> inputInt; // Try again.
				}
				cin.ignore(1000, '\n'); // Ignore rest.
				cout << "Enter the suit (c,d,h,s) of the card in the deck to swap with:";
				cin >> inputChar;
				while (!cin.good() || !(inputChar == 'c' || inputChar == 'd' || inputChar == 'h' || inputChar == 's')) {
					cout << "Invalid hand card. Try again:" << endl;
					cin.clear(); // Clear error flags.
					cin.ignore(1000, '\n'); // Ignore rest.
					cin >> inputChar; // Try again.
				}
				cin.ignore(1000, '\n'); // Ignore rest.
				bool checkInDeck = false;
				card *q = cardsList->deck;
				card *beforeQ = cardsList->head->next->next->next->next;
				int suitNum;
				switch (inputChar) {
				case 'd': suitNum = 1; break;
				case 'c':suitNum = 2; break;
				case 's':suitNum = 3; break;
				case 'h':suitNum = 4; break;
				}
				while (q != NULL) {
					if ((q->number == inputInt) && (q->suit == suitNum)) {
						checkInDeck = true;
						break;
					}
					beforeQ = beforeQ->next;
					q = q->next;
				}
				if (!checkInDeck) {
					cout << "Invalid deck card.";
					continue;
				}
				if (indexP == 0) {
					card *p = cardsList->head;
					cardsList->head = q;
					beforeQ->next = p;
					card *t = p->next;
					p->next = q->next;
					q->next = t;
				}
				else {
					card *p = cardsList->head->next;
					card *beforeP = cardsList->head;
					indexP--;
					while (indexP--) {
						p = p->next;
						beforeP = beforeP->next;
					}
					beforeP->next = q;
					beforeQ->next = p;
					card *t = p->next;
					p->next = q->next;
					q->next = t;
				}
				cout << "Your hand now contains :\n";
				card *p = cardsList->head;
				int count = 5;
				while (count--) {
					cout << char(69 - count) << ": " << cardName(*p) << endl;
					p = p->next;
				}
				continue;
			}
			else if (!cin.good()) {
				cout << "Sorry, I didn't understand, please try again..." << endl;
			}
			else if (checkString(inputStr)) {
				int keep[5] = { 0,0,0,0,0 };
				int count = 0;
				int j = 0;
				for (int i = 0; i < 5; i++) {
					string::size_type idx1 = inputStr.find(char(97 + i));
					string::size_type idx2 = inputStr.find(char(65 + i));
					if (idx1 != string::npos || idx2 != string::npos) {
						count++;
						keep[i] = 1;
					}
				}
				cout << "You kept " << count << " and drew " << 5 - count << " cards.\nYour hand now contains : " << endl;
				int A = 65;
				card *p = cardsList->head;
				for (int i = 0, j = 0; i < 5; i++, j++) {
					if (keep[i] == 1) {		// Keep
						cout << char(A++) << ": " << cardName(*p) << "(keep)" << endl;
						p = p->next;
					}
					else {		// Draw
						p = p->next;
						removeCard(cardsList, j--);
						if (cardsList->deck == NULL) {
							addList(cardsList);
						}
						cardsList->deck = cardsList->deck->next;
					}
				}
				while (p != cardsList->deck) {
					cout << char(A++) << ": " << cardName(*p) << endl;
					p = p->next;
				}
				checkResult(cardsList);
				cout << "You now have $" << money << endl;
				cout << "Press enter to play another round...";
				cin.get();
				count = 5;
				while (count--) {
					removeFirstCard(cardsList);
				}
				break;
			}
			else {
				cout << "Sorry, I didn't understand, please try again..." << endl;
			}
		}
		cout << endl << "---------------------------------------" << endl;
	}
	cout << "GAME OVER";
	deleteEverything(cardsList);
	system("pause");
	return 0;
}