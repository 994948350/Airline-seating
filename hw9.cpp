#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <cmath>
using namespace std;

class kmer{
	public:
		//constructor
		kmer(const string& val):value(val){}
		kmer(const string& val,int m_index):value(val){index.push_back(m_index);}
		//copy constructor
		kemr(const kmer& other){
			value=other.value;
			for(int i=0;i<index.size();i++){
				index.push_back(other.index[i]);
			}
		}
		
		string value;
		vector<int> index;
};

class hash{
	public:
		//consturctor
		hash():size(100),k_val(0){creat(0);}
		hash(int m_size, int k):size(m_size), k_val(k){creat(m_size);}
		
		//copy constructor,operator
		hash(const hash& other){
			copy(other);
		}
		
		hash& operator=(const hash& other){
			destroy();
			copy(other);
		}
		
		const int get_size()const{
			return size;
		}
		
		//find 
		vector<int> find(const string& value){
			vector<int> start_index;
			//get the index
			int index=my_hash(value);
			//iterator all the value in the list
			list<kmer>::iterator itr=table[index].begin();
			for(;itr!=table[index].end();itr++){
				//find and return
				if(itr->value==value){
					for(int i=0;i<itr->index.size();i++){
						start_index.push_back(itr->index[i]);
					}
					break;
				}
			}
			return start_index;
		}
		
		
		//modifier
		void insert(const string& value, int index){
			int table_index=my_hash(value);
			//if the list is not be initialized
			if(table[table_index].size()==0){
				kmer new_k(value,index);
				table[table_index].push_back(new_k);
			}
			//if the list is not null
			//consider the situation that there might be same key
			//iterator the list
			else{
				list<kmer>::iterator itr=table[table_index].begin();
				//check if the key is in the list
				bool find=false;
				for(;itr!=table[table_index].end();itr++){
					if(itr->value == value){
						itr->index.push_back(index);
						find=true;
						break;
					}
				}
				//if not find, push_back the list
				if(!find){
					kmer new_k(value,index);
					table[table_index].push_back(new_k);
				}
			}
		}
		
	
	private:
		int size;
		int k_val;
		vector<list<kmer> > table;
		
		//private helper function
		void creat(int m_size){
			list<kmer> tmp;
			for(int i=0;i<m_size;i++){
				table.push_back(tmp);
			}
		}
		
		//hash function
		int my_hash(const string& value){
			int index=0;
			for(int i=0;i<value.size();i++){
				index=index*8+value[i];
			}
			int ret=abs(index%size);
			return ret;
		}
		
		
		//helper copy;
		void copy(const hash& other){
			size=other.size;
			k_val=other.k_val;
			for(int i=0;i<other.table.size();i++){
				//creat a new list for every index
				list<kmer> new_list;
				//go through every list of other
				list<kmer>::const_iterator itr=(other.table[i]).begin();
				for(;itr!=other.table[i].end();itr++){
					//*itr might be a kmer object?
					//creat a new kmer 
					string value=itr->value;
					kmer new_kmer(value);
					for(int i=0;i<itr->index.size();i++){
						new_kmer.index.push_back(itr->index[i]);
					}
					new_list.push_back(new_kmer);
				}
				table.push_back(new_list);
			}
		}
		
		//destroy
		void destroy(){
			size=0;
			k_val=0;
			for(int i=0;i<table.size();i++){
				table[i].clear();
			}
			table.clear();
		}
	
};


void read_file(string& content, const string& filename, int k_val, hash& my_table){
	ifstream instr(filename.c_str());
	while(instr){
		string tmp;
		instr>>tmp;
		content+=tmp;
	}
	
	for(int i=0;i<content.size();i++){
		string tmp;
		for(int k=i;k<k_val+i&&k_val+i<=content.size();k++){
			tmp+=content[k];
		}
		//if size of tmp not equal to the k-val, should stop
		if(tmp.size()!=k_val) return;
		
		//cout<<i<<endl;
		//else
		my_table.insert(tmp,i);
	}
}




int main(int argc, char* argv[]){
	if(argc!=2){
		cout<<"not enough argument"<<endl;
		return 0;
	}
	

	hash my_table;
	
	ifstream instr(argv[1]);
	//content store the whole file string
	string content;
	
	string command;
	string filename;//the file need to open
	int size;
	int k_val;
	
	while(instr){
		
		instr>>command;
		if(command=="genome"){
			instr>>filename;
		}
		else if(command=="table_size"){
			instr>>size;
		}
		else if(command=="occupancy"){
			int occupancy;
			instr>>occupancy;
		}
		else if(command=="kmer"){
			instr>>k_val;
			hash tmp(size,k_val);
			my_table=tmp;

			//after table created, begin to read the file
			read_file(content, filename,k_val,my_table);
		}
		else if(command=="query"){
			int mismatch;
			string query;
			instr>>mismatch;
			instr>>query;
			
			string part=query.substr(0,k_val);
			vector<int> start_index;
			start_index=my_table.find(part);
			
			
			cout<<"Query: "<<query<<endl;
			
			int number=0;//check if there no match
			//start to compare each other
			for(int i=0;i<start_index.size();i++){
				int index_q=0;
				string match;
				int miss=0;//count the number of char not equal
				for(int j=start_index[i];j<start_index[i]+query.size()&&j<content.size();j++){
					if(content[j]!=query[index_q]) miss++;
					match+=content[j];
					index_q++;
				}
				if(miss<=mismatch&&match.size()==query.size()){
					number++;
					cout<<start_index[i]<<" "<<miss<<" "<<match<<endl;
				}	
			}
			if(number==0) cout<<"No Match"<<endl;
			
		}
		else if(command=="quit"){
			return 0;
		}
	}
}
