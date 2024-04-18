#include <iostream>
#include <vector>
#include <chrono>
#include <list>
#include <random>

const int TABLE_SIZE = 10007;  // Use a prime number for better distribution

// Hash function for integer keys
int hashFunction(int key, int size) {
    return key % size;
}

// Secondary hash function for double hashing
int doubleHash(int key) {
    return 7 - (key % 7);
}

// 1. Internal Hashing with Linear Probing
class LinearProbingHashTable {
private:
    std::vector<int> table;
    int currentSize;

public:
    LinearProbingHashTable() : table(TABLE_SIZE, -1), currentSize(0) {}

    void insert(int key) {
        int index = hashFunction(key, TABLE_SIZE);
        while (table[index] != -1) {
            index = (index + 1) % TABLE_SIZE;
        }
        table[index] = key;
        currentSize++;
    }

    void remove(int key) {
        int index = hashFunction(key, TABLE_SIZE);
        while (table[index] != -1 && table[index] != key) {
            index = (index + 1) % TABLE_SIZE;
        }
        if (table[index] == key) {
            table[index] = -1;  // Mark as deleted
            currentSize--;
            // Rehash all elements in the cluster
            index = (index + 1) % TABLE_SIZE;
            while (table[index] != -1) {
                int rehashKey = table[index];
                table[index] = -1;
                currentSize--;
                insert(rehashKey);
                index = (index + 1) % TABLE_SIZE;
            }
        }
    }

    bool search(int key) {
        int index = hashFunction(key, TABLE_SIZE);
        while (table[index] != -1) {
            if (table[index] == key)
                return true;
            index = (index + 1) % TABLE_SIZE;
        }
        return false;
    }
};

// 2. Internal Hashing with Double Hashing
class DoubleHashingHashTable {
private:
    std::vector<int> table;
    int currentSize;

public:
    DoubleHashingHashTable() : table(TABLE_SIZE, -1), currentSize(0) {}

    void insert(int key) {
        int index = hashFunction(key, TABLE_SIZE);
        int stepSize = doubleHash(key);
        while (table[index] != -1) {
            index = (index + stepSize) % TABLE_SIZE;
        }
        table[index] = key;
        currentSize++;
    }

    void remove(int key) {
        int index = hashFunction(key, TABLE_SIZE);
        int stepSize = doubleHash(key);
        while (table[index] != -1 && table[index] != key) {
            index = (index + stepSize) % TABLE_SIZE;
        }
        if (table[index] == key) {
            table[index] = -1;  // Mark as deleted
            currentSize--;
        }
    }

    bool search(int key) {
        int index = hashFunction(key, TABLE_SIZE);
        int stepSize = doubleHash(key);
        while (table[index] != -1) {
            if (table[index] == key)
                return true;
            index = (index + stepSize) % TABLE_SIZE;
        }
        return false;
    }
};

// 3. External Hashing (Separate Chaining)
class SeparateChainingHashTable {
private:
    std::vector<std::list<int>> table;

public:
    SeparateChainingHashTable() : table(TABLE_SIZE) {}

    void insert(int key) {
        int index = hashFunction(key, TABLE_SIZE);
        table[index].push_back(key);
    }

    void remove(int key) {
        int index = hashFunction(key, TABLE_SIZE);
        table[index].remove(key);
    }

    bool search(int key) {
        int index = hashFunction(key, TABLE_SIZE);
        for (int element : table[index]) {
            if (element == key)
                return true;
        }
        return false;
    }
};

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000000);

    LinearProbingHashTable lpht;
    DoubleHashingHashTable dhht;
    SeparateChainingHashTable scht;

    const int maxItems = TABLE_SIZE * 0.9;  // Up to 90% capacity
    std::vector<int> keys;

    for (int i = 1; i <= 9; i++) {
        int numItems = TABLE_SIZE * i / 10;  // 10%, 20%, ..., 90%
        while (keys.size() < numItems) {
            int key = dis(gen);
            if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
                keys.push_back(key);
                lpht.insert(key);
                dhht.insert(key);
                scht.insert(key);
            }
        }
        // TODO: сделать замеры времени поиска отдельно для каждой таблицы
        // Timing successful search
        auto start = std::chrono::high_resolution_clock::now();
        for (int key : keys) {
            //lpht.search(key);
            //dhht.search(key);
            scht.search(key);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << i * 10 << "% full: Search time = " << elapsed.count() * 1000 << " ms.\n";

        // Timing unsuccessful search
        std::vector<int> fakeKeys;
        while (fakeKeys.size() < numItems) {
            int fakeKey = dis(gen) + 1000001;  // Ensure these keys are not in the table
            if (std::find(fakeKeys.begin(), fakeKeys.end(), fakeKey) == fakeKeys.end()) {
                fakeKeys.push_back(fakeKey);
            }
        }

        // TODO: тут тоже отдельно для каждого измерять
        start = std::chrono::high_resolution_clock::now();
        for (int fakeKey : fakeKeys) {
            //lpht.search(fakeKey);
            dhht.search(fakeKey);
            //scht.search(fakeKey);
        }
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << i * 10 << "% full: Unsuccessful search time = " << elapsed.count() << " seconds.\n";
    }

    // Testing removal
    for (int key : keys) {
        lpht.remove(key);
        dhht.remove(key);
        scht.remove(key);
    }
    bool removalCorrect = true;
    for (int key : keys) {
        if (lpht.search(key) || dhht.search(key) || scht.search(key)) {
            removalCorrect = false;
            break;
        }
    }
    std::cout << "Removal correctness: " << (removalCorrect ? "Passed" : "Failed") << std::endl;

    return 0;
}