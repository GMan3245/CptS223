/*
Name: Gaynay Doo
Class: CPTS 223 Advanced Data Structure 1-2pm
PA: 1. Matching Game for Linux Commands and C++ Review 
*/

/*
ADVANTAGES/DISADVANTAGES LINKED LIST    
A link list can carry more things than a static list since you can always expand it.
A few drawbacks with a linked listrequires more memory, and traversing it lengthens it in comparison to an array.
ADVANTAGES/DISADVANTAGES ARRAY
An array search takes constant O(N) time.
A few drawbacks of an arrayWe will need to make a new array and copy the old array over if we have more items than our array can hold.
*/


#include "linkedlist.h"
using namespace std;


int main()
{
    
    LinkedList<string, string> list;
    string userName, newCommandToAdd, oldCommandTodelete;
    int selection, numQuestions, score;

    if (ifstream("game_state.txt")) {
        score = list.loadGame("game_state.txt");
        cout << "Previous game state loaded. Current score: " << score << endl;
    } else {
        // Read the CSV file and populate the linked list (you can replace "commands.csv" with your actual CSV file)
        string filename = "commands.csv";
        list.readCSV(filename);
    }

    while (true)
    {
        cout << "Please select an option listed below:\n";
        cout << "1. Game Rules\n2. Play Game\n3. Load Previous Game\n4. Add Command\n5. Remove Command\n6. Exit\n";
        cout << "Your selection: ";
        cin >> selection;     

        if (cin.fail()) // the input is not an integer
        {
            cout << "Your selection is invalid. Please select again." << endl;
            cout << "=============================================================================" << endl << endl;
            cin.clear();
            cin.ignore();
        }
        else
        {
            switch (selection)
            {
                case 1: // Game Rules
                {
                    cout << "================================= Game Rule ===================================" << endl;
                    cout << "To play the game, select \"2\" where you will be prompted for your name and number of questions.\n";
                    cout << "Each question presents a specific Linux command where you will be able to choose from 3 different options. Each correct answer yields a point.\n";
                    cout << "You can also add and delete commands as needed.\n";
                    cout << "================================= Game Rule ===================================" << endl << endl;

                    break;
                }

                case 2: case 3: // 2. Play Game; 3. Load Previous Game
                {
                    
                    cout << "Please enter your name: ";
                    cin >> userName;

                    list.readCSV("commands.csv");
                    vector<pair<string, string>> commandList = list.getAllCommands();
                    cout << "Enter the number of questions (5-30): ";
                    cin >> numQuestions;


                    if (numQuestions < 5 || numQuestions > 30) {
                        cout << "Please enter a valid number between 5 and 30!" << endl;
                    } 
                    else {
                    askQuestion(commandList, numQuestions);
                    }
                    
                    list.saveGame("game_state.txt", score);
                    cout << "Game state saved. Exiting...\n";

                    break;
                }

                case 4: // Add Command
                {
                    string command, description;
                    cout << "Enter the new command: ";
                    cin >> command;
                    cout << "Enter the description: ";
                    cin.ignore();
                    getline(cin, description);
                    list.addCommand(command, description, "command.csv");
                    break;
                }

                case 5: // Remove Command
                {
                    string command;
                    cout << "Enter the command to remove: ";
                    cin >> command;
                    list.removeCommand(command, "commands.csv");
                    break;
                }
                    
                case 6: // Exit
                {
                    return 0;
                }


            } // end of switch(selection)
        } // end of else: check (cin.fail())
        
    } // end of while(true)
} // end of int main()


