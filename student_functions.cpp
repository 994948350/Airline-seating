#include <fstream>
#include <iostream>
#include <string>
#include "SongLibrary.h"

//////////////////DO NOT CHANGE THIS CODE////////////////////////

//Construct a Song object with values
Song::Song(const std::string& title, const std::string& artist, 
     const std::string& album, unsigned int year){
	this->title = title;
	this->artist = artist;
	this->album = album;
	this->year = year;
	used = false;
	this->song_group_ptr = NULL;
}

//Song equality doesn't depend on pointers or being used
bool operator== (const Song& s1, const Song& s2){
	return (s1.getYear() == s2.getYear()) && (s1.getTitle() == s2.getTitle())
          && (s1.getAlbum() == s2.getAlbum()) && (s1.getArtist() == s2.getArtist());
}

//Modifier for group pointer
void Song::updateSongGroupPointer(SongGroupNode* ptr){
	if(ptr){
		//Check that ptr is actually for this Song.
		assert(ptr->m_song->value == *this); 
		song_group_ptr = ptr;
	}
	else{
		song_group_ptr = NULL;
	}
}

//Make output easier by overload operator<<
std::ostream& operator<< (std::ostream& ostr, const Song& song){
	if(!song.getSongGroupPointer()){
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
	       << song.getAlbum() << "\" (" << song.getYear() << ")" << std::endl;
	}
	else{
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
		<< song.getAlbum() << "\" (" << song.getYear() << ")" << " in Song Group "
		<< song.getSongGroupPointer()->id << std::endl;
	}
	return ostr;
}

/////////////////////////YOUR CODE STARTS HERE/////////////////////////////////


//////////////////////////////GROUP FUNCTIONS/////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are searching for
BEHAVIOR:
None
RETURN:
NULL if no list matches the ID, otherwise returns the GroupListNode* containing
the head of the list with ID id.
*/
GroupListNode* GroupExists(GroupListNode* song_groups, GroupID id){
    //search point to the head of the song_groups first
    //coner case assume: what if seach->value==null
    GroupListNode* search=song_groups;
	while(search!=NULL){
		if(search->value!=NULL&&search->value->id==id) {  //match id
			return search;
		}
		search=search->ptr;//step in to next song group
	}
	return NULL;
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are creating
BEHAVIOR:
Add a new song group to song_groups, give the new song group ID id. Order of
song groups is not guaranteed.
RETURN:
None
*/
void MakeGroup(GroupListNode*& song_groups, GroupID id){
	//make a new song, initialize every element
	SongGroupNode* a_new=new SongGroupNode;
	a_new->prev_song_ptr=NULL;
	a_new->next_song_ptr=NULL;
	a_new->prev_by_year_ptr=NULL;
	a_new->next_by_artist_ptr=NULL;
	a_new->m_song=NULL;
	a_new->id=id;
	//coner case: song_group is a null;
	if(song_groups==NULL){
		song_groups=new GroupListNode;
		song_groups->ptr=NULL;
		song_groups->value=a_new;
	}
	//song_group is not a null, add a new GroupListNode before the head
	else{
		GroupListNode* new_head=new GroupListNode;
		new_head->ptr=song_groups;
		new_head->value=a_new;
		song_groups=new_head;
	}
}


/*
ARGUMENTS:
-group_head points to the head of a song group
-song is the Song we are checking the group for
BEHAVIOR:
None
RETURN:
If song is found in the group return true. If song is not found,
return false.
*/
bool SongInGroup(SongGroupNode* group_head, const Song& song){
	SongGroupNode* search=group_head;
	while(search!=NULL){
		//coner case: search->m_song==NULL
		//m_song: tye Node<song>*, m_song->value: type song
		if(search->m_song!=NULL&&search->m_song->value==song)
			return true;
		search=search->next_song_ptr;
	}
	return false;
}

/*
ARGUMENTS:
-library_ptr points to the LibraryNode* containing the Song we want to add.
-list_ptr is the head of the song group we want to add the song to.
BEHAVIOR:
Modify the group pointed to by list_ptr to reflect that the song is in the group.
This may change the length of the list. Modifies the Song's members as well.
RETURN:
None
*/
void AddToGroup(LibraryNode* library_ptr, GroupListNode* list_ptr){
	SongGroupNode* search=list_ptr->value;//begining of the songgroup node
	//the value contain only the first pointer with "NULL" m_song
	if(search->m_song==NULL){
		search->m_song=library_ptr;
	}
	else{
		//to find where should add a new song
		while(search->next_song_ptr!=NULL){
			search=search->next_song_ptr;
		}
		search->next_song_ptr=new SongGroupNode;
		//initialize member variable for next location
		search->next_song_ptr->prev_song_ptr=search;
		search->next_song_ptr->next_by_artist_ptr=NULL;
		search->next_song_ptr->next_song_ptr=NULL;
		search->next_song_ptr->prev_by_year_ptr=NULL;
		search->next_song_ptr->m_song=library_ptr;
		search->next_song_ptr->id=list_ptr->value->id;
		search=search->next_song_ptr;  //move search to the new location
	}
	//recompute all next artist
	SongGroupNode* search_a=list_ptr->value;
	while(search_a!=NULL&&search_a->m_song!=NULL){
		SongGroupNode* next=search_a->next_song_ptr;
		std::string artist=search_a->m_song->value.getArtist();
		while(next!=NULL){
			if(artist==next->m_song->value.getArtist()){
				search_a->next_by_artist_ptr=next;
				break;
			}
			next=next->next_song_ptr;
		}
		search_a=search_a->next_song_ptr;
	}
	
	//recompute pre year
	RecomputePrevByYear(list_ptr);
	//change the member variable for library_ptr
	library_ptr->value.setUsedInGroup(true);
	library_ptr->value.updateSongGroupPointer(search);	
}

/*
ARGUMENTS:
-library_ptr points to the LibraryNode* containing the Song we want to remove from a group.
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Modifies the Song's member variables to reflect that it is not in a group. Modifies the group
so that the Song is not in the group. This function may cause song_groups to be modified as well.
RETURN:
None
*/
void RemoveFromGroup(LibraryNode* library_ptr, GroupListNode* song_groups){
	//change the library_ptr
	//coner case: 
	//1. there is only first one and needed to be removed
	//2. the first one need to be removed
	//3. the last one need to be removed
	//get the location of song which will be removed
	
	
//fing where is the group
	SongGroupNode* search=library_ptr->value.getSongGroupPointer();
	GroupListNode* search_groups=song_groups;
	while(search_groups!=NULL){
		if(search->id==search_groups->value->id){
			break;
		}
		else search_groups=search_groups->ptr;
	}
	
	
	//change the pre and next song pointer
	if(search->prev_song_ptr==NULL&&search->next_song_ptr==NULL){  //coner case 1
		search->m_song=NULL;
	
	}
	else if(search->prev_song_ptr==NULL){                         //coner case 2
		search->next_song_ptr->prev_song_ptr=NULL;
		search_groups->value=search->next_song_ptr;
		delete search;
	}
	else if(search->next_song_ptr==NULL){                        //coner case 3
		search->prev_song_ptr->next_song_ptr=NULL;
		delete search;
		
	}
	//in the middle
	else{
		search->prev_song_ptr->next_song_ptr=search->next_song_ptr;
		search->next_song_ptr->prev_by_year_ptr=search->prev_song_ptr;
		delete search;
	}
	
	//pointer for pre_year, next_artist renew:
	RecomputePrevByYear(song_groups);
	//recompute the artist
	SongGroupNode* search_a=song_groups->value;
	while(search_a!=NULL&&search_a->m_song!=NULL){
		//next start one after search
		SongGroupNode* next=search_a->next_song_ptr;
		std::string artist=search_a->m_song->value.getArtist();
		while(next!=NULL){
			if(artist==next->m_song->value.getArtist()){//mearsure if there artist the same
				search_a->next_by_artist_ptr=next;
				break;
			}
			next=next->next_song_ptr;
		}
		search_a=search_a->next_song_ptr;
	}
	//update the member variabel in library
	library_ptr->value.updateSongGroupPointer(NULL);
	library_ptr->value.setUsedInGroup(false);
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-list_ptr1 contains to the song group head of Group A
-list_ptr2 contains the song group head of Group B
BEHAVIOR:
Modify Group A so that at the end of its songs, the songs of Group B are added in the same
order they were found in Group B. Remove the now-empty Group B.
RETURN:
None
*/
void CombineGroups(GroupListNode*& song_groups, GroupListNode*& list_ptr1, GroupListNode*& list_ptr2){
	//link the value of list2 to very tail of value of list1 
	//coner case: 1. m_song of list is null  2. list_ptr2 only one object and contain "NULL" m_song
	SongGroupNode* search=list_ptr1->value;  //start a the begining.
	unsigned int id=list_ptr1->value->id;
	//the list1 m_song is a null
	if(search->m_song==NULL){
		SongGroupNode* temp=list_ptr1->value;
		list_ptr1->value=list_ptr2->value;
		delete temp;
	}
	else{
		//go to the tail of list 1
		while(search->next_song_ptr!=NULL){
			search=search->next_song_ptr;
		}
	//search is now point to the tail of the list_ptr now 
		if(list_ptr2->value->m_song==NULL){  //coner case 2:list 2 m_song is a null
			search->next_song_ptr=NULL;
			delete list_ptr2->value;
		}
		else{
			search->next_song_ptr=list_ptr2->value;
			list_ptr2->value->prev_song_ptr=search;	
		}
		list_ptr2->value=NULL;
	}
	
//go through all the list and change the id;
	SongGroupNode* search_id=list_ptr1->value;
	while(search_id!=NULL){
		search_id->id=id;
		search_id=search_id->next_song_ptr;
	}
	
//resigne the pointer of song to new location again
	SongGroupNode* resign=list_ptr1->value;
	while(resign!=NULL){
		resign->m_song->value.updateSongGroupPointer(resign);
		resign=resign->next_song_ptr;
	}

	//delete the list_ptr2
	//coner case: the list 2 start at first
	GroupListNode* temp=list_ptr2;
	GroupListNode* group_search=song_groups;
	if(group_search==list_ptr2){            //coner case: first one
		song_groups=list_ptr2->ptr;         //change the head
		delete list_ptr2;
	}
	// in the middle(common cases)
	else{
		while(group_search->ptr!=NULL){
			if(group_search->ptr==list_ptr2){
				group_search->ptr=list_ptr2->ptr;//even if list2 at the back, group_search point to null;
				break;
			}
			group_search=group_search->ptr;  
		}
		delete list_ptr2;
	}
	
	//change the  next_artist
	SongGroupNode* search_a=list_ptr1->value;
	while(search_a!=NULL&&search_a->m_song!=NULL){
		SongGroupNode* next=search_a->next_song_ptr;
		std::string artist=search_a->m_song->value.getArtist();
		while(next!=NULL){
			if(artist==next->m_song->value.getArtist()){
				search_a->next_by_artist_ptr=next;
				break;
			}
			next=next->next_song_ptr;
		}
		search_a=search_a->next_song_ptr;
	}
	//change the pre year
	RecomputePrevByYear(list_ptr1);
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-library_ptr points to the LibraryNode* containing the Song we want to remove from a group.
BEHAVIOR:
Remove the group from the list of song groups. No Song objects should remain associated with the
group, and the function should modify Song member variables appropriately to reflect this. Any
memory associated with the group must be released.
RETURN:
None
*/
void RemoveGroup(GroupListNode*& song_groups, GroupListNode* list_ptr){
	//coner case: 1. the list_ptr is only element 2. the list_ptr is the first one
	GroupListNode* temp=list_ptr;
	GroupListNode* search=song_groups;//start at the head
	if(search->ptr==NULL){            //coner case 1
		song_groups=NULL;
	}
	else if(search==list_ptr){        //coner case 2
		//make the head to be the second one
		song_groups=search->ptr; 
	}
	else{
		while(search!=NULL&&search->ptr!=NULL){
			if(search->ptr==list_ptr){
				search->ptr=list_ptr->ptr;
				break;
			}
			search=search->ptr;
		}
	}
	//change the memeber variable of song
	//make sure the m_song is not a null
	if(list_ptr->value->m_song!=NULL){
		list_ptr->value->m_song->value.updateSongGroupPointer(NULL);
		list_ptr->value->m_song->value.setUsedInGroup(false);
	}
	//release the memory
	SongGroupNode* temp_song=list_ptr->value;
	while(temp_song!=NULL){
		SongGroupNode* small_temp=temp_song->next_song_ptr;
		delete temp_song;
		temp_song=small_temp;
	}
	delete list_ptr;
	//release memory
	
}
/*
ARGUMENTS:
-list_ptr is the head of the song group we want to add the song to.
BEHAVIOR:
Recompute all prev_by_year_ptr members in the song group pointed to by
list_ptr.
RETURN:
None
*/
void RecomputePrevByYear(GroupListNode* list_ptr){
	SongGroupNode* search=list_ptr->value;    //head of the song start
	//find the tail
	while(search!=NULL&&search->next_song_ptr!=NULL){
		search=search->next_song_ptr;
	}
	//start from the tail, find and connect each pre_year pointer;
	while(search!=NULL){
		if(search->m_song!=NULL){
			unsigned int year=search->m_song->value.getYear();
			SongGroupNode* next=search;
			while(next!=NULL){
				if(next->m_song!=NULL&&year>next->m_song->value.getYear()){
					search->prev_by_year_ptr=next;
					break;
				}
				next=next->prev_song_ptr;
			}	
		}
		search=search->prev_song_ptr;	
	}

}

////////////////////////////PRINT FUNCTIONS////////////////////////////////////

/*
ARGUMENTS:
-library_head points to the first LibraryNode* of the song library
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the library in the order it was added.
RETURN:
None
*/
void PrintLibrary(LibraryNode* library_head,std::ostream& print_file){
	unsigned int count=0;    //count number of songs
	LibraryNode* search=library_head;  //start at the head of library
	while(search!=NULL){
		count++;
		print_file<<"\""<<search->value.getTitle()<<"\" by "
		<<search->value.getArtist()<<" on \""
		<<search->value.getAlbum()<<"\" ("<<
		search->value.getYear()<<")"<<std::endl;
		search=search->ptr;
	}

	print_file<<"Library contains "<<count<<" song(s)"<<std::endl;
}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to print
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the group in the order it was added.
RETURN:
None
*/
void PrintGroupSequential(SongGroupNode* group_head,std::ostream& print_file){
	print_file<<"List ID "<<group_head->id<<" contains:"<<std::endl;
	unsigned int count=0;   //count the number of song
	SongGroupNode* search=group_head;//start at the head
	while(search!=NULL){
		//make sure the m_song is not NULL(first one)
		if(search->m_song!=NULL){
			print_file<<"\""<<search->m_song->value.getTitle()<<"\" by "
			<<search->m_song->value.getArtist()<<" on \""
			<<search->m_song->value.getAlbum()<<"\" ("<<
			search->m_song->value.getYear()<<") in Song Group "
			<<search->id<<std::endl;
			count++;
		}
		search=search->next_song_ptr;
	}
	print_file<<count<<" song(s)"<<std::endl;
}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the prev_by_year_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupRewind(SongGroupNode* group_ptr,std::ostream& print_file){
	//start at the front
	unsigned int count=0;//count recored the number of song
	print_file<<"Rewind list for "<<group_ptr->m_song->value.getYear()<<std::endl;
	SongGroupNode* start=group_ptr;
	while(start!=NULL){
		count++;
		print_file<<"	#"<<count<<": \""<<start->m_song->value.getTitle()<<"\" by "
		<<start->m_song->value.getArtist()<<" on \""
		<<start->m_song->value.getAlbum()<<"\" ("<<
		start->m_song->value.getYear()<<") in Song Group "<<
		start->id<<std::endl;
		
		//go the next position
		start=start->prev_by_year_ptr;
	}
	print_file<<"Rewind list contained "<<count<<" song(s)."<<std::endl;

}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the next_by_artist_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupMarathon(SongGroupNode* group_ptr,std::ostream& print_file){
	//re-make the pointer of point by artist to make sure
	SongGroupNode* search=group_ptr;
	while(search!=NULL){
		SongGroupNode* next=search->next_song_ptr;
		std::string artist=search->m_song->value.getArtist();
		while(next!=NULL){
			if(artist==next->m_song->value.getArtist()){
				search->next_by_artist_ptr=next;
				break;
			}
			next=next->next_song_ptr;
		}
		search=search->next_song_ptr;
	}
	//start at the head given, according to the next_artist pointer to find one by one
	unsigned int count=1;
	SongGroupNode* start=group_ptr;
	print_file<<"Marathon list for "<<start->m_song->value.getArtist()<<std::endl;
	while(start!=NULL){
		print_file<<"	#"<<count<<": \""<<start->m_song->value.getTitle()<<"\" by "<<start->m_song->value.getArtist()<<" on \""
		<<start->m_song->value.getAlbum()<<"\" ("<<start->m_song->value.getYear()<<") in Song Group "<<
		start->id<<std::endl;
		start=start->next_by_artist_ptr;
		
		count++;
	}
	print_file<<"Marathon list contained "<<count-1<<" song(s)."<<std::endl;

}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to analyze
-print_file points to an ostream to print output to
BEHAVIOR:
Find the rewind list with the most songs in the song group pointed to by group_head.
Print the rewind list and the song count to print_file.
RETURN:
None
*/
void PrintLongestGroupRewind(GroupListNode* group_head,std::ostream& print_file){
//max_count sotr the number of songs,max_head point to the location the rewind list will start to print	
	unsigned int max_count=0;
	SongGroupNode* max_head=NULL;
	SongGroupNode* search=group_head->value;
	//start to search every location to find the longest
	while(search!=NULL){//search store the current location
		unsigned int count=0;
		SongGroupNode* pre_year=search;
		while(pre_year!=NULL){//pre_year finds the all pre_year ptr for current"search" 
			count++;
			pre_year=pre_year->prev_by_year_ptr;
		}
		if(count>max_count){
			max_head=search;
			max_count=count;
		}

		search=search->next_song_ptr;
	
	}
	print_file<<"Maximum size rewind list is for year "<<max_head->m_song->value.getYear()<<std::endl;
	int j=1;//j use to print the number sequence
	while(max_head!=NULL){
		print_file<<"	#"<<j<<": \""<<max_head->m_song->value.getTitle()<<"\" by "<<max_head->m_song->value.getArtist()<<" on \""
		<<max_head->m_song->value.getAlbum()<<"\" ("<<max_head->m_song->value.getYear()<<") in Song Group "<<
		max_head->id<<std::endl;
		max_head=max_head->prev_by_year_ptr;
		j++;
	}
	print_file<<"Longest rewind list contained "<<max_count<<" song(s)."<<std::endl;
}

///////////////////////////////CLEANUP/////////////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Delete every group. Do not leak. Modifies all groups and the song_groups list.
Update every Song to reflect not being in a group.
RETURN:
None
*/
void DeleteAllSongGroups(GroupListNode*& song_groups){
//outer list
	while(song_groups!=NULL){
		//iner song_groups;
		while(song_groups->value!=NULL){
			SongGroupNode* temp=song_groups->value;
			song_groups->value=song_groups->value->next_song_ptr;
			delete temp;
		}
		GroupListNode* temp=song_groups;
		song_groups=song_groups->ptr;
		delete temp;
	}
}
