// Description: An incomplete implementation of iterators for a
//              RingQueue class.
//
// Notes: The project DOES compile but there is no meaningful 
//              output
//
// Your job: To complete this set of classes so that the output
//              produced by 'main' (below), matches the sample
//              file provided at the end of this file.


#include <iostream>

// Forward declaration
template <typename ItemType, int MAX_SIZE>
class RingQueue;



template <typename ItemType, int MAX_SIZE>
class RingQueue {

	// Nested Forward declaration of RingQueue<ItemType,MAX_SIZE>::iterator
	// This is needed if one plans to turn this home-made iterator into
	// one of the special categories of iterators (e.g., input, output,
	// forward, etc.).
public:
	class iterator;



	// Aliases. 
	typedef ItemType* pointer;
	typedef ItemType& reference;



	// Definition of RingQueue<ItemType,MAX_SIZE>::iterator
public:
	class iterator {
	private:
		// A link to the parent container 
		RingQueue * parent;

		// The position within the RingQueue is determined
		// by how far ahead we are from the begining of the
		// queue.
		int offset;

	private:  // Private constructor???
		iterator(RingQueue* _parent, int _offset = 0)
			: parent(_parent), offset(_offset) { }


		// It is quite common for Containers and their iterators
		// to be friends. After all, they should work closely together.
		friend class RingQueue<ItemType, MAX_SIZE>;


	public:
		reference operator*() {
			return parent->buffer[(parent->begin_index + offset) % MAX_SIZE];
		}

		iterator& operator++() {
			++offset;
			return *this;
		}

		iterator operator++(int unused) {
			iterator copy(*this);
			++offset;
			return copy;
		}

		bool operator==(const iterator& rhs) const {
			return (offset == rhs.offset) && (parent == rhs.parent);
		}

		bool operator!=(const iterator& rhs) const {
			return !(offset == rhs.offset) && (parent == rhs.parent);
		}

	};



	class const_iterator{
	private:
	RingQueue* parent;
	int offset;

	private:
		// Only RingQueue objects can create const_iterators...
		const_iterator(RingQueue* _parent, int _offset = 0) : parent(_parent), offset(_offset) { }
		friend class RingQueue<ItemType, MAX_SIZE>;


	public:
	// ... however, const_iterators can be 'copied'.
		const_iterator(const const_iterator&) : parent(const_iterator.parent), offset(const_iterator.offset) {}
		friend class RingQueue<ItemType,MAX_SIZE>;


	public:
		const reference operator*() const {
			return parent->buffer[(parent->begin_index + offset) % MAX_SIZE];
		}

		const_iterator& operator++() {
			++offset;
			return *this;
		}

		const_iterator operator++(int unused) {
			iterator copy(*this);
			++offset;
			return copy;
		}

		bool operator==(const const_iterator& rhs) const {
			return (offset == rhs.offset) && (parent == rhs.parent);
		}

		bool operator!=(const const_iterator& rhs) const {
			return !(offset == rhs.offset) && (parent == rhs.parent);
		}
	};






	// Friendship goes both ways here.
	friend class iterator;
	friend class const_iterator;



private:
	// A fixed-size static array with constant capacity that represents 
	// the RingQueue
	ItemType buffer[MAX_SIZE];

	// The starting index. It changes according to a very 
	// specific set of rules (below).
	int begin_index;

	// The actual size of the RingQueue. Not to be confused with
	// its capacity. 
	int ring_size;



	// A helper function that computes the index of 'the end'
	// of the RingQueue
	int end_index() const {
		return (begin_index + ring_size) % MAX_SIZE;
	}



public:
	// Constructor
	RingQueue() : begin_index(0), ring_size(0) { }

	// Accessors. Note: 'back()' is not considered part of the array.
	ItemType front() const {
		if (ring_size == 0) std::cerr << "Warning: Empty ring!\n";
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		// Feel free to throw instead...


		return buffer[begin_index];
	}
	ItemType back() const {
		if (ring_size == 0) std::cerr << "Warning: Empty ring!\n";
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		// Feel free to throw instead...


		return buffer[end_index()-1];
	}



	// Mutators
	void push_back(const ItemType& value) {
		buffer[end_index()] = value;
		if (ring_size==MAX_SIZE) {
			++begin_index;
		}
		else {
			++ring_size;
		}
		return;
	}
	void pop_front() {
		++begin_index;
		--ring_size;
		return;
	}

	// Functions that return iterators
	iterator begin() {
		return iterator(this, 0);
	}
	iterator end() {
		return iterator(this, ring_size);
	}

	// Functions that return const_iterators
	const_iterator begin() const {
		return const_iterator(this, 0);
	}
	const_iterator end() const {
		return const_iterator(this, ring_size);
	}

	// Miscellaneous functions
	size_t size() const {
		return ring_size;
	}

	// Debugging functions
	void dump_queue() const {
		std::cout << "Raw queue...\n";
		for (size_t i = 0; i < MAX_SIZE; ++i)
			std::cout << "Val: " << buffer[i] << ", at: " << buffer + i << '\n';
		std::cout << '\n';
		return;
	}

};

int main() {
	RingQueue<int, 7> rq;
	rq.dump_queue();

	for (int i = 0; i < 8; ++i) {
		rq.push_back(i + 1);
	}
	rq.dump_queue();
	rq.pop_front();

	std::cout << "Queue via size: \n";

	// RingQueue<int,7>::iterator it = rq.begin() ; 
	auto it = rq.begin();
	for (size_t i = 0; i < rq.size(); ++i) {
		std::cout << "Value: " << *it << ", address: " << &(*it) << '\n';
		++it;
	}
	std::cout << '\n';



	// Uncomment the block below only when you have a working 
	// implementation of RingQueue<ItemType,int>::end(). 
	// If the implementation is not correct, it might result in 
	// an infinite loop.
	std::cout << "Queue via iterators: \n";
	for (auto it = rq.begin(); it != rq.end(); ++it) {
		std::cout << "Value: " << *it << ", address: " << &(*it) << '\n';
	}
	std::cout << '\n';



	rq.dump_queue();

	return 0;
}



/**
+++++++++++++++++++++++++++++++++++++++++++++++
The output of your program [once the missing
code is added] should look more or less like
the output below.

Note:
It is dependent on the device where
this driver is executed. However, it
should be clear that the difference
between consecutive memory addresses
is equal to the number reported by
'size_of( int )'.
+++++++++++++++++++++++++++++++++++++++++++++++

Raw queue...
Val: 2, at: 0x7ffcdeeaab40
Val: 0, at: 0x7ffcdeeaab44
Val: 4198285, at: 0x7ffcdeeaab48
Val: 0, at: 0x7ffcdeeaab4c
Val: 0, at: 0x7ffcdeeaab50
Val: 0, at: 0x7ffcdeeaab54
Val: 0, at: 0x7ffcdeeaab58

Raw queue...
Val: 8, at: 0x7ffcdeeaab40
Val: 2, at: 0x7ffcdeeaab44
Val: 3, at: 0x7ffcdeeaab48
Val: 4, at: 0x7ffcdeeaab4c
Val: 5, at: 0x7ffcdeeaab50
Val: 6, at: 0x7ffcdeeaab54
Val: 7, at: 0x7ffcdeeaab58

Queue via size:
Value: 3, address: 0x7ffcdeeaab48
Value: 4, address: 0x7ffcdeeaab4c
Value: 5, address: 0x7ffcdeeaab50
Value: 6, address: 0x7ffcdeeaab54
Value: 7, address: 0x7ffcdeeaab58
Value: 8, address: 0x7ffcdeeaab40

Queue via iterators:
Value: 3, address: 0x7ffcdeeaab48
Value: 4, address: 0x7ffcdeeaab4c
Value: 5, address: 0x7ffcdeeaab50
Value: 6, address: 0x7ffcdeeaab54
Value: 7, address: 0x7ffcdeeaab58
Value: 8, address: 0x7ffcdeeaab40

Raw queue...
Val: 8, at: 0x7ffcdeeaab40
Val: 2, at: 0x7ffcdeeaab44
Val: 3, at: 0x7ffcdeeaab48
Val: 4, at: 0x7ffcdeeaab4c
Val: 5, at: 0x7ffcdeeaab50
Val: 6, at: 0x7ffcdeeaab54
Val: 7, at: 0x7ffcdeeaab58
*/
