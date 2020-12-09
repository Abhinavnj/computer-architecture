#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Line {
    int valid;
    int tag;
    int count;
} Line;

int getArg1(char* val);
char* getArg2(char* val);
char* getArg3(char* val);
int getArg4(char* val);
int setValues(char* associativity, int cache_size, int block_size, int* lines_per_set, int* num_of_sets, int* num_of_lines);
int readCache(long mem_address, long tag, long set, Line** cache, int rows, int cols);
void freeCache(Line** cache, int rows);
int writeLRU(long mem_address, long tag, long set, Line** cache, int rows, int cols);
int writeFIFO(long mem_address, long tag, long set, Line** cache, int rows, int cols, int* count);
int readLRU(long mem_address, long tag, long set, Line** cache, int rows, int cols);

int getArg1(char* val) {
    int cache_size = atoi(val);
    if (cache_size == 0) {
        return 0;
    }

    return cache_size;
}

char* getArg2(char* val) {
    char* associativity = val;
    if (strcmp(associativity, "direct") == 0 || strcmp(associativity, "assoc") == 0) {
        return associativity;
    }
    
    char first6[7];
    strncpy(first6, associativity, 6);
    first6[6] = '\0';
    char* lastNum = associativity + 6;
    if (strcmp(first6, "assoc:") == 0 && atoi(lastNum) != 0) {
        return associativity;
    }

    return "error";
}

char* getArg3(char* val) {
    if (strlen(val) > 4) {
        return "error";
    }

    char* replace_policy = val;
    if (strcmp(replace_policy, "lru") || strcmp(replace_policy, "fifo")) {
        return replace_policy;
    }

    return "error";
}

int getArg4(char* val) {
    int block_size = atoi(val);
    if (block_size != 0) {
        return block_size;
    }

    return 0;
}

int setValues(char* associativity, int cache_size, int block_size, int* lines_per_set, int* num_of_sets, int* num_of_lines) {
    *num_of_lines = cache_size / block_size;

    if (strcmp(associativity, "direct") == 0) {
        *lines_per_set = 1;
        *num_of_sets = *num_of_lines;
        return 0;
    }
    if (strcmp(associativity, "assoc") == 0) {
        *num_of_sets = 1;
        *lines_per_set = *num_of_lines;
        return 0;
    }

    char first6[7];
    strncpy(first6, associativity, 6);
    first6[6] = '\0';
    int nWay = atoi(associativity + 6);
    if (strcmp(first6, "assoc:") == 0 && nWay != 0) {
        *lines_per_set = nWay;
        *num_of_sets = *(num_of_lines) / *(lines_per_set);
    }

    return 0;
}

int readCache(long mem_address, long tag, long set, Line** cache, int rows, int cols) {
    for (int i = 0; i < cols; i++) {
        // printf("cache[%ld][%d].tag: %d\n", set, i, cache[set][i].tag);
        // printf("cache[%ld][%d].valid: %d\n", set, i, cache[set][i].valid);
        if (cache[set][i].tag == tag && cache[set][i].valid == 1) { // short-circuiting
            return 1;
        }
    }

    return 0;
}

void freeCache(Line** cache, int rows) {
    for (int i = 0; i < rows; i++) {
        free(cache[i]);
    }
    free(cache);
}

int readLRU(long mem_address, long tag, long set, Line** cache, int rows, int cols) {
    int index = -1;
    
    // get index where item is stored
    for (int i = 0; i < cols; i++) {
        if (cache[set][i].tag == tag && cache[set][i].valid == 1) { // short-circuiting
            index = i;
        }
    }

    // move everything before item to right one
    for (int i = index; i > 0; i--) {
        cache[set][i].tag = cache[set][i-1].tag; // move every tag over to the right one spot
    }
    cache[set][0].tag = tag;

    return 0;
}

int writeLRU(long mem_address, long tag, long set, Line** cache, int rows, int cols) {
    int index = -1;
    
    // Case 1: empty
    for (int i = 0; i < cols; i++) {
        if (cache[set][i].tag == 0) {
            index = i;
            break;
        }
    }

    if (index != -1) { // if empty spot is found
        cache[set][index].valid = 1; // make empty spot valid
        for (int i = index; i > 0; i--) {
            cache[set][i].tag = cache[set][i-1].tag; // move every tag over to the right one spot
        }
        cache[set][0].tag = tag;

        return 1;
    }

    // Case 2: filled
    for (int i = cols-1; i > 0; i--) {
        cache[set][i].tag = cache[set][i-1].tag; // move every tag over to the right one spot
    }
    cache[set][0].tag = tag;
    
    return 0;
}

int writeFIFO(long mem_address, long tag, long set, Line** cache, int rows, int cols, int* count) {
    (*count) += 1;
    for (int i = 0; i < cols; i++) {
        if (cache[set][i].tag == 0) {
            cache[set][i].valid = 1;
            cache[set][i].tag = tag;
            cache[set][i].count = *count;
            return 1;
        }
    }

    int smallest = cache[set][0].count;
    int smallestIndex = 0;
    for (int i = 1; i < cols; i++) {
        if (cache[set][i].count < smallest) {
            smallest = cache[set][i].count;
            smallestIndex = i;
        }
    }

    cache[set][smallestIndex].tag = tag;
    cache[set][smallestIndex].valid = 1;
    cache[set][smallestIndex].count = *count;

    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 6) {
        printf("error");
        return 0;
    }

    // Get arguments
    int cache_size = getArg1(argv[1]);
    char* associativity = getArg2(argv[2]);
    char* replace_policy = getArg3(argv[3]);
    int block_size = getArg4(argv[4]);

    // Verify arguments
    if (cache_size == 0 || strcmp(associativity, "error") == 0 || block_size == 0 || strcmp(replace_policy, "error") == 0) {
        printf("error");
        return 0;
    }

    // Required values
    int lines_per_set;
    int num_of_sets;
    int num_of_lines;

    // Set required values based on input
    setValues(associativity, cache_size, block_size, &lines_per_set, &num_of_sets, &num_of_lines);

    // Tag, set, index bits
    // long set_index_bits = log2(num_of_lines / lines_per_set);
    int set_index_bits = log2(num_of_sets);
    int block_offset_bits = log2(block_size);
    // int tag_bits = 48 - (set_index_bits + block_offset_bits);
    // int tag_bits = log2(lines_per_set);

    FILE *fp;
    fp = fopen(argv[5], "r");
    if (fp == NULL) {
        printf("error");
        return 0;
    }

    // Create cache
    // Line cache[num_of_sets][lines_per_set];
    Line** cache = (Line**) malloc(num_of_sets * sizeof(Line*));
    for (int i = 0; i < num_of_sets; i++) { // initialize cache
        cache[i] = (Line*) malloc(lines_per_set * sizeof(Line));
    }

    // Initialize Lines is cache to tag=0, valid=0
    for (int i = 0; i < num_of_sets; i++) {
        for (int j = 0; j < lines_per_set; j++) {
            cache[i][j].tag = 0;
            cache[i][j].valid = 0;
            cache[i][j].count = 0;
        }
    }

    int mem_reads = 0;
    int mem_writes = 0;
    int cache_hits = 0;
    int cache_misses = 0;

    int lru = 0;
    if (strcmp(replace_policy, "lru") == 0) {
        lru = 1;
    }

    int count = 0;

    char operation;
    int mem_address;
    while (fscanf(fp, "%*s %c 0x%x", &operation, &mem_address) == 2) {
        // printf("mem_address: %lx\n", mem_address);
        long tag = mem_address >> (set_index_bits + block_offset_bits);
        long set = (mem_address >> (block_offset_bits)) & ((1 << set_index_bits)-1);
        // printf("tag: %ld\n", tag);
        // printf("set: %ld\n", set);

        // Check cache
        int isHit = readCache(mem_address, tag, set, cache, num_of_sets, lines_per_set);
        if (operation == 'R') { // if reading
            if (isHit == 1) { // if read from cache is a hit
                cache_hits++;
                if (lru == 1) {
                    readLRU(mem_address, tag, set, cache, num_of_sets, lines_per_set);
                }
            }
            else { // if the read is a miss
                cache_misses++;
                mem_reads++;
                // get from memory and put it in cache
                if (lru == 1) {
                    //Update if LRU
                    writeLRU(mem_address, tag, set, cache, num_of_sets, lines_per_set);
                }
                else {
                    // call FIFO
                    writeFIFO(mem_address, tag, set, cache, num_of_sets, lines_per_set, &count);
                }
            }
        }
        else { // if writing
            // read from cache
            if (isHit == 1) { // if read is a hit
                cache_hits++;
                mem_writes++;

                //Update if LRU
                if (lru == 1) {
                    readLRU(mem_address, tag, set, cache, num_of_sets, lines_per_set);
                }
            }
            else { // if read is a miss
                cache_misses++;
                mem_reads++;
                mem_writes++;
                // load address into cache
                if (lru == 1) {
                    // call LRU
                    writeLRU(mem_address, tag, set, cache, num_of_sets, lines_per_set);
                }
                else {
                    // call FIFO
                    writeFIFO(mem_address, tag, set, cache, num_of_sets, lines_per_set, &count);
                }
            }
        }
    }
    
    printf("Memory reads: %d\n", mem_reads);
    printf("Memory writes: %d\n", mem_writes);
    printf("Cache hits: %d\n", cache_hits);
    printf("Cache misses: %d\n", cache_misses);

    freeCache(cache, num_of_sets);

    return 0;
}
