#include "eight-puzzle.h"
#include <random>
#include <vector>
#include <algorithm>
#include <memory>
#include <queue>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
using namespace puzzle;
using namespace std;

void puzzle::shuffle_arr(int arr[][3])
{
	std::vector<int> v{ 1,2,3,4,5,6,7,8,0 };
	std::random_device rn;
	std::mt19937 rnd(rn());
	do {
		std::shuffle(v.begin(), v.end(), rnd);
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				arr[i][j] = v[3 * i + j];
	} while (valid_arr(arr));
}

//check the puzzle that is solvable
bool puzzle::valid_arr(int arr[][3])
{
	int i,j,k = 0, cnt = 0;
	int loc[8]{ 0 };
	for(i=0;i<3;++i)
		for (j = 0; j < 3; ++j) {
			if (arr[i][j] != 0) {
				loc[k] = arr[i][j];
				++k;
			}
			if (k == 8) break;
		}
	for (i = 0; i < 8; ++i) {
		for (j = i+1; j < 8; ++j) {
			if (loc[i] > loc[j]) ++cnt;
		}
	}
	return cnt % 2;
}
//bool puzzle::compare_f(node * left, node * right)
//{
//	return (left->get_h()>right->get_h());
//}

puzzle::node::node(int a[][3]) : parent{ nullptr }, f{ 0 }, g{ 0 }, h{ 0 }
{
	set_arr(a);
}

puzzle::node::node(int a[][3], node * p) : parent{ p }, f{ 0 }, g{ 0 }, h{ 0 }
{
	set_arr(a);
}

bool puzzle::node::operator== (const node& node) const
{
	int i, j;
	for (i = 0; i < 3; ++i)
		for (j = 0; j < 3; ++j)
			if (node.arr[i][j] != this->arr[i][j]) return false;
	return true;
}

void puzzle::node::set_arr(int a[][3])
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			arr[i][j] = a[i][j];
}

void puzzle::node::swap_arr_val(int i, int j, int m, int n)
{
	int temp = arr[m][n];
	arr[m][n] = arr[i][j];
	arr[i][j] = temp;
}

void puzzle::execute_algorithm(node * start,const int mode)
{
	ofstream output{ "output.txt", ios_base::out };
	ofstream history{ "history.txt",ios_base::out };

	vector<node*> openlist;
	vector<node*> closedlist;
	auto t1 = std::chrono::system_clock::now();
	node* end = astar(openlist, closedlist, start, mode, history);
	std::chrono::duration<double> sec = std::chrono::system_clock::now() - t1;
	history.flush();
	output << "Execution time: " << sec.count() << " seconds" << endl;
	if (end != nullptr)
		print_arr(end, output);
	else cout << "error!" << endl;
	cout << "Execution time: " << sec.count() << " seconds" << endl;
	delete_ele(openlist);
	delete_ele(closedlist);

	history.close();
	output.close();
}

node* puzzle::astar(vector<node*>& openlist, vector<node*> closedlist, node* start, const int mode, ofstream& history)
{
	vector<node*> successors;
	start->set_h(select_evaluation(start,mode));
	start->sum();
	openlist.push_back(start);
	node* endnode{ nullptr };

	node* current{ nullptr };
	node* same_in_open{ nullptr };
	node* same_in_closed{ nullptr };

	int score = 0;
	while (openlist.size() != 0){
		current = find_and_pop(openlist); // openlist에서 f값이 가장 작은 node를 꺼낸다.
		print_node(current, history);
		get_successors(successors, current); // 노드를 확장
		for (node* successor:  successors){
			score = select_evaluation(successor,mode);
			if (score == 0) // 평가함수가 0이면 종료
				return successor;
			successor->add_g(1); // g의 값을 1 증가 시킴. 이는 위치를 1회 변경시켰음을 나타냄.
			successor->set_h(score); // h값 설정
			successor->sum(); // f=g+h
			same_in_open = get_node_in_list(openlist, successor); // openlist에서 현재 상태와 같은 node를 찾음
			same_in_closed = get_node_in_list(closedlist, successor); // closedlist에서 현재 상태와 같은 node를 찾음
			//같은 노드가 있을 때 확장된 노드(successor)의 g 값이 더 크다면
			//g값과 parent를 successor의 것으로 교체함. 이는 initial node에서 current node 까지의 
			//최단경로를 설정하는 행동이다. 
			if (same_in_open){		
				if (same_in_open->get_g() > successor->get_g()) {
					same_in_open->set_g(successor->get_g());
					same_in_open->set_parent(successor->get_parent());
					same_in_open->sum();
				}
				else continue;
			}
			//closedlist에 동일한 node가 있다면 무시한다.
			else if (same_in_closed){
				continue;
			}
			//앞의 조건에 부합하지 않는 successor는 openlist에 넣는다.
			else{
				openlist.push_back(successor);
			}
		}	
		// 확장이 끝났으므로 closedlist에 넣는다.
		closedlist.push_back(current);
		successors.erase(successors.begin(), successors.end());
		history << "openlist size = " << openlist.size() << endl;
		history << "closedlist size = " << closedlist.size() << endl;
	}	
	return nullptr;
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
	int arr[3][3];
	int i, j, m, n, g = current->get_g();
	node* p{ 0 };
	for (i = 0; i<3; ++i)
		for (j = 0; j < 3; ++j)
		{
			arr[i][j] = current->get_arr_val(i, j);
			if (arr[i][j] == 0) { m = i; n = j; }
		}
	if (m - 1 > -1)
	{
		p = new node{ arr , current };
		p->set_g(g);
		p->swap_arr_val(m, n, m - 1, n);
		successors.push_back(p);
	}
	if (m + 1 < 3)
	{
		p = new node{ arr , current };
		p->set_g(g);
		p->swap_arr_val(m, n, m + 1, n);
		successors.push_back(p);
	}
	if (n - 1 > -1)
	{
		p = new node{ arr , current };
		p->set_g(g);
		p->swap_arr_val(m, n, m, n - 1);
		successors.push_back(p);
	}
	if (n + 1 < 3)
	{
		p = new node{ arr , current };
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

int puzzle::evaluation_f(node * node)
{
	int rval{ 0 }, i, j;
	for (i = 0; i<3; ++i)
		for (j = 0; j < 3; ++j)
		{
			if (goal[i][j] != 0 && node->get_arr_val(i, j) != goal[i][j])
				++rval;
		}
	return rval;
}

int puzzle::evaluation_f2(node * node)
{
	int rval{ 0 }, i, j, k, m, n;
	for (i = 0; i<3; ++i)
		for (j = 0; j < 3; ++j)
		{
			k = node->get_arr_val(i, j);
			if (k != 0)
			{
				m = k / 3;
				n = k % 3;
				rval += abs(i - m) + abs(j - n);
			}
		}
	return rval;
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
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 3; ++j)
			ofs << n->get_arr_val(i, j) << " ";	
	}
	ofs << "\n----------------\n";
	ofs.flush();
}

void puzzle::print_arr(node* n, ofstream& ofs)
{

	int i, j, len;
	vector<node*> ptrs;
	while (n != nullptr)
	{
		ptrs.push_back(n);
		n = n->get_parent();
	}
	len = ptrs.size();
	ofs << "Found at depth: " << len - 1 << "\n";
	while (len != 0)
	{
		n = ptrs[len - 1];
		cout << "\n";
		ofs << "\n";
		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
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
	ofs.flush();
}

void puzzle::delete_ele(vector<node*>& v)
{
	int s = v.size();
	int i;
	for (i = 0; i < s; ++i)
		delete v[i];
}

