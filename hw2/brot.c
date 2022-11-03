#include <stdlib.h>
#include <stdio.h>
#include "complex.h" 
 
// rip from https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C
// try "convert x.ppm x.png" and follow the install instructions to get a png

// In C, we make a 3 dimension unsigned char array of size * size * 3
// Hint - how many times should loop? How many times should you call malloc?
unsigned char ***create_base(int size)
{
	unsigned char *** array = (unsigned char ***)malloc(size*sizeof(unsigned char**));
	for (int i = 0; i < size; i++) 
	{
		array[i] = (unsigned char **) malloc(size*sizeof(unsigned char *));
		for (int j = 0; j < size; j++) 
		{
			array[i][j] = (unsigned char *)malloc(3*sizeof(unsigned char));
			for (int k = 0; k < 3; k++) 
			{
				array[i][j][k] = 0;
			}
		}
	}
	return array;
}

// Calculate z_(n+1) = z_n^2 + c and return the result
// in C, we accept two complex values and produce a complex output
// I've included sample code to work with complex values.
// Hint - don't use exponentiation
double complex m_seq(double complex z_n, double complex c)
{
	double a = creal(z_n), b = cimag(z_n); 
	double d = creal(c), e = cimag(z_n);
	double complex r = (a * a - b * b + d) + (2 * a * b * I + e);
	return r;
}

// in C we accept a complex value and an integer size and two integer pointers, and populate the integer points with the x and y results
// I've included sample code to zero out x and y.
void c2b(double complex c, int size, int *x, int *y)
{
	*x = (creal(c)+2) * size / 4;
	*y = (cimag(c)+2) * size / 4;
	return;
}

// in C, we use b2c to loop over all pixels rather than relying on randomization
// return the complex associated with a location x, y
// I've included sample code to work with complex values.
double complex b2c(int size, int x, int y)
{
	double a = x * 4 / size - 2, b = y * 4 / size - 2;
	double complex r = a + b * I;
	return r;
}

// in C we accept a complex value, and integer number of iterations, and returns with an int that represents whether c escapes, or exceeds absolute value 2 in iters applications of m_seq.
// I included the absolute value sample code
int escapes(double complex c, int iters)
{
	double complex z_n = c;
	for (int i = 0; i <= iters; i++) {
		z_n = m_seq(z_n, c);
		if (cabs(z_n) > 2) {
			return 1;
		}
	}
	return 0;
}

// in C, we accept a 3d array base, an integer for size and for iterations, a color channel of 0,1,2, and a complex value c
void one_val(unsigned char ***base, int size, int iters, int color, double complex c)
{
	double complex z_n = c;
	if (!escapes(c, iters)) {
		return;
	}
	for (int i = 0; i <= iters; i++) {
		if (cabs(z_n) > 2) {
			return;
		}
		int *x;
		int *y;
		printf("hellar");
		c2b(z_n, size, x, y);
		int v = base[*x][*y][color];
		printf("hellar");
		v += 25;
		if (v > 255){
			v = 255;
		}
		base[*x][*y][color] = v;
		z_n = m_seq(z_n,c);
	}
	return;
}

// in C, we accept a 3d array base, an integer for size and for iterations
void get_colors(unsigned char ***base, int size, int iters)
{
	int i_lst[3] = {iters, iters * 10, iters * 100};
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			for (int i = 0; i < 3; i++) {
				one_val(base, size, i_lst[i], i, b2c(x,y,size));
			}
		}
	}
	return;
}

// OPTIONAL
// That said, you images will look bad without this.
// In Python, we had a hacked together solution to normalizing counts, now we formalize it
// We accept a base, and normalize values to percentiles rather than counts
void normalize(unsigned char ***base, int size)
{
	return;
}
 
// Given an edge size and starting iteration count, make a buddhabrot.
// I'm leaving the ppm code
void make_brot(int size, int iters)
{
	FILE *fp = fopen("brot.ppm", "wb"); /* b - binary mode */
	fprintf(fp, "P6\n%d %d\n255\n", size, size);
	static unsigned char color[3];
	unsigned char ***base = create_base(size);
	get_colors(base, size, iters);
	fflush(stdout);
	for ( int x = 0 ; x < size ; x++ )
	{
		for ( int y = 0 ; y < size ; y++ )
		{

			fwrite(base[x][y], 1, 3, fp);
		}
	}
	fclose(fp);
	return;
}

 
int main()
{
	printf("check");
	make_brot(4000,50);
	return 0;
}
