#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<ctype.h>
#include<string.h>
#include<getopt.h>
#include "sorter.h"
#include<pthread.h>
#include<stddef.h>


//int readcsv(const char *filePath, const char *columnName, const char *dirSorted, int numCol)
void * readcsv(void * args)
{
	/**********************************************
	****** Define and Initialize variables ********
	**********************************************/

	//Deconstruct args
	struct readcsvStruct * fileInfo = (struct readcsvStruct *) args;
	char * filePath  = fileInfo->csv_path;
	int numCol = fileInfo->colCount;
	int colIndex = fileInfo->colIndex;
	int movieCol = fileInfo->mov_tit_index;
	//End deconstruction of arg

//	const char *columnName = "test";
//	const char *dirSorted = ".";

	//create struct to save rows in
	struct defineRow * myRow = (struct defineRow *) malloc (MAXROW * sizeof(struct defineRow));
	if (myRow == NULL)
	{
		printf("Cannot allocate memory for myRow");
	}

	int iString = 0, iField = 0, iCol = 0, iRow = 0,	// iString, iField, iCol, iRow = indexes to traverse the String, Field, Column & Row (respectively)
	totalRows = 0, 						// Counts how many rows are scanned in
//	colIndex = -1,						// Column index to be used to pull the info for sorting
	isNumber=0, isLetter=0, isPeriod=0;			// isNumber, Letter, or Period is used to count the type of character

	enum varType colType = EMPTY;				// colType determines what type is contained in the Sort Column

	FILE *fp;						// file pointer

	size_t bufsize = MAXBUF * sizeof(char);			// buffer size in order to bring in data from stdin
	char * buffer = (char*)malloc(bufsize*sizeof(char));	// buffer for reading in standard in
	if (buffer == NULL)					// Test to ensure buffer is not NULL
	{
		perror("Unable to allocate buffer");
		exit(1);
	}

//	char * p_firstRow;		// pointer to the column Name
//	char * p_comma;		// pointer to the comma
//
//	char * firstRow = (char*)malloc(bufsize*sizeof(char));	// buffer for reading in standard in
//	if (firstRow == NULL)					// Test to ensure buffer is not NULL
//	{
//		perror("Unable to allocate firstRow");
//		exit(1);
//	}
//
//	int movieCol = -1;			// column of the movie
//	ptrdiff_t diff;
//
//	char * test = (char*)malloc(MAXFIELDLEN*sizeof(char));
//	if (test == NULL)
//	{
//		printf("Could not allocate memory for test");
//		exit(1);
//	}

	/*************************************************
	******* End of Variables & Initialization ********
	*************************************************/

	/*******************************************************************
	******** Get and Store Header Data into firstRow*********** ********
	*******************************************************************/

//	pthread_mutex_lock(&lockTwo);

	fp = fopen(filePath, "r");						// open a file pointer in read mode
	if(fp == NULL)								// if the file pointer is Null (didn't work), print error and exit
	{
		printf("file cannot be opened.  Program will not exit");
		exit(1);
	}

	getline(&buffer, &bufsize, fp);						// put the first line of the file (header) into a buffer

//	fflush(stdout);

//	strncpy(firstRow, buffer, bufsize);
//	p_firstRow = firstRow;
//
//	for (iCol = 0; iCol < numCol-1; iCol++)
//	{
//		p_comma = strchr(p_firstRow,',');
//		diff = p_comma - p_firstRow + 1;
//		snprintf(test, diff, "%s%s", p_firstRow, '\0');
//
//		if(strcmp(test, "movie_title") == 0)
//		{
//			movieCol = iCol;
//		}
//		if(strcmp(test, columnName) == 0)
//		{
//			colIndex = iCol;
//			break;
//		}
//		p_firstRow = (p_comma + 1);
//	}
//	if (colIndex == -1)
//	{
//		p_comma = strchr(p_firstRow,'\n');
//		diff = p_comma - p_firstRow + 1;
//		snprintf(test, diff, "%s%s", p_firstRow, '\0');
//
//		if(strcmp(test, columnName) == 0)
//		{
//			colIndex = iCol;
//		}
//		if(strcmp(test, "movie_title") == 0)
//		{
//			movieCol = iCol;
//		}
//	}
//
//	free (test);

	/*******************************************
	******** End Getting Header Data ***********
	********************************************/


	/**************************************************************************
	*** This section pulls in the file line by line and temporarily stores	***
	*** into buffer.  The data looks for double quotes (") in order to		***
	*** parse out the movie titles.  The data uses commas to tokenize all 	***
	*** other columns.   All data gets stored into a Row Struct.			***
	*** At the same time we are bringing in the rows, we are splitting out 	***
	*** the column to sort & lowercasing it, and/or turning it into a #		***
	**************************************************************************/ 

//printf("WORKS-1 %s\n", filePath);

	// There are 3 versions of parsing.
	// FIRST WAY TO PARSE: Where movie_title does not exist (or it is after the column we want to sort)

//	if(movieCol == -1 || movieCol > colIndex)							// If the movie title column is AFTER the column we want to sort - do the following:
//	{
		iRow=0;							// set Row to 0, beause we will go through all rows

		while (getline(&buffer, &bufsize, fp) != -1)			// pull in each row - line for line, until there are no more lines
		{
			if(*(buffer) == EOF)
			{
				goto sorting;
			}
//printf("Row: %d ... FilePath: %s ... Buffer: %s\n", iRow, filePath, buffer);

//			fflush(fp);
			iString = 0;						// set iString to 0 (index to traverse the buffer string / wholerow string)
			iCol = 0;						// set iCol (the number of columns to go through) to 0

		
			/* **************************************************************** *
			 * *** While Loop - While the number of columns are less than	*** *
			 * *** the column index (the column we want to sort), just	*** *
			 * *** store the characters coming in, and count the commas.	*** *
			 * **************************************************************** */

			while (iCol < colIndex)					// while the number of columns are less than the column index (the column we want to sort), just store the characters coming in, and count the commas
			{
				if(*(buffer+iString) == '\"')
				{
//				printf("Inside Quote: %c", *(buffer+iString));
					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
					iString++;
					while (*(buffer+iString) != '\"')
					{
//						printf("%c", *(buffer+iString));
						if(*(buffer+iString) == '\n')
						{
							printf("Error: Opening double quote found without a closing quotation.");
							exit(1);
						}
						*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
						iString++;
					}
//					printf("%c\n", *(buffer+iString));
					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
					iString++;
				}									
				else if(*(buffer+iString) == ',')			// look for commas (to increment iCol)
				{
					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store all chars coming in
					iCol++;							// if you found a comma, increment the column (iCol)
					iString++;						// also point to the next spot on the buffer (and where you're storing the whole row)
				}
				else								// else if you did not find a comma, then just store the char & point to the next spot
				{
					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
					iString++;
				}
			}

			/* **************************************************************** *
			 * *** Once you are through with the while loop, you know you	*** *
			 * *** pointing at the first char of the column to be sorted	*** *
			 * *** so we are going to bring in this column, and also test	*** *
			 * *** if for various things to know if it's a string vs a num,	*** *
			 * *** and lowercase the values as well as remove any chars	*** *
			 * *** which are NOT a number (including peroods) or a letter.	*** *
			 * *** This will get stored in stringData & numData.		*** *
			 * **************************************************************** */  

			iField = 0;									// iField is used to traverse through stringData
			if(*(buffer+iString) == ',')							// if we start at a comma, we know the column has nothing in it
			{
				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);			// store the comma in the whole row
				*(myRow[iRow].stringData+iField) = '\0';				// store Null in stringData
				myRow[iRow].numData = 0;						// store a zero in numData
				iString++;								// increment where we are pointing to
			}
			else if(*(buffer+iString) == '\n' && *(buffer+iString) == EOF)			// if we start at a newline, then we know the column has nothing in it
			{
				*(myRow[iRow].stringData+iField) = '\0';				// store Null in stringData
				myRow[iRow].numData = 0;						// store a zero in numData
													// NOTE: We do not increment the code - so it skips over next part
													// 	 And we do not store the newline - because we do it at the end
				if(*(buffer+iString) == EOF)
				{
					*(myRow[iRow].wholeRow+iString) = '\n';
					totalRows++;
					goto sorting;
				}
			}
			else
			{
				while (*(buffer+iString) != ',' && *(buffer+iString) != '\n')		// while loop - to store column & test the data until we hit a comma or a newline
				{
					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);		// store character we are pointing to in whole row to preserve for priting

	
					if(*(buffer+iString) == '\"')			// since we found a quote - we know the string goes until the next quotation
					{
						isLetter++;
						iString++;
						while(*(buffer+iString) != '\"')	// look for a second quotation
						{
							if (*(buffer+iString) == '\n')		// if you find a newline before you fine a quote, exit - because the data is incorrect
							{
								printf("Error: movie_title column should have a closing quotion mark around the title.\n");
								printf("Please check incoming data (Row %d) and try again.\n", iRow+1);
								exit(1);
							}
							else
							{
								*(myRow[iRow].wholeRow+iString) = *(buffer+iString);		// store character we are pointing to in whole row to preserve for printing
								isLetter++;					// we call this a letter - because we know that the movie_title is a string (even if this isn't actually a letter)

								if(isalpha(*(buffer+iString)) != 0)				// See if this is a letter, if so - lowercase it and store it
								{
									*(myRow[iRow].stringData+iField) = tolower(*(buffer+iString));
									iField++;						// increment iField & iString
									iString++;
								}
								else if(isdigit(*(buffer+iString)) != 0)			// See if this is a number, if it is - store it
								{
									*(myRow[iRow].stringData+iField) = *(buffer+iString);
									iField++;						// increment iField & iString
									iString++;
								}
								else if(isblank(*(buffer+iString)) != 0)
								{
									*(myRow[iRow].stringData+iField) = *(buffer+iString);
									iField++;						// increment iField & iString
									iString++;
								}
								else								// if this is anything else - spaces, random characters, etc..... then:
								{
									iString++;						// ... we just increment the string; We do not want to store this in stringData
								}
							}
						}
						*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store the quote in the wholeRow
						iString++;						// point to the next spot on the buffer
						*(myRow[iRow].stringData+iField) = '\0';		// Store a Null in stringData so the string is Null terminated
					}
					else
					{	
						if(isalpha(*(buffer+iString)) != 0)				// See if this is a letter
						{
							isLetter++;						// if it's a letter, isLetter++ & store the lowercase version into stringData
							*(myRow[iRow].stringData+iField) = tolower(*(buffer+iString));
							iField++;						// increment iField & iString
							iString++;
						}
						else if(isdigit(*(buffer+iString)) != 0)			// See if this is a number
						{
							isNumber++;						// if it's a number, isNumber++ & store it into stringData
							*(myRow[iRow].stringData+iField) = *(buffer+iString);
							iField++;						// increment iField & iString
							iString++;
						}
						else if(isblank(*(buffer+iString)) != 0)
						{
							*(myRow[iRow].stringData+iField) = *(buffer+iString);
							iField++;						// increment iField & iString
							iString++;
						}
						else if(*(buffer+iString) == '.' && isLetter == 0)		//  See if this is a period (ONLY if isLetter is 0), Note: if we KNOW there is a letter - then we want to ignore periods, as this is a string  
						{
							isPeriod++;						// if this is a period isPeriod++ & store the period into the stringData
							*(myRow[iRow].stringData+iField) = *(buffer+iString);
							iField++;						// increment iField & iString
							iString++;
						}
						else								// if this is anything else - spaces, random characters, etc..... then:
						{
							iString++;						// ... we just increment the string; We do not want to store this in stringData
						}
					}
				}	
	
				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);			// Once out of the while loop - we store the comma or newline
				if(*(buffer+iString) != '\n')						// if this is NOT a newline, then we increment the string pointer
					iString++;
				*(myRow[iRow].stringData+iField) = '\0';				// Store a Null in stringData so the string is Null terminated
	
														// Once through storing the column, we determine the type
														// based on leters, numbers & periods
				if (isLetter >= 1)								// If there is at least 1 letter - we know this is a string
				{
					colType = STRING;
				}
				else if(colType != STRING && isNumber >= 0 && (isPeriod == 0 | isPeriod == 1))	// If there are numbers and/or periods, then it's a number
				{
					colType = DOUBLE;
					myRow[iRow].numData = atof(myRow[iRow].stringData);			// use atof to change string into double
				}

				isLetter = 0;			// change isLetter, isNumber & isPeriod back to 0 (for next row testing)
				isNumber = 0;
				isPeriod = 0;

			}

			/* **********************************************************
			 * *** After pulling out the column to be testing, we	*** *
			 * *** can continue to store the rest of the chars in 	*** *
			 * *** the whole row.					*** *
			 * ******************************************************** */
			
			while(*(buffer+iString) != '\n' && *(buffer+iString) != EOF && *(buffer+iString) != '\r' && iString < (bufsize-2))
			{
				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
				iString++;
			}
			if(*(buffer+iString) == '\n')
			{
				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);		// store the newline
				if(*(buffer + iString+ 1) == EOF)
				{
					totalRows++;
					goto sorting;
				}
			}
			else if(*(buffer+iString) == EOF)
			{
				*(myRow[iRow].wholeRow+iString) = '\n';
				totalRows++;
				goto sorting;
			}

			iRow++;
			totalRows++;
			memset (buffer, '\0', bufsize);	

		}
//	}



//	// There are 3 versions of parsing.
//	// SECOND WAY TO PARSE: Where movie_title IS the column we want to sort
//
//	else if(movieCol == colIndex)							// If the movie title column is AFTER the column we want to sort - do the following:
//	{
//		colType = STRING;					// We know that movie column is a string - so we can just set this up front.
//		iRow=0;								// set Row to 0, beause we will go through all rows
//		fflush(stdin);
//		fflush(stdout);
//		while (getline(&buffer, &bufsize, fp) != -1)			// pull in each row - line for line, until there are no more lines
//		{
//			iString = 0;						// set iString to 0 (index to traverse the buffer string / wholerow string)
//			iCol = 0;						// set iCol (the number of columns to go through) to 0
//
//			/* **************************************************************** *
//			 * *** While Loop - While the number of columns are less than	*** *
//			 * *** the column index (the column we want to sort), just	*** *
//			 * *** store the characters coming in, and count the commas.	*** *
//			 * **************************************************************** */
//
//			while (iCol < colIndex)					// while the number of columns are less than the column index (the column we want to sort), just store the characters coming in, and count the commas
//			{				
//				if(*(buffer+iString) == ',')			// look for commas (to increment iCol)
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store all chars coming in
//					iCol++;							// if you found a comma, increment the column (iCol)
//					iString++;						// also point to the next spot on the buffer (and where you're storing the whole row)
//				}
//				else								// else if you did not find a comma, then just store the char & point to the next spot
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
//					iString++;
//				}
//			}
//
//			/* **************************************************************** *
//			 * *** Once you are through with the while loop, you know you	*** *
//			 * *** pointing at the first spot of column to be sorted, 	*** *
//			 * *** so we are going to bring in this column.  We know this 	*** *
//			 * *** column is a string, so we can do less testing, and just 	*** *
//			 * *** lowercase the values as well as remove extra characters	*** *
//			 * *** which are NOT a number or a letter.	We have to look 	*** *
//			 * *** for quotes to make sure we captre the whole title. 	*** *
//			 * **************************************************************** */  
//
//
//			iField = 0;									// iField is used to traverse through stringData
//			if(*(buffer+iString) == ',')							// if we start at a comma, we know the column has nothing in it
//			{
//				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);			// store the comma in the whole row
//				*(myRow[iRow].stringData+iField) = '\0';				// store Null in stringData
//				myRow[iRow].numData = 0;						// store a zero in numData
//				iString++;								// increment where we are pointing to
//			}
//			else if(*(buffer+iString) == '\n')						// if we start at a newline, then we know the column has nothing in it
//			{
//				*(myRow[iRow].stringData+iField) = '\0';				// store Null in stringData
//				myRow[iRow].numData = 0;						// store a zero in numData
//													// NOTE: We do not increment the code - so it skips over next part
//			}										// 	 And we do not store the newline - because we do it at the end
//			else if(*(buffer+iString) == '\"')			// since we found a quote - we know the string goes until the next quotation
//			{
//				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store the quote in the wholeRow
//				iString++;
//				while(*(buffer+iString) != '\"')						// look for a second quotation
//				{
//					if (*(buffer+iString) == '\n')			// if you find a newline before you fine a quote, exit - because the data is incorrect
//					{
//						printf("Error: movie_title column should have a closing quotion mark around the title.\n");
//						printf("Please check incoming data (Row %d) and try again.\n", iRow+1);
//						exit(1);
//					}
//					else
//					{
//						*(myRow[iRow].wholeRow+iString) = *(buffer+iString);		// store character we are pointing to in whole row to preserve for printing
//
//						if(isalpha(*(buffer+iString)) != 0)				// See if this is a letter, if so - lowercase it and store it
//						{
//							*(myRow[iRow].stringData+iField) = tolower(*(buffer+iString));
//							iField++;						// increment iField & iString
//							iString++;
//						}
//						else if(isdigit(*(buffer+iString)) != 0)			// See if this is a number, if it is - store it
//						{
//							*(myRow[iRow].stringData+iField) = *(buffer+iString);
//							iField++;						// increment iField & iString
//							iString++;
//						}
//						else								// if this is anything else - spaces, random characters, etc..... then:
//						{
//							iString++;						// ... we just increment the string; We do not want to store this in stringData
//						}
//					}
//				}
//				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store the quote in the wholeRow
//				iString++;						// point to the next spot on the buffer
//				*(myRow[iRow].stringData+iField) = '\0';				// Store a Null in stringData so the string is Null terminated
//			}
//			else
//			{
//				while (*(buffer+iString) != ',' && *(buffer+iString) != '\n')		// while loop - to store column & test the data until we hit a comma or a newline
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store the character in the wholeRow
//					if(isalpha(*(buffer+iString)) != 0)				// See if this is a letter, if so - lowercase it and store it
//					{
//						*(myRow[iRow].stringData+iField) = tolower(*(buffer+iString));
//						iField++;						// increment iField & iString
//						iString++;
//					}
//					else if(isdigit(*(buffer+iString)) != 0)			// See if this is a number, if it is - store it
//					{
//						*(myRow[iRow].stringData+iField) = *(buffer+iString);
//						iField++;						// increment iField & iString
//						iString++;
//					}
//					else								// if this is anything else - spaces, random characters, etc..... then:
//					{
//						iString++;						// ... we just increment the string; We do not want to store this in stringData
//					}
//				}
//
//				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);			// Once out of the while loop - we store the comma or newline
//				if(*(buffer+iString) != '\n')						// if this is NOT a newline, then we increment the string pointer
//					iString++;
//				*(myRow[iRow].stringData+iField) = '\0';				// Store a Null in stringData so the string is Null terminated
//			}
//
//
//			/* **********************************************************
//			 * *** After pulling out the column to be testing, we	*** *
//			 * *** can continue to store the rest of the chars in 	*** *
//			 * *** the whole row.					*** *
//			 * ******************************************************** */
//	
//			while(*(buffer+iString) != '\n')
//			{
//				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
//				iString++;
//			}
//			*(myRow[iRow].wholeRow+iString) = *(buffer+iString);		// store the newline
//
//			iRow++;
//			totalRows++;
//			memset (buffer, '\0', bufsize);	
//		}			
//	}
//
//	// There are 3 versions of parsing.
//	// THIRD WAY TO PARSE: Where movie_title is BEFORE the column we want to sort
//
//	else if(movieCol < colIndex)							// If the movie title column is AFTER the column we want to sort - do the following:
//	{
//		iRow=0;								// set Row to 0, beause we will go through all rows
//		fflush(stdin);
//		fflush(stdout);
//		while (getline(&buffer, &bufsize, fp) != -1)			// pull in each row - line for line, until there are no more lines
//		{
//			iString = 0;						// set iString to 0 (index to traverse the buffer string / wholerow string)
//	
//
//			iCol = 0;						// set iCol (the number of columns to go through) to 0
//
//			/* **************************************************************** *
//			 * *** While Loop - While the number of columns are less than	*** *
//			 * *** the movieCol index (the movie_title column), just	*** *
//			 * *** store the characters coming in, and count the commas.	*** *
//			 * **************************************************************** */
//
//			while (iCol < movieCol)					// while the number of columns are less than the column index (the column we want to sort), just store the characters coming in, and count the commas
//			{				
//				if(*(buffer+iString) == ',')			// look for commas (to increment iCol)
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store all chars coming in
//					iCol++;							// if you found a comma, increment the column (iCol)
//					iString++;						// also point to the next spot on the buffer (and where you're storing the whole row)
//				}
//				else								// else if you did not find a comma, then just store the char & point to the next spot
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
//					iString++;
//				}
//			}
//
//			/* ************************************************************ *
//			 * *** If/Else & Loops - Look for first quote, then we look for	*** *
//			 * *** the second quotation, and we store all those chars	*** *
//			 * ************************************************************ */
//
//	
//			while(*(buffer+iString) != ',')
//			{
//				if(*(buffer+iString) == '\"')			// since we found a quote - we know the string goes until the next quotation
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store the quote in the wholeRow
//					iString++;
//					while(*(buffer+iString) != '\"')						// look for a second quotation
//					{
//						if (*(buffer+iString) == '\n')			// if you find a newline before you fine a quote, exit - because the data is incorrect
//						{
//							printf("Error: movie_title column should have a closing quotion mark around the title.\n");
//							printf("Please check incoming data (Row %d) and try again.\n", iRow+1);
//							exit(1);
//						}
//						else
//						{
//							*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store the character in the wholeRow
//							iString++;
//						}
//					}
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store the quote in the wholeRow
//					iString++;
//				}
//				else
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store the character in the wholeRow
//					iString++;
//				}
//			}
//			
//			*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store the comma in the wholeRow
//			iString++;
//			
//
//			/* ************************************************************ *
//			 * *** Now that we've passed the movie title - we want to 	*** *
//			 * *** look for the index column, so we start counting  	*** *
//			 * *** from movie column # since colIndex is > movieCol 	*** *
//			 * ************************************************************ */
//
//			iCol = movieCol+1;			// set iCol to movieCol to start counting from there
//			while (iCol < colIndex)					// while the number of columns are less than the column index (the column we want to sort), just store the characters coming in, and count the commas
//			{				
//				if(*(buffer+iString) == ',')			// look for commas (to increment iCol)
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);	// store all chars coming in
//					iCol++;							// if you found a comma, increment the column (iCol)
//					iString++;						// also point to the next spot on the buffer (and where you're storing the whole row)
//				}
//				else								// else if you did not find a comma, then just store the char & point to the next spot
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
//					iString++;
//				}
//			}
//
//			/* **************************************************************** *
//			 * *** Once you are through with the while loop, you know you	*** *
//			 * *** pointing at the first char of the column to be sorted	*** *
//			 * *** so we are going to bring in this column, and also test	*** *
//			 * *** if for various things to know if it's a string vs a num,	*** *
//			 * *** and lowercase the values as well as remove any chars	*** *
//			 * *** which are NOT a number (including peroods) or a letter.	*** *
//			 * *** This will get stored in stringData & numData.		*** *
//			 * **************************************************************** */  
//
//			iField = 0;									// iField is used to traverse through stringData
//			if(*(buffer+iString) == ',')							// if we start at a comma, we know the column has nothing in it
//			{
//				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);			// store the comma in the whole row
//				*(myRow[iRow].stringData+iField) = '\0';				// store Null in stringData
//				myRow[iRow].numData = 0;						// store a zero in numData
//				iString++;								// increment where we are pointing to
//			}
//			else if(*(buffer+iString) == '\n')						// if we start at a newline, then we know the column has nothing in it
//			{
//				*(myRow[iRow].stringData+iField) = '\0';				// store Null in stringData
//				myRow[iRow].numData = 0;						// store a zero in numData
//													// NOTE: We do not increment the code - so it skips over next part
//			}										// 	 And we do not store the newline - because we do it at the end
//			else
//			{
//				while (*(buffer+iString) != ',' && *(buffer+iString) != '\n')		// while loop - to store column & test the data until we hit a comma or a newline
//				{
//					*(myRow[iRow].wholeRow+iString) = *(buffer+iString);		// store character we are pointing to in whole row to preserve for priting
//
//					if(isalpha(*(buffer+iString)) != 0)				// See if this is a letter
//					{
//						isLetter++;						// if it's a letter, isLetter++ & store the lowercase version into stringData
//						*(myRow[iRow].stringData+iField) = tolower(*(buffer+iString));
//						iField++;						// increment iField & iString
//						iString++;
//					}
//					else if(isdigit(*(buffer+iString)) != 0)			// See if this is a number
//					{
//						isNumber++;						// if it's a number, isNumber++ & store it into stringData
//						*(myRow[iRow].stringData+iField) = *(buffer+iString);
//						iField++;						// increment iField & iString
//						iString++;
//					}
//					else if(*(buffer+iString) == '.' && isLetter == 0)		//  See if this is a period (ONLY if isLetter is 0), Note: if we KNOW there is a letter - then we want to ignore periods, as this is a string  
//					{
//						isPeriod++;						// if this is a period isPeriod++ & store the period into the stringData
//						*(myRow[iRow].stringData+iField) = *(buffer+iString);
//						iField++;						// increment iField & iString
//						iString++;
//					}
//					else								// if this is anything else - spaces, random characters, etc..... then:
//					{
//						iString++;						// ... we just increment the string; We do not want to store this in stringData
//					}
//				}	
//	
//				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);			// Once out of the while loop - we store the comma or newline
//				if(*(buffer+iString) != '\n')						// if this is NOT a newline, then we increment the string pointer
//					iString++;
//				*(myRow[iRow].stringData+iField) = '\0';				// Store a Null in stringData so the string is Null terminated
//	
//														// Once through storing the column, we determine the type
//														// based on leters, numbers & periods
//				if (isLetter >= 1)								// If there is at least 1 letter - we know this is a string
//				{
//					colType = STRING;
//				}
//				else if(colType != STRING && isNumber >= 0 && (isPeriod == 0 | isPeriod == 1))	// If there are numbers and/or periods, then it's a number
//				{
//					colType = DOUBLE;
//					myRow[iRow].numData = atof(myRow[iRow].stringData);			// use atof to change string into double
//				}
//
//				isLetter = 0;			// change isLetter, isNumber & isPeriod back to 0 (for next row testing)
//				isNumber = 0;
//				isPeriod = 0;
//
//			}
//
//			/* **********************************************************
//			 * *** After pulling out the column to be testing, we	*** *
//			 * *** can continue to store the rest of the chars in 	*** *
//			 * *** the whole row.					*** *
//			 * ******************************************************** */
//			
//			while(*(buffer+iString) != '\n')
//			{
//				*(myRow[iRow].wholeRow+iString) = *(buffer+iString);
//				iString++;
//			}
//			*(myRow[iRow].wholeRow+iString) = *(buffer+iString);		// store the newline
//			iRow++;
//			totalRows++;
//			memset (buffer, '\0', bufsize);	
//
//		}
//	}
//
	fclose(fp);							// close fp
	free (buffer);							// Free the buffer - as we no longer need it


//	pthread_mutex_unlock(&lockTwo);

	
	/**************************************************************************
	************************** End of Get/Store Rows **************************
	**************************************************************************/

	/*******************************************************************
	************************** Call Mergsort  **************************
	********************************************************************/

sorting:
//printf("Works-2: Filepath: %s, colType:  %d\n", filePath, colType);

	if (colType == 1)					// STRING = 1
	{
		smergeSort(myRow, 0, totalRows-1);
	}
	else if (colType == 2)					// DOUBLE = 2
	{
		fmergeSort(myRow, 0, totalRows-1);
	}
	else
	{
		smergeSort(myRow, 0, totalRows-1);
	//		printf("Could not determine type of column.  Please check the data.");
	//		exit(1);
	}
//printf("Works-3: Filepath: %s, colType:  %d\n", filePath, colType);

	/***********************************************************************
	************************** End Call Mergsort  **************************
	************************************************************************/
	pthread_mutex_lock(&lockThree);	


//	printf("filePath: %s ... total # of rows: %d\n", filePath, totalRows);
//	int a;
//	for(a=0; a<totalRows; a++)
//		printf("Row# %d ... sort field: %s ... row: %s\n",a, myRow[a].stringData, myRow[a].wholeRow); 

	if(bstBool == 0)
	{
		root = sortedArrayToBST(myRow, 0, totalRows);
		bstBool = 1;
//		printf("***************************************************\n");
//		preOrder(root);
//		printf("***************************************************\n");
	}
	else
	{
		for (iRow=0; iRow < totalRows; iRow++)
		{
			insert(myRow, iRow, colType, root);
		} 
//		printf("***************************************************\n");
//		preOrder(root);
//		printf("***************************************************\n");

	}
	pthread_mutex_unlock(&lockThree);

//printf("Works-4: Filepath: %s, colType:  %d\n", filePath, colType);
				

//	/**********************************************************************************
//	*** This section manipulates the path name that is passed in so we can create a	***
//	*** new csv file name.  The new naming convention will be either a path that 	***
//	*** the user designates, or the source file path.  Either way, each file will	***
//	*** have the text '-sorted-' followed by the column argument(s) that the user 	***
//	*** requested to be sorted appended to the file name (prior to the .csv).	***
//	*** If there is a file that has the same name as a different file (in 2 diff 	***
//	*** folders, then the first file called will get the name, and any files after	***
//	*** will get a _# appended to the filename to desigate the different file.	***
//	*** Ultimately, we will use newfilepath to print new csv file.			***
//	**********************************************************************************/
//
//	/*** New Variables Defined for this section ***/
//	FILE * newfile;							// pointer to the new file
//	int ipath=0;							// index for the filepath
//	int inewpath = 0;						// index for the new filepath
//	int i = 1;							// index to count # of copies a file is saved (for when diff files from diff directories that have the same name get sorted and saved to a dir specificed by the user)
//	int fileflag = 0;						// flag to help see if the file exists, and help increment naming convention of "i"
//
//	size_t lenFilepath = strlen(filePath);				// length of filepath which was passed into this function
//	size_t lenDirSorted = 0;					// length of the directory in which to store the sorted file
//
//	int findSlash=lenFilepath-1;					// initialize findSlash, so we can point to the end of the filepath
//	while (*(filePath+findSlash) != '/')				// traversing the filepath from the end - trying to find location of first slash (/)
//		findSlash--;
//
//	char * newfilepath = (char*) malloc ((2*255) * sizeof(char));	// filename can only be 255 chars long, and we multiply by 2 to give the directory 255 characters as well
//	if (newfilepath == NULL)
//	{
//		printf("Could not allocate memory for newfilepath");
//		exit(1);
//	}
//	memset(newfilepath, '\0', (2*255));				// set to null
//
//	/*** End New Variables & Initialization ***/
//
//	
//	/*** determine if this is default or a user specified directory ***/
//
//	if(strcmp(dirSorted, "\0") == 0)				// If what is passed in is default "\0", then the new path will use the source file
//	{
//		for (ipath = 0; ipath < findSlash; ipath++)		// Loop for 0 to findSlash (the length of the path minus the /filename)
//		{
//			*(newfilepath+ipath) = *(filePath + ipath);	// save the source path into newfilepath
//		}
//		lenDirSorted = strlen(newfilepath);			// find out how long the new file path is (store for later)
//	}
//	else								// Otherwise if not default, then save in the output directory requested byb user
//	{
//		lenDirSorted = strlen(dirSorted);			// find out how long the path the user passed in is
//		strcpy (newfilepath, dirSorted);			// copy the dierctory for sorted items into newfilepath
//	}
//
//	newfilepath = strcat (newfilepath, "/");			// Add a / to the path after the directory name
//
//	inewpath = lenDirSorted + 1;					// we want to start the index AFTER the length of the directory path plus the slash	
//	for (ipath = (findSlash+1); ipath < lenFilepath-4; ipath++)	// find JUST the file name - by starting at the location of the slash plus 1 space... and we go to the length of the filepath (minus 4 to leaving off .csv)
//	{
//		*(newfilepath+inewpath)=*(filePath+ipath);
//		inewpath ++;
//	}
//	
//	newfilepath = strcat(newfilepath, "-sorted-");			// Add -sorted-
//	newfilepath = strcat(newfilepath, columnName);			// Add the column to be sorted
//
//	lenFilepath = strlen(newfilepath);				// Find out newfilepath length (prior to adding .csv), so we know where to point to in order to add an additional # to show more than one copy of this file
//	newfilepath = strcat(newfilepath, ".csv");			// Add the .csv
//
//	while (fileflag == 0)						// While Loop to test to see how many copies of this file there is.
//	{
//		newfile = fopen(newfilepath, "r");			// Try to read file
//		if (newfile == NULL)					// If we cannot read the file - then set the fileflag to 1 in order to get out of while loop
//		{
//			fileflag = 1;
//		}
//		else							// else we append a # to the end of the file, and try to read it again
//		{
//			i++;
//			sprintf((newfilepath+lenFilepath), "_%d.csv",i);
//		}
//	}
//
//
//	/* ******** Once file has been tested, and name fleshed out - we Print the Results ******** */
//
//	newfile = fopen(newfilepath, "w+");				// point to the new file and use w+ to write a new one
//	fprintf(newfile,"%s",firstRow);
//
//	for(iRow=0; iRow< totalRows; iRow++)				// Loop for printing rows
//	{
//		fprintf(newfile,"%s",myRow[iRow].wholeRow);
//	}
//
//	fclose(newfile);	// close the file, as we no longer need it
//	free (fileInfo);
//	free (filePath);
//	free (newfilepath);	// Free the newfilepath
//	free (firstRow);
//	free (myRow);

	/**********************************************************************************
	****************************** End Print Results **********************************
	**********************************************************************************/
	
	return 0;
}

/******************************************************************************
********************** End of readCSV function ********************************
******************************************************************************/


