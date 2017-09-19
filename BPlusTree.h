/* ASSUMPTIONS:
1. When a node must be split and has an odd number of keys, the extra key will go to the new right-hand node.
2. There will never be a duplicate key passed to insert.
*/

#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
using namespace std;

#ifndef DS_BPLUSTREE
#define DS_BPLUSTREE

///////////////////////////////////////////////////////////////////////////////
//DO NOT CHANGE THIS CODE
///////////////////////////////////////////////////////////////////////////////
template <class T> class BPlusTree;

template <class T>
class BPlusTreeNode {
	public:
		BPlusTreeNode() : parent(NULL) {};
		bool is_leaf();
		bool contains(const T& key);

		//For grading only, this is actually bad to have because
		//it exposes a private member variable.
		BPlusTreeNode* get_parent() {
			return parent;
		}

		//We need this to let BPlusTree access private members
		friend class BPlusTree<T>;
	private:
		bool contains(const T& key,std::size_t low,std::size_t high);
		std::vector<T> keys;
		std::vector<BPlusTreeNode*> children;
		BPlusTreeNode* parent;
};

template <class T>
bool BPlusTreeNode<T>::is_leaf() {
	for(unsigned int i=0; i<children.size(); i++) {
		if(children[i]) {
			return false;
		}
	}
	return true;
}

template <class T>
bool BPlusTreeNode<T>::contains(const T& key) {
	return contains(key,0,keys.size()-1);
}

//Private method takes advantage of the fact keys are sorted
template <class T>
bool BPlusTreeNode<T>::contains(const T& key,std::size_t low,std::size_t high) {
	if(low>high) {
		return false;
	}
	if(low==high) {
		return key == keys[low];
	}
	std::size_t mid = (low+high)/2;
	if(key<=keys[mid]) {
		return contains(key,0,mid);
	} else {
		return contains(key,mid+1,high);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//Your BPlusTree implementation goes below this point.
///////////////////////////////////////////////////////////////////////////////////////
template <class T>
class BPlusTree {
	public:
		//constructor,copy constructor, =operator, destructor
		BPlusTree(int m_size):root(NULL), t_size(m_size) {}
		BPlusTree(const BPlusTree<T>& other):t_size(other.t_size) {
			copy_tree(other.root,root);
		}
		BPlusTree& operator=(const BPlusTree<T>& other) {
			if(&other!=this) {
				this->destroy_tree(root);
				copy_tree(other.root,root);
				t_size=other.t_size;
			}
			return *this;
		}

		~BPlusTree() {
			t_size=0;
			this->destroy_tree(root);
			root=NULL;
		}


		//find
		BPlusTreeNode<T>* find(const T& a_key) {
			return find(a_key,root);
		}

		void insert(const T& a_key) {
			insert(a_key,root);
		}


		//print side way
		void print_sideways(std::ofstream& outfile) {
			print_sideways(root,outfile,0);
		}
		void print_BFS(std::ofstream& outfile) {
			print_BFS(root,outfile);
		}
		void print_BFS_pretty(std::ofstream& outfile) {
			print_BFS_pretty(root,outfile);
		}


	private:
		BPlusTreeNode<T>* root;
		int t_size;

		//private helper function
		//helper copy constructor, =operator
		void copy_tree(BPlusTreeNode<T>* p,BPlusTreeNode<T>*& m_node) {
			if(!p) return;
			m_node=new BPlusTreeNode<T>;
			//copy value
			for(unsigned int i=0; i<p->keys.size(); i++) {
				m_node->keys.push_back(p->keys[i]);
			}
			if(p->is_leaf()) return;
			
			//copy children
			for(unsigned int i=0; i<p->children.size(); i++) {
				m_node->children.push_back(NULL);
			}
			//enter children and recursive
			for(unsigned int i=0; i<p->children.size(); i++) {
				copy_tree(p->children[i],m_node->children[i]);
				//point the parent pointer of children to itself
				m_node->children[i]->parent=m_node;
			}
			
		}

		//find
		BPlusTreeNode<T>* find(const T& a_key, BPlusTreeNode<T>* p) {
			//make sure the p is not a null
			if(!p) return NULL;
			//to record which child might be followed
			int i=0;
			int num=p->keys.size();
			for(; i<num; i++) {
				if(a_key<(p->keys)[i]) break;
			}
			//after for loop(already find which child might be follwed)
			//if is a leaf, means no such key
			if(p->is_leaf()) {
				return p;
			} else {
				return find(a_key,p->children[i]);
			}
		}

		//template <typename P>
		bool sort_child(const BPlusTreeNode<T>*& p1,const BPlusTreeNode<T>*& p2) {
			if((p1->keys[0])<(p2->keys[0])) return true;
			else return false;
		}

		//insert a key to the leaf
		void insert(const T& a_key,BPlusTreeNode<T>*& p) {
			//if root is a null
			if(!p) {
				p=new BPlusTreeNode<T>;
				(p->keys).push_back(a_key);
				return;
			}
			//if p is not a null
			//if p is a leaf
			else if(p->is_leaf()) {
				(p->keys).push_back(a_key);
				//then sort the keys in right direction
				sort((p->keys).begin(),(p->keys).end());
				split(root,p);
			}
			//p is not a leaf and null
			else {
				//go through the key, to find which child should go to
				unsigned int i=0;
				for(; i<(p->keys).size(); i++) {
					if(a_key< (p->keys)[i]) break;
				}
				//go to the next child
				insert(a_key,(p->children)[i]);
			}
		}


		//split:according to the size, split the leaf
		void split(BPlusTreeNode<T>*& root,BPlusTreeNode<T>* leaf) {
			if(!leaf) return;
			int num=leaf->keys.size();
			//if the leaf keys size less than the size, do nothing
			if(num<t_size) return;
			//below is when leaf is not a leaf

			//find the middle key
			int index_move=(leaf->keys).size()/2;
			T move_key=leaf->keys[index_move];

			//split the leaf in to two node(keep but edit the original node, creat a new one)
			//if is a leaf, not consider the children of leaf
			BPlusTreeNode<T>* new_node=new BPlusTreeNode<T>;
			if(leaf->is_leaf()) {
				for(int i=leaf->keys.size()-1; i>=index_move; i--) {
					new_node->keys.push_back(leaf->keys[i]);
					leaf->keys.pop_back();
				}
				sort(new_node->keys.begin(),new_node->keys.end());
				//renew the parent pointer
				new_node->parent=leaf->get_parent();
			} else { //leaf is not a leaf, take the children in to consideration
				for(int i=leaf->keys.size()-1; i>=index_move; i--) {
					//not add the key to the new node
					if(i!=index_move)
						new_node->keys.push_back(leaf->keys[i]);
					leaf->keys.pop_back();
				}
				sort(new_node->keys.begin(),new_node->keys.end());
				//operation to their children
				//remove the last two children from leaf, and add them to new_node in reverse order
				for(int i=leaf->children.size()-1; i>=index_move+1; i--) {
					unsigned int j=0;
					for(; j<new_node->children.size(); j++) {
						if(leaf->children[i]->keys[0]<new_node->children[j]->keys[0]) break;
					}
					new_node->children.insert(new_node->children.begin()+j,leaf->children[i]);
					//meanwhile, change the parent
					leaf->children[i]->parent=new_node;
					leaf->children.pop_back();
				}
			}

			//if this leaf is root(no parent)
			if(!leaf->get_parent()) {
				//creat a new node
				root=new BPlusTreeNode<T>;
				root->keys.push_back(move_key);
				root->children.push_back(leaf);
				root->children.push_back(new_node);
				//renew the parent of leaf and new_node
				leaf->parent=root;
				new_node->parent=root;
			} else { //the leaf is not root
				//bring the remove key to the parent
				leaf->parent->keys.push_back(move_key);
				sort(leaf->parent->keys.begin(),leaf->parent->keys.end());
				//add the new node to the parent's children vector
				unsigned int j=0;
				for(; j<leaf->parent->children.size(); j++) {
					if(new_node->keys[0]<leaf->parent->children[j]->keys[0]) break;
				}
				leaf->parent->children.insert(leaf->parent->children.begin()+j,new_node);
				new_node->parent=leaf->get_parent();
				//recursive
				split(root,leaf->parent);
			}
		}


		void print_sideways(BPlusTreeNode<T>* p,std::ofstream& outfile, int depth) {
			//if p is a null
			if(!p) {
				outfile<<"Tree is empty."<<endl;
				return;
			}
			//if p is a leaf
			if(p->is_leaf()) {
				for(int i=0; i<depth; i++) {
					outfile<<"\t";
				}
				//print out all the keys
				for(unsigned int i=0; i<p->keys.size(); i++) {
					outfile<<p->keys[i];
					if(i+1<p->keys.size()) outfile<<",";
				}
				outfile<<endl;
			} else {
				print_sideways(p->children[0],outfile,depth+1);
				for(int i=0; i<depth; i++) {
					outfile<<"\t";
				}
				for(unsigned int i=0; i<p->keys.size(); i++) {
					outfile<<p->keys[i];
					if(i+1<p->keys.size()) outfile<<",";
				}
				outfile<<endl;
				for(unsigned int j=1; j<p->children.size(); j++) {
					print_sideways(p->children[j],outfile,depth+1);
				}
			}
		}

		void print_BFS(BPlusTreeNode<T>* p,std::ofstream& outfile) {
			if(!p) outfile<<"Tree is empty."<<endl;
			list<BPlusTreeNode<T>* > current_row;
			list<BPlusTreeNode<T>* > next_row;
			if(p) {
				current_row.push_back(p);
			}
			while(!current_row.empty()) {
				while(!current_row.empty()) {
					BPlusTreeNode<T>*& tmp=current_row.front();
					for(unsigned int i=0; i<tmp->keys.size(); i++) {
						outfile<<tmp->keys[i];
						if(i+1<tmp->keys.size()) outfile<<",";
					}
					for(unsigned int i=0; i<tmp->children.size(); i++) {
						next_row.push_back(tmp->children[i]);
					}
					if(current_row.size()!=1) outfile<<"\t";
					current_row.pop_front();
				}
				outfile<<endl;

				current_row=next_row;
				next_row.clear();
			}
		}

		void destroy_tree(BPlusTreeNode<T>* p) {
			if (!p) return;
			if(p->is_leaf()) {
				delete p;
				return;
			}
			for(unsigned int i=0; i<p->children.size(); i++) {
				destroy_tree(p->children[i]);
			}
			delete p;
		}


		int find_depth(BPlusTreeNode<T>* p,int deapth) {
			if(p->is_leaf()) {
				return deapth;
			} else {
				find_depth(p->children[0],deapth+1);
			}
		}
		//----------------extra credit-------------------
		void map_create(map<int,vector <vector <BPlusTreeNode<T>*> > >& nodes, BPlusTreeNode<T>* p) {
			list<BPlusTreeNode<T>* >current_row;
			list<BPlusTreeNode<T>* >next_row;
			if(p) {
				current_row.push_back(p);
			}
			//start at the second layer
			//the nodes int first layer will always be 1
			int layer=2;

			while(!current_row.empty()) {
				//to store the nodes part by part in next layer
				vector<vector <BPlusTreeNode<T>*> >layer_nodes;
				while(!current_row.empty()) {
					//the inner list of layer_node
					vector<BPlusTreeNode<T>* >parts;
					//for convinient add child to the next_row
					BPlusTreeNode<T>*& tmp = current_row.front();
					for(int i=0; i<tmp->children.size(); i++) {
						next_row.push_back(tmp->children[i]);
						parts.push_back(tmp->children[i]);
					}
					//bring the parts to layer_node
					if(parts.size()!=0)
						layer_nodes.push_back(parts);
					//delete the first in current row
					current_row.pop_front();
				}
				//bring the layer_node according the layer;
				nodes[layer]=layer_nodes;
				current_row=next_row;
				next_row.clear();
				layer++;
			}
		}


		void print_BFS_pretty(BPlusTreeNode<T>* root,int layer,const vector<int>& index,
		                      vector<string>& printfile,std::ofstream& outfile,
		                      map<int,vector <vector <BPlusTreeNode<T>*> > >& nodes
		                     ) {
			if(layer==1) {
				for(int i=0; i<root->keys.size(); i++) {
					outfile<<root->keys[i];
					if(i+1<root->keys.size()) outfile<<",";
				}
				outfile<<endl;
				//print out other lines
				for(int i=printfile.size()-1; i>=0; i--) {
					outfile<<printfile[i]<<endl;
				}
				return;
			} else {
				vector<int> new_index;
				int num_index=0;
				string tmp;
				//go through every part
				for(int i=0; i<nodes[layer].size(); i++) {
					//every nodes in the part
					for(int j=0; j<nodes[layer][i].size(); j++) {
						//every key in every node
						for(int k=0; k<nodes[layer][i][j]->keys.size(); k++) {
							stringstream ss;
							ss<<nodes[layer][i][j]->keys[k];
							tmp+=ss.str();
							if(k+1<nodes[layer][i][j]->keys.size()) tmp+=",";
						}
						string s(index[num_index]-tmp.size(),' ');
						tmp+=s;
						num_index++;
					}
					new_index.push_back(tmp.size());
				}
				printfile.push_back(tmp);
				print_BFS_pretty(root,layer-1,new_index,printfile,outfile,nodes);
			}
		}

		//driver of print_bfs
		void print_BFS_pretty(BPlusTreeNode<T>* root,std::ofstream& outfile) {
			map<int,vector <vector <BPlusTreeNode<T>*> > > nodes;
			map_create(nodes,root);
			int layer=nodes.size();
			string tmp;
			//store which space is the begining of one part
			vector<int> index;
			//printfile store the lines needs to print
			vector<string> printfile;
			//go through every part
			for(int i=0; i<nodes[layer].size(); i++) {
				//every nodes in the part
				for(int j=0; j<nodes[layer][i].size(); j++) {
					//every key in every node
					for(int k=0; k<nodes[layer][i][j]->keys.size(); k++) {
						stringstream ss;
						ss<<nodes[layer][i][j]->keys[k];
						tmp+=ss.str();
						if(k+1<nodes[layer][i][j]->keys.size()) tmp+=",";
					}
					tmp+=" ";
				}
				tmp+="     ";
				index.push_back(tmp.size());
			}
			printfile.push_back(tmp);

			print_BFS_pretty(root,layer-1,index,printfile,outfile,nodes);
		}
};

#endif
