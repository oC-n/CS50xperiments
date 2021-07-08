// Test implementation of example in "Tries" short video, CS50x 2021.
// We are setting up a structure containing universities and their foundation dates.
// Foundation years are used as keys.

#include <stdio.h>
#include <stdlib.h> // Needed for malloc
#include <stdbool.h>
#include <string.h>

// Define trie structure as per video.
typedef struct _trie
{
    char university[20];
    struct _trie* paths[10];
}
trie;
// For each node here, there is room for a 20-char string
// and an array of 10 pointers.

// Function Prototypes
trie* addToTrie(trie *t, int digits);
void initpaths(trie *t);
void freetrie(trie *t);
void insert(trie *t, char *name, int year);
bool search(trie *t, char *name, int year);

int main(void)
{
    // Set up root node & initialise
    trie *founded = malloc(sizeof(trie));
    if (founded != NULL)
    {
        founded->university[0] = '\0';
        initpaths(founded);

        // Populate trie with university foundation years.
        insert(founded, "Harvard", 1636);
        insert(founded, "Yale", 1701);
        insert(founded, "Dartmouth", 1769);

        // Test finding content: true
        printf("Harvard, founded 1636, %s in the trie.\n", search(founded, "Harvard", 1636) ? "is" : "is not");

        // Test finding content: false
        printf("Princeton, founded 1746, %s in the trie.\n", search(founded, "Princeton", 1746) ? "is" : "is not");

        // Free memory
        freetrie(founded);
        return 0;
    }
    else
    {
        return 1; // Memory couldn't be allocated.
    }
}

// Initialise paths: Set all paths in a trie to NULL pointers.
void initpaths(trie *t)
{
    for (int i = 0; i < 10; i++)
    {
        t->paths[i] = NULL;
    }
}

// This function inserts a new university to the trie construct.
// It calls on addToTrie to create the nodes and links between them,
// then adds the name to the final node.
void insert(trie *t, char *name, int year)
{
    trie *leaf = addToTrie(t, year);    // This is the "destination" node
    // Ideally, we would check this was successful and return a fault (replace void above) if not.
    strcpy(leaf->university, name); // Add university name to destination node.
}

// This routine creates a path from the root to the "destination" node,
// adding new nodes where required.
// I assume here that the key could be an integer of any (more or less) length,
// rather than a year specifically.
trie* addToTrie(trie *t, int digits)
{
    if (digits >= 0)    // Check we have something to work with.
    {
        if (digits / 10 == 0)   // We are down to a single digit, by input or recursion.
        {
            // Test whether path already exists
            if (t->paths[digits] == NULL)
            {
                // write & initialise first node/trie
                trie *newnode = malloc(sizeof(trie));
                if (newnode != NULL)
                {
                    t->paths[digits] = newnode;
                    initpaths(newnode);
                }
                else
                {
                    return NULL;
                }
            }
            return t->paths[digits];    // return address of current node (new or existing)
        }
        else    // multiple digits remaining
        {
            // Call addToTrie again on digits to the left of the last
            trie *rootnode = addToTrie(t, digits / 10);
            // rootnode will be the node upstream of that for the current digit.

            // Test whether path from rootnode to here already exists.
            // digits % 10 isolates current digit.
            if (rootnode->paths[digits % 10] == NULL)
            {
                // add next node
                trie *newnode = malloc(sizeof(trie));
                if (newnode != NULL)
                {
                    rootnode->paths[digits % 10] = newnode;
                    initpaths(newnode);
                }
            }
            return rootnode->paths[digits % 10];    // return address of current node (new or existing)
        }
    }
    else
    {
        return NULL;    // no key, so return null pointer
    }
}

// Search for specified university name & year in trie structure.
bool search(trie *t, char *name, int year)
{
    // Here we convert the key to a string, to let us step through it.
    // We assume a maximum key length of 19.
    char route[20] = "";
    int digit;
    sprintf(route, "%i", year);
    int ndigits = strlen(route);
    if (ndigits > 0)
    {
        // pointer is used to navigate through the structure, one digit at a time.
        trie *pointer = t;
        for (int i = 0; i < ndigits; i++)
        {
            digit = (int)route[i] - 48; // Back to integer value of digit character (ASCII 48 is 0)
            if (pointer->paths[digit] == NULL)
            {
                return false;   // Dead end.
            }
            else
            {
                pointer = pointer->paths[digit];    // Move pointer down path.
            }
        }
        if (strcmp(pointer->university, name) == 0) // Check now that contents at destination match.
        {
            return true;
        }
    }
    return false;   // Also return false if anything went wrong.
}

// Free all the memory in the trie construct.
void freetrie(trie *t)
{
    for (int i = 0; i < 10; i++)
    {
        if (t->paths[i] != NULL)
        {
            freetrie(t->paths[i]);
        }
    }
    free(t);
    return;
}