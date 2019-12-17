#include <iostream>
#include <fstream>
#include "string.h"
#include "realizetree.h"

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	TString cmd;
	TSerializableRbTree rbTree;

	while (std::cin >> cmd) {
		switch (cmd[0]) {
			case '+':
				rbTree.SerializeInsert();
				break;
			case '-':
				rbTree.SerializeRemove();
				break;
			case '!':
				rbTree.SaveLoad();
				break;
			default:
				rbTree.Check(cmd);
				break;
		}
	}
	return 0;
}