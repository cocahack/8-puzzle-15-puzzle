#include "eight-puzzle.h"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace puzzle;
using namespace std;

int main()
{
	int init_state[3][3];
	char c;
	int input_mode{ 0 }, h_mode{ 0 };

	cout << "Select mode. (1)Random input (2)Direct input\n";
	while (cin >> c)
	{
		if (c == '1' || c == '2') { input_mode = c - '0'; break; }
		else cout << "\nCannot input the character except '1' and '2'\n";
		cout << "Select mode. (1)Random input (2)Direct input\n";
	}

	if (input_mode == 1) {
		cout << "Initializing...\n";
		shuffle_arr(init_state);
	}
	else {
		int i, j;
		do {
			cout << "Input the test case.\n";
			for (i = 0; i < 3; ++i)
				for (j = 0; j < 3; ++j)
					cin >> init_state[i][j];
		} while (valid_arr(init_state));
	}
	cout << "Select heuristic function. (1)Simple heuristic (2)Manhattan distance\n";
	while (cin >> c)
	{
		if (c == '1' || c == '2') { h_mode = c - '0'; break; }
		else cout << "\nCannot input the character except '1' and '2'\n";
		cout << "Select heuristic function. (1)Simple heuristic (2)Manhattan distance\n";
	}
	node start{ init_state };
	execute_algorithm(&start, h_mode);
	return 0;
}
