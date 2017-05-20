#include "fifteen-puzzle.h"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace puzzle;
using namespace std;

void print_arr(node* n, ofstream& ofs)
{

	int i, j, len;
	vector<node*> ptrs;
	while (n != nullptr)
	{
		ptrs.push_back(n);
		n = n->get_parent();
	}
	len = ptrs.size();
	ofs << "Solution with " << len-1 << " moves:\n";
	while (len != 0)
	{
		n = ptrs[len - 1];
		cout << "\n";
		ofs << "\n";
		for (i = 0; i < 4; ++i)
		{
			for (j = 0; j < 4; ++j)
			{
				cout << n->get_arr_val(i, j) << " ";
				ofs << n->get_arr_val(i, j) << " ";
			}
			cout << "\n";
			ofs << "\n";
		}
		--len;
	}
	ofs << "\0";
	ofs.flush(); ofs.close();
}

int main()
{
	ofstream ofs{ "output.txt", ios_base::out };
	int init_state[4][4];
	char c;
	int input_mode, h_mode;
	//string algo_mode;
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
			for (i = 0; i < 4; ++i)
				for (j = 0; j < 4; ++j)
					cin >> init_state[i][j];
			//} while (valid_arr(init_state));
		} while (!isSolvable(init_state));
	}
	cout << "Select heuristic function. (1)Simple heuristic (2)Manhattan distance\n";
	while (cin >> c)
	{
		if (c == '1' || c == '2') { h_mode = c - '0'; break; }
		else cout << "\nCannot input the character except '1' and '2'\n";
		cout << "Select heuristic function. (1)Simple heuristic (2)Manhattan distance\n";
	}
	node start{ init_state };
	auto t1 = std::chrono::system_clock::now();
	node* end = astar(&start, h_mode);
	std::chrono::duration<double> sec = std::chrono::system_clock::now() - t1;
	ofs << "Execution time: " << sec.count() << " seconds" << endl;
	if (end != nullptr)
		print_arr(end, ofs);
	else cout << "error!" << endl;
	cout << "Execution time: " << sec.count() << " seconds" << endl;

	return 0;
}
