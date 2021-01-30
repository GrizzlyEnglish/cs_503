#include <iostream>
#include <cstddef>

using std::cout;
using std::endl;

/**
 * HashEntry for storing key value pairs
 */
class HashEntry
{
private:
    // The key for the entry
    int key;
    // The value for the entry
    int value;
public:

    /**
     * Inits a new hash entry with the given key and value
     */
    HashEntry(int key, int value)
    {
        this->key = key;
        this->value = value;
    }

    ~HashEntry()
    {
    }

    /**
     * Updates the key, for rehashing
     */
    void setKey(int k){
        this->key = k;
    }

    /**
     * Get the key
     */
    int getKey()
    {
        return key;
    }

    /**
     * Get the value
     */
    int getValue()
    {
        return value;
    }
};

/**
 * Hashmap stores key value pairs based on a hash function
 * that locates the position of the value in the array.
 */
class HashMap
{
private:
    // The map itself
    HashEntry **table;
    // The size of the map
    int TABLE_SIZE;

    /**
     * Helper function that passes the table
     */
    int hash(int x){
        return hash(x, table);
    }

    /**
     * The hash function. We will probe just once if a collision
     * is deteced.
     */
    int hash(int x, HashEntry** entryTable)
    {
        int hash = x % TABLE_SIZE;
        if (entryTable[hash] != NULL)
        {
            cout << "Collision at ";
            cout << hash;
            cout << " with x of ";
            cout << x << endl;
            hash += 1;
        }
        return hash;
    }

    /**
     * Rehash the given table. For the sake of simplicity
     * we just double the size and add 3 for a more odd prime-ish
     * number. This will remap the entries into their new locations
     */
    void rehash()
    {
        cout << "Rehashing Table previous size ";
        cout << TABLE_SIZE;

        int prevSize = TABLE_SIZE;

        TABLE_SIZE = TABLE_SIZE * 2 + 3;
        HashEntry** newTable = new HashEntry *[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            newTable[i] = NULL;
        }

        for (int i = 0; i < prevSize; i++) 
        {
            if (table[i] != NULL) {
                int hash = this->hash(table[i]->getValue(), newTable);
                table[i]->setKey(hash);
                newTable[hash] = table[i];
            }
        }

        delete table;
        table = newTable;

        cout << " New table size ";
        cout << TABLE_SIZE << endl;
    }
public:

    /**
     * Inits a hashmap with the given size,
     * setting all entries as null
     */
    HashMap(int size)
    {
        TABLE_SIZE = size;
        table = new HashEntry *[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            table[i] = NULL;
        }
    }

    /**
     * Gets a value give the key
     */
    int get(int key)
    {
        int hash = this->hash(key);
        if (table[hash] == NULL)
            return -1;
        else
            return table[hash]->getValue();
    }

    /**
     * Inserts a value into the hashmap. This will
     * linear probe once, and then rehash if collision
     * happens both times 
     */
    void insert(int value)
    {
        int hash = this->hash(value);

        if (table[hash] != NULL)
        {
            this->rehash();
            hash = this->hash(value);
        }

        HashEntry *entry = new HashEntry(hash, value);
        table[hash] = entry;
    }

    /**
     * Prints out the hasha map with key value pairs
     * in order to show its contents. It finishes
     * the line with a count of elements
     */
    void print()
    {
        int emptyCells = 0;
        cout << "Cell    Value" << endl;
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            HashEntry *entry = table[i];
            cout << i;
            if (entry == NULL)
            {
                emptyCells++;
                cout << "    NULL" << endl;
            }
            else
            {
                cout << "    ";
                cout << entry->getValue() << endl;
            }
        }

        cout << "Has ";
        cout << TABLE_SIZE - emptyCells;
        cout << " elements in hash map" << endl;
    }

    ~HashMap()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
            if (table[i] != NULL)
                delete table[i];
        delete[] table;
    }
};

/**
 * Ryan English
 * Homework 4
 * 
 * Create a hashmap of size 17 and add 12 elements
 * Then create a hashmap of size 7 and add 12 elements, this
 * will have a lot of collision, thus we will rehash the table
 * to fit the 12 elements
 */
int main(int argc, const char *argv[])
{
    // Problem one insert ints with table size 17
    cout << "Problem One" << endl;
    int arr[] = {121, 81, 16, 100, 25, 0, 1, 9, 4, 36, 64, 49};

    HashMap *map = new HashMap(17);

    for (int i = 0; i < 12; i++)
    {
        map->insert(arr[i]);
    }

    map->print();

    // Problem two insert with table size 7
    cout << "Problem Two" << endl;
    map = new HashMap(7);

    for (int i = 0; i < 12; i++)
    {
        map->insert(arr[i]);
    }

    map->print();
}