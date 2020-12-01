//HW6 by Daniel Pae
//SU Net ID: dpae01@syr.edu  SUID: 945110212
//CIS554 HW6   Due: 11:59PM, Friday, Nov. 8.
#include <iostream>
#include <memory>
#include <math.h>

using namespace std;

class node {
public:
	int value;
	shared_ptr<node> right;
	shared_ptr<node> l_child;
	shared_ptr<node> r_child;
	node() {}
	node(int i) { value = i; }
};

class tree {
public:
	shared_ptr<node> root;
	int level;
	tree() { level = 0; }

	//Implement all member functions below
	tree(int n);//constructor for n-level tree
	//and initilize values as shown in the diagram; 0, 1, 2, ...
	//Note that root node is at level 1 and its value will be initialized to 0

	tree(const tree& T);//copy constructor
	~tree();//destructor
	tree(tree&& T); //move constructor

	tree(const initializer_list<int>& V);//The first number in V is tree level;
	//the rest are values from top to bottom and from left to right
	//For example, to create the tree with n=3 in the diagram,
	//tree T1 = {3, 0,1,2,3,4,5,6}; //where the first 3 is tree level, and the rest are values

	void operator= (const tree& R);//L-value operator=
	void operator= (tree&& R); //R-value operator=


	tree ThreeTimes(); //return a tree with all node value being three times
	friend ostream& operator<<(ostream& str, const tree& T);

	int sum(shared_ptr<node> p);//sum of node values in sub-tree rooted at p
	void delete_level(int i); // Delete nodes at level i. Some nodes at level(s) higher
	//than i will also be deleted accordingly. As described in class.  (Also see the
	//example in the main function.)

	shared_ptr<node> find(int i); //find the first node with value i and return
	//its address; if not found, return nullptr;

	//ADDED HELPER FUNCTION FOR CONSTRUCTOR AND DELETE_LEVEL
	shared_ptr<node> index(int i); //returns node i nodes from the root, following right values of nodes
};

shared_ptr<node> tree::index(int i) {
	shared_ptr<node> curr = root;
	while (i > 0) {
		curr = curr->right; 
		i--;
	}
	return curr;
}
tree::tree(int n) {
	level = n;
	if (n == 0) return;
	root = make_shared<node>(0);
	shared_ptr<node> curr = root;
	int i = 0;
	for (i = 1; i < n; i++) {
		for (int l = 0; l < pow(2, i); l++) {
			curr->right = make_shared<node>(pow(2, i) + l - 1);
			curr = curr->right;
		}
	}
	curr->right = root;
	curr = root;
	for (i = 0; i < n - 1; i++) {
		for (int l = 0; l < pow(2, i); l++) {
			curr->l_child = index((pow(2, i) + l - 1) * 2 + 1);
			curr->r_child = index((pow(2, i) + l - 1) * 2 + 2);
			curr = curr->right;
		}
	}
}
tree::tree(const tree& T) {
	if (!T.root) return;
	level = T.level;
	root = make_shared<node>(T.root->value);
	shared_ptr<node> currT = T.root->right;
	shared_ptr<node> currO = root;
	while (currT != T.root) {
		currO->right = make_shared<node>(currT->value);
		currO = currO->right;
		currT = currT->right;
	}
	currO->right = root;
	currO = root;
	for (int i = 0; i < level - 1; i++) {
		for (int l = 0; l < pow(2, i); l++) {
			currO->l_child = index((pow(2, i) + l - 1) * 2 + 1);
			currO->r_child = index((pow(2, i) + l - 1) * 2 + 2);
			currO = currO->right;
		}
	}
}

tree::~tree() {
	shared_ptr<node> curr = root;
	if (!curr) return;
	while (curr->right != root) {
		curr = curr->right;
	}
	curr->right.reset();
	root.reset();
}

tree::tree(tree&& T) {
	level = T.level;
	root = T.root;
	T.level = 0;
	T.root = nullptr;
}

tree::tree(const initializer_list<int>& V) {
	//if (V.size < 1) return;
	auto it = V.begin();
	level = *it;
	//if (V.size < 2) return;
	it++;
	root = make_shared<node>(*it);
	it++;
	shared_ptr<node> curr = root;
	while (it != V.end()) {
		curr->right = make_shared<node>(*it);
		curr = curr->right;
		it++;
	}
	curr->right = root;
	curr = root;
	for (int i = 0; i < level - 1; i++) {
		for (int l = 0; l < pow(2, i); l++) {
			curr->l_child = index((pow(2, i) + l - 1) * 2 + 1);
			curr->r_child = index((pow(2, i) + l - 1) * 2 + 2);
			curr = curr->right;
		}
	}
}

void tree::operator=(const tree& R) {
	if (root) {
		shared_ptr<node> curr = root;
		while (curr->right != root) {
			curr = curr->right;
		}
		curr->right.reset();
		root.reset();
	}
	level = R.level;
	if (!R.root) return;
	root = make_shared<node>(R.root->value);
	shared_ptr<node> currT = R.root->right;
	shared_ptr<node> currO = root;
	while (currT != R.root) {
		currO->right = make_shared<node>(currT->value);
		currO = currO->right;
		currT = currT->right;
	}
	currO->right = root;
	currO = root;
	for (int i = 0; i < level - 1; i++) {
		for (int l = 0; l < pow(2, i); l++) {
			currO->l_child = index((pow(2, i) + l - 1) * 2 + 1);
			currO->r_child = index((pow(2, i) + l - 1) * 2 + 2);
			currO = currO->right;
		}
	}
}

void tree::operator=(tree&& R) {
	shared_ptr<node> curr = root;
	while (curr->right != root) {
		curr = curr->right;
	}
	curr->right.reset();
	root.reset();
	level = R.level;
	root = R.root;
	R.level = 0;
	R.root = nullptr;
}

tree tree::ThreeTimes() {
	tree T = tree();
	if (!root) return T;
	T.level = level;
	T.root = make_shared<node>(root->value * 3);
	shared_ptr<node> currT = T.root;
	shared_ptr<node> currO = root->right;
	while (currO != root) {
		currT->right = make_shared<node>(currO->value * 3);
		currT = currT->right;
		currO = currO->right;
	}
	currT->right = T.root;
	currT = T.root;
	for (int i = 0; i < level - 1; i++) {
		for (int l = 0; l < pow(2, i); l++) {
			currO = T.root;
			for (int p = 0; p < (pow(2, i) + l - 1) * 2 + 1; p++) {
				currO = currO->right;
			}
			currT->l_child = currO;
			currO = T.root;
			for (int p = 0; p < (pow(2, i) + l - 1) * 2 + 2; p++) {
				currO = currO->right;
			}
			currT->r_child = currO;
			currT = currT->right;
		}
	}
	return T;
}

ostream& operator<<(ostream& str, const tree& T) {
	shared_ptr<node> curr = T.root;
	while (curr->right != T.root) {
		str << curr->value << " ";
		curr = curr->right;
	}
	str << curr->value;
	return str;
}

int tree::sum(shared_ptr<node> p) {
	int ans = 0;
	if (p->l_child) ans += sum(p->l_child);
	if (p->r_child) ans += sum(p->r_child);
	ans += p->value;
	return ans;
}

void tree::delete_level(int i) {
	i--;
	shared_ptr<node> curr = index(pow(2, i + 1) - 1);
	shared_ptr<node> temp;
	index(pow(2, i) - 2)->right = curr;
	for (int q = 0; q < level - i; q++) {
		for (int j = pow(2, i + q); j < pow(2, i + q + 1); j += (2 * (q+1))) {
			for (int h = 0; h < (2 * (q + 1)) / 2; h++) {
				curr = curr->right;
			}
			temp = curr;
			for (int x = 0; x < (2 * (q + 1)) / 2; x++) {
				temp = temp->right;
			}
			curr->right = temp;
			curr = curr->right;
		}
	}
	curr = root;
	for (int r = 0; r < i - 1; r++) {
		for (int l = 0; l < pow(2, r); l++) {
			curr->l_child = index((pow(2, r) + l - 1) * 2 + 1);
			curr->r_child = index((pow(2, r) + l - 1) * 2 + 2);
			curr = curr->right;
		}
	}
}

shared_ptr<node> tree::find(int i) {
	shared_ptr<node> curr = root;
	while (curr->right != root) {
		if (curr->value == i) return curr;
		curr = curr->right;
	}
	if (curr->value == i) return curr;
	return nullptr;
}

int main() {
	tree T1(3);
	
	cout << T1 << endl; //will print 0 1 2 3 4 5 6
	
	tree T2 = { 4, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	cout << T2 << endl; //will print 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
	
	tree T3(T2);
	cout << T3 << endl; //will print 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24

	tree T4;
	T4 = T3;
	cout << T4 << endl; //will print 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
	
	T4 = T3.ThreeTimes();
	cout << T4 << endl;//will print 30 33 36 39 42 45 48 51 54 57 60 63 66 69 72
	
	T4.delete_level(3);
	cout << T4 << endl;//will print 30 33 36 51 57 63 69
	
	cout << T3.sum(T3.find(12)) << endl; //will print 133

	
	
	getchar();
	getchar();
	return 0;
}