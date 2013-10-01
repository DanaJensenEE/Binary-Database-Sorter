/***************************************************************************//**
 * @file prog1.cpp
 *
 * @mainpage Program 1 - Binary Database Sorter
 *
 * @section course_section Course Information
 *
 * @authors Dana Jensen
 *
 * @date February 25, 2013
 *
 * @par Instructor:
 *        Roger Schrader
 *
 * @par Course:
 *         CSC 250 '–' Section 1 '–' 10:00 am
 *
 * @par Location:
 *         Class Room Building
 *
 * @section program_section Program Information
 *
 * @details This program will read in a binary file that contains the HAM club's
 * records. When used with the command line the program requires the records file,
 * a output file name for the good files, and a output file name for the bad records.
 * This program will read in a single record that is stored in a binary file. It will
 * then check each part of the record to find if there will be any errors when it is
 * transferred to the new database system. If there are errors the record is marked
 * to be output to the bad record file otherwise it will be put into a file filled
 * with records that will not have errors when transferring. When being output with
 * errors it will put an entry after each record detailing what the errors were.
 *
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *      None
 *
 * @par Usage:
   @verbatim
   c:\prog1.exe Input.bin GoodOutput.bin BadOutput.txt
   @endverbatim
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 *
 * @bug
 *
 * @todo Finish documenting with doxygen tags
 * @todo Fix Extra formatting in bad file output.
 *
 * @par Modifications and Development Timeline:
   @verbatim
   Date          Modification
   ------------  ---------------------------------------------------------------
   Feb 20, 2013  Began writing input function and save function.
                 Finished convert ZipCode and Date functions.
   Feb 21, 2013  Finished writing input and output good file functions.
                 Began writing error checking functions.
                 Finished writing Name, Address, and City check functions.
   Feb 23, 2011  Finished State, Zip, Birth, License, Expiration, Class, Call Sign.
                 Finished Bad Output file function.
   Feb 24, 2013  Testing.
   Feb 25, 2013  Finished Documentation.
                 Submission.

   @endverbatim
 *
 *****************************************************************************/
#pragma pack(1)
#include "functions.h"

/***************************************************************************//*
 * @author Dana Jensen
 *
 * @par Description:
 * This is the starting point of the program, it receives command line 
 * arguements and will exit the program if there are not enough commandline 
 * arguements passed in. This function calls the readRecords function that will
 * handle the records.
 *
 * @param[in] argc - the number of arguments from the command prompt.
 * @param[in] argv - a 2d array of characters containing the arguments.
 *
 * @returns 0 if incorrect number of command line arguements are given.
 *****************************************************************************/
int main ( int argc, char*argv1[] )
{
    if ( argc < 4 )
    {
        cout << "Usage Error: Incorrect Number of Command Line Arguements" 
			 << endl;
        cout << "For Proper Usage: prog1.exe inputData.bin";
		cout <<	" goodData.bin badData.txt" << endl;
    }
    else
    {
        readRecords ( argv1 );
    }
    return 0;
}