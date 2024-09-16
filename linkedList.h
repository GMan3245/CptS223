#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Node class with two types: T1 for the first value, and T2 for the second value
template <typename T1, typename T2>
class Node {
public:
    T1 command;   // first data of type T1 (command)
    T2 description; // second data of type T2 (description)
    Node* next;  // pointer to the next node

    // Constructor to initialize a node
    Node(T1 cmd, T2 desc) {
        command = cmd;
        description = desc;
        next = nullptr;
    }
};

// LinkedList class that uses the Node with template types T1 and T2
template <typename T1, typename T2>
class LinkedList {
public:
    Node<T1, T2>* head;

    // Constructor to initialize an empty linked list
    LinkedList() {
        head = nullptr;
    }

    // Function to insert a new node at the end of the list
    void append(T1 cmd, T2 desc) {
        Node<T1, T2>* newNode = new Node<T1, T2>(cmd, desc);  // create a new node
        if (head == nullptr) {          // if list is empty, set new node as head
            head = newNode;
            return;
        }
        Node<T1, T2>* last = head;
        while (last->next != nullptr) {  // traverse to the last node
            last = last->next;
        }
        last->next = newNode;           // link last node to new node
    }

    // Function to read CSV and populate the linked list
    void readCSV(const string& filename) {
        ifstream file(filename);  // open the file
        if (!file.is_open()) {
            cerr << "Error: Could not open the file!" << endl;
            return;
        }

        string line;
        while (getline(file, line)) {   // read each line from the CSV
            stringstream ss(line);
            string name, description;

            // Parse name and description from CSV (assuming name,description format)
            if (getline(ss, name, ',') && getline(ss, description, ',')) {
                append(name, description);  // add to the linked list
            }
        }

        file.close();  // close the file
    }

    // Function to write the linked list back to the CSV
    void writeCSV(const string& filename) {
        ofstream file(filename);  // open the file in write mode
        if (!file.is_open()) {
            cerr << "Error: Could not open the file for writing!" << endl;
            return;
        }

        Node<T1, T2>* current = head;
        while (current != nullptr) {
            file << current->command << "," << current->description << endl;
            current = current->next;
        }

        file.close();  // close the file
    }

    // Function to get all commands and descriptions from the linked list
    vector<pair<T1, T2>> getAllCommands() {
        vector<pair<T1, T2>> commandList;
        Node<T1, T2>* current = head;

        while (current != nullptr) {
            commandList.push_back(make_pair(current->command, current->description));
            current = current->next;
        }

        return commandList;
    }

    // Function to add a new command
    void addCommand(T1 cmd, T2 desc, const string& filename) {
        append(cmd, desc);  // Add the command to the linked list
        writeCSV(filename);  // Update the CSV file with the new command
        cout << "Command added successfully!\n";
    }

    // Function to remove a command by name
    void removeCommand(T1 cmd, const string& filename) {
        if (head == nullptr) {
            cout << "The list is empty. Nothing to remove.\n";
            return;
        }

        // If the head is the node to be removed
        if (head->command == cmd) {
            Node<T1, T2>* temp = head;
            head = head->next;
            delete temp;
            writeCSV(filename);  // Update the CSV file
            cout << "Command removed successfully!\n";
            return;
        }

        // Traverse the list to find the node to remove
        Node<T1, T2>* current = head;
        while (current->next != nullptr && current->next->command != cmd) {
            current = current->next;
        }

        if (current->next == nullptr) {
            cout << "Command not found in the list.\n";
            return;
        }

        // Remove the node
        Node<T1, T2>* temp = current->next;
        current->next = current->next->next;
        delete temp;
        writeCSV(filename);  // Update the CSV file
        cout << "Command removed successfully!\n";
    }

    // Function to display all commands in the list
    void displayCommands() {
        Node<T1, T2>* current = head;
        while (current != nullptr) {
            cout << "Command: " << current->command << " - Description: " << current->description << endl;
            current = current->next;
        }
    }

    int loadGame(const string& filename) {
        ifstream file(filename);  // open the file
        if (!file.is_open()) {
            cerr << "Error: Could not open the file for loading game state!" << endl;
            return -1;
        }

        string line;
        getline(file, line);  // Read score
        int score = 0;
        if (line.substr(0, 6) == "Score:") {
            score = stoi(line.substr(6));
        }

        // Clear current list
        Node<T1, T2>* current = head;
        while (current != nullptr) {
            Node<T1, T2>* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;

        // Read commands
        while (getline(file, line)) {
            stringstream ss(line);
            string name, description;

            // Parse name and description from CSV (assuming name,description format)
            if (getline(ss, name, ',') && getline(ss, description, ',')) {
                append(name, description);  // add to the linked list
            }
        }

        file.close();  // close the file
        return score;
    }
    void saveGame(const string& filename, int score) {
        ofstream file(filename);  // open the file in write mode
        if (!file.is_open()) {
            cerr << "Error: Could not open the file for saving game state!" << endl;
            return;
        }

        // Save score
        file << "Score:" << score << endl;

        // Save commands
        Node<T1, T2>* current = head;
        while (current != nullptr) {
            file << current->command << "," << current->description << endl;
            current = current->next;
        }

        file.close();  // close the file
    }
};

int getRandomNumber(int min, int max)
{
    return rand() %(max - min + 1) + min;
}

void askQuestion(vector<pair<string, string>>& commandList, int numQuestions) {
    srand(time(0));  // Initialize random seed
    int score = 0;   // Initialize player score

    for (int i = 0; i < numQuestions; ++i) {
        // Get a random command
        int correctIndex = getRandomNumber(0, commandList.size() - 1);
        pair<string, string> correctCommand = commandList[correctIndex];

        // Store descriptions for the options (1 correct, 2 random incorrect)
        vector<string> options;
        options.push_back(correctCommand.second);

        while (options.size() < 3) {
            int randIndex = getRandomNumber(0, commandList.size() - 1);
            if (randIndex != correctIndex) {  // Ensure no duplicate correct answer
                options.push_back(commandList[randIndex].second);
            }
        }

        // Shuffle the options to randomize their positions
        random_shuffle(options.begin(), options.end());

        // Display the question
        cout << "Question " << i + 1 << ": What does the command \"" << correctCommand.first << "\" do?" << endl;
        for (int j = 0; j < options.size(); ++j) {
            cout << j + 1 << ": " << options[j] << endl;
        }

        // Prompt user to select an answer
        int userChoice;
        cout << "Enter the number of your choice: ";
        cin >> userChoice;

        // Verify the answer
        if (userChoice < 1 || userChoice > 3) {
            cout << "Invalid choice. No points awarded.\n";
        } else {
            if (options[userChoice - 1] == correctCommand.second) {
                cout << "Correct!\n";
                score++;
            } else {
                cout << "Incorrect. The correct answer is: " << correctCommand.second << endl;
            }
        }

        cout << "Current score: " << score << "\n" << endl;
    }

    cout << "Quiz finished! Your final score is: " << score << " out of " << numQuestions << endl;
}