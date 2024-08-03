/**
 * @file menu.h
 * @author Jake Donovan (jmpatte8)
 * This header file is responsible for defining all necessary structs
 * needed in our menu.c file and declaring all of our Menu functions that we will need
 * to call in other files
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"

/**
 * A MenuItem which holds an id, name, category, and cost
*/
struct MenuItemStruct {
    char id[INITIAL_CAPACITY];
    char name[NAME_ARRAY_INVALID_IDX];
    char category[CATEGORY_MAX_SIZE - 1];
    int cost;
};

/** A typedef to make the creation and allocation for MenuItems easier */
typedef struct MenuItemStruct MenuItem;

/**
 * A menu which holds a list of MenuItems
*/
struct MenuStruct {
    // a pointer to a list of menu items should
    // start with an intial capacity of 5
    // this might need to be fixed later
    MenuItem **list;
    int count;
    int capacity;
};

/** A typedef to make the creation/allocation of our Menu structs easier */
typedef struct MenuStruct Menu;

/**
 * An OrderItem which holds a pointer to a MenuItem and a quantity
*/
struct OrderItemStruct {
    MenuItem * item;
    int quantity;
};

/** A typedef for an OrderItem which makes the use/allocation of OrderItems easier */
typedef struct OrderItemStruct OrderItem;

/**
 * An Order struct which holds a list of OrderItems, a count, and a quantity
*/
struct OrderStruct {
    OrderItem **list;
    int count;
    int capacity;
};

/** A typedef for an Order which makes the use/allocation of Orders easier */
typedef struct OrderStruct Order;

/**
 * This function allocates storage for the Menu, and initializes it's fields
 * to store a resizable array and returns a pointer to the new menu. (like a constructor in Java)
 * @return a pointer to a new Menu
*/
Menu *makeMenu();

/**
 * This function frees the memory used to store the given menu, including
 * freeing space for all MenuItems, freeing the resizable array of pointers and freeing
 * space for the Menu struct itself
 * @param menu a pointer to a Menu
*/
void freeMenu( Menu *menu );

/**
 * This function reads all menu items from a menu item file with the given name.
 * It makes an instance of the MenuItem struct for each one and stores a pointer
 * to that MenuItem in the resizable array in menu.
 * @param filename a pointer to a menu item file
 * @param menu a pointer to a menu
*/
void readMenuItems( char const *filename, Menu *menu);

/**
 * This function sorts the menu items in the given menu and then prints them. It uses the first function
 * pointer parameter to decide the order for sorting menu items. It uses the second function pointer parameter
 * together with the string, str, which is passed to the function, to decide the menu items to print. This function
 * will be used to list menu items fro the list menu and lissta category commands.
 * @param menu a pointer to a Menu
 * @param str a pointer to a string
*/
void listMenuItems( Menu *menu, int (*compare) (void const *va, void const *vb), bool (*test) (MenuItem const *item, char const *str), char const *str );