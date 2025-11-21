/*****************************************************
 OS Final Project

 This program implements the OPT and *** page replacement algorithms,
 calculating the number of page faults and displaying an output that
 visualizes the page replacement.

 Carson Stell, Dakota Verser
*****************************************************/

using namespace std;

#include <iostream>
#include <fstream>
#include <string>

struct OptResult {
	char** frame;
	bool* page_fault_occured;
};

OptResult opt(int num_memory_frames, string page);
void display_output(int num_memory_frames, string page, char** frame, bool* page_fault_occured);

int main() {

	// Initialize variables
	string file_name;
	char running = 'y';
	int num_memory_frames;

	do {

		// Allows user to enter name of the file they'd like examine
		cout << "Enter the file name: ";
		getline(cin, file_name);
		cout << "file_name = " << file_name;

		// Read from the file
		string output;
		ifstream Read(file_name);
		getline(Read, output);

		// Output the information from the file onto the console
		cout << "\n\n\n" << "File contents: " << output;

		// Identify the number of memory frames
		num_memory_frames = output[2] - '0';

		// Parse the rest of the string to be the pages to be examined
		string page;
		for (int i = 4; output[i] != NULL; i++) {
			if (output[i] == ',') {
				;
			}
			else {
				page += output[i];
			}
		}

		// Identify which page replacement algorithm is being used, use respective
		// function associated with said algorithm
		if (output[0] == 'O') {
			OptResult result = opt(num_memory_frames, page);
			display_output(num_memory_frames, page, result.frame, result.page_fault_occured);
		}

		else if (output[0] == 'F') {
			;
		}

		else if (output[0] == 'L') {
			;
		}

		else {
			cout << "\n\n* ERROR: Invalid choice of replacement algorithm";
		}


		// Ask user if they'd like to run the program again
		cout << "\n\nWould you like to run the program again? (Y / N) : ";
		cin >> running;

		string dummy;
		getline(cin, dummy);
		cout << "\n\n";

	} while (running == 'y' || running == 'Y');
}

// Optimal page replacement algorithm
OptResult opt(int num_memory_frames, string page) {

	// Dynamically allocate 2d array of characters, initialize space as 'empty' value
	char** frame = new char* [page.length()];
	bool* page_fault_occured = new bool[page.length()];

	for (int i = 0; i < page.length(); ++i) {
		frame[i] = new char[num_memory_frames];
	}

	for (int i = 0; i < page.length(); i++) {
		page_fault_occured[i] = false;
		for (int j = 0; j < num_memory_frames; j++) {
			frame[i][j] = ' ';
		}
	}

	// Declare variables
	int num_page_faults = 0;

	// IMPLEMENT OPT PAGE REPLACEMENT ALGORITHM
	for (int frame_index = 0; frame_index < page.length(); frame_index++) {

		// Copy the page memory from the last time index
		if (frame_index > 0) {
			for (int memory_index = 0; memory_index < num_memory_frames; memory_index++) {
				frame[frame_index][memory_index] = frame[frame_index - 1][memory_index];
			}
		}

		bool page_fault = false;

		// detect if page is already present
		bool already_present = false;
		for (int memory_index = 0; memory_index < num_memory_frames; memory_index++) {
			if (frame[frame_index][memory_index] == page[frame_index]) {
				already_present = true;
				break;
			}
		}

		// If page already exists, no fault, skip insertion
		if (already_present) {
			page_fault_occured[frame_index] = false;
			continue;
		}

		// Find next-use distance for each loaded page (OPT logic)
		int largest_distance = -1;
		int memory_index_of_largest_distance = 0;

		for (int memory_index = 0; memory_index < num_memory_frames; memory_index++) {

			char current_page = frame[frame_index][memory_index];

			// If memory frame is empty, this memory frame should be chosen
			if (current_page == ' ') {
				largest_distance = 9999;
				memory_index_of_largest_distance = memory_index;
				break;
			}

			// Compute distance until next use
			int d = 1;
			while (frame_index + d < page.length() &&
				page[frame_index + d] != current_page) {
				d++;
			}

			// If never used again
			if (frame_index + d >= page.length()) {
				d = 9999;
			}

			if (d > largest_distance) {
				largest_distance = d;
				memory_index_of_largest_distance = memory_index;
			}
		}

		// Replace page in selected frame
		frame[frame_index][memory_index_of_largest_distance] = page[frame_index];
		page_fault = true;
		num_page_faults++;

		page_fault_occured[frame_index] = page_fault;
	}

	OptResult result;
	result.frame = frame;
	result.page_fault_occured = page_fault_occured;

	return result;
}

// Displays the ouptut in a neat, readable format
void display_output(int num_memory_frames, string page, char** frame, bool* page_fault_occured) {
	
	cout << "\n\n\n";

	// Display queue of pages on top row
	for (int i = 0; i < page.length(); i++) {
		cout << page[i] << "  ";
	}

	cout << "\n";

	for (int i = 0; i < page.length(); i++) {
		cout << "---";
	}

	cout << "\n";

	// Display each of the memory frames for each new page iteration
	for (int memory_index = 0; memory_index < num_memory_frames; memory_index++) {
		for (int frame_index = 0; frame_index < page.length(); frame_index++) {
			if (page_fault_occured[frame_index] == false) {
				cout << "   ";
			}
			else {
				cout << frame[frame_index][memory_index] << "  ";
			}
		}
		cout << "\n";
	}
}