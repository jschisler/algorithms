#include <string>
#include <vector>
#include <list>
#include <functional>
#include <set>
#include <unordered_map>

struct Item { 
	std::string key; 
	int value; 
};

size_t hash_function(const std::string&); 
typedef std::vector<Item*> Hashtable;
typedef std::vector<std::list<Item*>> Hashtable2;

/*
	Notice that the hash function above is defined to take a key type instead 
	of an Item. This is because we want to use the same hash function for both 
	insertion and retrieval. Also, we take the liberty of storing pointers 
	instead of values in the hash table. While it is true that Item is plain 
	old data and simple to copy, hash tables often contain complex data 
	structures for which it is better to store references instead of copies.
*/

/*
	The insert operation for a hash table requires calculating hash values and 
	addressing collisions if they occur. Collision resolution is discussed in 
	another section below. This listing provides an incomplete hash table 
	implementation that does not guarantee retrieval of stored elements. 
	Instead we evict older entries when collision is detected. Such a scheme is 
	useful in cache implementations, but not as useful for using hash tables 
	for histograms.
	
	Note that in order to convert the hash value to a valid index, we mod out 
	the hash value by the size of the vector.
*/
void insert(Hashtable* table, Item* entry) { 
	auto index = hash_function(entry->key) % table->size(); 
	(*table)[index] = entry; 
}

/*
	Implementing the find operation in a hash table depends on the hash value 
	of a key and the collision resolution scheme. As multiple keys can hash to 
	the same value, it is important that retrieval determine if an entry 
	agrees with the key used in retrieval. For that reason there is an 
	equality check after retrieval by index.
*/
Item* find(Hashtable* table, const std::string& key) { 
	auto index = hash_function(key) % table->size(); 
	
	if ((*table)[index] != nullptr && key == (*table)[index]->key)
		return (*table)[index]; 
	
	return nullptr;
}

/*
	Commonly, a hash table exposes a third method for dynamic resizing. Dynamic 
	resizing refers to increasing the size of the underlying vector while 
	maintaining the key to hash value mapping of the table. There are two steps. 
	The first is allocation of a new buffer to store the elements. And the 
	second is a rehashing of the elements in the original table to the second.

	During dynamic resizing, a rehash is necessary. Because of this it is sometimes 
	beneficial to update the hash function at the same time. Such an update may 
	be as simple as changing an offset for the hash value calculation. 
	
	While this implementation provides a possibly lossy implementation of a hash 
	table, any of the collision resolution schemes below guarantee retrieval of 
	all elements inserted into the hash table.
*/
void resize(Hashtable* table, size_t size) { 
	Hashtable new_table(size); 
	
	for (auto entry : *table) { 
		if (entry != nullptr) { 
			auto index = hash_function(entry->key) % new_table.size(); 
			new_table[index] = entry; 
		}
	} 
	
	table->swap(new_table);
}

/*
	Open addressing is a scheme by which collisions are resolved by inserting 
	an element at the next available bucket in the hash table given some 
	iteration scheme. Commonly, open addressing involves simply iterating through 
	the table. This is called open addressing with linear probing. However, open 
	addressing can be implemented by other probing schemes such as successively 
	doubling the index value. If no available bucket is found, the table is 
	dynamically resized and the insert resumes. Note that after resizing, we are 
	guaranteed to find an available bucket. Dynamically resizing and a possible 
	linear time enumeration are the cost the hash table pays for guaranteeing 
	retrieval. 
	
	We implement a hash table supporting open addressing with linear probing. 
	Insertion is only a matter of finding an open bucket.
*/

void insert(Hashtable* table, Item* entry) {
	while (true) {
		auto index = hash_function(entry->key) % table->size();

		for (size_t offset = 0; offset < table->size(); ++offset) {
			size_t bucket_index = (index + offset) % table->size();

			if ((*table)[bucket_index] == nullptr) {
				(*table)[bucket_index] = entry;
				return;
			}
		}

		resize(table, table->size() * 2 + 1);
	}
}

/*
	To ensure that a false negative is not returned on lookup, an open addressing 
	scheme must iterate through all successive buckets. Only this makes sure 
	that an entry is not present at successive locations. Caution must be 
	exercised to not loop continuously.
*/
Item* find(Hashtable* table, const std::string& key) { 
	size_t index = hash_function(key) % table->size(); 
	
	for (size_t offset = 0; offset < table->size(); ++offset) { 
		auto bucket_index = (index + offset) % table->size(); 
		auto& item = (*table)[bucket_index]; 
		
		if (item == nullptr) 
			return nullptr; 
		
		if (key == item->key) 
			return item;
	} 
	
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
//  Hashtable2
//

/*
	When an element is inserted into a hash table with separate chaining, it is 
	simply pushed onto the list stored at the bucket. This provides constant 
	time insertion. It also guarantees that an item can be retrieved once it 
	has been inserted as there is no eviction.
*/
void insert(Hashtable2* table, Item* entry) {
	auto index = hash_function(entry->key) % table->size(); 
	(*table)[index].push_back(entry);
}

/*
	Retrieval from separate chaining requires enumerating the lists to 
	complete the search for an entry. As above, we must ensure the element’s 
	key matches the parameter. That this is requirement is clear, as with 
	separate chaining it is common for the table to have multiple entries with 
	the same hash value.
*/
Item* find(Hashtable2* table, const std::string& key) {
	int index = hash_function(key) % table->size(); 
	
	for (auto entry : (*table)[index]) {
		if (key == entry->key) 
			return entry;
	} 
	
	return nullptr;
}

/*
	Cuckoo hashing is a modern approach to collision resolution that provides 
	expected constant time lookup and insertion when the load of the table is 
	low. Instead of linear probing, cuckoo hashing uses a combination of two 
	hash functions and two tables to find the next available bucket for a 
	value. The hash functions and tables are labeled primary and secondary, 
	corresponding to the order they are probed. 
	
	To realize expected constant time operations, cuckoo hashing requires a 
	more complex definition for the hash table. We use a structure to group 
	the members.
*/
struct CuckooHashTable { 
	typedef std::function<size_t(const std::string&)> HashFunction; 
	typedef HashFunction CuckooHashFunctions[2]; 
	typedef Hashtable CuckooHashTables[2]; 
	CuckooHashFunctions hash_functions; 
	CuckooHashTables hash_tables; 
	
	explicit CuckooHashTable(size_t size); 
	void insert(Item* entry); 
	void resize(size_t size); 
	Item* find(const std::string&); 
};

/*
	Insertions begin by checking the first table. If the hash value bucket is 
	occupied, its contents are replaced with the new entry and the old entry is 
	promoted to the second table. This process repeats until all entries are 
	added or else an infinite loop is detected. In the later case, the table is 
	resized and the process continues. In the code below, we use table_index to 
	track if we are operating on the primary or secondary table. The set is 
	used to detect if a loop is encountered. Also we use references to make the 
	code easier to read.

	The resize is necessary if an infinite loop is detected. With a poor choice 
	of hash function, it is possible for the insert-resize process to also loop. 
	To insure against this it is advisable to update the hash function during 
	resize. Other than that, the resize member is straightforward.
*/
void CuckooHashTable::insert(Item* entry) {
	std::set < size_t > visited[2]; 
	size_t table_index = 0; 
	size_t index; 
	
	do {
		auto& table = hash_tables[table_index]; 
		auto hashfn = hash_functions[table_index]; 
		index = hashfn(entry->key) % table.size(); 
		
		if (table[index] == nullptr) {
			table[index] = entry;
			return;
		} 
		
		std::swap(entry, table[index]); 
		visited[table_index].insert(index); 
		table_index = (table_index == 0) ? 1 : 0;
	} 
	
	while (visited[table_index].end() == visited[table_index].find(index)); 
	
	resize(hash_tables[0].size() * 2 + 1);
	insert(entry);
}

/*
	Lookup in a hash table with cuckoo hashing must determine if the key is in 
	the first or second table. The later check is only needed when the hash 
	table bucket in the first table is occupied by an element not equal to the 
	value seen.
*/
Item* CuckooHashTable::find(const std::string& key) {
	for (size_t table_index = 0; table_index < 2; ++table_index) { 
		auto& table = hash_tables[table_index]; 
		auto hashfn = hash_functions[table_index]; 
		auto index = hashfn(key) % table.size(); 
		
		if (table[index] == nullptr) 
			return nullptr; 
		
		if (table[index]->key == key) 
			return table[index]; 
	} 
	
	return nullptr;
}

/*
	Consider the problem of finding the most common element in large list with 
	many distinct elements. 
	
	To solve this problem, we want to build a histogram of the count of each 
	distinct element. To construct this histogram we will require a container 
	to store an entry that is a tuple of the element and a counter. The 
	element is the key of this entry, and the counter is the entry’s value. 
	
	When elements are first encountered, we add an entry with count 1. 
	Otherwise we increment the counter associated with that element. So clearly,
	for each element in the list we need to support both insertion and lookup. 
	In a binary tree, each lookup can require order log n comparison, and each 
	insert can require order log n comparisons. With an efficient hash table we 
	can reduce each lookup and insertion to expected constant time and solve the 
	problem with a linear scan. 
	
	After scanning the input, the hash table has a count of the frequency of each 
	element in the list. We can keep track of the current most common element during 
	iteration. If the k most common elements are desired, we can use a bounded size 
	min heap as in the previous section.
*/
std::string find_most_common_element(std::istream& stream, std::unordered_map < std::string, size_t > * hash_table) {
	std::string str; 
	
	while (!stream.eof() && stream >> str) { 
		(*hash_table)[str] += 1; 
	} 
	
	std::string max_str; 
	size_t max_count = 0; 
	
	for (auto& entry : * hash_table) {
		if (entry.second > max_count) {
			max_str = entry.first; max_count = entry.second;
		}
	} 
	
	return max_str;
}