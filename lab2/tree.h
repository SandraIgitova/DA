#ifndef TREE_H
#define TREE_H

namespace NDs {

	template<class K, class V>
	class TRbTree {

	public:
	
		TRbTree() : Size(0) {
			Nil = new TRbNode();
			Root = Nil;
		}

		~TRbTree() {
			RbTreeClean(Root);
			delete Nil;
			Size = 0;
		}

		void Insert(const K& key, const V& val) {
			TRbNode* node = new TRbNode(key, val);
			TRbNode* p = Nil;
			TRbNode* tmp = Root;
			while (tmp != Nil) {
				p = tmp;
				if (node->Key < tmp->Key)
					tmp = tmp->Left;
				else
					tmp = tmp->Right;
			}
			node->Parent = p;
			if (p == Nil)
				Root = node;
			else if (node->Key < p->Key)
				p->Left = node;
			else
				p->Right = node;
			node->Left = Nil;
			node->Right = Nil;
			node->Clr = RED;
			InsertFix(node);
			++Size;
		}

		void Remove(const K& key) {
			TRbNode* node = Root;
			while (node != Nil) {
				if (key == node->Key)
					break ;
				node = (key < node->Key) ? node->Left : node->Right;
			}
			if (node == Nil)
				throw std::logic_error("Element with this key doesn't exist");
			Delete(node);
			--Size;
		};

		bool Find(const K& key, V& val) const {
			TRbNode* node = Root;
			while (node != Nil) {
				if (node->Key == key) {
					val = node->Val;
					return true;
				}
				node = (key < node->Key ? node->Left : node->Right);
			}
			return false;
		}

		const V& GetValue(const K& key) const {
			TRbNode* node = Root;
			while (node != Nil) {
				if (node->Key == key)
					break;
				node = (key < node->Key ? node->Left : node->Right);
			}
			if (node == Nil)
				throw std::logic_error("Element with this key doesn't exist");
			return node->Val;
		}

		V& GetValue(const K& key) {
			return const_cast<V&> (
				static_cast<const TRbTree<K, V>&>(*this).GetValue(key)
				);
		}

		void Print() {
			TreePrint(Root);
		}

	protected:
		enum TRbColor {RED, BLACK};

		struct TRbNode {
			K Key;
			V Val;
			TRbColor Clr;
			TRbNode* Left;
			TRbNode* Right;
			TRbNode* Parent;

			TRbNode():
			Key(), Val(), Clr(BLACK),
			Left(nullptr), Right(nullptr), Parent(nullptr)
			{
			}

			TRbNode(const K& key, const V& val):
			Key(key), Val(val), Clr(RED),
			Left(nullptr), Right(nullptr), Parent(nullptr)
			{
			}

			bool IsLeftSon() {
				return this == Parent->Left;
			}
		};
		TRbNode* Root;
		TRbNode* Nil;
		size_t Size;

		TRbNode* RotateLeft(TRbNode* node) {
			TRbNode* res = node->Right;
			node->Right = res->Left;
			if (res->Left != Nil)
				res->Left->Parent = node;
			res->Parent = node->Parent;
			if (node->Parent == Nil)
				Root = res;
			else if (node == node->Parent->Left)
				node->Parent->Left = res;
			else
				node->Parent->Right = res;
			res->Left = node;
			node->Parent = res;
			return res;
		}

		TRbNode* RotateRight(TRbNode* node) {
			TRbNode* res = node->Left;
			node->Left = res->Right;
			if (res->Right != Nil)
				res->Right->Parent = node;
			res->Parent = node->Parent;
			if (node->Parent == Nil)
				Root = res;
			else if (node->Parent->Left == node)
				node->Parent->Left = res;
			else
				node->Parent->Right = res;
			res->Right = node;
			node->Parent = res;
			return res;
		}

		void InsertFixLeft(TRbNode** node) {
			TRbNode* uncle = (*node)->Parent->Parent->Right;
			if (uncle->Clr == RED) { // Case when uncle is red
				(*node)->Parent->Clr = BLACK;
				uncle->Clr = BLACK;
				(*node)->Parent->Parent->Clr = RED;
				(*node) = (*node)->Parent->Parent;
			} else{
				if ((*node) == (*node)->Parent->Right) { // Case when uncle is black and node is right son
					(*node) = (*node)->Parent;
					RotateLeft(*node);
				}
				(*node)->Parent->Clr = BLACK;
				(*node)->Parent->Parent->Clr = RED;
				RotateRight((*node)->Parent->Parent);
			}
		}

		void InsertFixRight(TRbNode** node) {
			TRbNode* uncle = (*node)->Parent->Parent->Left;
			if (uncle->Clr == RED) { // Case when uncle is red
				(*node)->Parent->Clr = BLACK;
				uncle->Clr = BLACK;
				(*node)->Parent->Parent->Clr = RED;
				(*node) = (*node)->Parent->Parent;
			} else {
				if ((*node) == (*node)->Parent->Left) { // Case when uncle is black and node is left son
					(*node) = (*node)->Parent;
					RotateRight(*node);
				}
				(*node)->Parent->Clr = BLACK;
				(*node)->Parent->Parent->Clr = RED;
				RotateLeft((*node)->Parent->Parent);
			}
		}

		void InsertFix(TRbNode* node) {
			while (node->Parent->Clr == RED)
				node->Parent->IsLeftSon() ? InsertFixLeft(&node) : InsertFixRight(&node);
			Root->Clr = BLACK;
		}

		void TreePrint(TRbNode* t) {
			if (t == Nil)
				return ;
			TreePrint(t->Left);
			std::cout << t->Key << ": " << t->Val << "\n";
			TreePrint(t->Right);
		}

		void Transplant(TRbNode* u, TRbNode* v) {
			if (u->Parent == Nil)
				Root = v;
			else if (u->Parent->Left == u)
				u->Parent->Left = v;
			else
				u->Parent->Right = v;
			v->Parent = u->Parent;
		}

		TRbNode* Minimum(TRbNode* u) {
			TRbNode* tmp;
			TRbNode* res;
			res = Nil;
			tmp = u;
			while (tmp != Nil) {
				res = tmp;
				tmp = tmp->Left;
			}
			return res;
		}

		void DeleteFixLeft(TRbNode** u) {
			TRbNode* v = (*u)->Parent->Right;
			if (v->Clr == RED) {
				v->Clr = BLACK;
				v->Parent->Clr = RED;
				RotateLeft((*u)->Parent);
				v = (*u)->Parent->Right;
			}
			if (v->Left->Clr == BLACK && v->Right->Clr == BLACK) {
				v->Clr = RED;
				*u = (*u)->Parent;
			} else {
				if (v->Right->Clr == BLACK) {
					v->Left->Clr = BLACK;
					v->Clr = RED;
					RotateRight(v);
					v = (*u)->Parent->Right;
				}
				v->Clr = v->Parent->Clr;
				v->Parent->Clr = BLACK;
				v->Right->Clr = BLACK;
				RotateLeft(v->Parent);
				*u = Root;
			}
		}

		void DeleteFixRight(TRbNode** u) {
			TRbNode* v = (*u)->Parent->Left;
			if (v->Clr == RED) {
				v->Clr = BLACK;
				v->Parent->Clr = RED;
				RotateRight((*u)->Parent);
				v = (*u)->Parent->Left;
			}
			if (v->Left->Clr == BLACK && v->Right->Clr == BLACK) {
				v->Clr = RED;
				*u = (*u)->Parent;
			} else {
				if (v->Left->Clr == BLACK) {
					v->Right->Clr = BLACK;
					v->Clr = RED;
					RotateLeft(v);
					v = (*u)->Parent->Left;
				}
				v->Clr = v->Parent->Clr;
				v->Parent->Clr = BLACK;
				v->Left->Clr = BLACK;
				RotateRight(v->Parent);
				*u = Root;
			}
		}

		void DeleteFixUp(TRbNode* u) {
			while (u != Root && u->Clr == BLACK)
				u->IsLeftSon() ? DeleteFixLeft(&u) : DeleteFixRight(&u);
			u->Clr = BLACK;
		}

		void Delete(TRbNode* u) {
			TRbNode* delNode = u;
			TRbNode* delNodeSon = Nil;
			TRbColor delNodeClr = delNode->Clr;
			if (u->Left == Nil) {
				delNodeSon = u->Right;
				Transplant(u, u->Right);
			} else if (u->Right == Nil) {
				delNodeSon = u->Left;
				Transplant(u, u->Left);
			} else {
				delNode = Minimum(u->Right);
				delNodeClr = delNode->Clr;
				delNodeSon = delNode->Right;
				if (delNode->Parent == u)
					delNodeSon->Parent = delNode;
				else {
					Transplant(delNode, delNode->Right);
					delNode->Right = u->Right;
					delNode->Right->Parent = delNode;
				}
				Transplant(u, delNode);
				delNode->Left = u->Left;
				delNode->Left->Parent = delNode;
				delNode->Clr = u->Clr;
			}
			if (delNodeClr == BLACK)
				DeleteFixUp(delNodeSon);
			delete u;
		}

		void RbTreeClean(TRbNode* root) {
			if (root == Nil)
				return ;
			RbTreeClean(root->Left);
			RbTreeClean(root->Right);
			delete root;
		}
	};
}

#endif