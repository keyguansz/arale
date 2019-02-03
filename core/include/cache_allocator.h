#include "ctype.h"
#include "list.h"

#ifndef __CACHE_ALLOCATOR_H__
#define __CACHE_ALLOCATOR_H__

#define CONTENT_SIZE 1024*4*32

enum CACHE_STATUS
{
    CACHE_FREE = 0,
    CACHE_USING
};

typedef struct core_mem_cache
{
    int objsize;
    struct list_head list;               //this is used for list all cache mem(node)
    struct list_head full_node_list;          //all memory has been consumed(node)
    struct list_head free_node_list;          //all memory has been released(node)
    struct list_head partial_node_list;       //some memory hase been consumed,some has been released(node)
    struct list_head lru_free_content_list;      //recent freezed memory (content)
    struct list_head global_ll;
}core_mem_cache;

//one slab
typedef struct core_mem_cache_node
{
    struct list_head list; //for full_list/free_list_partial_list
    addr_t start_pa;
    addr_t end_pa;
    core_mem_cache *cache;
    int nr_free;

}core_mem_cache_node;

typedef struct core_mem_cache_content
{
    struct list_head ll;
    core_mem_cache_node *head_node;
    int is_using;
    addr_t start_pa;
}core_mem_cache_content;

public core_mem_cache *creat_core_mem_cache(size_t size);
public void *cache_alloc(core_mem_cache *cache);
public void cache_free(core_mem_cache *cache,addr_t addr);
public void cache_destroy(core_mem_cache *cache);
public void cache_allocator_init();
public void cache_allocator_start_monitor();
public uint32_t cache_free_statistic();


#endif
