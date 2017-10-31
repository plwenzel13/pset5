/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Node Definition
typedef struct node
{
    struct node* children[27];
    bool is_word;
}
node;

// Root Node
node* root;

// Size of trie
unsigned int words = 0;

/**
 * Returns index of letter within trie array
 */
int getIndex(const char c)
{
    if (c == '\'') 
    {
        return 26;    
    }
    else 
    {
        return tolower(c) % 'a';
    }
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // Set cursor to root
    node* cursor = root;
    
    // for each letter in input word
    for (int position= 0; word[position] != '\0'; position++) 
    {
        // Find the index of the letter
        int index = getIndex(word[position]);
        
        // got to corresponding element in children
        if (cursor->children[index] == NULL) 
        {
            // if NULL word is mispelled
            return false;
        }
        
        // if not NULL, move to next letter
        cursor = cursor->children[index];
    }
    
    // once at end of input word, check if is_word is true
    return cursor->is_word;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Create space for root
    root = calloc(1, sizeof(node));
    
    // Initialize number of nodes
    words = 0;
    
    // Read dictionary
    FILE* dictFile = fopen(dictionary, "r");
    if (dictFile == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    // Set cursor to root
    node* cursor = root;
    
    // Read each character in dictionary
    for (int letter = tolower(fgetc(dictFile)); letter != EOF; letter = fgetc(dictFile))
    {
        // Check if newline
        if (letter == '\n') 
        {
            // mark as word
            cursor->is_word = true;
            
            // Increment number of nodes
            words++;
            
            // reset cursor to root to traverse trie again
            cursor = root;
        }
        else 
        {
            // Find the index of the letter
            int index = getIndex(letter);
            
            // Check if node exists for letter
            if (cursor->children[index] == NULL) 
            {
                // Create new node
                cursor->children[index] = calloc(1,sizeof(node));
            }

            // Move to next node
            cursor = cursor->children[index];
        }
    }
    
    // Close dictionary
    fclose(dictFile);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words;
}

// Check node children to see if they can be freed
bool clear(node* ptr)
{
    // Go through node's children
    for (int letter= 0; letter< 27; letter++)
    {
        // If child is pointer, recursively check that one as well
        if (ptr->children[letter] != NULL) 
        {
            clear(ptr->children[letter]);
        }
    }
    
    // If all chilren are null, free node
    free(ptr);
    
    return true;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // Start at root
    return clear(root);
}