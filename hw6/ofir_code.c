#include <assert.h>
#include <limits.h>																
#include <math.h>																
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//defines and typedefs
typedef struct key key;
typedef struct map map;
#define SAME 1
#define FIVE 5

    
struct key {
    char *word;
    int appears_note;
    int appears_magazine;
    key *next;
};

struct map {
    key *head;
    key *tail;
};
// declartions
char* readline();
char** split_string(char*);
void print_yes_or_no(int state);
void checkMagazine(int magazine_count,
		           char** magazine, 
				   int note_count, 
				   char** note);
struct key *create_new_key(char *word_data ,int where);
void insert_into_map(struct map **map, char **word_data,int where);
void free_map (struct key *key);
void map_maker(struct map **map ,char **data ,int length, int where);
void chk_map(struct map *map);



int main()
{
    char** mn = split_string(readline());

    char* m_endptr;															
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;															
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());						

    char** magazine = malloc(m * sizeof(char*));
    																			//fill the magazine arr
    for (int i = 0; i < m; i++) {												//not forget to free **magazine
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());								//fill the note arr
    																			//not forget to free **note
    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

void print_yes_or_no(int state){
    if(!state){
    	printf("NO\n");
    }
    else{
    	printf("YES\n");
    }

}

void checkMagazine(int magazine_count,
		           char** magazine,
				   int note_count,
				   char** note) {

    int where = 1;
    struct map *map=malloc(sizeof(struct map));
    insert_into_map(&map,&magazine,where);
    map_maker( &map , &(magazine +1) , magazine_count, where);
    map_maker( &map , &note ,note_count,!(where));
    chk_map(map);
    free_map(map -> head);

    

}

// creates a new key and returns it
//magazine =1
//note=0
struct key *create_new_key(char *word_data ,int where){
	struct key *key_ptr = malloc(sizeof(struct key));
	key_ptr -> word = *word_data;
	if (where){
		key_ptr -> appears_magazine = 1;
        key_ptr -> appears_note = 0;
	}
	else {
		key_ptr -> appears_note = 1;
        key_ptr -> appears_magazine = 0;
	}
		
	key_ptr -> next = NULL;
	return key_ptr;
	
}
//insert new key into the map 
void insert_into_map(struct map **map, char **word_data,int where){
	struct key *key_ptr = create_new_key(&word_data,where);
	if(!(*map)->head){
		(*map) -> head = key_ptr;
	}
	else {
		(*map) -> tail -> next = key_ptr;
	}
	(*map) -> tail = key_ptr;
			
}

// free the map from a given key
void free_map (struct key *key){
	while (key){
		struct key *temp = key;
		key = key -> next;
		free(temp);
	}
}

// creates the maps without make double keys
void map_maker(struct map **map ,char **data ,int length, int where){
    struct key *curr_key = ((*map) -> head);
    for(int i=0 ;i < length;i++){
    	char *temp = *(data +i);
    	while ((curr_key->next)!= NULL ){
    	    char *str1= &(curr_key -> word);
    	    int a=my_strcmp(*str1, *temp);
    		if (a){
                if (where){
    			    curr_key -> appears_magazine
                     = (curr_key -> appears_magazine) +1;
                } 
                else{
                    curr_key -> appears_note = (curr_key -> appears_note) +1;
                }
				break;
    		}
    		else {
    			if (!(curr_key -> next)){
    				insert_into_map(map ,temp, where)  ;
    			}
    			else{
    			    curr_key = curr_key -> next;	
    			}
    		}
    	}
    	
    }
}

void chk_map(struct map *map){
    int can_make_note = 1;
    key *curr_key = (map -> head);
    while(curr_key != NULL){
        if((curr_key -> appears_magazine) < (curr_key -> appears_note)){
            can_make_note = !(can_make_note);
            break;
        }
        else{
            curr_key = curr_key -> next;
        }
    }
    print_yes_or_no(can_make_note);
}
int my_strcmp(char *str1, char *str2){
        for (int i=0;i<FIVE;i++){
            if((str1+i)!=(str2+i)){
                return !SAME;
            }
        }
        return SAME;
}
