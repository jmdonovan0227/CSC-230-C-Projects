/**
   @file trig.c
   @author Jake Donovan (jmpatte8)
   This file is responsible for taking an angle between PI and -PI and returning
   an approximation of the sin and cos of that angle within 0.000001 of accuracy
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** Approximation of PI.  For now, we're using our own constant for
    this. There's also one provided by the gnu math library, once we
    learn about that.
*/
#define PI 3.14159265358979323846
/** Iteration zero for our loop */
#define ITERATION_ZERO 0
/** Iteration one for our loop */
#define ITERATION_ONE 1
/** Iteration two for our loop */
#define ITERATION_TWO 2
/** Iteration three for our loop */
#define ITERATION_THREE 3
/** Used for correctly spacing double digit values when printing */
#define DOUBLE_DIGIT_DOUBLE 9.0
/** Our accuracy threshold for sin and cos values */
#define ACCURACY 0.000001
/** Our first plus when creating our table header */
#define FIRST_PLUS 6
/** Our second plus when creating our table header */
#define SECOND_PLUS 20
/** Our third plus when creating our table header */
#define THIRD_PLUS 34
/** Our table header length in characters */
#define HEADER_LENGTH 48
/** Our plus signs for our table header */
#define PLUS '+'
/** Our dash signs for our table header */
#define DASH '-'
/** Used to check for double digit integers */
#define DOUBLE_DIGIT_INT 9

/**
   Gets the angle from the user using scanf(), if the
   angle is less than -PI or greater than PI, and if the angle
   is not a double, the program will exit unsuccessfully with
   an error message "Invalid Input" 
   @return the angle received from the user
*/
double getAngle()
{
       // We are going to save our angle here
       double angle = 0.0;
 
       // Prompt for user input using scanf()
       int matches = scanf( "%lf", &angle );
 
       // We have recieved valid input, return what we have
       if ( matches == 1 ) {
           if ( angle < PI && angle > -PI ) {
               return angle;
           }
    
           else {
               printf( "Invalid input\n" );
               exit(1);
           }
       }
 
       // We either do not have any input or we have invalid input
       else {
           // We didn't recieve valid input
           printf( "Invalid input\n" );
     
           // Exit unsuccessfully
           exit(1);
       }
 
       // We should not ever reach this statement
       return 0.0;
}

/**
   Calculate the difference between our current sin and current cos against
   our previous value for sin and cos, if the values both differ by 0.000001
   for sin and cos, when we return from this method, our tableRow() function will
   know to stop calculating sin, cos, and tan after that current iteration
   @param the current value of sin and cos
   @param the previous value of sin and cos
   @return the difference between the current and previous values for sin and cos
   as a double
*/
double difference( double a, double b )
{
       // Return the difference between the current values of sin and cos
       return a - b;
}

/**
   Constructs the table header for our table that holds our sin, cos, and tan
   values
*/
void tableHeader()
{
     // First print the top half of the table header
     printf( "terms |         sin |         cos |         tan \n" );
 
     int dashes = 0;
 
     // by referencing image in project 1 description, the header should have 45 dashes       // and 3 plus characters
     while ( dashes < HEADER_LENGTH ) {
           if ( dashes == FIRST_PLUS|| dashes == SECOND_PLUS || dashes == THIRD_PLUS ) {
               putchar( PLUS );
           }
      
           else {
               putchar( DASH );
           }
      
           dashes++;
     }
 
    // end with a newline
    printf( "\n" );
}

/** 
   Calculates and returns our values for sin, cos, and tan
   when determining accurate representation of sin and cos in radians
   of angle (within 0.000001 for sin and cos), also prints the tan value at
   each iteration, and prints the number of terms that it takes to reach those
   "accurate" values by checking the difference between the current values for sin
   and cos which is passed to our difference function to determine when we need to stop
   @param terms the number of terms it will take us to find our sin and cos values
   @param s our angle passed from user input for sin
   @param c our angle passed from user input for cos
   @param t our angle passed from user input for tan 
*/
void tableRow( int terms, double s, double c, double t )
{
     // Keep track of previous sin (1 has no meaning here)
     double prevSin = 0.0;
     // Keep track of previous cos
     double prevCos = 0.0;
     // Current value of sin
     double currSin = 0.0;
     // Current value of cos
     double currCos = 0.0;
     // holds value of tan
     double tan = 0.0;
 
     // Keep peforming Taylor series calculations until we have a cos and sin value
     // that do not differ by more than 10^-6 -> 0.000001
     bool check = false;
     // Use this to keep track of where we are at in the Taylor series calculation
     // this helps create the correct pattern in our loop below
     int iteration = 0;
     // Use this to save the value when we call difference(a, b) for sin values
     double resultSin = 0.0;
     // Use this to save the value when we call difference(a, b) for cos values
     double resultCos = 0.0;
 
     // Use this value to actually keep track of how many times we have looped so we can
     // calculate factorials correctly for example 7! (we need to keep track of how many
     // we have looped as well as what iteration we are on)
     int fValIter = 0;
     // Track the number of terms we have
     int trackTerms = terms;
 
     // Keep looping until our sin and cos values are accurate to within 0.000001 
     while ( check == false ) {
           // Conditional 1
           if ( iteration == ITERATION_ZERO ) {
               // Calculate sin and check
               if ( fValIter == 0 ) {
                   // sin
                   currSin = 0.0;
           
                   // cos
                   currCos = 1.0;
           
                   // tan
                   tan = ( currSin / currCos );
           
                   // NOTE in first pass we do not need to check the difference AKA the  
                   // first row of terms
               }
        
               else {
                   // sin
                   currSin = currSin + 0.0;
            
                   // cos
            
                   // first calculate the number of factorial values we need and save in 
                   // this total, use a loop, our fvalIter keeps track of the number of
                   // of times we have been through our loop in total which will tell us
                   // what factorial value we need to divide by
                   double fact = 1.0;
            
                   for ( int i = 1; i <= fValIter; i++ ) {
                       fact *= i;
                   }
            
                   // then calculate the power value (what are we raising our x value to)
                   // we need another loop here, our base is the current value of cos
                   // our power can also be found by using fValIter, which is the number
                   // of times we have been in this loop in total
                   double power = 1.0;
            
                   for ( int i = 0; i < fValIter; i++) {
                       power *= c;
                   }
            
                   // declare the currCos here
                   currCos += ( power / fact );
            
            
                   // Calculate difference for sin
                   resultSin = difference( currSin, prevSin );
            
                   if ( resultSin < 0 ) {
                       resultSin *= -1.0;
                   }
            
                   // Calculate difference fo cos
                   resultCos = difference( currCos, prevCos );
            
                   if ( resultCos < 0 ) {
                       resultCos *= -1.0;
                   }
            
                   // calculate tan here
                   tan = ( currSin / currCos );
            
                   // Check if we are done
                   if ( resultSin <= ACCURACY && resultCos <= ACCURACY ) {
                       // we are done after this iteration
                       check = true;
                   }
               }
        
               // Update the number of terms aka rows of terms we have
               trackTerms++;
        
               // print
               if ( trackTerms > DOUBLE_DIGIT_INT ) {
                   printf( "   " );
                   printf( "%d", trackTerms );
                   printf( " |  " );
                   printf( "%10.7f", currSin );
                   printf( " |  " );
                   printf( "%10.7f", currCos );
           
                   double checkTan = 0.0;
           
                   checkTan = tan;
           
                   if ( checkTan < 0 ) {
                       checkTan *= -1.0;
                   }
           
                   if ( checkTan > DOUBLE_DIGIT_DOUBLE ) {
                       printf( " | " );
                       printf( "%10.7f\n", tan );
                   }
           
                   else {
                       printf( " |  " );
                       printf( "%10.7f\n", tan );
                   }
               }
        
               else {
                   printf( "    " );
                   printf( "%d", trackTerms );
                   printf( " |  " );
                   printf( "%10.7f", currSin );
                   printf( " |  " );
                   printf( "%10.7f", currCos );
            
                   double checkTan = 0.0;
           
                   checkTan = tan;
            
                   // If tan is negative, turn it in to a positive
                   // we are checking for double digit tan values below
                   // which we will need to adjust our spacing in this case
                   if ( checkTan < 0 ) {
                       checkTan *= -1.0;
                   }
           
                   if ( checkTan > DOUBLE_DIGIT_DOUBLE ) {
                       printf( " | " );
                       printf( "%10.7f\n", tan );
                   }
           
                   else {
                       printf( " |  " );
                       printf( "%10.7f\n", tan );
                   }
               }
        
        
               // update previous values
               prevSin = currSin;
               prevCos = currCos;
           }
      
           // Conditional 2
           if ( iteration == ITERATION_ONE ) {        
               // sin
               // first calculate the number of factorial values we need and save in this
               // total, use a loop, our fValIter is the number of times we have been by
               // we can use fValIter, which is the number of times we have looped - 1,
               // because we don't count the first iteration
               double fact = 1.0;
            
               for ( int i = 1; i <= fValIter; i++ ) {
                   fact *= i;
               }
            
               // then calculate the power value (what are we raising our x value to)
               // we need another loop here, we can use fValIter again which is the 
               // number of times we have been through our loop - 1
               double power = 1.0;
            
               for ( int i = 0; i < fValIter; i++ ) {
                   power *= s;
               }
            
               // declare the currCos here
               currSin += (power / fact);
            
            
               // cos
               currCos = currCos + 0.0;
            
               // Calculate difference for sin
               resultSin = difference( currSin, prevSin );
            
               if ( resultSin < 0 ) {
                   resultSin *= -1.0;
               }
            
               // Calculate difference fo cos
               resultCos = difference( currCos, prevCos );
            
               if ( resultCos < 0 ) {
                   resultCos *= -1.0;
               }
            
               // calculate tan here
               tan = ( currSin / currCos );
            
               // Check if we are done
               if ( resultSin <= ACCURACY && resultCos <= ACCURACY ) {
                   // we are done after this iteration
                   check = true;
               }
        
               // Update the number of terms aka rows of terms we have
               trackTerms++;
        
               // print
               if ( trackTerms > DOUBLE_DIGIT_INT ) {
                   printf( "   " );
                   printf( "%d", trackTerms );
                   printf( " |  " );
                   printf( "%10.7f", currSin );
                   printf( " |  " );
                   printf( "%10.7f", currCos );
           
                   double checkTan = 0.0;
           
                   checkTan = tan;
           
                   if ( checkTan < 0 ) {
                       checkTan *= -1.0;
                   }
           
                   if ( checkTan > DOUBLE_DIGIT_DOUBLE ) {
                       printf( " | " );
                       printf( "%10.7f\n", tan );
                   }
           
                   else {
                       printf( " |  " );
                       printf( "%10.7f\n", tan );
                   }
               }
        
               else {
                   printf( "    " );
                   printf( "%d", trackTerms );
                   printf( " |  " );
                   printf( "%10.7f", currSin );
                   printf( " |  " );
                   printf( "%10.7f", currCos );
            
                   double checkTan = 0.0;
           
                   checkTan = tan;
           
                   if ( checkTan < 0 ) {
                       checkTan *= -1.0;
                   }
           
                   if ( checkTan > DOUBLE_DIGIT_DOUBLE ) {
                       printf( " | " );
                       printf( "%10.7f\n", tan );
                   }
           
                   else {
                       printf( " |  " );
                       printf( "%10.7f\n", tan );
                   }
               }
        
        
               // update previous values
               prevSin = currSin;
               prevCos = currCos;
           }
      
          // Conditional 3
          if ( iteration == ITERATION_TWO ) {
              // sin
              currSin = currSin + 0.0;
            
              // cos
            
             // first calculate the number of factorial values we need and save in this
             // total, use a loop our fValIter represents the number of times we have
             // been in our loop - 1, we can use fValIter to find our factorial
             // value we need to divide by
             double fact = 1.0;
            
             for ( int i = 1; i <= fValIter; i++ ) {
                 fact *= i;
             }
            
             // then calculate the power value (what are we raising our x value to)
             // we need another loop here we can also use fValIter to calculate our power
             // we can also use our fValIter here to calculate our power value
             // that should be in the numerator divide by the above
             // calculated factorial
             double power = 1.0;
            
             for ( int i = 0; i < fValIter; i++ ) {
                 power *= c;
             }
            
             // declare the currCos here
             currCos -= ( power / fact );
            
            
             // Calculate difference for sin
             resultSin = difference( currSin, prevSin );
            
             if ( resultSin < 0 ) {
                 resultSin *= -1.0;
             }
            
             // Calculate difference fo cos
             resultCos = difference( currCos, prevCos );
            
             if ( resultCos < 0 ) {
                 resultCos *= -1.0;
             }
            
             // calculate tan here
             tan = ( currSin / currCos );
            
             // Check if we are done
             if ( resultSin <= ACCURACY && resultCos <= ACCURACY ) {
                 // we are done after this iteration
                 check = true;
             }
        
             // Update the number of terms aka rows of terms we have
             trackTerms++;
        
             // print
             if ( trackTerms > DOUBLE_DIGIT_INT ) {
                 printf( "   " );
                 printf( "%d", trackTerms );
                 printf( " |  " );
                 printf( "%10.7f", currSin );
                 printf( " |  " );
                 printf( "%10.7f", currCos );
           
                 double checkTan = 0.0;
           
                 checkTan = tan;
           
                 if ( checkTan < 0 ) {
                     checkTan *= -1.0;
                 }
           
                 if ( checkTan > DOUBLE_DIGIT_DOUBLE ) {
                     printf( " | " );
                     printf( "%10.7f\n", tan );
                 }
           
                 else {
                     printf( " |  " );
                     printf( "%10.7f\n", tan );
                 }
             }
        
             else {
                 printf( "    " );
                 printf( "%d", trackTerms );
                 printf( " |  " );
                 printf( "%10.7f", currSin );
                 printf( " |  " );
                 printf( "%10.7f", currCos );
            
                 double checkTan = 0.0;
           
                 checkTan = tan;
           
                 if ( checkTan < 0 ) {
                     checkTan *= -1.0;
                 }
           
                 if ( checkTan > DOUBLE_DIGIT_DOUBLE ) {
                     printf( " | " );
                     printf( "%10.7f\n", tan );
                 }
           
                 else {
                     printf( " |  " );
                     printf( "%10.7f\n", tan );
                 }
             }
        
        
             // update previous values
             prevSin = currSin;
             prevCos = currCos;
          }
      
          // Conditional 4
          if ( iteration == ITERATION_THREE ) {
             // sin
             // first calculate our factorial value at this current point in the loop
             // we can use fValIter for this as it is our number of times we have
             // been through this loop - 1 as the first iteration we do not
             // add to our factorial value we are dividing by below
             double fact = 1.0;
            
             for ( int i = 1; i <= fValIter; i++ ) {
                 fact *= i;
             }
            
             // then calculate the power value (what are we raising our x value to)
             // we need another loop here, which we can use fValIter to find
             // our power value at this current point in loop which
             // should be our numerator value divided by the above
             // factorial value
             double power = 1.0;
            
             for ( int i = 0; i < fValIter; i++ ) {
                 power *= s;
             }
            
             // declare the currCos here
             currSin -= ( power / fact );
            
            
             // cos
             currCos = currCos + 0.0;
            
             // Calculate difference for sin
             resultSin = difference( currSin, prevSin );
            
             if ( resultSin < 0 ) {
                 resultSin *= -1.0;
             }
            
             // Calculate difference fo cos
             resultCos = difference( currCos, prevCos );
            
             if ( resultCos < 0 ) {
                 resultCos *= -1.0;
             }
            
             // calculate tan here
             tan = ( currSin / currCos );
            
             // Check if we are done
             if ( resultSin <= ACCURACY && resultCos <= ACCURACY ) {
                 // we are done after this iteration
                 check = true;
             }
        
             // Update the number of terms aka rows of terms we have
             trackTerms++;
        
             // print
             if ( trackTerms > DOUBLE_DIGIT_INT ) {
                 printf( "   " );
                 printf( "%d", trackTerms );
                 printf( " |  " );
                 printf( "%10.7f", currSin );
                 printf( " |  " );
                 printf( "%10.7f", currCos );
           
                 double checkTan = 0.0;
           
                 checkTan = tan;
           
                 if ( checkTan < 0 ) {
                     checkTan *= -1.0;
                 }
           
                 if ( checkTan > DOUBLE_DIGIT_DOUBLE ) {
                     printf( " | " );
                     printf( "%10.7f\n", tan );
                 }
           
                 else {
                     printf( " |  " );
                     printf( "%10.7f\n", tan );
                 }
             }
        
             else {
                 printf("    ");
                 printf("%d", trackTerms);
                 printf(" |  ");
                 printf("%10.7f", currSin);
                 printf(" |  ");
                 printf("%10.7f", currCos);
            
                 double checkTan = 0.0;
           
                 checkTan = tan;
           
                 if ( checkTan < 0 ) {
                     checkTan *= -1.0;
                 }
           
                 if ( checkTan > DOUBLE_DIGIT_DOUBLE ) {
                     printf( " | " );
                     printf( "%10.7f\n", tan );
                 }
           
                 else {
                     printf( " |  " );
                     printf( "%10.7f\n", tan );
                 }
             }
        
        
             // update previous values
             prevSin = currSin;
             prevCos = currCos;
          }
      
          // update iterations
          if ( iteration == ITERATION_THREE ) {
              iteration = 0;
          }
      
          else {
              iteration++;
          }
      
          // Update fValIter
          fValIter++;
     }
}

/**
   Program starting point, main() calls all of this file's functions in order to receive
   user input for a valid angle, and then we pass to other functions in order to accurately
   calculate our sin, cos, and tan in radians for that angle we received from the user
   @return successful exit status
*/
int main()
{ 
    // Get the angle
    double angle = getAngle();
  
    // Print the table header
    tableHeader();
 
    // Pass the angle to tableRow() for calculations
    tableRow( 0, angle, angle, angle );

    // Return successfully
    return EXIT_SUCCESS;
}
