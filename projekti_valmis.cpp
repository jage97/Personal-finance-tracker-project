// projekti.cpp : Defines the entry point for the console application.
//_CRT_SECURE_NO_WARNINGS
#include <string>
#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
using namespace std;

class Transaction {
	friend ostream &operator<<(ostream &out, const Transaction &p);
public:
	friend class Account;
private:
	string description;
	string date;
	float amount;
};

class Account {
	friend ostream &operator<<(ostream &out, const Account &a);
public:
	Account();
	void initialize();
	void saveToDisk();
	void readFromDisk();
	void printTransactions();
	void newExpence();
	void newIncome();
	void newBalance(float i);
	float getAmount();
private:
	vector<Transaction> allTransactions;
	float balance;
	Transaction t;
};
void Account::newBalance(float i) {
	balance += i;
}

void Account::newIncome() {
	float temp;
	do {
		cout << "Give the amount of money you want to add: ";
		cin >> temp;
		if (cin.fail()) {
			cout << "Please input numbers!!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else if (!cin.fail()) {
			if (temp < 0) {
				cout << "Please only enter positive numbers." << endl;
			}
			if (temp >= 0) break;
		}
	} while (1);
	t.amount = temp;
	cin.ignore();
	cout << endl << "Give a description for you're transaction: ";
	getline(cin, t.description);
	cout << endl << "Give a date for you're transaction (DD-MM-YYYY): ";
	getline(cin, t.date);
	cout << endl << t;
	cout << endl << endl;
	newBalance(t.amount);				//Balance is updated
	allTransactions.push_back(t);		//The new transaction is pushed into the vector
}
void Account::initialize() {
	allTransactions.clear();
	balance = 0;
}
void Account::newExpence() {
	float temp;
	do {
		cout << "Give the amount of money spent: ";
		cin >> temp;
		if (cin.fail()) {
			cout << "Please input numbers!!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else if (!cin.fail()){
			if (temp < 0) {
				cout << "Please only enter positive numbers." << endl;
			}
			if (temp >= 0) break;
		}
	} while (1);
	cin.ignore();
	t.amount = (temp*-1);
	cout << endl << "Give a description for you're transaction: ";
	getline(cin, t.description);
	cout << endl << "Give a date for you're transaction (DD-MM-YYYY): ";
	getline(cin, t.date);
	cout << endl << t;
	cout << endl << endl;
	newBalance(t.amount);				//Balance is updated
	allTransactions.push_back(t);		//The new transaction is pushed into the vector
}
void Account::readFromDisk() {
	float tempAmount;
	string tempDate;
	string tempDesc;
	ifstream records;
	int i = 0;
	records.open("data.txt");
	if (records.fail()) {
		cout << "error opening file!" << endl;
	}
	if (records.is_open()) {
		allTransactions.clear();
		while (!records.eof()) {
			if (i == 0) {				//First line inside the file is the balance. The rest are transactions
				records >> balance;
				i++;
			}
			records >> tempDesc;
			transform(tempDesc.begin(), tempDesc.end(), tempDesc.begin(), [](char ch) {	//turn underscores into spaces
				return ch == '_' ? ' ' : ch;
			});
			records >> tempDate;
			records >> tempAmount;
			if (records.eof()) break;
			new Transaction(t);
			t.amount = tempAmount;
			t.date = tempDate;
			t.description = tempDesc;
			allTransactions.push_back(t);
		}
		records.close();
		i--;
	}
	if (allTransactions.empty()) {
		cout << endl << "No records on disk!" << endl;
	}
}
void Account::saveToDisk() {
	ofstream records;
	records.open("data.txt");
	if (records.fail()) {
		cout << "error opening file" << endl;
		exit(1);
	}
	records << Account::balance << endl;
	for (int i = 0;i < allTransactions.size();i++) {
		transform(allTransactions[i].description.begin(), allTransactions[i].description.end(), allTransactions[i].description.begin(), [](char ch) {	//turn spaces into underscores
			return ch == ' ' ? '_' : ch;
		});
		records << allTransactions[i].description << endl << allTransactions[i].date << endl << allTransactions[i].amount << endl;

		transform(allTransactions[i].description.begin(), allTransactions[i].description.end(), allTransactions[i].description.begin(), [](char ch) {	//turn underscores back to spaces
			return ch == '_' ? ' ' : ch;
		});
	}
	cout << endl << "Records saved!" << endl;
	records.close();
}
void Account::printTransactions() {
	float sum = 0;
	for (int i = 0;i < allTransactions.size();i++) {
		cout << endl << "Transaction nr." << i + 1 << allTransactions[i];
		sum += allTransactions[i].amount;
	}
	if (sum > 0) {
		cout << endl << "You are saving money!";
	}
	else if (sum < 0) {
		cout << endl << "You are spending more than you earn!";
	}
	cout << endl << endl;
}
Account::Account() {
	balance = 0;
}

ostream &operator<<(ostream &out, const Account &a) {
	cout << a.balance;
	return out;
};
ostream &operator<<(ostream &out, const Transaction &p) {
	cout << " Description: " << p.description << " Date: " << p.date << " Amount:" << p.amount;
	return out;
};
float Account::getAmount() {
	return balance;
}

int main(void) {
	cout << fixed << setprecision(2);
	Account person;
	int i = 0;
	char c;
	do {
		cout << "Do you want to read previous records from the disk? (y or n): ";
		cin >> c;
		cout << endl;
		if (c == 'y') {
			person.readFromDisk();
			break;
		}
		else if (c == 'n') {
			break;
		}
		else {
			cout << "Please enter either y or n" << endl;
		}
	} while (1);

	do {
		cout << "Your current account balance is: " << person << " euros" << endl << endl;
		cout << "what would you like to do?" << endl;
		cout << "1. Initialize" << endl;
		cout << "2. Save records to disc" << endl;
		cout << "3. Read records from disc" << endl;
		cout << "4. Add money" << endl;
		cout << "5. Spend money" << endl;
		cout << "6. Print transactions" << endl;
		cout << "7. Exit program" << endl;
		cin >> i;
		if (i == 1) {
			do {
				cout << endl << "Are you sure you want to clear all records?(y = yes n = no): ";
				cin >> c;
				cout << endl;
				if (c == 'y') {
					person.initialize();
					break;
				}
				else if (c == 'n') {
					break;
				}
				else {
					cout << "Please enter either y or n" << endl;
				}
			} while (1);
		}

		else if (i == 2) {
			person.saveToDisk();
		}
		else if (i == 3) {
			person.readFromDisk();
		}
		else if (i == 4) {
			person.newIncome();
		}
		else if (i == 5) {
			person.newExpence();
		}
		else if (i == 6) {
			person.printTransactions();
		}
	} while (i != 7);
	return 0;
}