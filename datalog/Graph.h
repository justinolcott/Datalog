#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <algorithm>
#include "Rule.h"
#include "Predicate.h"

class Graph {
private:
	int length;
	int visits;
	std::map<int, std::set<int> > edges;
	std::map<int, std::set<int> > reverseEdges;
	std::map<int, bool> visited;
	std::vector<int> postOrder;
	std::stack<int> stack;
	std::vector <std::pair<int, std::set<int> > > sccs;
public:
	Graph(std::vector<Rule*> rules) {
		length = 0;
		visits = 0;
		for (unsigned int it = 0; it < rules.size(); it++) {
			edges.insert(std::pair<int, std::set<int> >(it, std::set<int>()));
			visited.insert(std::pair<int, bool>(it, false));
			length++;
		}

		int ii = 0;
		for (Rule* i : rules) {
			int jj = 0;
			for (Predicate* j : i->getBody()) {
				int kk = 0;
				for (Rule* k : rules) {
					if (j->getHead() == k->getHead()->getHead()) {
						reverseEdges[kk].insert(ii);
						edges[ii].insert(kk);
					}
					kk++;
				}
				jj++;
			}
			ii++;
		}
	}

	void clearVisits() {
		visits = 0;
		for (auto i : visited) {
			visited[i.first] = false;
		}
	}

	std::string toString() {
		std::stringstream ss;
		for (int i = 0; i < length; i++) {
			ss << "R" << i << ":";
			std::string delim = "";
			for (auto j : edges[i]) {
				ss << delim << "R" << j;
				delim = ",";
			}
			ss << std::endl;
		}
		return ss.str();
	}

	void dfs(int node) {
		visited[node] = true;
		//std::cout << node << "+";
		for (auto i : reverseEdges[node]) {
			if (!visited[i]) {
				dfs(i);
			}
		}
		//std::cout << node << "-";
		postOrder.push_back(node);
		visits++;
	}

	void dfs(int node, std::set<int>& tree) {
		visited[node] = true;
		//std::cout << std::endl << "visiitng " << node << std::endl;
		//std::cout << node << std::endl;
		for (auto i : edges[node]) {
			//std::cout << node << ":" << i;
			if (!visited[i]) {
				//std::cout << std::endl << i <<  " isnt visited" << std::endl;
				dfs(i, tree);
				//break;
			}
		}
		tree.insert(node);
	}

	void scc(int node) {
		std::set<int> tree;
		//std::cout << "starting " << node << std::endl;
		dfs(node, tree);
		sccs.push_back(std::pair<int, std::set<int> >(node, tree));
	}

	void dfsForest() {
		clearVisits();
		while (visits != length) {
			for (auto i : visited) {
				if (!i.second) {
					dfs(i.first);
				}
			}
		}
	}

	void reversePostOrder() {
		std::vector<int> reverse;
		for (int i = postOrder.size() - 1; i >= 0; i--) {
			reverse.push_back(postOrder.at(i));
		}
		postOrder = reverse;
	}

	void findSCC() {
		//std::cout << PO() << std::endl;
		reversePostOrder();
		clearVisits();
		for (auto i : postOrder) {
			if (!visited[i]) {
				scc(i);
			}
		}
		//for (auto i : sccs) {
		//	for (auto j : i.second) {
				//std::cout << j;
		//	}
		//}

		//std::cout << sccs.size();
	}

	std::vector<std::pair<int, std::set<int> > > getSCCs() {
		return sccs;
	}

	std::string PO() {
		std::stringstream ss;
		for (auto i : postOrder) {
			ss << i << ",";
		}
		ss << std::endl;
		return ss.str();
	}

};
#endif
