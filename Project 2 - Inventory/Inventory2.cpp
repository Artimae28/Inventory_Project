#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

void displayHelpText();

int main()
{
	string command;
	bool keepRunning = true;
	cout << "Supported commands: \n"
		<< "     p    Build Packed Data File.\n"
		<< "     u    Create unpacked (text) data from packed data.\n"
		<< "     h    Print this help text.\n"
		<< "     q    Quit (exit) the program." << endl;
	cout << endl;
	//Write our command loop, essentially our main menu
	while (keepRunning == true)
	{
		cout << "Command: ";
		getline(cin, command);

		if (command == "h")
		{
			displayHelpText();
		}
	}
}

void displayHelpText()
{
	cout << "Supported commands: \n"
		<< "     p    Build Packed Data File.\n"
		<< "     u    Create unpacked (text) data from packed data.\n"
		<< "     h    Print this help text.\n"
		<< "     q    Quit (exit) the program." << endl;
	cout << endl;
}