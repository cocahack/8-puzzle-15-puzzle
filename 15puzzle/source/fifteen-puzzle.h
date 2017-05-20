#pragma once
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

namespace puzzle
{
	static const int goal[4][4] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };
	int random(int min, int max);
	void shuffle_arr(int arr[][4]);
	bool valid_arr(int arr[][4]);
	
	class node
	{
	public:
		node(int a[][4]);
		node(int a[][4], node* p);
		~node() { delete parent; }

		bool operator==(node& node);
		bool operator!=(node& node) { return !((*this) == node); }

		void set_arr(int a[][4]);
		void set_parent(node* node) { parent = node; }
		node* get_parent() { return parent; }
		int get_arr_val(int i, int j) { return arr[i][j]; }
		void swap_arr_val(int i, int j, int m, int n);
		int get_f() { return f; }
		int get_g() { return g; }
		int get_h() { return h; }
		void sum() { f = g + h; }
		void set_h(int hh) { h = hh; }
		void set_g(int gg) { g = gg; }
		void add_g(int gg) { g += gg; }
	private:
		node* parent;
		int arr[4][4];
		int f, g, h;
	};

	bool compare_f(node* left, node* right);

	template <class T, class S, class C>
	S& Container(priority_queue<T, S, C>& q) {
		struct HackedQueue : private priority_queue<T, S, C> {
			static S& Container(priority_queue<T, S, C>& q) {
				return q.*&HackedQueue::c;
			}
		};
		return HackedQueue::Container(q);
	}


	node* astar(node* start, int mode);
	node* find_and_pop(vector<node*>& openlist);
	void get_successors(vector<node*>& successors, node* current);
	int select_evaluation(node* node, int n);
	int evaluation_f(node* node);
	int evaluation_f2(node* node);
	bool is_exist_in_list(vector<node*>& list, node * successor);
	node* get_node_in_list(vector<node*>& list, node * successor);
	void print_node(node* n, ofstream& ofs);


	bool isSolvable(int puzzle[][4]);
	int findXPosition(int puzzle[][4]);
	int getInvCount(int arr[]);
}