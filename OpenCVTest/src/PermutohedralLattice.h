#include<opencv2/core/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>


#include<iostream>
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <math.h>
#include <vector>
#include <memory>
using std::vector;

class HashTablePermutohedral
{
public:
    HashTablePermutohedral(int kd, int vd) : kd(kd), vd(vd) {
        filled = 0;
        entries.resize(1 << 15);
        keys.resize(kd*entries.size()/2);
        values.resize(vd*entries.size()/2, 0.0f);
    }

    // Returns the number of vectors stored.
    int size() { return filled; }

    // Returns a pointer to the keys array.
    vector<short> &getKeys() { return keys; }

    // Returns a pointer to the values array.
    vector<float> &getValues() { return values; }

    // Looks up the value vector associated with a given key. May or
    // may not create a new entry if that key doesn’t exist.
    float *lookup(const vector<short> &key, bool create = true) {
        // Double hash table size if necessary
        if (create && filled >= entries.size()/2) { grow(); }

        // Hash the key
        size_t h = hash(&key[0]) % entries.size();

        // Find the entry with the given key
        while (1) {
            Entry e = entries[h];

            // Check if the cell is empty
            if (e.keyIdx == -1) {
                if (!create) return NULL;// Not found

                // Need to create an entry. Store the given key.
                for (int i = 0; i < kd; i++) {
                keys[filled*kd+i] = key[i];
                }
                e.keyIdx = filled*kd;
                e.valueIdx = filled*vd;
                entries[h] = e;
                filled++;
                return &values[e.valueIdx];
            }

            // check if the cell has a matching key
            bool match = true;

            for (int i = 0; i < kd && match; i++) {
                match = keys[e.keyIdx+i] == key[i];
            }
            if (match) {
                return &values[e.valueIdx];
            }

            // increment the bucket with wraparound
            h++;
            if (h == entries.size()) { h = 0; }
        }
    }

    // Hash function used in this implementation. A simple base conversion.
    size_t hash(const short *key) {
        size_t h = 0;
        for (int i = 0; i < kd; i++) {
            h += key[i];
            h *= 2531011;
        }
        return h;
    }
private:
    // Grows the hash table when it runs out of space
    void grow() {
        // Grow the arrays
        values.resize(vd*entries.size(), 0.0f);
        keys.resize(kd*entries.size());
        vector<Entry> newEntries(entries.size()*2);

        // Rehash all the entries
        for (size_t i = 0; i < entries.size(); i++) {
            if (entries[i].keyIdx == -1) { continue; }
            size_t h = hash(&keys[entries[i].keyIdx]) % newEntries.size();
            while (newEntries[h].keyIdx != -1) {
                h++;
                if (h == newEntries.size()) { h = 0; }
            }
            newEntries[h] = entries[i];
        }
        entries.swap(newEntries);
    }

    // Private struct for the hash table entries.
    struct Entry {
        Entry() : keyIdx(-1), valueIdx(-1) {}
        int keyIdx;
        int valueIdx;
    };

    vector<short> keys;
    vector<float> values;
    vector<Entry> entries;
    size_t filled;
    int kd, vd;
};

class PermutohedralLattice
{

};