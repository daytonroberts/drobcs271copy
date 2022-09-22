//assignment0insertionsort

# include <stdio.h>
# include <math.h>

// A simple program to sort an array of integers in C

/* YOUR WORK HERE */

// Input: An array of integers and its length
// Output: Nothing
// Side Effect: The input array is sorted from least to greatest
void sort(int *arr, int len)
{
	int i, key, j;
	for (i = 1; i < len; i++) {
		key = arr[i];
		j = i -1;

		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j = j -1;
		}
		arr[j + 1] = key;
} // I object to this indentation
}

/* END YOUR WORK */

/* FEEDBACK FUNCTIONS */

// Input: An array of integers and its length
// Output: 1 if the array is sorted least to greatest, 0 otherwise
// Side Effect: None
int is_sorted(int *arr, int len)
{
	int i = 0;
	while (i < (len - 2))
	{
		if (arr[i] > arr[i+1])
		{
			return 0;
		}
		i++;
	}
	return 1;
}

// The special main function

int main()
{
	int test_arr[10] = {9,6,4,2,8,1,3,5,7,0};
	sort(test_arr, 10);
	for (int i = 1; i < 10; i++)
	{ printf("%d ", test_arr[i]);
	 }
	if (is_sorted(test_arr, 10))
	{
		printf("Good job!\n");
	} else {
		printf("Keep it up!\n");
	}
	return 0;
}
