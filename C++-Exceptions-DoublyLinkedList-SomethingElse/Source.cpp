#include <iostream>
#include <exception>

using namespace std;

class Node {
public:
	int value;
	Node* prev;
	Node* next;

	Node(int v, Node* p, Node* n) {
		this->value = v;
		prev = p;
		next = n;
	}
};

class ListIsEmptyException : public exception {
public:
	ListIsEmptyException(char const* message) : exception(message) {};
};

class InvalidIndex : public exception {
	size_t index;
public:
	InvalidIndex(char const* message, size_t index) : exception(message), index(index) {};

	void Message()const {
		cout << what()  << ". Index " << index << " is out of range" << endl;
	}
};

class List {
	Node* head;
	Node* tail;
public:
	List() {
		head = tail = nullptr;
	}

	Node* Find(size_t index) {
		if (head != nullptr || (head == nullptr && index == 1)) {
			Node* current = head;
			int i = 1;
			for (; current != nullptr && i != index; current = current->next, i++);
			if (current == nullptr && i != index) {
				throw InvalidIndex("Couldn't find the item", index);
			}
			return current;
		}
		else {
			throw ListIsEmptyException("Couldn't find the item, list is empty");
			return nullptr;
		}
	}

	void InsertAt(int value, size_t index = 1) {
		if (Find(index) != nullptr || head == nullptr) {
			Node* node = Find(index);
			Node* newNode = new Node(value, node != nullptr ? node->prev : nullptr, node);
			if (head == nullptr) {
				head = tail = newNode;
				return;
			}
			else if (index == 1) {
				node->prev = newNode;
				head = newNode;
				return;
			}
			node->prev->next = newNode;
			node->prev = newNode;
		}
		else {
			tail->next = new Node(value, tail, nullptr);
			tail = tail->next;
		}
	}

	void DeleteAt(size_t index = 1) {
		if (index <= 0)return;
		if (head != nullptr) {
			Node* node = Find(index);
			if (node == nullptr)node = tail;
			if (head == tail) {
				delete head;
				head = tail = nullptr;
				return;
			}
			node->prev != nullptr ? node->prev->next = node->next : nullptr;
			node->next != nullptr ? node->next->prev = node->prev : nullptr;
			if (node == head) {
				head = head->next;
			}
			else if (node == tail) {
				tail = tail->prev;
			}
			delete node;
		}
	}

	void RemoveDublicates() {
		Node* current = head;
		for (int i = 1, pos; current != nullptr; current = current->next, i++) {
			pos = Contains(current->value, i);
			while (pos > 0) {
				DeleteAt(pos);
				pos = Contains(current->value, pos - 1);
			}
		}
	}

	void Reverse() {
		swap(head, tail);
		Node* current = head;
		while (current != nullptr) {
			swap(current->next, current->prev);
			current = current->next;
		}
	}

	void PrintList()const {
		Node* current = head;
		while (current != nullptr) {
			cout << current->value << "   ";
			current = current->next;
		}cout << endl << endl;
	}

	void PrintReverse() {
		Node* current = tail;
		while (current != nullptr) {
			cout << current->value << "   ";
			current = current->prev;
		}cout << endl << endl;
	}

	int GetSize()const {
		int i = 0;
		Node* current = head;
		while (current != nullptr) {
			i++;
			current = current->next;
		}
		return i;
	}

	bool Contains(int value) {
		Node* current = head;
		while (current != nullptr) {
			if (current->value == value)return true;
			current = current->next;
		}
		return false;
	}

	int Contains(int value, int start) {
		Node* current = Find(start + 1);
		for (int i = start + 1; current != nullptr; current = current->next, i++) {
			if (current->value == value)return i;
		}
		return -999999;
	}

	int CountOccurrences(int value) {
		Node* current = head;
		int counter = 0;
		while (current != nullptr) {
			if (current->value == value)counter++;
			current = current->next;
		}
		return counter;
	}

	int GetFirst() {
		if (head != nullptr) {
			return head->value;
		}
		throw ListIsEmptyException("First item couldn't be found, list is empty");
		return -9999;
	}

	int GetLast() {
		if (tail != nullptr)return tail->value;
		throw ListIsEmptyException("Last item couldn't be found, list is empty");
		return -9999;
	}

	int GetAt(size_t index) {
		Node* current = head;
		for (int i = 1; current != nullptr && i != index; current = current->next, i++);
		if (current != nullptr)return current->value;
		throw InvalidIndex("Item with this index wasn't found", index);
		return -9999;
	}

	~List() {
		while (head != nullptr) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
	}
};



void main() {
	List list; 
	cout << "Empty list exception :\n";
	try {
		list.Find(3);
	}
	catch (const ListIsEmptyException& ex) {
		cout << ex.what() << endl;
	}

	list.InsertAt(1);
	list.InsertAt(2, 2);
	list.PrintList();
	cout << "Invalid index exception :\n";
	try {
		list.Find(5);
	}
	catch (const InvalidIndex& ex) {
		ex.Message();
	}

	cout << endl;
	try {
		list.DeleteAt();
		list.DeleteAt(4);
	}
	catch (const InvalidIndex& ex) {
		ex.Message();
	}

	list.PrintList();
	list.DeleteAt();
	list.PrintList();
	try {
		list.GetFirst();
	}
	catch (const ListIsEmptyException& ex) {
		cout << ex.what() << endl;
	}

	try {
		list.GetLast();
	}
	catch (const ListIsEmptyException& ex) {
		cout << ex.what() << endl;
	}

	list.InsertAt(1);
	try {
		cout << list.GetAt(1) << endl;
		cout << list.GetAt(6) << endl;
	}
	catch (const InvalidIndex& ex) {
		ex.Message();
	}

}