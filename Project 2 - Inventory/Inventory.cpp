//   CSC237 Project2:  Reading/Writing files from one format to another.
//   Student:   Arti Kuhrmeier
//   Due Date:  11/18/24
//   Description: 
//      This program reads a text document, puts the text into an array, and displays the array.
//      It allows the user to enter their own data, or modify existing data by either adding or removing unit quantities.
//      It can also output the modified data into a new file, which can then be read by the program.
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "InventoryItem.h"

using namespace std;

//Function prototypes, listed in order.
void addParts(InventoryItem inventoryItems[], int itemCount);
void displayHelpText();
int readInventoryFromFile(InventoryItem inventoryItems[], int itemCount);
int splitLineToArray(string inputText, string inputDelim, string outputFieldArray[], int maxFields);
void createNewInventoryItem(InventoryItem inventoryItems[], int& itemCount);
void writeInventoryToFile(InventoryItem inventoryItems[], int itemCount);
void displayInventoryList(InventoryItem inventoryItems[], int itemCount);
void removeParts(InventoryItem inventoryItems[], int itemCount);
bool quitCommand();
bool confirmYN(string promptText);

int main()
{
	/*Create an array of 100 InventoryItem objects
	Create an initial item count
	Set up a command loop boolean with a way to turn it off*/
	const int ARRAY_SIZE = 100;
	InventoryItem inventoryItems[ARRAY_SIZE];
	int itemCount = 0;
	string command;
	bool keepRunning = true;

	//Run the help text first and foremost.
	//This allows the user to see the commands.
	displayHelpText();

	/*Simple command loop which keeps the program running, and gives the user a sort of Main Menu.*/
	while (keepRunning == true)
	{
		cout << "Command: ";
		getline(cin, command);

		if (command == "a")
		{
			addParts(inventoryItems, itemCount);
		}
		else if (command == "h")
		{
			displayHelpText();
		}
		else if (command == "i")
		{
			itemCount = readInventoryFromFile(inventoryItems, itemCount);
		}
		else if (command == "n")
		{
			createNewInventoryItem(inventoryItems, itemCount);
		}
		else if (command == "o")
		{
			writeInventoryToFile(inventoryItems, itemCount);
		}
		else if (command == "p")
		{
			displayInventoryList(inventoryItems, itemCount);
		}
		else if (command == "q")
		{
			keepRunning = !quitCommand();
		}
		else if (command == "r")
		{
			removeParts(inventoryItems, itemCount);
		}
	}
	return 0;
}

/*The addParts function asks the user to choose an item from the array.
After a few checks and balances, it allows the user to add a specified amount of units to the chosen item.*/
void addParts(InventoryItem inventoryItems[], int itemCount) //Command: a
{
	int itemNum;
	int unitQuantity;
	cout << "Choose an Item Number: ";
	cin >> itemNum;
	cin.clear();
	cin.ignore(123, '\n');
	if (itemNum >= itemCount)
	{
		cout << "ERROR: No such item exists." << endl;
	}
	else
	{
		cout << "How many parts to add? ";
		cin >> unitQuantity;
		cin.clear();
		cin.ignore(123, '\n');
		if (unitQuantity < 0)
		{
			cout << "ERROR: Cannot enter a negative number!" << endl;
		}
		else
		{
			unitQuantity = inventoryItems[itemNum].getUnits() + unitQuantity;
			if (unitQuantity < 0 || unitQuantity > 30)
			{
				cout << "ERROR: Item cannot have more than 30 units!" << endl;
			}
			else
			{
				inventoryItems[itemNum].setUnits(unitQuantity);
			}
		}
	}
}

/*readInventoryFromFile asks the user to input a file name. It then reads the file, and converts the text into their respective data types.
Afterward, it puts the data fields into an array by using the SplitLineToArray function.*/
int readInventoryFromFile(InventoryItem inventoryItems[], int itemCount) //Command: i
{
	const int MAX_NUMBER_OF_FIELDS = 20;
	const string DEFAULT_DELIMITER = "|";

	ifstream inputFile;
	string inputFileName;
	string inputBuffer;
	string itemArray[MAX_NUMBER_OF_FIELDS];
	int fieldCount = 0;
	int lineCount = 0;

	cout << "Enter name of input file: ";
	getline(cin, inputFileName);
	inputFile.open(inputFileName);
	// Check for file open error.
	if (inputFile.fail())
	{
		cout << "(line " << __LINE__ << ") Error opening file:  " << inputFileName << endl;
	}

	while (getline(inputFile, inputBuffer)) 
	{
		splitLineToArray(inputBuffer, DEFAULT_DELIMITER, itemArray, MAX_NUMBER_OF_FIELDS);
		lineCount++;

		string description = itemArray[1];
		double cost = stod(itemArray[2]);
		int units = stoi(itemArray[3]);

		inventoryItems[itemCount].setDescription(description);
		inventoryItems[itemCount].setCost(cost);
		inventoryItems[itemCount].setUnits(units);
		itemCount++;
	}
	cout << lineCount << " records loaded to array." << endl;
	inputFile.close();
	return itemCount;
}

//splitLineToArray reads a deliminted file and converts it into an array, to be used by the program.
int splitLineToArray(string inputText, string inputDelim,
	string outputFieldArray[], int maxFields)
{
	//	Local Variables:
	int nextFieldPosition = 0;		//	Start position of data field.
	int foundPosition;				//  Position where delimiter was found,
	int fieldCount = 0;		        //  Number of data fields found so far,
	bool endOfLine = false;			//	Flag -- end of input line detected,
	string dataField;				//  Copy of the data field.

	if (inputDelim == "") {
		cout << "ERROR(splitLineToArray): the inputDelim parameter "
			<< "must NOT be the empty string." << endl;
		return 0;
	}
	while (!endOfLine)
	{
		foundPosition = inputText.find(inputDelim, nextFieldPosition);
		if (foundPosition != string::npos)
		{
			// (Here if 'inputDelim' was found in the input string.)
			// Copy the data field contents from the input string to the
			// 'dataField' local variable.
			dataField = inputText.substr(nextFieldPosition, foundPosition - nextFieldPosition);
			fieldCount++;

			// Adjust 'nextFieldPosition' as preparation for finding NEXT delimiter.
			nextFieldPosition = foundPosition + inputDelim.length();
		}
		else
		{
			// (Here if the string 'find' function reached the end of the input 
			// string WITHOUT finding the inputDelim value.)
			endOfLine = true;   // (The "while-loop" will not repeat again.)

			// Set foundPosition to the LENGTH of the original inputText, to handle
			// the case where there is more text AFTER the last occurrence of the 
			// inputDelim string.
			foundPosition = inputText.length();

			// If there is text to output to the caller, use the substr function
			// (in the string class) to copy that text to the dataField local 
			// variable, and increment the fieldCount local variable.
			if (foundPosition > nextFieldPosition)
			{
				dataField = inputText.substr(nextFieldPosition,
					foundPosition - nextFieldPosition);
				fieldCount++;
			}
		}

		if (fieldCount > 0)
		{
			if (fieldCount > maxFields)
			{
				cout << "ERROR at line __LINE__: too many input fields, fieldCount="
					<< fieldCount << endl;
			}
			else
			{
				outputFieldArray[fieldCount - 1] = dataField;
			}
		}
	} // (end while)

	return fieldCount;
} // (end function splitLineToArray )

/*createNewInventoryItem allows the user to create an inventory item from scratch.*/
void createNewInventoryItem(InventoryItem inventoryItems[], int& itemCount)
{
	string description;
	double cost;
	int units;

	cout << "Enter description for new Item: ";
	getline(cin, description);

	cout << "Enter unit cost for new Item: ";
	cin >> cost;
	while (cost < 0)
	{
		cout << "ERROR: Cost cannot be negative!" << endl;
		cout << "Enter unit cost for new Item: ";
		cin >> cost;
	}

	cout << "Enter initial quantity for the new Item: ";
	cin >> units;
	cin.clear();
	cin.ignore(123, '\n');
	while (units < 0 || units > 30)
	{
		cout << "ERROR: initial quantity must be >= zero and <= 30." << endl;
		cout << "Enter initial quantity for the new Item: ";
		cin >> units;
		cin.clear();
		cin.ignore(123, '\n');
	}

	inventoryItems[itemCount].setDescription(description);
	inventoryItems[itemCount].setCost(cost);
	inventoryItems[itemCount].setUnits(units);
	cout << "Announcing a new inventory Item: " << inventoryItems[itemCount].getDescription() << endl;
	itemCount++;
	cout << "We now have " << itemCount << " different inventory Items in stock!" << endl;
}

/*writeInventoryToFile takes the InventoryItem array and converts it into pipe-delimited text in a text file.
NOTE: The created file can be read from the program using the readInventoryFromFile function!*/
void writeInventoryToFile(InventoryItem inventoryItems[], int itemCount) //Command: o
{
	ofstream outputFile;
	string outputFileName;
	const string DEFAULT_DELIMITER = "|";

	cout << "Enter name of output file: ";
	getline(cin, outputFileName);
	outputFile.open(outputFileName);

	// Check for file open error.
	if (outputFile.fail())
	{
		cout << "(line " << __LINE__ << ") Error opening file:  " << outputFileName << endl;
	}

	for (int i = 0; i < itemCount; i++)
	{
		outputFile << i << DEFAULT_DELIMITER << inventoryItems[i].getDescription()
			<< DEFAULT_DELIMITER << inventoryItems[i].getCost()
			<< DEFAULT_DELIMITER << inventoryItems[i].getUnits() << endl;
	}
	cout << itemCount << " records written to file." << endl;
	outputFile.close();
}

/*displayInventoryList prints out the array into an easily readable format.*/
void displayInventoryList(InventoryItem inventoryItems[], int itemCount) //Command: p
{
	cout << endl;
	cout << "Item Num" << setw(15) << "Description" << setw(46) << "Cost" << setw(15) << "Quantity" <<  endl;
	cout << "--------" << setw(15) << "-----------" << setw(46) << "----" << setw(15) << "--------" << endl;
	for (int i = 0; i < itemCount; i++)
	{
		cout << "   " << left << setw(9) << i << setw(52) << inventoryItems[i].getDescription() 
			 << right << fixed << setprecision(2) << setw(5) << inventoryItems[i].getCost()
			 << setw(15) << inventoryItems[i].getUnits() << endl;
	}
	cout << itemCount << " records." << endl;
}

/*removeParts works just like addParts, but in reverse.*/
void removeParts(InventoryItem inventoryItems[], int itemCount) //Command: r
{
	int itemNum;
	int unitQuantity;
	cout << "Choose an Item Number: ";
	cin >> itemNum;
	cin.clear();
	cin.ignore(123, '\n');
	if (itemNum >= itemCount)
	{
		cout << "ERROR: No such item exists." << endl;
	}
	else
	{
		cout << "How many parts to remove? ";
		cin >> unitQuantity;
		cin.clear();
		cin.ignore(123, '\n');
		if (unitQuantity < 0)
		{
			cout << "ERROR: Cannot enter a negative number!" << endl;
		}
		else if (unitQuantity > inventoryItems[itemNum].getUnits())
		{
			cout << "ERROR: You are attempting to remove more parts than the Item currently holds." << endl;
		}
		else
		{
			unitQuantity = inventoryItems[itemNum].getUnits() - unitQuantity;
			inventoryItems[itemNum].setUnits(unitQuantity);
		}
	}
}

/*Here we show a reference table for commands. This is also shown as soon as the program starts.*/
void displayHelpText() //Command: h
{
	cout << "Supported commands: \n"
		<< "     a    Add parts.\n"
		<< "     h    Print Help text.\n"
		<< "     i    Input inventory data from a file.\n"
		<< "     n    New inventory item.\n"
		<< "     o    Output inventory data to a file.\n"
		<< "     p    Print inventory list.\n"
		<< "     q    Quit (end the program).\n"
		<< "     r    Remove parts." << endl;
	cout << endl;
}

/*This quit command confirms whether or not you want to exit the program.*/
bool quitCommand() //Command: q
{
	return confirmYN("Are you sure that you want to exit the program? ");
}

/*This block of code sets various flags to confirm whether or not the user's input was deliberate.
In this case, it's used to confirm that the user wishes to exit the program.*/
bool confirmYN(string promptText)
{
	string inputString;
	bool confirmationValue = false;
	bool inputIsOK = false;

	do
	{
		// input from user 
		cout << promptText;
		getline(cin, inputString);

		if (inputString == "y" || inputString == "Y")
		{
			confirmationValue = true;
			inputIsOK = true;
		}
		else if (inputString == "n" || inputString == "N")
		{
			confirmationValue = false;
			inputIsOK = true;
		}
		else
		{
			cout << "Invalid input:  " << inputString << ".  Please input y or n." << endl;
		}
	} while (inputIsOK == false);
	return confirmationValue;
}