// Program 
// February 17, 2024
// This program simulates a database to store animal types, count, and
// endangered status with a sort and search function
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <array>
#include <vector>
#include <cctype>
using namespace std;

// declare constants
const int
MIN_COUNT = 0,			// minimum number of animals
ADD_CHOICE = 1,			// choice to add an animal
DISPLAY_CHOICE = 2,		// choice to display animals
ENDANGERED_CHOICE = 3,	// choice to display endangered animals
QUIT_CHOICE = 4,		// choice to quit program
MENU_CHOICE_SIZE = 1,	// size of menu choice field
ENDANGERED_POP = 100,	// population considered endangered
TYPE_SIZE = 30;			// size of the animal type field

// define structures
struct species
{
	char typeAnimal[TYPE_SIZE] = "none";	// stores type of animal
	int numAnimal = 0;						// stores number of animals
	bool endangered = false;				// flag for endangered
};

// define function prototypes
void header();							// display program info to user
void dataEntry(vector<species>&);		// allows data entry of animals
void dataDisplay(vector<species>);		// displays data to user
void endangeredDisplay(const vector<species>);// displays endangered animals
void selectionSort(vector<species>&);	// sorts an array by selection sorting
void vectorSwap(species&, species&);// swaps two vector elements
bool positiveValid(int);				// validates a number is positive
bool menuValid(int);					// validates menu is between first and quit option
bool noneValid(const char name[]);		// validates user input is not none
bool duplicateValid(const vector<species>,
	const char name[]);					// validates user has not entered duplicate
bool exitProgram();						// ask user if they really want to quit

int main()
{
	// define variables
	string input;					// temporary string for user input
	int menuChoice;					// user entered menu choice
	vector<species> animalPen = {	// initialize a new vector for struct
		{"California Puma",2005,false},
		{"Bengal Tiger",97,true},
		{"White Rhino",45,true},
		{"California Condor",275,false},
		{"Grey Wolf",77,true},
		{"Three Toed Sloth",1725,false}
	};

	// call header
	header();

	do
	{
		// display a menu and  ask user for a menu choice
		do
		{
			cout << ADD_CHOICE << ". Add animal(s)\n";
			cout << DISPLAY_CHOICE << ". Display animals\n";
			cout << ENDANGERED_CHOICE << ". Display endangered animals\n";
			cout << QUIT_CHOICE << ". Quit\n\n";
			cout << "Enter the menu choice: ";
			getline(cin, input);
		} while (menuValid(stoi(input)));
		menuChoice = stoi(input);

		// switch statement based on user choice
		switch (menuChoice)
		{
		case ADD_CHOICE:
		{
			dataEntry(animalPen);
			break;
		}
		case DISPLAY_CHOICE:
		{
			dataDisplay(animalPen);
			break;
		}
		case ENDANGERED_CHOICE:
		{
			endangeredDisplay(animalPen);
			break;
		}
		case QUIT_CHOICE:
		{
			if (exitProgram())
				menuChoice = ADD_CHOICE;
			break;
		}
		default:
		{
			cout << "Something went wrong\n\n";
			break;
		}
		}
	} while (menuChoice != QUIT_CHOICE);

	// clear the vector
	animalPen.clear();

	// goodbye message
	cout << "\nThank you for using the CMSY 171 Animal Count Program\n\n";

	// end of main
	system("pause");
	return 0;
}

// shows program title, copyright, and program info to user
void header()
{
	cout << "\t\t\t\tCMSY 171 Lab 2\n"
		<< "Copyright 2024 - Howard Community College All rights reserved;"
		<< "Unauthorized duplication prohibited.\n\n"
		<< "\t\t\tWelcome to CMSY-171 Animal Count Program\n\n";
}


// validates menu choice, returns true if invalid and keeps looping, returns false if
// valid and exits loop
bool menuValid(int choice)
{
	if (ADD_CHOICE > choice || choice > QUIT_CHOICE)
	{
		cout << "Invalid choice! Try again: \n\n";
		return true;
	}
	return false;
}

// tests if animal count is positive, returns true if invalid and keeps looping,
// returns false if valid and exits loop
bool positiveValid(int num)
{
	if (num < MIN_COUNT)
	{
		cout << "Invalid count! Try again\n";
		return true;
	}
	return false;
}

// tests if user entered none. If user entered none, return true and stop loop
// if not none, return false and ask for more info
bool noneValid(const char name[])
{
	char temp[TYPE_SIZE];	// temp cstring to test none
	for (int index = 0; index < TYPE_SIZE; index++)
		temp[index] = toupper(name[index]);
	if (!strcmp(temp, "NONE"))
		return true;
	return false;
}

// ask user if they really want to quit, return true if they do not and keep
// looping, return false if they want to quit and leave loop
bool exitProgram()
{
	char temp[TYPE_SIZE];	// holds user's choice to quit
	cout << "\nDo you wish to exit the program (Y or N): ";
	cin.getline(temp, TYPE_SIZE);
	while (toupper(temp[0]) != 'Y' && toupper(temp[0]) != 'N')
	{
		cout << "Please enter Y or N: ";
		cin.getline(temp, TYPE_SIZE);
	}
	if (toupper(temp[0]) == 'Y')
		return false;
	else
		return true;
}

// lets user enter data on the animal types, if user enters none, quit out
// Added "&" to the argument because otherwise the changes were local
// and did not appear to the dataDisplay function
void dataEntry(vector<species>& animalPen)
{
	string input;				// temporary input string
	bool noneFlag = true;		// flag for none 
	species newAnimal;			// temporary struct for data entry

	// loop until none input
	while (noneFlag)
	{
		do // ask user for animal name, if duplicate then repeat prompt
		{  // if not duplicate, or user entered none, stop looping
			cout << "Please enter an animal type (none to stop): ";
			cin.getline(newAnimal.typeAnimal, TYPE_SIZE);
		} while (duplicateValid(animalPen, newAnimal.typeAnimal));
		// if user types none, set flag to false and exit loop
		if (noneValid(newAnimal.typeAnimal))
			noneFlag = false;

		// if not none and not duplicate, ask for the rest of info. 
		// Added endangered=false because it would always be true once 
		// one entry is true, never false anymore
		if (noneFlag)
		{
			do
			{
				cout << "Enter the animal type's count: ";
				getline(cin, input);
			} while (positiveValid(stoi(input)));
			newAnimal.numAnimal = stoi(input);
			if (newAnimal.numAnimal < ENDANGERED_POP)
				newAnimal.endangered = true;
			else
				newAnimal.endangered = false;
			// Add data from temp struct to permanent struct
			animalPen.push_back(newAnimal);
		}
	}
	cout << endl;
}

// test if name is a duplicate. If it is, return true and keep looping
// if not, return false and exit loop
bool duplicateValid(const vector <species> animal, const char name[])
{
	for (auto index : animal)
		if (!strcmp(index.typeAnimal, name))
		{
			cout << "Error - this animal is already in the database. "
				<< "Please reenter.\n";
			return true;
		}
	return false;
}


// display contents of animal array to user, if an entry has 0 number of
// animals, end the loop and redisplay menu
void dataDisplay(vector<species> animals)
{
	// if the vector is empty, give an error 
	if (animals.empty())
		cout << "You have not entered any data, returning to menu.\n\n";
	else
	{
		// send the vector to a sort function
		selectionSort(animals);
		cout << "\nThere are " << animals.size() << " animals in the database.\n";
		for (int index = 0; index < animals.size(); index++)
		{
			cout << "\nAnimal: " << animals.at(index).typeAnimal << endl;
			cout << "Has a count of: " << animals.at(index).numAnimal << endl;

			if (animals.at(index).endangered)
				cout << "This animal is endangered!!\n\n";
			else
				cout << "This animal is not endangered!!\n\n";
		}

	}
}

// display endangered species to user, if there are no endangered species
// then counter variable is equal to 0, which is the same as false
// if counter is incremented, it is not 0, which is the same as true
// if counter is false, display error message and redisplay menu
void endangeredDisplay(const vector<species> animals)
{
	int counter = 0;	// temporary counter variable
	for (auto status : animals)
		if (status.endangered)
		{
			cout << "Animal: " << status.typeAnimal << " is endangered!!\n";
			counter++;
		}
	if (!counter)
		cout << "There are no endangered species. Redisplaying menu.\n";
	cout << endl;
}

// Sort an array using selection sorting in alphabetical order according
// to the ADT animal type/name
void selectionSort(vector<species> &animal)
{
	string min;
	int index;

	for (int i = 0; i < animal.size(); i++)
	{
		index = i;
		min = animal.at(i).typeAnimal;
		for (int j = i + 1; j < animal.size(); j++)
		{
			if (animal.at(j).typeAnimal < min)
			{
				index = j;
				min = animal.at(j).typeAnimal;
			}
		}
		vectorSwap(animal.at(index), animal.at(i));
	}
}

// Swaps two vector elements, using a temporary ADT
void vectorSwap(species& a, species& b)
{
	species temp;
	temp = a;
	a = b;
	b = temp;
}