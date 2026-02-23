/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2026
* Date: 27.01.2026
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "water_seven.h"

using namespace std;

// Access the global variable declared in water_seven.cpp
extern int num_character;
void test_task_4(char character[FIXED_CHARACTER][MAX_NAME], 
    int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex, 
    int repairCost) {
    ifstream file("input/cipher.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open cipher.txt" << endl;
        return;
    }

    char cipherText[100]; 
    char resultText[15];

    // Use getline to ensure we read the full message including spaces before the #
    if (file.getline(cipherText, 100)) {
        cout << "--- Decode Message ---" << endl;
        cout << "Input Cipher: [" << cipherText << "]" << endl;
	// Assign value to test
	conflictIndex = 120;
	repairCost = 1500;
        // Call your Task 4 function
        decodeCP9Message(character, hp, skill, conflictIndex, repairCost,cipherText, resultText);

         cout << "Final Decoded Result: [" << resultText << "]" << endl;
    }

    file.close();
}
int main(int argc, const char * argv[]) {
    // 1. Check if the filename argument was provided
if (argc < 2) {
	cerr << "Usage: ./main <input_filename>" << endl;
	return 1;
    }

    // 2. Declare variables using the constants from your header [cite: 169, 170, 171]
    char character[FIXED_CHARACTER][MAX_NAME];
    int hp[FIXED_CHARACTER];
	int skill[FIXED_CHARACTER];
    int shipHP = 0;
    int repairCost = 0;

    // 3. Call your readInput function with argv[1]
    bool success = readInput(argv[1], character, hp, skill, shipHP, repairCost);

    // 4. Output results for verification
    if (success) {
	cout << "--- Ship Information ---" << endl;
	cout << "Ship HP: " << shipHP << endl;
	cout << "Repair Cost: " << repairCost << endl;

	cout << "--- Crew Information ---" << endl;
	for (int i = 0; i < num_character; i++) {
	    cout << "[" << i << "] Name: " << character[i]
		 << " | HP: " << hp[i]
		 << " | Skill: " << skill[i] << endl;
	}
    }
    else {
	cerr << "Error: Could not process file " << argv[1] << endl;
    }
    // Task 1: Evaluate the condition of the GOING_MERRY ship 
    cout << "--- Evaluate new repair cost ---" << endl;	
    repairCost = damageEvaluation(shipHP, repairCost);
    cout << "New Repair Cost: " << repairCost << endl;
    
    // Task 2: Conflict Between Luffy and Usopp
    cout << "--- Calculate conflict index ---" << endl;
    int conflictIndex = conflictSimulation (character, hp, skill, shipHP, repairCost);
    cout << "Conflict index: " << conflictIndex << endl;
    // Task 3: The duel between Luffy and Usopp
    cout << "--- Resolve duel between Luffy and Usopp ---" << endl;
    char duel[FIXED_CHARACTER][MAX_NAME];
    resolveDuel(character, hp, skill, conflictIndex, repairCost, duel);
    cout << "The returened duel:" << endl;
    for (int i = 0; i < FIXED_CHARACTER; i++){
	    if (duel[i][0]!='\0'){
		    cout << "Participant " << i << ": "<< duel[i] << endl;
	    }
    }
    // Task 4: The  departure of Nico Robin
    test_task_4(character, hp, skill, conflictIndex, repairCost);
    return 0;
}
