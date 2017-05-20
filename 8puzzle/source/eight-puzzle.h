#pragma once
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

namespace puzzle
{
	static int goal[3][3] = { 0,1,2,3,4,5,6,7,8 };
	void shuffle_arr(int arr[][3]);
	bool valid_arr(int arr[][3]);
	class node
	{
	public:
		node(int a[][3]);
		node(int a[][3], node* p);
		~node() { }

		bool operator==(const node& node) const;
		bool operator!=(const node& node) const { return !((*this) == node); }
		bool operator<(const node& node) const { return f < node.f; }
		bool operator>(const node& node) const { return f > node.f; }
		bool operator<=(const node& node) const { return f <= node.f; }
		bool operator>=(const node& node) const { return f >= node.f; }

		void set_arr(int a[][3]);

		void set_parent(node* node) { parent = node; }
		node* get_parent() { return parent; }

		int get_arr_val(int i, int j) { return arr[i][j]; }
		void swap_arr_val(int i, int j, int m, int n);

		int get_f() { return f; }
		int get_g() { return g; }
		int get_h() { return h; }
		void sum() { f = g + h; }

		void set_h(int hh) { h = hh; }
		void set_g(int gg) { g = gg;  }
		void add_g(int gg) { g += gg; }
	private:
		node* parent;
		int arr[3][3];
		int f, g, h;
	};

	//bool compare_f(node* left, node* right);

	/*template <class T, class S, class C>
	S& Container(priority_queue<T, S, C>& q) {
		struct HackedQueue : private priority_queue<T, S, C> {
			static S& Container(priority_queue<T, S, C>& q) {
				return q.*&HackedQueue::c;
			}
		};
		return HackedQueue::Container(q);
	}*/

	void execute_algorithm(node* start,const int mode);

	node* astar(vector<node*>& openlist, vector<node*> closedlist, node* start, const int mode, ofstream& history);
	node* find_and_pop(vector<node*>& openlist);
	void get_successors(vector<node*>& successors, node* current);
	int select_evaluation(node* node, int n);
	int evaluation_f(node* node);
	int evaluation_f2(node* node);
	bool is_exist_in_list(vector<node*>& openlist, node * successor);
	node* get_node_in_list(vector<node*>& list, node * successor);
	void print_node(node* n, ofstream& ofs);
	void print_arr(node* n, ofstream& ofs);
	
	void delete_ele(vector<node*>& v);
}