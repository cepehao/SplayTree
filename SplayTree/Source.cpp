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

//Печать дерева
void PrintTree(PNode root, int lvl) {
	if (!root) return;
	PrintTree(root->Right, lvl + 1);
	for (int i = 0; i < lvl; i++)
		cout << "    ";
	cout << root->Data << endl;
	PrintTree(root->Left, lvl + 1);
}

//Правый поворот
void RightRotation(PNode& root, PNode ptr) {
	PNode rightChild = ptr->Right;
	PNode father = ptr->Parent;
	if (ptr->Parent->Parent != nullptr) {
		PNode grandFather = father->Parent;
		if (grandFather->Right != nullptr) {
			if (ptr == grandFather->Right->Left)
				grandFather->Right = ptr;
			else grandFather->Left = ptr;
			ptr->Parent = grandFather;
		}
		else {
			grandFather->Left = ptr;
			ptr->Parent = grandFather;
		}
	}
	else ptr->Parent = nullptr;
	ptr->Right = father;
	father->Parent = ptr;
	father->Left = rightChild;
	if (rightChild != nullptr)
		rightChild->Parent = father;

	while (root->Parent != nullptr)
		root = root->Parent;
}

//Левый поворот
void LeftRotation(PNode& root, PNode ptr) {
	PNode leftChild = ptr->Left;
	PNode father = ptr->Parent;
	if (ptr->Parent->Parent != nullptr) {
		PNode grandFather = father->Parent;
		if (grandFather->Left != nullptr) {
			if (ptr == grandFather->Left->Right)
				grandFather->Left = ptr;
			else grandFather->Right = ptr;
			ptr->Parent = grandFather;
		}
		else {
			grandFather->Right = ptr;
			ptr->Parent = grandFather;
		}
	}
	else ptr->Parent = nullptr;
	ptr->Left = father;
	father->Parent = ptr;
	father->Right = leftChild;
	if (leftChild != nullptr)
		leftChild->Parent = father;

	while (root->Parent != nullptr)
		root = root->Parent;
}

//Функция splaying
void Splay(PNode& root, PNode ptr) {
	while (ptr->Parent != nullptr) {
		//zig & zag
		if (ptr->Parent->Parent == nullptr) {
			if (ptr == ptr->Parent->Right)
				LeftRotation(root, ptr);
			else RightRotation(root, ptr);
			continue;
		}
		//zig-zig & zag-zig		
		if (ptr->Parent->Parent->Right != nullptr) {
			if (ptr == ptr->Parent->Parent->Right->Right) {
				LeftRotation(root, ptr->Parent);
				LeftRotation(root, ptr);
				continue;
			}
			else
				if (ptr == ptr->Parent->Parent->Right->Left) {
					RightRotation(root, ptr);
					LeftRotation(root, ptr);
					continue;
				}
		}
		//zag-zag & zig-zag
		if (ptr->Parent->Parent->Left != nullptr) {
			if (ptr == ptr->Parent->Parent->Left->Left) {
				RightRotation(root, ptr->Parent);
				RightRotation(root, ptr);
			}
			else
				if (ptr == ptr->Parent->Parent->Left->Right) {
					LeftRotation(root, ptr);
					RightRotation(root, ptr);
				}
		}
	}
	root = ptr;
}

//Вставка нового элемента
void InsertInTree(PNode& root, PNode& newElem, PNode parent) {
	if (!root) {
		newElem->Left = nullptr;
		newElem->Right = nullptr;
		if (parent == nullptr) newElem->Parent = nullptr;
		else newElem->Parent = parent;
		root = newElem;
		return;
	}
	if (newElem->Data < root->Data)
		InsertInTree(root->Left, newElem, root);
	else if (newElem->Data > root->Data)
		InsertInTree(root->Right, newElem, root);

}

//Вставка с последущим расширением
void Insert(PNode& root, string D) {
	PNode newElem = new TNode;
	newElem->Data = D;
	InsertInTree(root, newElem, nullptr);
	Splay(root, newElem);
	PrintTree(root, 0);
	cout << endl << endl << endl;
}

//Создать дерево из идентификаторов файла input
void CreateTree(PNode& root) {
	ifstream input("input.txt");
	string x;
	while (input >> x)
		Insert(root, x);
}

//Поиск элемента в дереве
PNode FindElement(PNode root, string x, PNode& prev) {
	PNode ptr = root;
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

//Поиск с расширением
void Find(PNode& root, string x) {
	PNode prev = nullptr;
	PNode ptr = FindElement(root, x, prev);
	if (ptr == nullptr) {
		cout << "Element " << x << " not found. Splaying parent of return ptr" << endl;
		Splay(root, prev);
		PrintTree(root, 0);
		cout << endl << endl << endl;
		return;
	}
	Splay(root, ptr);
	PrintTree(root, 0);
	cout << endl << endl << endl;
}

//Если у удаляемой вершины после операции splay нет
//правого поддерева, то оставляем дерево начиная с левого поддерева
void MakeLeftChildTreeRoot(PNode& root, PNode delElem) {
	root = delElem->Left;
	root->Parent = nullptr;
	delete delElem;
	PrintTree(root, 0);
	cout << endl << endl << endl;
}

//Если у удаляемой вершины после операции splay нет
//левого поддерева, то оставляем дерево начиная с правого поддерева
void MakeRightChildTreeRoot(PNode& root, PNode delElem) {
	root = delElem->Right;
	root->Parent = nullptr;
	delete delElem;
	PrintTree(root, 0);
	cout << endl << endl << endl;
}

//Удаление
void Delete(PNode& root, string x) {
	PNode prev = nullptr;
	PNode delElem = FindElement(root, x, prev);
	if (delElem == nullptr) {
		cout << "Element " << x << " not found. Splaying parent of return ptr" << endl;
		Splay(root, prev);
		PrintTree(root, 0);
		cout << endl << endl << endl;
		return;
	}
	Splay(root, delElem);
	if (delElem->Right == nullptr && delElem->Left == nullptr) {
		delete delElem;
		cout << "Tree is empty!";
		exit(1);
	}
	if (delElem->Right == nullptr) {
		root = delElem;
		MakeLeftChildTreeRoot(root, delElem);
	}
	else if (delElem->Left == nullptr) {
		root = delElem;
		MakeRightChildTreeRoot(root, delElem);
	}
	else {
		PNode leftRoot = delElem->Left;
		PNode rightRoot = delElem->Right;
		leftRoot->Parent = nullptr;
		rightRoot->Parent = nullptr;
		delete delElem;
		PNode largeLeft = leftRoot;
		while (largeLeft->Right != nullptr)
			largeLeft = largeLeft->Right;
		Splay(leftRoot, largeLeft);
		largeLeft->Right = rightRoot;
		rightRoot->Parent = largeLeft;
		largeLeft->Parent = nullptr;
		root = largeLeft;
		PrintTree(root, 0);
		cout << endl << endl << endl;
	}
}

int main() {
	PNode root = nullptr;
	CreateTree(root);
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
			Insert(root, argument);
		}
		else if (function == "Find") {
			Find(root, argument);
		}
		else if (function == "Delete") {
			Delete(root, argument);
		}
		cout << "Next operation: ";
		cin >> function;
	}
}
