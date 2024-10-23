#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "simd_string.h"

clock_t startm, stopm;
#define START                      \
  if ((startm = clock()) == -1)    \
  {                                \
    printf("Error calling clock"); \
    exit(1);                       \
  }
#define STOP                       \
  if ((stopm = clock()) == -1)     \
  {                                \
    printf("Error calling clock"); \
    exit(1);                       \
  }
#define PRINTTIME(msg) printf("%6.3f seconds used by the processor by %s version.\n", ((double)stopm - startm) / CLOCKS_PER_SEC, msg);

// Function to generate random strings
void generate_random_string(char *str, size_t length)
{
  for (size_t i = 0; i < length; i++)
  {
    str[i] = 'A' + rand() % 26; // Generate a random character between 'A' and 'Z'
  }
  str[length] = '\0'; // Null-terminate the string
}

#define TEST_CASES 10           // Number of test cases
#define STRING_LENGTH 100000000 // Length of each string

int main()
{
  srand((unsigned int)time(NULL)); // Seed for random number generation

  // Create an array of strings for testing
  char *test_strings[TEST_CASES][2]; // Each test case has two strings for comparison

  // Initialize test strings
  for (int i = 0; i < TEST_CASES; i++)
  {
    // Allocate memory for both strings in the test case
    test_strings[i][0] = (char *)malloc(STRING_LENGTH + 1);
    test_strings[i][1] = (char *)malloc(STRING_LENGTH + 1);

    // Generate random strings
    generate_random_string(test_strings[i][0], STRING_LENGTH);
    if (i % 2 == 0)
    { // Make every even-indexed test case equal
      strcpy(test_strings[i][1], test_strings[i][0]);
    }
    else
    { // Make every odd-indexed test case unequal
      generate_random_string(test_strings[i][1], STRING_LENGTH);
    }
  }

  START;
  for (int i = 0; i < TEST_CASES; i++)
  {
    simd_str simd_str1, simd_str2;
    simd_str_init(&simd_str1, test_strings[i][0], STRING_LENGTH);
    simd_str_init(&simd_str2, test_strings[i][1], STRING_LENGTH);

    int simd_equal = simd_str_eq(&simd_str1, &simd_str2);

    simd_str_free(&simd_str1);
    simd_str_free(&simd_str2);
  }
  STOP;
  PRINTTIME("SIMD");

  printf("\n\n");

  START;
  for (int i = 0; i < TEST_CASES; i++)
  {
    int std_equal = strcmp(test_strings[i][0], test_strings[i][1]) == 0;
  }
  STOP;
  PRINTTIME("NOSIMD");

  return 0;
}
