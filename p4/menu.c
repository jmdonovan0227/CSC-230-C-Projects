/**
 * @file menu.c
 * @author Jake Donovan (jmpatte8)
 * Our implementation file for our menu.h which implements all behavior for reading and working with our Menu
 * items.
*/

#include "menu.h"
#include "input.h"

/**
 * This function allocates storage for the Menu, and initializes it's fields
 * to store a resizable array and returns a pointer to the new menu. (like a constructor in Java)
 * @return a pointer to a new Menu
*/
Menu *makeMenu()
{
    // create a new menu struct with an initial capacity of 5 and set count to 0
    // this will create an empty menu
    Menu * menu = (Menu *)malloc(sizeof(Menu));

    // set capacity to 5
    menu->capacity = INITIAL_CAPACITY;

    // dynamically allocate our list field which is an array of pointers to MenuItems
    menu->list = (MenuItem **)malloc(menu->capacity * sizeof(MenuItem *));

    // set count to 0
    menu->count = 0;

    return menu;
}

/**
 * This function frees the memory used to store the given menu, including
 * freeing space for all MenuItems, freeing the resizable array of pointers and freeing
 * space for the Menu struct itself
 * @param menu a pointer to a Menu
*/
void freeMenu( Menu *menu )
{
    // Free each MenuItem
    for(int i = 0; i < menu->count; i++){
        free(menu->list[i]);
    }

    // Free list of pointers
    free(menu->list);

    // Free the Menu
    free(menu);
}

/**
 * This function reads all menu items from a menu item file with the given name.
 * It makes an instance of the MenuItem struct for each one and stores a pointer
 * to that MenuItem in the resizable array in menu.
 * @param filename a pointer to a menu item file
 * @param menu a pointer to a menu
*/
void readMenuItems( char const *filename, Menu *menu)
{
    FILE * fp = fopen(filename, "r");
    bool invalid = false;
    //

    // while we haven't hit the end of the file
    while(!invalid){
        char * line = readLine(fp);

        if(line == NULL){
            invalid = true;
            break;
        }

        char copy[MAX_LINE_LENGTH] = "";
        strcpy(copy, line);
        int offset = 0;
        int total = 0;
        int matches = 0;
        char id[ID_ARRAY_MAX_SIZE] = "";
        char category[CATEGORY_MAX_SIZE] = "";
        char name[NAME_MAX_SIZE] = "";
        char cost[ID_ARRAY_MAX_SIZE] = "";


        // check for valid id
        matches = sscanf(copy, "%[0-9]%n", id, &offset);

        // check if we have exactly for digits (less than)
        if(strlen(id) != MAX_ID_DIGITS){
            // throw an error
            free(line);
            freeMenu(menu);
            fclose(fp);
            fprintf(stderr, "Invalid menu file: %s\n", filename);
            exit(EXIT_FAILURE);
        }
        
        // check if we were able to match an id
        // this might need to be 1 but pretty certain this should be 2 for 2 conversion specifiers
        if(matches == 1){
            // update total
            total += offset;

            matches = sscanf(copy + total, " %[a-zA-Z]%n", category, &offset);

            total += offset;

            // our category is valid
            if(matches == 1){
                matches = sscanf(copy + total, " %[0-9]%n", cost, &offset);

                int costVal = 0;

                int validNumber = sscanf(cost, "%d", &costVal);

                if(validNumber != 1){
                    //
                }

                // cost is less than or equal to zero error check
                if(costVal <= 0){
                    free(line);
                    freeMenu(menu);
                    fclose(fp);
                    fprintf(stderr, "Invalid menu file: %s\n", filename);
                    exit(EXIT_FAILURE);
                }

                total += offset;

                // our cost was valid
                if(matches == 1){
                    matches = sscanf(copy + total, " %[a-zA-Z0-9 ]%n", name, &offset);

                    total += offset;

                    // too long of a name
                    if((name[NAME_ARRAY_INVALID_IDX] >= 'a' && name[NAME_ARRAY_INVALID_IDX] <= 'z') || (name[NAME_ARRAY_INVALID_IDX] >= 'A' && name[NAME_ARRAY_INVALID_IDX] <= 'Z') || (name[NAME_ARRAY_INVALID_IDX] >= '0' && name[NAME_ARRAY_INVALID_IDX] <= '9')){
                        // we have a name that is too long
                        free(line);
                        freeMenu(menu);
                        fclose(fp);
                        fprintf(stderr, "Invalid menu file: %s\n", filename);
                        exit(EXIT_FAILURE);
                    }

                    // our name was valid
                    if(matches == 1){
                        // construct a MenuItem struct with all fields
                        MenuItem menuItem = {};
                        // copy id array
                        strncpy(menuItem.id, id, strlen(id));
                        // copy name 
                        strncpy(menuItem.category, category, strlen(category));
                        // copy category
                        strncpy(menuItem.name, name, strlen(name));
                        // assign cost
                        menuItem.cost = costVal;

                        bool invalidId = false;

                        // check for duplicate id
                        for(int i = 0; i < menu->count; i++){
                            if(strcmp(menu->list[i]->id, menuItem.id) == 0){
                                invalidId = true;
                                break;
                            }
                        }

                        // duplicate id
                        if(invalidId) {
                            free(line);
                            freeMenu(menu);
                            fclose(fp);
                            fprintf(stderr, "Invalid menu file: %s\n", filename);
                            exit(EXIT_FAILURE);
                        }

                        // check if we have enough space in our Menu
                        if(menu->count >= menu->capacity){
                            menu->capacity = menu->capacity * DOUBLE_CAPACITY;
                            menu->list = realloc(menu->list, menu->capacity * sizeof(MenuItem *));
                        }

                        // allocate space for new menu item in Menu
                        menu->list[menu->count] = (MenuItem *)malloc(sizeof(MenuItem));

                        // add constructed MenuItem to menu
                        *menu->list[menu->count] = menuItem;

                        // increment menu->count
                        menu->count = menu->count + 1;

                        // free current line
                        free(line);
                    }

                    // our name was not valid
                    else {
                        // free the current line
                        free(line);

                        // free menu
                        freeMenu(menu);

                        // close file
                        fclose(fp);

                        // throw error message
                        // our id was too long
                        fprintf(stderr, "Invalid menu file: %s\n", filename);

                        exit(EXIT_FAILURE);
                    }
                }

                // our cost was not valid
                else {
                    // free the current line
                    free(line);

                    // free menu
                    freeMenu(menu);

                    // close file
                    fclose(fp);

                    // throw error message
                    fprintf(stderr, "Invalid menu file: %s\n", filename);

                    exit(EXIT_FAILURE);
                }
            }

            // our category is not valid
            else {
                // free the current line
                free(line);

                // free menu
                freeMenu(menu);

                // close file
                fclose(fp);

                // throw error message
                fprintf(stderr, "Invalid menu file: %s\n", filename);

                exit(EXIT_FAILURE);
            }
        }

        // our id was not valid
        else {
            // free the current line
            free(line);

            // free menu
            freeMenu(menu);

            // close file
            fclose(fp);

            // throw error message
            fprintf(stderr, "Invalid menu file: %s\n", filename);

            exit(EXIT_FAILURE);
        }
    }

    // close file pointer
    if(fp){
        fclose(fp);
    }
}

/**
 * This function sorts the menu items in the given menu and then prints them. It uses the first function
 * pointer parameter to decide the order for sorting menu items. It uses the second function pointer parameter
 * together with the string, str, which is passed to the function, to decide the menu items to print. This function
 * will be used to list menu items fro the list menu and lissta category commands.
 * @param menu a pointer to a Menu
 * @param str a pointer to a string
*/
void listMenuItems( Menu *menu, int (*compare) (void const *va, void const *vb), bool (*test) (MenuItem const *item, char const *str), char const *str )
{
    // print header
    fprintf(stdout, "ID   Name                 Category        Cost");
    // separate header with a newline
    fprintf(stdout, "\n");
    // sort our menu with qsort
    if(menu->count == 0){
        return;
    }
    
    qsort(menu->list, menu->count, sizeof(menu->list[0]), compare);
    // print contents
    for(int i = 0; i < menu->count; i++){
        // if we have a str to compare against
        // meaning str is not NULL
        if(str){
            if((test)(menu->list[i], str)){
                int dollar = menu->list[i]->cost / CONVERT_COST_INT;
                int cents = menu->list[i]->cost % CONVERT_COST_INT;
                fprintf(stdout,"%s ", menu->list[i]->id);
                fprintf(stdout, "%s", menu->list[i]->name);

                int length = strlen(menu->list[i]->name);
                
                while(NAME_ARRAY_INVALID_IDX - length != 0){
                    fprintf(stdout, " ");
                    length++;
                }

                fprintf(stdout, "%s", menu->list[i]->category);

                length = strlen(menu->list[i]->category);

                while(CATEGORY_MAX_SIZE - length != 0){
                    fprintf(stdout, " ");
                    length++;
                }

                fprintf(stdout, "$ ");
                fprintf(stdout, "%2d", dollar);
                fprintf(stdout, ".");

                if(cents < SINGLE_DIGIT_CENTS_CONV){
                    fprintf(stdout, "0%d", cents);
                }

                else {
                    fprintf(stdout, "%d", cents);
                }

                fprintf(stdout, "\n");
            }
        }

        // otherwise print everything
        else {
            int dollar = menu->list[i]->cost / CONVERT_COST_INT;
            int cents = menu->list[i]->cost % CONVERT_COST_INT;
            fprintf(stdout,"%s ", menu->list[i]->id);
            fprintf(stdout, "%s", menu->list[i]->name);

            int length = strlen(menu->list[i]->name);
                
            while(NAME_ARRAY_INVALID_IDX - length != 0){
                fprintf(stdout, " ");
                length++;
            }

            fprintf(stdout, "%s", menu->list[i]->category);

            length = strlen(menu->list[i]->category);

            while(CATEGORY_MAX_SIZE - length != 0){
                fprintf(stdout, " ");
                length++;
            }

            fprintf(stdout, "$ ");
            fprintf(stdout, "%2d", dollar);
            fprintf(stdout, ".");

            if(cents < SINGLE_DIGIT_CENTS_CONV){
                fprintf(stdout, "0%d", cents);
            }

            else {
                fprintf(stdout, "%d", cents);
            }

            fprintf(stdout, "\n");
        }
    }

    fprintf(stdout, "\n");
}