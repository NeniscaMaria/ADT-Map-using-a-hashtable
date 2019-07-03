//ADT Map – using a hashtable with coalesced chaining
#include "ExtendedTest.h"
#include "ShortTest.h"
#include "Map.h"
#include "MapIterator.h"
#include <assert.h>
#include <iostream>
using namespace std;

int main() {

	testAll();
	testAllExtended();
	//testing jumpForward
	Map m;
	MapIterator it1 = m.iterator();
	m.add(1, 2);
	m.add(2, 3);
	m.add(3, 4);
	m.add(4, 5);
	m.add(5, 6);
	MapIterator it = m.iterator();
	it.jumpForward(1);
	assert(it.getCurrent().first == 2);
	assert(it.getCurrent().second == 3);
	it.jumpForward(3);
	assert(it.getCurrent().first == 5);
	assert(it.getCurrent().second == 6); 
	try {
		it.jumpForward(-1);
		assert(false);
	}
	catch (exception) {
		assert(true);
	}
	return 0;
}