#include <string>
#include <list>
#include <unordered_map>

/*
A cache is a data structure used to reduce retrieval time of data from a remote
store. A subset of elements is stored locally, and retrieval first looks
locally before looking remotely. Caches are bounded and store only a subset of
the data available from the remote store. For this reason they must implement
an eviction policy. A most recently used cache is one that evicts the least
recently used element. In this subsection, we solve the problem of implementing
a MRU cache.

The MRU cache must efficiently support both retrieval and priority updating. To
implement this data type we use a combination of two containers we have
already visited. We use the unordered_map implementation of a hash table for
lookup, and a doubly linked-list to maintain priority.

To facilitate retrieval in the linked-list, the hash table used for retrieval
stores the address of the nodes of the linked-list. The linked-list stores the
priority of each entry in the cache by the order of the nodes. The head of the
linked-list always contains the most recently accessed item, and the tail will
be the least recently accessed item. In order to maintain correctness, the
cache must be updated by the calling accessed whenever an element is used.
*/
struct MruCache {
	typedef std::string key_type; 
	typedef int value_type; 
	typedef std::pair<key_type, value_type> Item; 
	typedef std::list<const Item*> List; 
	List list; 
	typedef std::unordered_map<key_type, List::iterator> Hashtable;
	Hashtable table;

	const size_t bound; 
	MruCache(size_t bound); 
	void store(const Item* item); 
	const Item* find(const key_type& key);
};

/*
	The find method uses the hash table for quick retrieval of items from the 
	cache. When find retrieves an item that item is accessed and the priority 
	must be updated. To update the priority without allocating new memory, we 
	move the location of item’s node within the linked-list to the head. This 
	is done with the splice method.
*/
const MruCache::Item* MruCache::find(const key_type& key) { 
	auto iter = table.find(key); 
	
	if (table.end() != iter) { 
		list.splice(list.begin(), list, iter->second); 
		return *(iter->second); 
	} 
	
	return nullptr; 
}

/*
	When local retrieval fails, a method must call the remote store. The item 
	retrieved is then saved to the cache by the store member function. It is 
	within store that the eviction policy is implemented. The item stored is 
	prepended to the front of the list, and the hash table is updated with the 
	item’s key value. If the cache is above capacity after the item is added, 
	the least recently used item is removed. This item is found at the tail of 
	the list. When removed from the priority queue, the item must also be 
	removed from the hash table.
*/
void MruCache::store(const Item* item) {
	list.push_front(item); 
	table[item->first] = list.begin(); 
	
	if (list.size() > bound) { 
		table.erase(list.back()->first); 
		list.pop_back(); 
	}
}