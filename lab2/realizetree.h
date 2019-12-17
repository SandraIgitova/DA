#ifndef REALIZE_TREE_H
#define REALIZE_TREE_H

#include <cstdint>
#include <fstream>
#include <algorithm>
#include "tree.h"
#include "string.h"

using TUll = unsigned long long;

class TSerializableRbTree: public NDs::TRbTree<TString, TUll> {
public:

	void SerializeInsert() {
		TString key;
		TUll val = 0;

		std::cin >> key >> val;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		if (Find(key, val)) {
			std::cout << "Exist\n";
		} else {
			Insert(key, val);
			std::cout << "OK\n";
		}
	}

	void SerializeRemove() {
		TString key;
		TUll val = 0;

		std::cin >> key;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		if (!Find(key, val)) {
			std::cout << "NoSuchWord\n";
		} else {
			Remove(key);
			std::cout << "OK\n";
		}
	}

	void SaveLoad() {
		TString subCommand;
		TString filename;

		std::cin >> subCommand >> filename;
		if (subCommand[0] == 'S') {
			if (!Serialize(filename)) {
				std::cout << "ERROR: File doesn't open\n";
				return ;
			};
		} else {
			if (!Deserialize(filename)) {
				std::cout << "ERROR: File doesn't open\n";
				return ;
			};
		}
		std::cout << "OK\n";
	}

	void Check(const TString& rawKey) {
		TString key(rawKey);
		TUll val = 0;

		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		if (Find(key, val))
			std::cout << "OK: " << val << "\n";
		else
			std::cout << "NoSuchWord\n";
	}

private:

	bool Serialize(const TString& filename) {
		std::ofstream os;
		os.open(filename.CStr(), std::ios_base::binary | std::ios_base::out);
		if (os.is_open())
			SerializeImpl(Root, os);
		else
			return false;
		os.close();
		return true;
	}

	bool Deserialize(const TString& filename) {
		std::ifstream is;
		is.open(filename.CStr(), std::ios_base::binary | std::ios_base::in);
		if (is.is_open()) {
			RbTreeClean(Root);
			Root = DeserializeImpl(is, Nil);
		} else
			return false;
		is.close();
		return true;
	}

	void SerializeImpl(const TRbNode* root, std::ostream& os) const {
		if (root == Nil)
			return ;
		uint16_t keySize = root->Key.Size();
		os.write(reinterpret_cast<char*>(&keySize), sizeof(keySize));
		os.write(root->Key.CStr(), keySize);
		os.write((char*)&root->Val, sizeof(root->Val));
		os.write((char*)&root->Clr, sizeof(root->Clr));

		bool hasLeft = root->Left != Nil;
		bool hasRight = root->Right != Nil;

		os.write(reinterpret_cast<char*>(&hasLeft), sizeof(hasLeft));
		os.write(reinterpret_cast<char*>(&hasRight), sizeof(hasRight));

		if (hasLeft)
			SerializeImpl(root->Left, os);
		if (hasRight)
			SerializeImpl(root->Right, os);
	}

	TRbNode* DeserializeImpl(std::istream& is, TRbNode* parent) {
		TRbNode* root = Nil;

		// Read size of string key
		uint16_t keySize;
		is.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));

		if (is.gcount() == 0)
			return root;

		// Read string key
		char* key = new char[keySize + 1];
		key[keySize] = 0;
		is.read(key, keySize);

		//Read value
		TUll value;
		is.read(reinterpret_cast<char*>(&value), sizeof(value));

		// Read color
		TRbColor clr;
		is.read(reinterpret_cast<char*>(&clr), sizeof(clr));

		// Read children
		bool hasLeft = false;
		bool hasRight = false;
		is.read(reinterpret_cast<char*>(&hasLeft), sizeof(hasLeft));
		is.read(reinterpret_cast<char*>(&hasRight), sizeof(hasRight));

		root = new TRbNode();
		root->Key.CStrMove(key);
		key = nullptr;
		root->Val = value;
		root->Clr = clr;
		root->Parent = parent;

		if (hasLeft)
			root->Left = DeserializeImpl(is, root);
		else
			root->Left = Nil;
		if (hasRight)
			root->Right = DeserializeImpl(is, root);
		else
			root->Right = Nil;

		return root;
	}


};

#endif