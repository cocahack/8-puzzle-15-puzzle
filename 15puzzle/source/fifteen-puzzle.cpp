#include "fifteen-puzzle.h"
#include <random>
#include <vector>
#include <queue>
#include <algorithm>
#include <memory>
#include <map>
#include <iostream>
#include <fstream>
#include <functional>
using namespace puzzle;
using namespace std;

int puzzle::random(int min, int max)
{
	std::random_device rn;
	std::mt19937 rnd(rn());
	std::uniform_int_distribution<int> range{ min, max };
	return range(rnd);
}

void puzzle::shuffle_arr(int arr[][4])
{
	std::vector<int> v{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };
	std::random_device rn;
	std::mt19937 rnd(rn());
	do {
		std::shuffle(v.begin(), v.end(), rnd);
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				arr[i][j] = v[4 * i + j];
	} while (!isSolvable(arr));
}
bool puzzle::valid_arr(int arr[][4])
{
	int i, j, k = 0, cnt = 0;
	int loc[15];
	for (i = 0; i<4; ++i)
		for (j = 0; j < 4; ++j) {
			if (arr[i][j] != 0) {
				loc[k] = arr[i][j];
				++k;
			}
			if (k == 15) break;
		}
	for (i = 0; i < 15; ++i) {
		for (j = i + 1; j < 15; ++j) {
			if (loc[i] > loc[j]) ++cnt;
		}
	}
	return cnt % 2;
}

int puzzle::getInvCount(int arr[])
{
	int inv_count = 0;
	for (int i = 0; i < 4 * 4 - 1; i++)
	{
		for (int j = i + 1; j < 4 * 4; j++)
		{
			// count pairs(i, j) such that i appears
			// before j, but i > j.
			if (arr[j] && arr[i] && arr[i] > arr[j])
				inv_count++;
		}
	}
	return inv_count;
}

// find Position of blank from bottom
int puzzle::findXPosition(int puzzle[][4])
{
	// start from bottom-right corner of matrix
	for (int i = 4 - 1; i >= 0; i--)
		for (int j = 4 - 1; j >= 0; j--)
			if (puzzle[i][j] == 0)
				return 4 - i;
}

bool puzzle::isSolvable(int arr[][4])
{
	int zero_i;
	int i, j, k = 0, cnt = 0;
	int loc[15];
	for (i = 0; i<4; ++i)
		for (j = 0; j < 4; ++j) {
			if (arr[i][j] != 0) {
				loc[k] = arr[i][j];
				++k;
			}
			else zero_i = i;
			if (k == 15) break;
		}
	for (i = 0; i < 15; ++i) {
		for (j = i + 1; j < 15; ++j) {
			if (loc[i] > loc[j]) ++cnt;
		}
	}
	
	return (zero_i % 2) ^ (cnt % 2);
}

node* puzzle::find_and_pop(vector<node*>& openlist)
{
	int len = openlist.size();
	int loc = 0;
	node* returnval = nullptr;
	int c_f = 0;
	int lowest = openlist[0]->get_f();
	for (int i = 0; i < len; ++i)
	{
		c_f = openlist[i]->get_f();
		if (c_f <= lowest)
		{
			lowest = c_f;
			loc = i;
		}
	}
	returnval = openlist[loc];
	openlist.erase(openlist.begin() + loc);
	return returnval;
}
void puzzle::get_successors(vector<node*>& successors, node * current)
{

	int arr[4][4];
	int i, j, m, n, g = current->get_g();
	node* p{ 0 };
	for (i = 0; i<4; ++i)
		for (j = 0; j < 4; ++j)
		{
			arr[i][j] = current->get_arr_val(i, j);
			if (arr[i][j] == 0) { m = i; n = j; }
		}
	if (m - 1 > -1)
	{
		p = unique_ptr<node>{ new node{ arr , current } }.release();
		p->set_g(g);
		p->swap_arr_val(m, n, m - 1, n);
		successors.push_back(p);
	}
	if (m + 1 < 4)
	{
		p = unique_ptr<node>{ new node{ arr , current } }.release();
		p->set_g(g);
		p->swap_arr_val(m, n, m + 1, n);
		successors.push_back(p);
	}
	if (n - 1 > -1)
	{
		p = unique_ptr<node>{ new node{ arr , current } }.release();
		p->set_g(g);
		p->swap_arr_val(m, n, m, n - 1);
		successors.push_back(p);
	}
	if (n + 1 < 4)
	{
		p = unique_ptr<node>{ new node{ arr , current } }.release();
		p->set_g(g);
		p->swap_arr_val(m, n, m, n + 1);
		successors.push_back(p);
	}
}
int puzzle::select_evaluation(node * node, int n)
{
	switch (n)
	{
	case 1:
		return evaluation_f(node);
	case 2:
		return evaluation_f2(node);
	}
	return 0;
}
bool puzzle::compare_f(node * left, node * right)
{
	return (left->get_f()>right->get_f());
}
node* puzzle::astar(node* start, int mode)
{
	ofstream ofs{ "history.txt",ios_base::out };
	
	vector<node*> closedlist;
	priority_queue<node*,vector<node*>, std::function<bool(node*,node*)>> tovisit(compare_f);
	vector<node*> successors;
	start->set_h(select_evaluation(start, mode));
	start->sum();
	//openlist.push_back(start);

	tovisit.push(start);
	vector<node*>& openlist = Container(tovisit);
	node* endnode{ nullptr };
	node* same_state{ nullptr };
	node* current{ nullptr };
	int loc = 0;
	int f = 0;
	int e = 0;
	int cnt = 0;
	while (tovisit.size() != 0)
	{	
		current = tovisit.top();
		tovisit.pop();
		closedlist.push_back(current);
		if (cnt % 10000 == 0) {
			print_node(current, ofs);
			ofs << "openlist size = " << openlist.size() << endl;
			ofs << "closedlist size = " << closedlist.size() << endl;
		}

		f = current->get_f();
		get_successors(successors, current);
		for (node* node :  successors)
		{
			e = select_evaluation(node, mode);
			if (e == 0)
				return node;
			node->add_g(1);
			node->set_h(e);
			node->sum();
			if (is_exist_in_list(closedlist, node)){
				continue;
			}
			if (!is_exist_in_list(openlist, node)) {
				tovisit.push(node);
			}
			else if(node->get_g()>=get_node_in_list(openlist,node)->get_g()){
				continue;
			}

		}
		/*for (int i = 0; i < successors.size(); ++i)
		{
			if (i == 0) f = successors[i]->get_f();
			if (is_exist_in_list(openlist, successors[i]) || f < successors[i]->get_f())
			{
				continue;
			}
			else if (is_exist_in_list(closedlist, successors[i]) || f < successors[i]->get_f())
			{
				continue;
			}
			else
			{
				f = successors[i]->get_f();
				tovisit.push(successors[i]);
			}
		}*/
		//openlist = Container(tovisit);
		//closedlist.push_back(current);
		successors.erase(successors.begin(), successors.end());
		++cnt;
		/*ofs << "openlist size = " << openlist.size() << endl;
		ofs << "closedlist size = " << closedlist.size() << endl;*/
	}

	ofs << "\0";
	ofs.flush();
	ofs.close();
	return nullptr;
}
bool puzzle::is_exist_in_list(vector<node*>& list, node * successor)
{
	int len = list.size();
	for (int i = 0; i < len; ++i)
		if ((*list[i]) == (*successor)) return true;
	return false;
}

node* puzzle::get_node_in_list(vector<node*>& list, node * successor)
{
	int len = list.size();
	for (int i = 0; i < len; ++i)
		if ((*list[i]) == (*successor)) return list[i];
	return nullptr;
}

void puzzle::print_node(node * n, ofstream& ofs)
{
	ofs << "\n----------------\n";
	ofs << "f: " << n->get_f() << " g: " << n->get_g() << " h: " << n->get_h() << "\n";
	int i, j;
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
			ofs << n->get_arr_val(i, j) << " ";
	}
	ofs << "\n----------------\n";
	ofs.flush();
}
int puzzle::evaluation_f(node * node)
{
	int rval{ 0 }, i, j;
	for (i = 0; i<4; ++i)
		for (j = 0; j < 4; ++j)
		{
			if (goal[i][j] != 0 && (node->get_arr_val(i, j) != goal[i][j]))
				++rval;
		}
	return rval;
}

int puzzle::evaluation_f2(node * node)
{
	int rval{ 0 }, i, j, k, m, n;
	for (i = 0; i<4; ++i)
		for (j = 0; j < 4; ++j)
		{
			k = node->get_arr_val(i, j);
			if (k != 0)
			{
				m = k / 4;
				n = k % 4;
				rval += abs(i - m) + abs(j - n);
			}
		}
	return rval;
}

puzzle::node::node(int a[][4]) : parent{ nullptr }, f{ 0 }, g{ 0 }, h{ 0 }
{
	set_arr(a);
}

puzzle::node::node(int a[][4], node * p) : parent{ p }, f{ 0 }, g{ 0 }, h{ 0 }
{
	set_arr(a);
}

bool puzzle::node::operator==(node& node)
{
	int i, j;
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			if (node.arr[i][j] != this->arr[i][j]) return false;
	return true;
}

void puzzle::node::set_arr(int a[][4])
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			arr[i][j] = a[i][j];
}

void puzzle::node::swap_arr_val(int i, int j, int m, int n)
{
	int temp = arr[m][n];
	arr[m][n] = arr[i][j];
	arr[i][j] = temp;
}
