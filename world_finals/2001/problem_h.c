/* 
   Solution to Professor Monotonic's Networks
   ACM Programming Contest World Finals 2001
   Problem H

   Bernie Pope, 2013.

   Solution test by http://uva.onlinejudge.org/index.php

   Standard Input looks like this:

   4 5
   1 2 3 4 1 3
   2 4 2 3
   8 0
   3 3
   1 2 2 3 1 2
   0 0 

   Standard Output looks like this:

   Case 1 is a sorting network and operates in 3 time units.
   Case 2 is not a sorting network and operates in 0 time units.
   Case 3 is a sorting network and operates in 3 time units.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_COMPARATORS 150
#define MAX_WIRES 12
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

typedef unsigned int uint;

typedef struct {
   uint wire1;   /* zero based */
   uint wire2;
} link;

typedef enum { False, True } bool;

link links[MAX_COMPARATORS];
uint num_wires = 0;
uint num_comparators = 0;

void print_bits(int n, uint bits)
{
    int i;
    uint bit;

    for (i = 0; i < n; i++)
    {
       bit = bits & (1 << i);
       putchar(bit ? '1' : '0');
    }
    putchar('\n');
}

uint comparator(uint wires, uint wire1, uint wire2)
{
   uint bit1_mask, bit2_mask, bit1, bit2;

   bit1_mask = 1 << wire1;
   bit2_mask = 1 << wire2;
   bit1 = wires & bit1_mask;
   bit2 = wires & bit2_mask;
   /* if the first bit is 1 and the second is 0 swap them over */
   if (bit1 && !bit2) {
      wires ^= (bit1_mask | bit2_mask);
   }
   return wires;
}

uint network(uint wires)
{
   uint i;

   for (i = 0; i < num_comparators; i++)
   {
      wires = comparator(wires, links[i].wire1, links[i].wire2);
   }
   return wires;
}

/* check if the bit string matches the pattern 0*1* */
bool monotone_increasing(uint bits)
{
   uint prev = 0, pos, bit;
    
   for (pos = 0; pos < num_wires; pos++)
   { 
      bit = bits & (1 << pos);
      if (bit < prev)
      {
         return False;
      }
      prev = bit;
   }
   return True;
}

/* maximum path in the network */
uint depth()
{
   uint max_depth = 0, i, new_depth;
   uint wire1, wire2;
   uint ds[MAX_WIRES] = {0};

   for (i = 0; i < num_comparators; i++)
   {
      wire1 = links[i].wire1;
      wire2 = links[i].wire2;
      new_depth = MAX(ds[wire1], ds[wire2]) + 1;
      ds[wire1] = ds[wire2] = new_depth;
      if (new_depth > max_depth)
      {
         max_depth = new_depth;
      }
   }
   return max_depth;
}

int main(void)
{
   uint case_count = 1, read_comparators, wire1, wire2;
   int scanned, i;
   bool sorting;
   char *decision;

   while (True)
   {
      read_comparators = 0;
      scanned = fscanf(stdin, " %u %u ", &num_wires, &num_comparators);

      if ((scanned != 2) || ((num_wires == 0) && (num_comparators == 0)))
      {
         break;
      }

      while (read_comparators < num_comparators)
      {
         scanned = fscanf(stdin, " %u %u ", &wire1, &wire2);
         if (scanned != 2)
         {
            fprintf(stderr, "Could not read two wires\n");
            exit(-1);
         }
         /*  wires are 1 based in input and 0 based in this program */
         links[read_comparators].wire1 = wire1 - 1;
         links[read_comparators].wire2 = wire2 - 1;
         read_comparators++;
      }

      sorting = True;
      for (i = 0; i < 1 << num_wires; i++)
      {
         if (! monotone_increasing(network(i)))
         {
            sorting = False;
            break;
         }
      }
      decision = sorting ? "is" : "is not";
      printf("Case %d %s a sorting network and operates in %d time units.\n", case_count, decision, depth());
      case_count++;
   }
   return 0;
}
