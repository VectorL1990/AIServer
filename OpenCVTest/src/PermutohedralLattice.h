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
public:
    PermutohedralLattice(int pd, int vd, int n) :
        d(pd), vd(vd), n(n), hashTable(pd, vd) {

        // Allocate storage for various arrays
        elevated.resize(d+1);
        scaleFactor.resize(d);
        greedy.resize(d+1);
        rank.resize(d+1);
        barycentric.resize(d+2);
        canonical.resize((d+1)*(d+1));
        key.resize(d+1);
        replay.resize(n*(d+1));
        nReplay = 0;

        // compute the coordinates of the canonical simplex, in which
        // the difference between a contained point and the zero
        // remainder vertex is always in ascending order.
        for (int i = 0; i <= d; i++) {
            for (int j = 0; j <= d-i; j++) {
                canonical[i*(d+1)+j] = i;
            }
            for (int j = d-i+1; j <= d; j++) {
                canonical[i*(d+1)+j] = i - (d+1);
            }
        }

        // Compute part of the rotation matrix E that elevates a
        // position vector into the hyperplane
        for (int i = 0; i < d; i++) {
            // the diagonal entries for normalization
            scaleFactor[i] = 1.0f/(sqrtf((float)(i+1)*(i+2)));

            // We presume that the user would like to do a Gaussian
            // blur of standard deviation 1 in each dimension (or a
            // total variance of d, summed over dimensions.) Because
            // the total variance of the blur performed by this
            // algorithm is not d, we must scale the space to offset
            // this.
            //
            // The total variance of the algorithm is:
            // [variance of splatting] +
            // [variance of blurring] +
            // [variance of splatting]
            // = d(d+1)(d+1)/12 + d(d+1)(d+1)/2 + d(d+1)(d+1)/12
            // = 2d(d+1)(d+1)/3.
            //
            // So we need to scale the space by (d+1)sqrt(2/3).

            scaleFactor[i] *= (d+1)*sqrtf(2.0/3);
        }
    }

private:
    int d, vd, n;
    vector<float> elevated, scaleFactor, barycentric;
    vector<short> canonical, key, greedy;
    vector<char> rank;

    struct ReplayEntry {
        int offset;
        float weight;
    };
    vector<ReplayEntry> replay;
    int nReplay;

    HashTablePermutohedral hashTable;
};