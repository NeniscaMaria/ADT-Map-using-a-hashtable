#include "MapIterator.h"
#include <stdexcept>

MapIterator::MapIterator(const Map& ref) : c(ref) {
	//find the first non-empty position
	int current = 0;
	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	while (this->c.T[current] == empty and this->position<this->c.m)
		current++;
	this->position = current;
	this->elementsIterated = 0;
	//O(m)
}

void MapIterator::first(){
	this->position = 0;
	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	while (this->c.T[this->position] == empty and this->position < this->c.m)
		this->position++;
	this->elementsIterated = 0;
	//O(m)
}

void MapIterator::next(){
	if (!this->valid())
		throw std::invalid_argument("");
 	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	this->position++;
	while (this->c.T[this->position] == empty and this->position<this->c.m)
		this->position++;
	this->elementsIterated++;
	//O(m)
}

bool MapIterator::valid() const
{
	return this->position<this->c.m;
	//theta(1)
}

TElem MapIterator::getCurrent() const
{
	if (!this->valid())
		throw std::invalid_argument("");
	return this->c.T[this->position];
	//theta(1)
}

void MapIterator::jumpForward(int k)
{
	if (k <= 0)
		throw std::invalid_argument("");
	if (this->valid() == false)
		throw std::invalid_argument("");
	TElem empty;
	empty.first = INT_MAX;
	empty.second = INT_MAX;
	if (this->c.s - this->elementsIterated < k)
		this->position = this->c.m; //making the iterator invalid
	for (int i = 0; i < k; i++){
		this->position++;
		while (this->c.T[this->position] == empty)
			this->position++;
		this->elementsIterated++;
	}
	//O(m)
}
