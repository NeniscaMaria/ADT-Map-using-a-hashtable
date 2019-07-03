#pragma once
#include "Map.h"

class MapIterator {

private:

	friend class Map;
	MapIterator(const Map& ref);
	const Map& c;
	/* representation specific for the iterator*/
	int position;
	int elementsIterated;

public:
	//sets the iterator to the first element of the container
	void first();

	//moves the iterator to the next element
	//throws exception if the iterator is not valid
	void next();

	//checks if the iterator is valid
	bool valid() const;

	//returns the value of the current element from the iterator
	// throws exception if the iterator is not valid
	TElem getCurrent() const;

	void jumpForward(int k);
};


