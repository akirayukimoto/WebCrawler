
//
// CS251 Data Structures
// Hash Table
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Each hash entry stores a key, object pair
template <typename Data>
struct HashTableTemplateEntry {
  const char * _key;
  Data _data;
  HashTableTemplateEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTableTemplate {
 public:
  // Number of buckets
  enum { TableSize = 2039};
  
  // Array of the hash buckets.
  HashTableTemplateEntry<Data> **_buckets;
  
  // Obtain the hash code of a key
  int hash(const char * key);
  
 public:
  HashTableTemplate();
  
  ~HashTableTemplate();
  // Add a record to the hash table. Returns true if key already exists.
  // Substitute content if key already exists.
  bool insertItem( const char * key, Data data);

  // Find a key in the dictionary and place in "data" the corresponding record
  // Returns false if key is does not exist
  bool find( const char * key, Data * data);

  // Removes an element in the hash table. Return false if key does not exist.
  bool removeElement(const char * key);

  // Returns the data that corresponds to this index.
  // Data is initialized to 0s if the entry does not exist
  Data operator[] (const char * &key);
};

template <typename Data>
int HashTableTemplate<Data>::hash(const char * key)
{
  int h = 0;
  const char * p = key;
  while (*p) {
    h += *p;
    p++;
  }
  return h % TableSize;
}

template <typename Data>
HashTableTemplate<Data>::HashTableTemplate()
{
  // Add implementation here
  	_buckets = new HashTableTemplateEntry<Data>*[TableSize];	
	for (int i = 0; i < TableSize; i++) {
		_buckets[i] = NULL;
	}
}

template <typename Data>
HashTableTemplate<Data>::~HashTableTemplate()
{
	HashTableTemplateEntry<Data> *curr;
	//HashTableTemplateEntry<Data> *next;
	for (int i = 0; i < TableSize; i++) {
		curr = _buckets[i];
		while (curr) {
			//next = curr->_next;
			HashTableTemplateEntry<Data> *temp = curr;
			curr = curr->_next;
			free((void *)temp->_key);
			delete temp;
		}
		_buckets[i] = NULL;
	}
	delete _buckets;
}

template <typename Data>
bool HashTableTemplate<Data>::insertItem( const char * key, Data data)
{
  // Add implementation here
  int h = hash(key);
  HashTableTemplateEntry<Data> *entry = _buckets[h];
  while (entry != NULL) {
  	if (!strcmp(key, entry->_key)) {
		entry->_data = data;
		return true;
	}
	entry = entry->_next;
  }
  HashTableTemplateEntry<Data> *e = new HashTableTemplateEntry<Data>;
  e->_key = strdup(key);
  e->_data = data;
  if (_buckets[h] == NULL) e->_next = NULL;
  else e->_next = _buckets[h];
  _buckets[h] = e;
  return false;
}

template <typename Data>
bool HashTableTemplate<Data>::find( const char * key, Data * data)
{
  // Add implementation here
	int h = hash(key);
	HashTableTemplateEntry<Data> * entry = _buckets[h];
	while (entry != NULL) {
		if (!strcmp(key, entry->_key)) {
			*data = entry->_data;
			return true;
		}
		entry = entry->_next;
	}
	return false;
}

template <typename Data>
Data HashTableTemplate<Data>::operator[] (const char * &key) {
  //Data d;
  //return d;
	Data d;
	bool answer = find(key, &d);
	if (answer) return d;
	else return NULL;
}

template <typename Data>
bool HashTableTemplate<Data>::removeElement(const char * key)
{
  // Add implementation here
	int h = hash(key);
	HashTableTemplateEntry<Data> *entry = _buckets[h];
	HashTableTemplateEntry<Data> *prev = NULL;
	while (entry != NULL) {
		if (!strcmp(key, entry->_key)) {
			if (prev != NULL) prev = entry->_next;
			else _buckets[h] = entry->_next;
			free((void *)entry->_key);
			delete entry;
			return true;
		}
		prev = entry;
		entry = entry->_next;
	}
	return false;
}

template <typename Data>
class HashTableTemplateIterator {
  int _currentBucket;
  HashTableTemplateEntry<Data> *_currentEntry;
  HashTableTemplate<Data> * _hashTable;
 public:
  HashTableTemplateIterator(HashTableTemplate<Data> * hashTable);
  bool next(const char * & key, Data & data);
};

template <typename Data>
HashTableTemplateIterator<Data>::HashTableTemplateIterator(HashTableTemplate<Data> * hashTable)
{
  // Add implementation here
	_currentBucket = 0;
	_currentEntry = NULL;
	_hashTable = hashTable;
}

template <typename Data>
bool HashTableTemplateIterator<Data>::next(const char * & key, Data & data)
{
  // Add implementation here
	if (_currentEntry != NULL) {
		if (_currentEntry->_next != NULL) {
			key = _currentEntry->_next->_key;
			data = _currentEntry->_next->_data;
			_currentEntry = _currentEntry->_next;
			return true;
		}
	}
	int i = _currentBucket;
	int tableSize = 2039;
	i++;
	while (_hashTable->_buckets[i] == NULL) i++;
	if (_hashTable->_buckets[i] != NULL && i < tableSize) {
		_currentEntry = _hashTable->_buckets[i];
		key = _hashTable->_buckets[i]->_key;
		data = _hashTable->_buckets[i]->_data;
		_currentBucket = i;
		return true;
	}
	return false;

}
