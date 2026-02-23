#include "water_seven.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Internal variable
int num_character = 0;

// Decleration of internal functions

/**
 * @brief Searches for a character's index in the current tracking array.
 * * @param name The name of the character to search for as a string.
 * @param character The 2D array containing the names of characters already processed.
 * @return int The index of the character if found; otherwise, returns -1. 
 */
static int _find_charater_order (string name, char character[FIXED_CHARACTER][MAX_NAME]);

/**
 * @brief Manages character information by either adding new entries or updating existing ones.
 * * @param name The name of the character.
 * @param val1 The clamped endurance (HP) value. 
 * @param val2 The clamped skill value. 
 * @param character The 2D array storing character names in order of appearance. 
 * @param hp The array storing endurance values.
 * @param skill The array storing skill values.
 * @return true If data was handled successfully.
 * @return false If the number of unique characters exceeds the permitted FIXED_CHARACTER limit.
 */
static bool _handle_character_data(
	string name, int val1, int val2, char character[FIXED_CHARACTER][MAX_NAME],
	int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER]);

/**
* @brief Consider whether a positive integer number is a perfect number
* * @param num The number is considered
* @return true If the number is perfect number, flase in other case 
*/
static bool _is_perfect_number (int num);

/**
 * @brief Calculate the sum of digits of the positive number
 * * @param num The number is used to calculate the sum of digits
 * @return The sum of its digits if possible, -1 if any error occur.
*/
static int _sum_digits (int num);
/**
 * @brief Calculate the conflict index
 * * @param luffy_skill : The skill value of LUFFY
 * @param usopp_skill : .................. USOPP
 * @param repairCost : The ship repair cost 
 * @param shipHP : The integrity value of the ship
 * @return Final conflict index
*/ 
static int _calculate_confict_index (int luffy_skill, int usopp_skill, int repairCost, int shipHP);

/**
 * @brief Calculate the intervention cost 
 * * @param charater : Array storing the name of character
 * @param hp : An array storing the endurance values corresponding to each character
 * @param intervention_cost: Returned array using to store the calculated intervention cost.
*/
static void _calculate_intervention_cost (char charater[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int intervention_cost[FIXED_CHARACTER]);

/**
 * @brief Evaluates all 32 possible subsets of supporters to find the optimal group.
 * * @param skill The array storing the overall skill values of all 7 characters
 * @param int_cost The array storing pre-calculated intervention costs for each character.
 * @param supporters_idx An array containing the indices of the 5 "Members" (excluding Luffy and Usopp)
 * @param s_count The number of potential supporters to evaluate (fixed at 5)
 * @param U Usopp's calculated resistance value in the duel
 * @param luffy_skill The skill value corresponding to Luffy
 * * @return Returns the bitmask (0-31) representing the chosen subset indices from supporters_idx
*/
static int _find_best_choice(
    int skill[FIXED_CHARACTER], 
    int int_cost[FIXED_CHARACTER], 
    int supporters_idx[], 
    int s_count, 
    int U, 
    int luffy_skill);


/**
 * @brief Assigns the names of the optimal subset members to the duel array.
 * * @param character The array containing all 7 character names
 * @param supporters_idx The original indices of the 5 potential supporters
 * @param s_count The number of potential supporters (5)
 * @param best_mask The bitmask returned by _find_best_choice.
 * @param duel The output 2D array to store selected names
 */
static void _assign_duel_participants(
    char character[FIXED_CHARACTER][MAX_NAME], 
    int supporters_idx[], 
    int s_count, 
    int best_mask, 
    char duel[FIXED_CHARACTER][MAX_NAME]);

/**
 * @brief Reverses the string in segments of size n.
 * @param message The null-terminated string to be modified in-place.
 * @param n The size of each block.
 */
static void _block_wise_reversal(char* message, int n);

/**
 * @brief Performs a reverse Caesar shift on alphanumeric characters.
 * @param message The string to decrypt in-place.
 * @param key The shift value (0-25).
 */
static void _circular_shift_reversal(char* message, int key);

/**
 * @brief Checks if a specific substring exists within the message.
 * @param message The decoded plainText.
 * @param sub The substring to search for (e.g., "CP9").
 * @return true if found, false otherwise.
 */
static bool _check_contains(const char* message, const char* sub);

/**
 * @brief Validates the checksum of the cipherText and extracts the message.
 * @param cipherText The input string in format "message#XY".
 * @param message Pointer to be allocated with the extracted message.
 * @return true if (ASCII_sum % 100) == XY, false otherwise.
 */
static bool _check_sum_cipher(const char* cipherText, char*& message);

// -------------- Implementation of internal functions ---------------- //
static int _find_charater_order (string name, char character[FIXED_CHARACTER][MAX_NAME])
{
	for (int i = 0; i < num_character; i++){
		if (strcmp(character[i], name.c_str()) == 0){
			// Match the name
			return i;
		}
	}
	return -1;
}

static bool _handle_character_data(
	string name, int val1, int val2, char character[FIXED_CHARACTER][MAX_NAME],
	int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER])
{
	int order_charater = _find_charater_order(name,character);
	if (order_charater == -1 ){
		// New character
		if (num_character >= FIXED_CHARACTER){
			cerr << "Number of characters exceeding the permition" << endl;
			return false;
		}
		else {
			// Add new character information
			strncpy(character[num_character], name.c_str(), MAX_NAME);
			character[num_character][MAX_NAME - 1] = '\0'; // Ensure ending charater exist
			hp[num_character] = val1;
			skill[num_character] = val2;
			num_character ++; // Increase number of characters
		}
	}
	else {
		// Existing character, replace information
		hp[order_charater]  = val1;
	        skill[order_charater] = val2;	
	}
	return true;
}

static bool _is_perfect_number (int num)
{
	int sum_digits = _sum_digits(num);
	int sum_divisors = 0; 
	for (int i = 1; i <= static_cast<int>(sum_digits/2) ; i ++){
			if (sum_digits % i == 0){
					sum_divisors += i;
			}
	}
	#ifdef DEBUG 
		cout << sum_digits << (sum_digits == sum_divisors ? " is " : " is not ") << "perfect number" << endl;  
	#endif
	return sum_digits == sum_divisors;
}

static int _sum_digits (int num)
{
	#ifdef DEBUG
		int saved_num = num;
	#endif
	if (num < 0){
		cerr << "Sum digits throw error: The number is negative" << endl;
		return -1;
	}
	int sum = 0, digit = 0;
	while (num != 0){
		digit = num % 10;
		sum += digit;
		num = num / 10;
	}
	#ifdef DEBUG 
		cout << "The sum of digits of " << saved_num << " is " << sum << endl;
	#endif
	return sum;
}

static int _calculate_confict_index (int luffy_skill, int usopp_skill, int repairCost, int shipHP)
{
	// First stage 
	int conflictIndex = ceil(luffy_skill - usopp_skill + (repairCost / 100) + ((500 - shipHP) / 50)); 
	#ifdef DEBUG
	    cout << "Conflict calculation" << endl;
	    cout << "First stage conflict index: " << conflictIndex << endl;
	#endif
	int steps = 0;
        while (conflictIndex < 255 && steps < 10){
		int id = conflictIndex % 6; // Event occurs
		#ifdef DEBUG 
		    cout << "At step " << steps << " id = " << id << endl;
		#endif
		switch (id) {
			case 0: 
			// All event occur
			    conflictIndex += 255;
			    break;
			case 1:
			// Crew receives unfavorable information
			    conflictIndex += 20;
			    break;
			case 2:
			// Luffy decides to abandon the Going Merry
			    conflictIndex += 50;
			    break;
			case 3:
			    conflictIndex += 70;
			    break;
			case 4:
			    conflictIndex +=90;
			    break;
			case 5:
			    conflictIndex +=100;
			    break;
			default:
			    cerr << "Something wrongs when trying to calculate the conflictIndex" << endl;
			    return -1;
		}
		steps++;
    		#ifdef DEBUG 
		    cout << "End of step " << steps << " conflictIndex = " << conflictIndex << endl;
		#endif
	}
	#ifdef DEBUG
	    cout << "Final stage conflict index: " << conflictIndex << endl; 
	#endif
    	return conflictIndex;
}

static void _calculate_intervention_cost(
    char character[FIXED_CHARACTER][MAX_NAME], 
    int hp[FIXED_CHARACTER], 
    int intervention_cost[FIXED_CHARACTER]) 
{
    for (int i = 0; i < FIXED_CHARACTER; i++) {
        // Check if the character is Luffy or Usopp 
        if (strcmp(character[i], "LUFFY") == 0 || strcmp(character[i], "USOPP") == 0) {
            intervention_cost[i] = -1;
            #ifdef DEBUG
                cout << "Index [" << i << "] " << character[i] << ": Excluded (Participant)" << endl;
            #endif
        } else {
            // Formula: cost = (hp mod 10) + 1 
            intervention_cost[i] = (hp[i] % 10) + 1;
            #ifdef DEBUG
                cout << "Index [" << i << "] " << character[i] 
                     << ": Cost = (" << hp[i] << " % 10) + 1 = " << intervention_cost[i] << endl;
            #endif
        }
    }
}

static int _find_best_choice(
    int skill[FIXED_CHARACTER], 
    int int_cost[FIXED_CHARACTER], 
    int supporters_idx[], 
    int s_count, 
    int U, 
    int luffy_skill) 
{
    int best_mask = -1;
    int min_total_cost = INT_MAX; // Initialize with a very high value (INT_MAX)
    int min_members = 7;             // Max possible members + 1

    // 1 << s_count (where s_count is 5) generates 32 iterations (1 --> (1 << 5) --> 0b100000 = 32) 
    for (int mask = 0; mask < (1 << s_count); mask++) {
        int current_support = 0;
        int current_cost = 0;
        int current_members = 0;

        // Check each of the 5 potential supporters
        for (int i = 0; i < s_count; i++) {
            if ((mask >> i) & 1) { // If bit i is set, include this supporter
                int idx = supporters_idx[i];
                current_support += skill[idx]; 
                current_cost += int_cost[idx]; 
                current_members++; 
            }
        }

        // Condition for victory: Luffy's skill + total support must be >= U 
        if (luffy_skill + current_support >= U) {
            // Rule 1: Select subset with minimum total intervention cost 
            if (current_cost < min_total_cost) {
                min_total_cost = current_cost;
                min_members = current_members;
                best_mask = mask;
            } 
            // Rule 2: If costs are equal, priority goes to fewer members 
            else if (current_cost == min_total_cost && current_members < min_members) {
                min_members = current_members;
                best_mask = mask;
            }
        }
    }
    return best_mask;
}

static void _assign_duel_participants(
    char character[FIXED_CHARACTER][MAX_NAME], 
    int supporters_idx[], 
    int s_count, 
    int best_mask, 
    char duel[FIXED_CHARACTER][MAX_NAME]) 
{
    int duel_pos = 0;

    // Only proceed if a valid subset was found
    if (best_mask != -1) {
        for (int i = 0; i < s_count; i++) {
            // Check if the i-th supporter is part of the best subset
            if ((best_mask >> i) & 1) {
                // Copy the name into the duel array 
                strncpy(duel[duel_pos], character[supporters_idx[i]], MAX_NAME);
		#ifdef DEBUG
                    cout << "Supporter added to duel[" << duel_pos << "]: " << duel[duel_pos] << endl;
                #endif
                duel_pos++;
            }
        }
    }

    // Ensure remaining rows in the duel array are empty 
    for (int i = duel_pos; i < FIXED_CHARACTER; i++) {
        duel[i][0] = '\0';
    }
    #ifdef DEBUG
        cout << "Total supporters helping Luffy: " << duel_pos << endl;
        cout << "-----------------------------------" << endl;
    #endif
}

static void _block_wise_reversal(char* message, int n) {
    int len = strlen(message);
    for (int i = 0; i < len; i += n) {
        int start = i;
        int end = (i + n - 1 < len) ? (i + n - 1) : (len - 1);
        
        while (start < end) {
            char temp = message[start];
            message[start] = message[end];
            message[end] = temp;
            start++;
            end--;
        }
    }
}

static void _circular_shift_reversal(char* message, int key) {
    for (int i = 0; message[i] != '\0'; i++) {
        char c = message[i];
        if (c >= 'a' && c <= 'z') {
            message[i] = (c - 'a' - key + 26) % 26 + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            message[i] = (c - 'A' - key + 26) % 26 + 'A';
        } else if (c >= '0' && c <= '9') {
            message[i] = (c - '0' - key % 10 + 10) % 10 + '0';
        }
        // Spaces and other characters are left as-is
    }
}

static bool _check_contains(const char* message, const char* sub) {
    if (strstr(message, sub) != nullptr) {
        return true;
    }
    return false;
}

static bool _check_sum_cipher(const char* cipherText, char*& message) {
    // Find the LAST occurrence of '#'
    const char* hashPos = strrchr(cipherText, '#');
    if (!hashPos) return false;

    // The checksum digits are immediately after the last '#'
    int msgLen = hashPos - cipherText;
    message = new char[msgLen + 1];
    strncpy(message, cipherText, msgLen);
    message[msgLen] = '\0';

    // Calculate ASCII sum of everything BEFORE the last '#'
    long long asciiSum = 0;
    for (int i = 0; i < msgLen; i++) {
        asciiSum += (unsigned char)message[i];
    }

    // Convert the characters after '#' to an integer
    int expectedChecksum = atoi(hashPos + 1);
    #ifdef DEBUG 
	cout << "CHECK SUM: " << expectedChecksum << " ascii Sum: " << asciiSum << endl; 
    #endif

    return (asciiSum % 100) == expectedChecksum;
}

/*----------------------- TASK ----------------------------*/
//Task 0
bool readInput(
    const string &filename,
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], 
    int skill[FIXED_CHARACTER], int &shipHP, int &repairCost)
{
	ifstream inFile(filename);
	if (inFile.is_open()){
		string name;
		int val1;
		int val2;
		while (inFile >> name >> val1 >> val2){
			if (name == "GOING_MERRY"){
				// Ship data
				// Preprocessing
				if (val1 < MIN_SHIPHP){
					val1 = MIN_SHIPHP;
				}
				else if (val1 > MAX_SHIPHP){
					val1 = MAX_SHIPHP;
				}

				if (val2 < MIN_REPAIR_COST){
					val2 = MIN_REPAIR_COST;
				}
				else if (val2 > MAX_REPAIR_COST){
					val2 = MAX_REPAIR_COST;
				}
				shipHP = val1;
				repairCost = val2;
			}
			else {
				// Charater data 
				// Preprocessing
				if (val1 < MIN_HP){
					val1 = MIN_HP;
				}
				else if (val1 > MAX_HP){
					val1 = MAX_HP;
				}

				if (val2 < MIN_SKILL){
					val2 = MIN_SKILL;
				}
				else if (val2 > MAX_SKILL){
					val2 = MAX_SKILL;
				}
				bool re_handle_character = _handle_character_data(name, val1,  val2, character,  hp,  skill);
				if (!re_handle_character){
					// False when handle character data
					return false;
				}
			}
		}
	}
	else {
		cerr << "Failed when trying to read input file " << filename << endl;
		return false;
	}
        return true;
}



// Task 1
int damageEvaluation(int shipHP, int repairCost)
{
	float new_repair_cost = 0.0;
	if (_is_perfect_number(shipHP) && shipHP < 455){
		new_repair_cost = repairCost + repairCost * 0.5; // Increase repairCost to 50%
		return static_cast<int>(ceil(new_repair_cost));
	}
   	return repairCost;
}

// Task 2
int conflictSimulation(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int shipHP, int repairCost)
{
	int luffy_index = _find_charater_order("LUFFY", character);
	int usopp_index = _find_charater_order("USOPP", character);
	if (luffy_index == -1 || usopp_index == -1){
		cerr << "Failed when trying read usop and luffy index" << endl;
		return -1;
	}
	#ifdef DEBUG
	    cout << "LUFFY INDEX: " << luffy_index << endl;
	    cout << "USOPP INDEX: " << usopp_index << endl;
	#endif
	try {
	    // Read information for calculating conflict index 
	    int luffy_skill = skill[luffy_index];
	    int usopp_skill = skill[usopp_index];
	    return _calculate_confict_index(luffy_skill, usopp_skill, repairCost,shipHP); 
	}
	catch (...){
		cerr << "Task 2: Something wrongs when trying read information" << endl;
		return -1;
	}
}

// Task 3
void resolveDuel(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int conflictIndex, int repairCost, char duel[FIXED_CHARACTER][MAX_NAME])
{
        //TODO: Output assign to duel parameter
	int intervention_cost[FIXED_CHARACTER];
	_calculate_intervention_cost(character, hp, intervention_cost);
    	// Identify indices for supporters only
	int supporters_idx[5];
	int s_count = 0;
	int luffy_idx = _find_charater_order("LUFFY", character);
	int usopp_idx = _find_charater_order("USOPP", character);
	for (int i = 0; i < FIXED_CHARACTER; i++) {
		if (i != luffy_idx && i != usopp_idx) {
		    supporters_idx[s_count++] = i;
		}
	}
	// Calculate the Usopp's resistance
	int usopp_index = _find_charater_order("USOPP", character);
	int U = ceil(skill[usopp_index] + (conflictIndex / 20) + (repairCost) / 500);
	#ifdef DEBUG
	    cout << "Usopp's resistance: " << U << endl;
	#endif

	// Find best choice
	int best_choice = _find_best_choice(skill, intervention_cost, supporters_idx, s_count, U, skill[luffy_idx]);
	#ifdef DEBUG
	    cout << "The best choice is " << best_choice << endl;
	#endif
	// Assigns to duel parameter
	_assign_duel_participants(character, supporters_idx, s_count, best_choice, duel);
}

// Task 4
void decodeCP9Message(char character[FIXED_CHARACTER][MAX_NAME], 
    int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex, 
    int repairCost, char cipherText[], char resultText[]){
        //TODO: Output assign to resultText parameter
	// Valid the check sum 
	char* message = nullptr;
	bool is_checksum_pass = _check_sum_cipher(cipherText, message);
	if (!is_checksum_pass){
		#ifdef DEBUG
		    cout << "CHECK SUM FAILED" << endl;
		#endif
		resultText[0] = '\0';
		if (message != nullptr){
			delete[] message;
			message = nullptr;
		}
	}
	else {
	    // Parse message successfully and 
	    int key = (conflictIndex + repairCost) % 26;
	    int block_wise = (key % 5) + 4;
	    #ifdef DEBUG
		cout << "Key: " << key << " block_wise: " << block_wise  << endl;
	    #endif
	    // Block-wise revesal
	    _block_wise_reversal (message, block_wise);
	    #ifdef DEBUG
		cout << "After reverse block-wisely: " << message << endl;
	    #endif
	    // Reverse circular shift
	    _circular_shift_reversal (message, key);
	    #ifdef DEBUG
		cout << "After reverse circular shiftly: " << message << endl;
	    #endif   
	    bool is_constain_sub = false;
	    is_constain_sub = _check_contains(message, "CP9") || _check_contains(message, "ENIESLOBBY");
	    if (!is_constain_sub){
		    char myBuffer[] = "plainText_FALSE";
		    strncpy(resultText, myBuffer, 15);
	    }
	    else{
		    char myBuffer[] = "plainText_TRUE";
		    strncpy(resultText,myBuffer,14);
	    }
	}
    }

// Task 5
int analyzeDangerLimit(int grid[MAX_GRID][MAX_GRID], int rows, int cols){
    return 0;
}


bool evaluateRoute(int grid[MAX_GRID][MAX_GRID], int rows, int cols, int dangerLimit){
    return false;
}


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
///////////////////////////////////////////////
