// This file is auto generated by icy-table.
#ifndef TABLE_COMPILER_INDEX
#define TABLE_COMPILER_INDEX
#define array_element_size(array) sizeof(array[0])
#define array_count(array) (sizeof(array)/array_element_size(array))
#include "icydb.h"
#include <stdlib.h>
#endif


ptr_to_u32_table * ptr_to_u32_table_create(const char * optional_name){
  static const char * const column_names[] = {(char *)"key", (char *)"value"};
  static const char * const column_types[] = {"pointer", "u32"};
  ptr_to_u32_table * instance = calloc(sizeof(ptr_to_u32_table), 1);
  instance->column_names = (char **)column_names;
  instance->column_types = (char **)column_types;
  
  icy_table_init((icy_table * )instance, optional_name, 2, (unsigned int[]){sizeof(pointer), sizeof(u32)}, (char *[]){(char *)"key", (char *)"value"});
  
  return instance;
}

void ptr_to_u32_table_insert(ptr_to_u32_table * table, pointer * key, u32 * value, size_t count){
  void * array[] = {(void* )key, (void* )value};
  icy_table_inserts((icy_table *) table, array, count);
}

void ptr_to_u32_table_set(ptr_to_u32_table * table, pointer key, u32 value){
  void * array[] = {(void* )&key, (void* )&value};
  icy_table_inserts((icy_table *) table, array, 1);
}

void ptr_to_u32_table_lookup(ptr_to_u32_table * table, pointer * keys, size_t * out_indexes, size_t count){
  icy_table_finds((icy_table *) table, keys, out_indexes, count);
}

void ptr_to_u32_table_remove(ptr_to_u32_table * table, pointer * keys, size_t key_count){
  size_t indexes[key_count];
  size_t index = 0;
  size_t cnt = 0;
  while(0 < (cnt = icy_table_iter((icy_table *) table, keys, key_count, NULL, indexes, array_count(indexes), &index))){
    icy_table_remove_indexes((icy_table *) table, indexes, cnt);
    index = 0;
  }
}

void ptr_to_u32_table_clear(ptr_to_u32_table * table){
  icy_table_clear((icy_table *) table);
}

void ptr_to_u32_table_unset(ptr_to_u32_table * table, pointer key){
  ptr_to_u32_table_remove(table, &key, 1);
}

bool ptr_to_u32_table_try_get(ptr_to_u32_table * table, pointer * key, u32 * value){
  void * array[] = {(void* )key, (void* )value};
  void * column_pointers[] = {(void *)table->key, (void *)table->value};
  size_t __index = 0;
  icy_table_finds((icy_table *) table, array[0], &__index, 1);
  if(__index == 0) return false;
  unsigned int sizes[] = {sizeof(pointer), sizeof(u32)};
  for(int i = 1; i < 2; i++){
    if(array[i] != NULL)
      memcpy(array[i], column_pointers[i] + __index * sizes[i], sizes[i]); 
  }
  return true;
}

void ptr_to_u32_table_print(ptr_to_u32_table * table){
  icy_table_print((icy_table *) table);
}

size_t ptr_to_u32_table_iter(ptr_to_u32_table * table, pointer * keys, size_t keycnt, pointer * optional_keys_out, size_t * indexes, size_t cnt, size_t * iterator){
  return icy_table_iter((icy_table *) table, keys, keycnt, optional_keys_out, indexes, cnt, iterator);

}
