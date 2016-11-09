#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "token.h"

typedef struct entry_s {
	char *key;
	token_t *value;
	struct entry_s *next;
} entry_t;

typedef struct hashtable_s {
	int size;
	entry_t **table;	
} hashtable_t;

hashtable_t *hashtable_new( int size );

int hashtable_hash( hashtable_t *hashtable, char *key );

entry_t *hashtable_newpair( char *key, token_t *value );

void hashtable_set( hashtable_t *hashtable, char *key, token_t *value );

token_t *hashtable_get( hashtable_t *hashtable, char *key );

#endif

