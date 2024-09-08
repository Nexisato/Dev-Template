#include <iostream>
#include <vector>
#include <cassert>
using namespace std;


class MemoryPool {
private:
    char* pool;
    size_t* free_list;
    size_t next_free;
    size_t block_size;
    size_t num_blocks;
public:
    MemoryPool(size_t block_size, size_t num_blocks): block_size(block_size), num_blocks(num_blocks) {
        pool = new char[block_size * num_blocks];
        free_list = new size_t[num_blocks];
        for (int i = 0; i < num_blocks; ++i) {
            free_list[i] = i;
        }
        next_free = 0;
    }
    ~MemoryPool() {
        delete []free_list;
        delete []pool;
    }

    void *allocate() {
        if (next_free >= num_blocks) {
            return nullptr;
        }
        size_t block_idx = free_list[next_free++];
        return pool + (block_idx * block_size);

    }

    void deallocate(void* ptr) {
        if (ptr == nullptr) return;
        char* char_ptr = static_cast<char*>(ptr);
        assert(char_ptr >= pool && char_ptr < (pool + (num_blocks * block_size)));
        size_t block_idx = (char_ptr - pool) / (block_size);
        assert(block_idx < num_blocks);
        free_list[--next_free] = block_idx;
    }


};



int main() {




    return 0;
}