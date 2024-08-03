/**
 * @file kiosk.c
 * @author Jake Donovan (jmpatte8)
 * This file is responsible for using behaviors defined in input.c and menu.c
 * and being able to display all returned information for the user to see
 * in the terminal
*/

#include "input.h"
#include "menu.h"

/**
 * Compare are menuItems to make sure they are compared
 * first by category, and if they have the same id, then
 * sort them by id
 * @param va a void const pointer that we will type cast as a pointer to a MenuItem pointer and compare
 * @param vb a void const pointer that we will type cast as a pointer to a MenuIteme pointer and compare
 * @return compare result, 1 = va comes before vb, 0 = va and vb have the same precedence, and -1 meaning va comes before
 * vb
*/
int static menuComp(void const *va, void const *vb)
{
    MenuItem * const * a = va;
    MenuItem * const * b = vb;

   if(strcmp((*a)->category, (*b)->category) == 0){
    return strcmp((*a)->id, (*b)->id);
   }

   else {
    return strcmp((*a)->category, (*b)->category);
   }

   return 0;
}

/**
 * Compare OrderItems first by their cost, then by their
 * ids if their cost is the same
 * @param va a pointer to an OrderItem * that we will use to compare it's MenuItem's cost and id against vb
 * @param vb a pointer to an OrderItem * that we will use to compare it's MenuItem's cost and id against va
 * @return the comparison value when comparing va against vb = 1, 0, or -1
*/
int static orderComp(void const *va, void const *vb)
{
   OrderItem * const *a = va;
   OrderItem * const *b = vb;
   int qa = (*a)->quantity;
   int qb = (*b)->quantity;

   if(qa > 1 && qb > 1){
       if(((*a)->quantity *(*a)->item->cost) > ((*b)->quantity * (*b)->item->cost)){
        return -1;
       }

       else if(((*a)->quantity *(*a)->item->cost) < ((*b)->quantity * (*b)->item->cost)){
        return 1;
       }
       
       else {
        return strcmp((*a)->item->id, (*b)->item->id);
       }
   }

   else if(qa > 1 && qb <= 1){
    if(((*a)->quantity *(*a)->item->cost) > (*b)->item->cost){
        return -1;
    }
    
    else if(((*a)->quantity *(*a)->item->cost) < (*b)->item->cost){
        return 1;
    }
    
    else {
        return strcmp((*a)->item->id, (*b)->item->id);
    }
   }

   else if(qb > 1 && qa <= 1){
    if((*a)->item->cost > ((*b)->quantity * (*b)->item->cost)){
        return -1;
    }
    
    else if((*a)->item->cost < ((*b)->quantity * (*b)->item->cost)){
        return 1;
    }
    
    else {
        return strcmp((*a)->item->id, (*b)->item->id);
    }
   }

   else {
    if((*a)->item->cost > (*b)->item->cost){
        return -1;
    }
    
    else if((*a)->item->cost < (*b)->item->cost){
        return 1;
    }
    
    else {
        return strcmp((*a)->item->id, (*b)->item->id);
    }
   }

   return 0;
}

/**
 * This function will be used to compare MenuItems against a passed param pointer str
 * to determine if the MenuItem category of the current MenuItem is equal to the passed str
 * category, which means this function will return true
 * @param item a MenuItem pointer
 * @param str a category string
 * @return true if the MenuItem->category is equal to string otherwise return false
*/
bool static catCheck(MenuItem const *item, char const *str)
{
    return strcmp(item->category, str) == 0;
}

/**
 * This function sorts the order items in the given order and then prints them.
 * It uses the function pointer paramater to decide the order for sorting order items.
 * This function will be used to list order items for the list order command.
 * @param order a pointer to an Order
*/
void listOrderItems( Order *order, int (*compare)( void const *va, void const *vb))
{
    // qsort() each entry
     qsort(order->list, order->count, sizeof(order->list[0]), compare);
    // set bool if order is removed aka quantity = 0
    bool removed = false;

    //"ID   Name                Quantity Category        Cost"
    //"                                                      "
    // print header
    fprintf(stdout, "%s\n", "ID   Name                 Quantity Category        Cost");

    // total
    int total = 0;

    // print each entry
    for(int i = 0; i < order->count; i++){
        removed = order->list[i]->quantity == 0;

        if(!removed){
            fprintf(stdout, "%4s", order->list[i]->item->id);
            fprintf(stdout, " ");
            int length = strlen(order->list[i]->item->name);
            fprintf(stdout, "%s", order->list[i]->item->name);
                
            while(NAME_ARRAY_INVALID_IDX - length != 0){
                fprintf(stdout, " ");
                length++;
            }

            fprintf(stdout, "%8d", order->list[i]->quantity);

            fprintf(stdout, " ");

            fprintf(stdout, "%s", order->list[i]->item->category);

            length = strlen(order->list[i]->item->category);

            while(CATEGORY_MAX_SIZE - length != 0){
                fprintf(stdout, " ");
                length++;
            }

            int dollar = (order->list[i]->quantity *order->list[i]->item->cost) / CONVERT_COST_INT;
            int cents = (order->list[i]->quantity *order->list[i]->item->cost) % CONVERT_COST_INT;
            
            fprintf(stdout, "$");
            fprintf(stdout, "%3d", dollar);
            fprintf(stdout, ".");

            if(cents < SINGLE_DIGIT_CENTS_CONV){
                fprintf(stdout, "0%d", cents);
            }

            else {
                fprintf(stdout, "%2d", cents);
            }

            fprintf(stdout, "\n");

            total += (order->list[i]->quantity * order->list[i]->item->cost);
        }
    }

    // print total
    fprintf(stdout, "%s", "Total");
    int len = strlen("Total");
    fprintf(stdout, " ");
    while(len != MAX_LINE_LENGTH){
        fprintf(stdout, " ");
        len++;
    }

    fprintf(stdout, "$");
    int dollar = total / CONVERT_COST_INT;
    int cents = total % CONVERT_COST_INT;
    fprintf(stdout, "%3d", dollar);
    fprintf(stdout, ".");
    
    if(cents < SINGLE_DIGIT_CENTS_CONV){
        fprintf(stdout, "0%d", cents);
    }

    else {
        fprintf(stdout, "%2d", cents);
    }

    //
    fprintf(stdout, "\n");
}

/**
 * Program starting point. Calls all required files and functions
 * to examine Menus full of food items and keeps track of there ID, name, category, and cost
 * @return program exit status
*/
int main(int argc, char *argv[])
{
    // step 1: read each input file, open them and read contents in to menu
    // break apart each input file by using sscanf to find the the text files
    // have a check to make sure you have at least 1 input text file or throw an error
    // also check that when you construct a file pointer that the file pointer was able to be constructed
    // if it was pass to Menu class to construct our Menu

    // make sure we have enough command line arguments
    if(argc < MIN_ARGS){
        // we don't have at least 1 input file
        // throw an error
        fprintf(stderr, "%s\n", "usage: kiosk <menu-file>*");
        exit(EXIT_FAILURE);
    }

    // construct a new empty menu
    Menu * menu = makeMenu();

    FILE *fp;

    for(int i = 1; i < argc; i++){
        fp = fopen(argv[i], "r");

        // if we were able to open our file meaning it was valid
        if(fp){
            // close the current file pointer and let readMenuItems pass to readLine()
            // and read file contents
            fclose(fp);
            // read menu items
            readMenuItems(argv[i], menu);
        }

        else {
            //fclose(fp);
            // free menu
            freeMenu(menu);
            // else we have an invalid file name and we need to throw an error
            fprintf(stderr, "Can't open file: %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }

    //fclose(fp);

    // step 2: prompt for commands until we hit EOF or quit command and print each command
    // define char arrays for all valid commands
    char q[] = "quit";
    char l[] = "list";
    char r[] = "remove";
    char a[] = "add";

    // Keep track of when we receive a quit command (maybe invalid input as well)
    bool hasQuitCommand = false;
    // make sure we get valid input as well
    int matches;
    // construct and order in the case we receive an order command = add, remove, etc
    Order * order = (Order *)malloc(sizeof(Order));
    // set initial capacity to 5
    order->capacity = INITIAL_CAPACITY;
    // dynamicallly allocate our order list
    order->list = (OrderItem **)malloc(order->capacity * sizeof(OrderItem *));
    // set our order count to zero
    order->count = 0;
    // save our offset total here
    int total = 0;
    // modify our offset value and add to tal
    int offset = 0;
    //
    char * str;

    while(!hasQuitCommand){
         // prompt for input
         fprintf(stdout, "cmd> ");
         // call readLine() to read the input we got
         str = readLine(stdin);

         if(str == NULL){
            for(int i = 0; i < order->count; i++){
                free(order->list[i]->item);
            }

            for(int i = 0; i < order->count; i++){
                free(order->list[i]);
            }
            
            // free list of order pointers
            free(order->list);
            
            // free the order
            free(order);
            
            // free menu 
            freeMenu(menu);

            //free(str);

            break;
         }

         // might have to check if str is null before we do anything
         // construct an array that holds the first word = our command
         char cmd[COMMAND_MAX_SIZE] = "";
         // scan the first word from str into an array so we can compare
         matches = sscanf(str, "%[a-z]%n", cmd, &offset);
         // update total
         total += offset;

         // first check if the command is a word
         if(matches == 1){
            // compare cmd to all valid command char arrays to see if we have a match
            // quit command
            if((strcmp(cmd, q)) == 0){
                // free str
                free(str);

                for(int i = 0; i < order->count; i++){
                    free(order->list[i]);
                }

                // free list of order pointers
                free(order->list);

                // free the order
                free(order);

                // free menu 
                freeMenu(menu);

                // print quit command
                fprintf(stdout, "%s\n", q);

                // break out of loop and return successfully below
                hasQuitCommand = true;
            }

            // remove command
            else if((strcmp(cmd, r)) == 0){
                // get the id that we are trying to remove
                char orderId[ID_ARRAY_MAX_SIZE] = "";

                sscanf(str + total, " %[0-9]%n", orderId, &offset);

                // total
                total += offset;

                // the index of a menu item we want to remove from order
                int matchIndex = 0;

                // keep track of whether we have a matching order in our order list already
                bool found = false;

                if(strlen(orderId) == MAX_ID_DIGITS){
                    for(int i = 0; i < order->count; i++){
                        if(strcmp(order->list[i]->item->id, orderId) == 0){
                            matchIndex = i;
                            found = true;
                            break;
                        }
                    }

                    if(found){
                        // get quantity
                        int quantity = 0;

                        int match = sscanf(str + total, "%d%n", &quantity, &offset);

                        // total
                        total += offset;

                        if(match == 1){
                            // if quantity that is to be removed is greater than 0
                            // and not greater than our current quantity of the specified item
                            // then remove
                            if(quantity <= order->list[matchIndex]->quantity && quantity > 0){
                                order->list[matchIndex]->quantity = order->list[matchIndex]->quantity - quantity;
                                //
                                fprintf(stdout, "%s\n", str);
                                // print an extra newline
                                fprintf(stdout, "\n");
                                //
                                total = 0;
                                // free current string
                                free(str);
                            }

                            else {
                                fprintf(stdout, "%s\n", str);
                                // free current string
                                free(str);
                                //
                                fprintf(stdout, "Invalid command\n");
                                fprintf(stdout, "\n");
                            }
                        }

                        else {
                            fprintf(stdout, "%s\n", str);
                            // free current string
                            free(str);
                            //
                            fprintf(stdout, "Invalid command\n");
                            fprintf(stdout, "\n");
                        }
                    }


                    else {
                        fprintf(stdout, "%s\n", str);
                        // free current string
                        free(str);
                        //
                        fprintf(stdout, "Invalid command\n");
                        fprintf(stdout, "\n");
                    }
                }

                else {
                    fprintf(stdout, "%s\n", str);
                    // free current string
                    free(str);
                    //
                    fprintf(stdout, "Invalid command\n");
                    fprintf(stdout, "\n");
                }
            }

            // list command
            else if((strcmp(cmd, l)) == 0){
                // construct a new array of size 15
                // so we can hold up to any category value
                char check[CATEGORY_MAX_SIZE] = "";

                // check for next word to see if we are asked to list the menu, an order, or a category
                // remember to use the offset total
                sscanf(str + total, " %[a-zA-Z0-9]%n", check, &offset);

                // update total
                total += offset;

                // create null terminated keywords array
                char category[] = "category";
                char menuStr[] = "menu";
                char orderStr[] = "order";

                // we have a category if we are here
                if((strncmp(check, category, strlen(check))) == 0){
                    // declare an array to hold the specified category
                    char itemCat[CATEGORY_MAX_SIZE] = "";
                    // scan for category name
                    sscanf(str + total, " %[a-zA-Z]%n", itemCat, &offset);
                    // update total
                    total += offset;
                    // print newline
                    fprintf(stdout, "%s\n", str);
                    // free current string
                    free(str);
                    // pass category and compare function to listOrderItems()
                    // to print the specified category
                    listMenuItems(menu, menuComp, catCheck, itemCat);
                    // reset total
                    total = 0;
                }

                // we are being asked to print the menu
                else if(strcmp(check, menuStr) == 0){
                    fprintf(stdout, "%s\n", str);
                    char extra[CATEGORY_MAX_SIZE] = "";

                    int extraInput = sscanf(str + total, " %[a-zA-Z0-9]", extra);

                    if(extraInput == 1){
                        free(str);
                        fprintf(stdout, "Invalid command\n");
                        fprintf(stdout, "\n");
                        total = 0;
                    }

                    else {
                        free(str);
                        listMenuItems(menu, menuComp, catCheck, NULL);
                        total = 0;
                    }
                }

                // we are being asked to print our current order
                else if(strcmp(check, orderStr) == 0){
                    fprintf(stdout, "%s\n", str);
                    free(str);
                    listOrderItems(order, orderComp);
                    total = 0;
                    fprintf(stdout, "\n");
                }

                // we have hit an invalid list command if we are here
                else {
                    fprintf(stdout, "%s\n", str);
                    // free current string
                    free(str);
                    // we have an invalid list command
                    // put something here later
                    fprintf(stdout, "Invalid command\n");
                    fprintf(stdout, "\n");
                }
            }

            // add command
            else if((strncmp(cmd, a, strlen(a))) == 0){
                // get the id
                char orderId[ID_ARRAY_MAX_SIZE];

                sscanf(str + total, " %[0-9]%n", orderId, &offset);

                total += offset;

                if(strlen(orderId) == MAX_ID_DIGITS) {
                    OrderItem * item = (OrderItem *)malloc(sizeof(OrderItem));

                    bool notValidId = true;

                    for(int i = 0; i < menu->count; i++){
                        if(strcmp(menu->list[i]->id, orderId) == 0) {
                            item->item = menu->list[i];
                            item->quantity = 0;
                            notValidId = false;
                        }
                    }

                    // we didn't find a matching id in menu
                    if(notValidId) {
                        fprintf(stdout, "%s\n", str);
                        // free current str
                        free(str);
                        // 
                        free(item);
                        // quantity is not greater than 0
                        fprintf(stdout, "Invalid command\n");
                        fprintf(stdout, "\n");
                        continue;
                    }

                    // get the quantity
                    int x = 0;
                    sscanf(str + total, "%d%n", &x, &offset);

                    // total
                    total += offset;

                    if(x > 0){
                        item->quantity = x;

                        bool exists = false;

                        int index = 0;

                        for(int i = 0; i < order->count; i++){
                            if(strcmp(order->list[i]->item->name, item->item->name) == 0){
                                exists = true;
                                index = i;
                                break;
                            }
                        }

                        // if the order item already exists update the quantity
                        if(exists){
                            order->list[index]->quantity += item->quantity;
                            fprintf(stdout, "%s\n", str);
                            // free current string
                            free(str);
                            free(item);
                            fprintf(stdout, "\n");
                            total = 0;
                        }

                        // otherwise add the new order item to our list
                        else {
                            // allocate space for new order item
                            // similar to how map allocates
                            if(order->count >= order->capacity){
                                order->capacity = order->capacity * DOUBLE_CAPACITY;
                                order->list = realloc(order->list, order->capacity * sizeof(OrderItem *));
                            }

                            order->list[order->count] = (OrderItem *)malloc(sizeof(OrderItem));

                            order->list[order->count++] = item;

                            fprintf(stdout, "%s\n", str);
                            fprintf(stdout, "\n");

                            total = 0;

                            // free current str
                            free(str);
                        }
                    }

                    else {
                        fprintf(stdout, "%s\n", str);
                        // free current str
                        free(str);
                        free(item);
                        // quantity is not greater than 0
                        fprintf(stdout, "Invalid command\n");
                        fprintf(stdout, "\n");
                    }
                }

                else {
                    fprintf(stdout, "%s\n", str);
                    // free current string
                    free(str);
                    // invalid id
                    fprintf(stdout, "Invalid command\n");
                    fprintf(stdout, "\n");
                }
            }

            // we don't have a valid command if we are here
            else {
                fprintf(stdout, "%s\n", str);
                // free current string
                free(str);
                fprintf(stdout, "Invalid command\n");
                fprintf(stdout, "\n");
            }
         }

         // we don't have a match if we get here
         else {
            fprintf(stdout, "%s\n", str);
            // free current str
            free(str);
            // we do not want to get here
            // determine what to do here later but definitely break here
            fprintf(stdout, "Invalid command\n");
            fprintf(stdout, "\n");
         }
    }


    // step 3: exit successfully
    return EXIT_SUCCESS;
}