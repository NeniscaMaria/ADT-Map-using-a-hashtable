#include "Map.h"
#include "MapIterator.h"
#include <iostream>

Map::Map()
{
	this->m = 20;
	this->T = new TElem[this->m];
	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	for (int i = 0; i < this->m; i++)
		this->T[i] = empty;
	this->next = new int[this->m];
	for (int i = 0; i < this->m; i++)
		this->next[i] = -1;
	this->firstFree = 0;
	this->s = 0;
	//theta(m)
}

void Map::changeFirstFree() {
	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	while (this->firstFree < this->m and this->T[firstFree] != empty)
		this->firstFree++;
	//O(m)
}

void Map::resize() {
	this->m *= 2;
	TElem* newList = new TElem[this->m];
	int* newNext = new int[this->m];
	//resizing next; we don't need to copy the values from this->next because we will perform the rehash which will change the links
	for (int i = 0; i < this->m ; i++)
		newNext[i] = -1;
	//delete[] next;
	this->next = newNext;
	//resizing T
	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	for (int i = 0; i < this->m; i++)
		newList[i] = empty;
	
	this->firstFree = 0;
	//rehashing
	//we need to reset firstFree according to the elements in the newList
	for (int i = 0; i < this->m / 2; i++) {
		if (this->T[i] != empty)//this means that there is an element that needs to be rehashed
		{
			auto pos = this->h(this->T[i].first);//the first element of the pair is the key; the second is the value
			if (newList[pos] == empty) {
				newList[pos] = this->T[i];
				while (this->firstFree < this->m and newList[firstFree] != empty)
					this->firstFree++;
			}
			else {
				auto current = pos;
				while (this->next[current] >= 0)
					current = this->next[current];
				while (this->firstFree < this->m and newList[firstFree] != empty)
					this->firstFree++;
				newList[this->firstFree] = this->T[i];
				this->next[current] = this->firstFree;
				while (this->firstFree < this->m and newList[firstFree] != empty)
					this->firstFree++;
			}
		}
	}
	//delete[] T;
	this->T = newList;
	//theta(m)
}

TValue Map::add(TKey c, TValue v)
{// adds a pair (key,value) to the map
	//if the key already exists in the map, then the value associated to the key is replaced by the new value and the old value is returned
	//if the key does not exist, a new pair is added and the value null is returned
	
	TElem element;
	element.first = c;
	element.second = v;
	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	//we search the map for this key
	this->changeFirstFree();
	if (this->firstFree == this->m) {
		this->resize();
	}
	auto pos = this->h(c); //position where the element should be
	auto current = pos;
	//Here we treat the case when the key already exists in the hash table and we need to replace the value. We won't increment the size.
	if (this->T[current] != empty) {
		if (this->T[current].first == c) {
			TValue oldValue = this->T[current].second;
			this->T[current].second = v;
			return oldValue;
		}
		while (current != -1) {
			if (this->T[current].first == c) { 
				TValue oldValue = this->T[current].second;
				this->T[current].second = v;
				return oldValue;
			}
			current = this->next[current];
		}
	}
	//Here we treat the case when the key does not exist in the map.
	if (this->T[pos] == empty) {
		this->T[pos] = element;
		this->s++;
	}
	else {
		current = pos;
		while (this->next[current] != -1)
			current = this->next[current];
		this->T[this->firstFree] = element;
		this->next[this->firstFree] = -1;
		this->next[current] = this->firstFree;
		this->changeFirstFree();
		this->s++;
	}
	return NULL_TVALUE;
	//O(m);
}

TValue Map::search(TKey c){
	//searches for the key and returns the value associated with the key if the map contains the key or null: NULL_TVALUE otherwise
	TValue search(TKey c);
	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	auto pos = this->h(c);
	auto current = pos;
	if (this->T[current] != empty) {
		if (this->T[current].first == c) {
			return this->T[current].second;
		}
		while (current != -1) {
			if (this->T[current].first == c) { 
				return this->T[current].second;
			}
			current = this->next[current];
		}
	}
	return NULL_TVALUE;
	//O(m)
}

TValue Map::remove(TKey c)
{
	//removes a key from the map and returns the value associated with the key if the key existed or null: NULL_TVALUE otherwise
	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	auto i = this->h(c);
	auto j = -1;
	auto idx = 0;
	while (idx < this->m and j == -1)
		if (this->next[idx] == i)
			j = idx;
		else
			idx += 1;
	while (i != -1 and this->T[i].first != c) {
		j = i;
		i = this->next[i];
	}
	if(i==-1)
		return NULL_TVALUE;
	else {
		bool over = false;
		auto el = empty;
		do {
			auto p = this->next[i];
			auto pp = i;
			while (p != -1 and this->h(this->T[p].first) != i) {
				pp = p;
				p = this->next[p];
			}
			if (p == -1)
				over = true;
			else {
				el = this->T[i];
				this->T[i] = this->T[p];
				j = pp;
				i = p;
			}
		} while (!over);
		if(el==empty)
			el = this->T[i];
		if (j != -1)
			this->next[j] = this->next[i];
		this->T[i] = empty;
		this->next[i] = -1;
		if (this->firstFree > i)
			this->firstFree = i;
		this -> s -= 1;
		return el.second;
	}
	//O(m)
}

int Map::size() const
{
	return this->s;
	//theta(1)
}

bool Map::isEmpty() const{
	return this->s==0;
	//theta(1)
}

MapIterator Map::iterator() const{
	return MapIterator(*this);
	//O(m)
}

int Map::h(TKey k){
	if (k < 0)
		k *= (-1);
	return k%this->m;
	//theta(1)
}

Map::~Map(){
	delete[] T;
	//delete[] next;
	//theta(1)
}
