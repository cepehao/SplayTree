#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct TNode {
	string Data;
	TNode* Left;
	TNode* Right;
	TNode* Parent;
}; typedef TNode* PNode;

//Print tree
void Print_Tree(PNode Root, int lvl) {
	if (!Root) return;
	Print_Tree(Root->Right, lvl + 1);
	for (int i = 0; i < lvl; i++)
		cout << "    ";
	cout << Root->Data << endl;
	Print_Tree(Root->Left, lvl + 1);
}

//Right rotation
void Right_Rotation(PNode& Root, PNode ptr) {
	PNode right_child = ptr->Right;
	PNode father = ptr->Parent;
	if (ptr->Parent->Parent != nullptr) {
		PNode grandfather = father->Parent;
		if (grandfather->Right != nullptr) {
			if (ptr == grandfather->Right->Left)
				grandfather->Right = ptr;
			else grandfather->Left = ptr;
			ptr->Parent = grandfather;
		}
		else {
			grandfather->Left = ptr;
			ptr->Parent = grandfather;
		}
	}
	else ptr->Parent = nullptr;
	ptr->Right = father;
	father->Parent = ptr;
	father->Left = right_child;
	if (right_child != nullptr)
		right_child->Parent = father;

	while (Root->Parent != nullptr)
		Root = Root->Parent;
}

//Left rotation
void Left_Rotation(PNode& Root, PNode ptr) {
	PNode left_child = ptr->Left;
	PNode father = ptr->Parent;
	if (ptr->Parent->Parent != nullptr) {
		PNode grandfather = father->Parent;
		if (grandfather->Left != nullptr) {
			if (ptr == grandfather->Left->Right)
				grandfather->Left = ptr;
			else grandfather->Right = ptr;
			ptr->Parent = grandfather;
		}
		else {
			grandfather->Right = ptr;
			ptr->Parent = grandfather;
		}
	}
	else ptr->Parent = nullptr;
	ptr->Left = father;
	father->Parent = ptr;
	father->Right = left_child;
	if (left_child != nullptr)
		left_child->Parent = father;

	while (Root->Parent != nullptr)
		Root = Root->Parent;
}

//Splaying
void Splay(PNode& Root, PNode ptr) {
	while (ptr->Parent != nullptr) {
		//zig & zag
		if (ptr->Parent->Parent == nullptr) {
			if (ptr == ptr->Parent->Right)
				Left_Rotation(Root, ptr);
			else Right_Rotation(Root, ptr);
			continue;
		}
		//zig-zig & zag-zig		
		if (ptr->Parent->Parent->Right != nullptr) {
			if (ptr == ptr->Parent->Parent->Right->Right) {
				Left_Rotation(Root, ptr->Parent);
				Left_Rotation(Root, ptr);
				continue;
			}
			else
				if (ptr == ptr->Parent->Parent->Right->Left) {
					Right_Rotation(Root, ptr);
					Left_Rotation(Root, ptr);
					continue;
				}
		}
		//zag-zag & zig-zag
		if (ptr->Parent->Parent->Left != nullptr) {
			if (ptr == ptr->Parent->Parent->Left->Left) {
				Right_Rotation(Root, ptr->Parent);
				Right_Rotation(Root, ptr);
			}
			else
				if (ptr == ptr->Parent->Parent->Left->Right) {
					Left_Rotation(Root, ptr);
					Right_Rotation(Root, ptr);
				}
		}
	}
	Root = ptr;
}

//Insert new element
void Insert_Into_Tree(PNode& Root, PNode& new_el, PNode Predok) {
	if (!Root) {
		new_el->Left = nullptr;
		new_el->Right = nullptr;
		if (Predok == nullptr) new_el->Parent = nullptr;
		else new_el->Parent = Predok;
		Root = new_el;
		return;
	}
	if (new_el->Data < Root->Data)
		Insert_Into_Tree(Root->Left, new_el, Root);
	else if (new_el->Data > Root->Data)
		Insert_Into_Tree(Root->Right, new_el, Root);

}

//Insert with splaying
void Insert(PNode& Root, string D) {
	PNode new_el = new TNode;
	new_el->Data = D;
	Insert_Into_Tree(Root, new_el, nullptr);
	Splay(Root, new_el);
	Print_Tree(Root, 0);
	cout << endl << endl << endl;
}

//Create tree by identificators from file input
void Create_Tree(PNode& Root) {
	ifstream input("input.txt");
	string x;
	while (input >> x)
		Insert(Root, x);
}

//Searching element in tree
PNode Find_Element(PNode Root, string x, PNode& prev) {
	PNode ptr = Root;
	while (ptr) {
		if (x == ptr->Data) return ptr;
		if (x < ptr->Data) {
			prev = ptr;
			ptr = ptr->Left;
		}
		else if (x > ptr->Data) {
			prev = ptr;
			ptr = ptr->Right;
		}
	}
	return nullptr;
}

//Searching with splaying
void Find(PNode& Root, string x) {
	PNode prev = nullptr;
	PNode ptr = Find_Element(Root, x, prev);
	if (ptr == nullptr) {
		cout << "Element " << x << " not found. Splaying parent of return ptr" << endl;
		Splay(Root, prev);
		Print_Tree(Root, 0);
		cout << endl << endl << endl;
		return;
	}
	Splay(Root, ptr);
	Print_Tree(Root, 0);
	cout << endl << endl << endl;
}

//≈сли у удал€емой вершины после операции splay нет
//правого поддерева, то оставл€ем дерево начина€ с левого поддерева
void Make_Left_Child_Root_Of_The_Tree(PNode& Root, PNode del_el) {
	Root = del_el->Left;
	Root->Parent = nullptr;
	delete del_el;
	Print_Tree(Root, 0);
	cout << endl << endl << endl;
}

//If deleted element after operationg splay has
//no left child then leave tree from right child
void Make_Right_Child_Root_Of_The_Tree(PNode& Root, PNode del_el) {
	Root = del_el->Right;
	Root->Parent = nullptr;
	delete del_el;
	Print_Tree(Root, 0);
	cout << endl << endl << endl;
}

//Delete element
void Delete(PNode& Root, string x) {
	PNode prev = nullptr;
	PNode del_el = Find_Element(Root, x, prev);
	if (del_el == nullptr) {
		cout << "Element " << x << " not found. Splaying parent of return ptr" << endl;
		Splay(Root, prev);
		Print_Tree(Root, 0);
		cout << endl << endl << endl;
		return;
	}
	Splay(Root, del_el);
	if (del_el->Right == nullptr && del_el->Left == nullptr) {
		delete del_el;
		cout << "Tree is empty!";
		exit(1);
	}
	if (del_el->Right == nullptr) {
		Root = del_el;
		Make_Left_Child_Root_Of_The_Tree(Root, del_el);
	}
	else if (del_el->Left == nullptr) {
		Root = del_el;
		Make_Right_Child_Root_Of_The_Tree(Root, del_el);
	}
	else {
		PNode left_root = del_el->Left;
		PNode right_root = del_el->Right;
		left_root->Parent = nullptr;
		right_root->Parent = nullptr;
		delete del_el;
		PNode large_left = left_root;
		while (large_left->Right != nullptr)
			large_left = large_left->Right;
		Splay(left_root, large_left);
		large_left->Right = right_root;
		right_root->Parent = large_left;
		large_left->Parent = nullptr;
		Root = large_left;
		Print_Tree(Root, 0);
		cout << endl << endl << endl;
	}
}

int main() {
	PNode Root = nullptr;
	Create_Tree(Root);
	string function, argument;
	cout << "Enter names of operations (Insert, Find, Delete). For stop enter Stop: ";
	cin >> function;
	while (function != "Stop") {
		if (!(function == "Insert" || function == "Find" || function == "Delete")) {
			cout << "Operation not found. Enter next operation" << endl;
			cin >> function;
			continue;
		}
		cout << "Enter argument: ";
		cin >> argument;
		if (function == "Insert") {
			Insert(Root, argument);
		}
		else if (function == "Find") {
			Find(Root, argument);
		}
		else if (function == "Delete") {
			Delete(Root, argument);
		}
		cout << "Next operation: ";
		cin >> function;
	}
}
