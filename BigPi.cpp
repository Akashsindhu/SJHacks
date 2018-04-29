/********************************************************************************************
 * Name        : BigPi.cpp
 * Version     : 1.0
 * Description : This program will compute and print the first million decimal places of pi.
 ********************************************************************************************/

#include <iostream>
#include <iomanip>
#include <mpir.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <thread>
#include <algorithm>
#include <fstream>


using namespace std;
using namespace std::chrono;

const int MAX_ITERATIONS     = 11; /* Number of iterations required to calculate first million decimal places of pi */
const int PLACES             = 1000000; /* desired decimal places */
const int PRECISION          = PLACES + 1; /* +1 for the digit 3 before the decimal */
const int BIRTHDAYPRECISION  = 8; /* +1 for the digit 3 before the decimal */

const int BASE       = 10; /* base 10 numbers */
const int BIT_COUNT  = 8; /* bits per machine word */

const int BLOCK_SIZE = 10; /* print digits in blocks */
const int LINE_SIZE  = 100; /* digits to print per line */
const int LINE_COUNT = PLACES/LINE_SIZE; /* lines to print */
const int GROUP_SIZE = 5; /* line grouping size */

/**
 * display prints the first 1000000 digits of Pi.
 * @param Pi contains the value of Pi to be printed.
 */
void display(const mpf_t &Pi, const mpf_t &Birthday);
long find_substring(string str, string pattern);

/*
 * The main uses the Borwein algorithm to calculate digits of Pi
 */
int main()
{
	// NEW CODE - START
	string temp;
	//cout << "Enter input birthdate : ";
	cout<<"Enter the Birthday in the most minimalist form ddmmyy"<<endl;
	cin >> temp;
	const char *birthdate = temp.c_str();
	
	/* Set input to birthdate */
	mpf_t Birthdate;
	mpf_init(Birthdate);
	mpf_set_str(Birthdate, birthdate, BASE);
	// NEW CODE - END	
	
	/* Set the clock to note the start time before computation */
	steady_clock::time_point start_time = steady_clock::now();

	/* Set the precision for Pi */
	mpf_set_default_prec(BIT_COUNT*PRECISION);

	/* Calculation of a0 - START */
	mpf_t a0, aFour, aTwo, aSix, aSqrtOFTwo, aMultipliedValue; /* Declare float variables */
	mpf_init(a0); mpf_init(aFour); mpf_init(aTwo); mpf_init(aSix); mpf_init(aSqrtOFTwo); mpf_init(aMultipliedValue); /* Initialize a0 */
	mpf_set_str (aFour, "4", BASE); mpf_set_str (aTwo, "2", BASE); mpf_set_str (aSix, "6", BASE); /* Set values */
	mpf_sqrt(aSqrtOFTwo, aTwo); /* SQRT(2) */
	mpf_mul(aMultipliedValue, aFour, aSqrtOFTwo); /* Multiply values */
	mpf_sub(a0, aSix, aMultipliedValue); /* Subtract and calculate a0 */
	/* Calculation of a0 - END */

	/* Calculation of y0 - START */
	mpf_t y0, yOne; /* Declare float variables */
	mpf_init(y0); mpf_init(yOne); /* Initialize y0 */
	mpf_set_str (yOne, "1", BASE); /* Set value */
	mpf_sub(y0, aSqrtOFTwo, yOne); /* Subtract and calculate a0 */
	/* Calculation of y0 - END */

	/* This algorithm is quartic each iteration increases the number of correct digits by a factor of 4 */
	for (int i = 1; i <= MAX_ITERATIONS; i++)
	{
		/* Calculation of yi - START */
		mpf_t yToPowerFour, yOneMinusYToPowerFour, ySqrtOfFour, yNumerator, yDenominator; /* Declare float variables */
		mpf_init(yToPowerFour); mpf_init(yOneMinusYToPowerFour); mpf_init(ySqrtOfFour); mpf_init(yNumerator); mpf_init(yDenominator);
		mpf_pow_ui(yToPowerFour, y0, 4u); /* Calculate to the power four value as per formula */
		mpf_sub(yOneMinusYToPowerFour, yOne, yToPowerFour); /* Perform substraction as per formula */
		mpf_sqrt(ySqrtOfFour, yOneMinusYToPowerFour); mpf_sqrt(ySqrtOfFour, ySqrtOfFour); /* Perform sqrt as per formula as per formula */
		mpf_sub(yNumerator, yOne, ySqrtOfFour); /* Calculate numerator as per formula */
		mpf_add(yDenominator, yOne, ySqrtOfFour); /* Calculate denominator as per formula */
		mpf_div(y0, yNumerator, yDenominator); /* Calculate y0 as per formula */
		/* Calculation of yi - END */

		/* Calculation of ai - START */
		mpf_t aOnePlusYOfi, aOnePlusYOfiToPowerFour, aPart1; /* Declare float variables */
		mpf_init(aOnePlusYOfi); mpf_init(aOnePlusYOfiToPowerFour); mpf_init(aPart1); /* Declare float variables */
		mpf_add(aOnePlusYOfi, yOne, y0); /* Perform addition as per formula */
		mpf_pow_ui(aOnePlusYOfiToPowerFour, aOnePlusYOfi, 4u); /* Calculate to the power four value as per formula */
		mpf_mul(aPart1, aOnePlusYOfiToPowerFour, a0); /* Calculate part 1 of a as per formula */

		mpf_t aYPowerTwo, aPart2; /* Declare float variables */
		mpf_init(aYPowerTwo); mpf_init(aPart2); /* Declare float variables */
		mpf_pow_ui(aYPowerTwo, y0, 2u); /* Calculate to the power two value as per formula */
		mpf_add(aPart2, aOnePlusYOfi, aYPowerTwo); /* Calculate part 2 of a as per formula */

		mpf_mul(aPart2, y0, aPart2); /* Calculate part 2 of a as per formula */

		mpf_t aTwoToThePowerValue, aTwoToThePower; /* Declare float variables */
		mpf_init(aTwoToThePowerValue); mpf_init(aTwoToThePower); /* Declare float variables */
		double twoToThePowerValue = (2 * i) + 1; /* Calculate the power value which changes with i */
		mpf_pow_ui(aTwoToThePower, aTwo, twoToThePowerValue); /* Calculate to the power two value as per formula */

		mpf_mul(aPart2, aTwoToThePower, aPart2); /* Calculate part 2 of a as per formula */

		mpf_sub(a0, aPart1, aPart2); /* Calculate y0 as per formula */
		/* Calculation of ai - END */
	}

	/* Calculate Pi */
	mpf_t Pi; /* Declare float variable to hold value of Pi */
	mpf_init(Pi); /* Initialize Pi */
	mpf_div(Pi, yOne, a0); /* Take inverse of Pi */

	cout << "pi to "<< PLACES << " places:" << endl << endl;

	/* Stop the clock to note the end time after computation */
	steady_clock::time_point end_time = steady_clock::now();
	//sleep_until(system_clock::now() + 25s);
	display(Pi, Birthdate); /* Display Pi value */

  	//return 0;
	cout << "Elapsed time: " << (duration_cast<microseconds>(end_time - start_time).count() * 0.000001) << " seconds" << endl;
	return 0;
}

void display(const mpf_t &Pi, const mpf_t &Birthday)
{
	char *pointer = NULL;
	char *stringReference;
	char *birthdayReference;
	mp_exp_t expptr;
	birthdayReference = mpf_get_str(pointer, &expptr, BASE, BIRTHDAYPRECISION, Birthday);
	stringReference = mpf_get_str(pointer, &expptr, BASE, PRECISION, Pi);
	string mainString(stringReference), birthdayString(birthdayReference);
	
	//cout << stringReference[0] << ".";
	long pos = 0;
	ofstream myfile;
  	myfile.open ("example.txt");
	myfile << mainString;
  	myfile.close();
	string STRING;
	ifstream infile;
	//infile.open ("example.txt");
        //while(!infile.eof()) // To get you all the lines.
        //{
	  //      getline(infile,STRING); // Saves the line in STRING.
	        //cout<<STRING; // Prints our STRING.
        //}
	//infile.close();
	pos = find_substring(mainString,birthdayString);
	//pos = find_substring(STRING,birthdayString);

	cout<<"Birthday found at "<<pos<<endl;
//cout<<mainString<<endl;
//pos = mainString.find(birthdayString);
//	std::string::iterator it_mainString_begin;
//	std::string::iterator it_mainString_end;
//	std::string::iterator it_birthdayString_begin;
//	std::string::iterator it_birthdayString_end;
//	std::string::iterator birthdayIt;
//	std::string::iterator position;
//	it_mainString_begin = mainString.begin();
//	it_mainString_end = mainString.end();
//	it_birthdayString_begin = birthdayString.begin();
//	it_birthdayString_end = birthdayString.end();
//	//position = std::search(it_mainString_begin,it_mainString_end,it_birthdayString_begin,it_birthdayString_end);
//	while(position!=it_mainString_end)
//	{
//		if(*position == *it_birthdayString_begin){
//			while(*position)
//		}
//	}
//	
//	if (position!=birthdayString.end())
//	{
//		cout << "Substring found at index " << it_mainString_begin-position;
//	}
//	else
//	{
//		cout << "Substring not found at all in parent string" << endl;
//	}
/*	for(int i = 1; i <= PRECISION; i++)
	{
		cout << stringReference[i];
		if(i % (LINE_SIZE * GROUP_SIZE) == 0)
			cout << endl << endl << "  ";
		if((i % (LINE_SIZE) == 0) && (i % (LINE_SIZE * GROUP_SIZE) != 0))
			cout << endl << "  ";
		if((i % BLOCK_SIZE == 0) && (i % (LINE_SIZE) != 0) && (i % (LINE_SIZE * GROUP_SIZE) != 0))
			cout << " ";
	}*/
}

long find_substring(string str, string pattern) {

	// Step 0. Should not be empty string
	if( str.size() == 0 || pattern.size() == 0)
		return -1;

	// Step 1. Compute failure function
	long failure[pattern.size()];
	std::fill( failure, failure+pattern.size(), -1);

	for(long r=1, l=-1; r<pattern.size(); r++) {

		while( l != -1 && pattern[l+1] != pattern[r])
			l = failure[l];

		// assert( l == -1 || pattern[l+1] == pattern[r]);
		if( pattern[l+1] == pattern[r])
			failure[r] = ++l;
	}

	// Step 2. Search pattern
	long tail = -1;
	for(long i=0; i<str.size(); i++) {

		while( tail != -1 && str[i] != pattern[tail+1])
			tail = failure[tail];

		if( str[i] == pattern[tail+1])
			tail++;

		if( tail == pattern.size()-1)
			return i - tail;
	}

	return -1;
}

