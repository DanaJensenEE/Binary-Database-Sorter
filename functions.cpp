/***************************************************************************//*
* @file functions.cpp
******************************************************************************/
#pragma pack(1)
#include "functions.h"

/***************************************************************************//*
* @par Description:
* This function is called by the main function and is used to open the input 
* file that was passed in from the command line arguements. It will then check 
* to see if the file opened correctly and safely exit if it did not open. After
* confirming file is open it will then dynamically allocate memory in the form
* of the structure Record that will hold the individual records while they are
* checked. This function calls the checkRecord function and uses the array
* "errors" to decide which output file the record will be output to.
*
*
* @param[in] argv - a 2d array of characters containing the arguments.
*
******************************************************************************/
void readRecords ( char *argv1[] )
{
    int endoffile;
    int count;
    int i = 0;
    int j = 0;
    int numberOfErrors = 0;
    bool errors[22] = {false};
    Record *record = NULL;
    ifstream oldDataFile;
    ofstream goodData;
    ofstream badData;

    //Opening and checking file
    oldDataFile.open ( argv1[1], ios::out | ios::binary );
    if ( oldDataFile.is_open() == false )
    {
        cout << "Database To Be Tested Could Not Be Open" << endl;
        safeExit ( record );
    }
    //Finding how many records are in the file
    oldDataFile.seekg ( 0, ios::end );
    endoffile = int ( oldDataFile.tellg() );
    oldDataFile.seekg ( 0, ios::beg );
    count = endoffile / sizeof ( Record );
    //Dynamically allocate memory for record
    record = new ( nothrow ) Record;
    if ( record == NULL )
        safeExit ( record );
    //Read in and check each record one by one.
    while ( oldDataFile.read ( ( char * ) & ( record[0] ), sizeof ( Record ) ))
    {
        //Read in Record
        //oldDataFile.read ( ( char * ) & ( record[0] ), sizeof ( Record ) );
        //Check Record For errors
        checkRecord ( record, errors );

        for ( j = 0; j < 22; j++ )
        {
            if ( errors[j] == true )
            {
                numberOfErrors += 1;
            }
        }
        //If there were errors output record to bad file, else to good file.
        if ( numberOfErrors > 0 )
            saveBadRecord ( record, argv1, errors );
        else
            saveGoodRecord ( record, argv1 );

        for ( j = 0; j < 22; j++ )
            errors[j] = false;
        numberOfErrors = 0;
        i++;
    }
    //deallocate memory and exit.
    safeExit ( record );
}
/***************************************************************************//* 
* @par Description:
* This function is called by the checkRecords function and is used to store
* a record that has been showed to be error free into a binary file specified
* by the user via command line arguement. It opens the output file and checks
* for its validity. It then stores the array into the end of the file, closes
* the file and then returns back into readRecords.
*
* @param[in] argv - a 2d array of characters containing the arguments.
* @param[in] record - a structure that holds a record and its contents.
*
*
******************************************************************************/
void saveGoodRecord ( Record *record, char *argv1[] )
{
    //Open and Check file
    ofstream goodData;
    goodData.open ( argv1[2], ios::app | ios::out | ios::binary );
    if ( goodData.is_open() == false )
    {
        cout << "Good Entry File Could Not Be Opened" << endl;
        safeExit ( record );
    }
    //Output good record.
    goodData.write ( ( char * ) &record[0], sizeof ( Record ) );
    goodData.close();
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecords function and is used to open the
* bad record output file that was passed in from the command line arguements.
* It will then check to see if the file opened correctly and safely exit if it
* did not open. After confirming file is open it will save the record that was
* also passed in from readRecords in a human readable form. After it outputs
* the record it will then check the "errors" array and output what errors
* occured under the record in a readable format. It will then close the file
* and return back to the readRecords function.
*
*
* @param[in] argv - a 2d array of characters containing the arguments.
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void saveBadRecord ( Record *record, char *argv1[], bool errors[] )
{
    int i = 0;
    int temp1, temp2, temp3;
    int cityStateZipSize;
    string tempstring;
    ofstream badData;
    ostringstream convert5;
    ostringstream convert4;
    //Open and Check File
    badData.open ( argv1[3], ios::app | ios ::out );
    if ( badData.is_open() == false )
    {
        cout << "Bad Entry File Could Not Be Opened" << endl;
        safeExit ( record );
    }
    //Print Bad Entry with formatting
    //Name
    badData << record[0].name;
    tempstring = record[0].name;
    temp1 = 30 - tempstring.length();
    for ( i = 0; i < temp1; i++ )
    {
        badData << " ";
    }
    i = 0;
    //BirthDate
    convertDate ( temp1, temp2, temp3, record, 1 );
    badData << "Birth Date:      ";
    badData << temp1 << "/" << temp2 << "/" << temp3;
    badData << endl;
    //Address
    badData << record[0].address;
    tempstring = record[0].address;
    temp1 = 30 - tempstring.length();
    for ( i = 0; i < temp1; i++ )
    {
        badData << " ";
    }
    i = 0;
    //LicensedDate
    convertDate ( temp1, temp2, temp3, record, 2 );
    badData << "License Date:    ";
    badData << temp1 << "/" << temp2 << "/" << temp3;
    badData << endl;
    //City
    tempstring = record[0].city;
    temp1 = tempstring.length();
    badData.write ( record[0].city, temp1 );
    badData << ", ";
    cityStateZipSize = temp1 + 2;
    //State
    badData.write ( record[0].state, 2 );
    badData << " ";
    cityStateZipSize = cityStateZipSize + 3;
    //ZipCode
    convertZipCode ( temp1, temp2, record );
    badData << temp1 << "-" << temp2;
    convert5 << temp1;
    tempstring = convert5.str();
    cityStateZipSize = cityStateZipSize + tempstring.length() + 1;
    convert4 << temp2;
    tempstring = convert4.str();
    cityStateZipSize = cityStateZipSize + tempstring.length();
    //ExpirationDate
    temp1 = 30 - cityStateZipSize;
    for ( i = 0; i < temp1; i++ )
        badData << " ";
    convertDate ( temp1, temp2, temp3, record, 3 );
    badData << "Expiration Date: ";
    badData << temp1 << "/" << temp2 << "/" << temp3;
    badData << endl;
    //Call Sign
    badData << "Call Sign: ";
    tempstring = record[0].callSign;
    tempstring.resize ( 5 );
    badData << tempstring;
    badData.flush();
    //Radio Class
    badData << "              ";
    badData << "Radio Class:     ";
    badData.write ( ( char * ) &record[0].radioClass, 1 );
    badData << endl;
    badData << endl;

    //Printing Errors

    if ( errors[0] == true )
        badData << "Name Field" << '\n' << '\t' << "-"
                << "Invalid Character in the Name Field" << endl;
    if ( errors[1] == true )
        badData << "Address Field" << '\n' << '\t' << "-"
                << "Invalid Character in the Address Field" << endl;
    if ( errors[2] == true )
        badData << "City Field" << '\n' << '\t' << "-"
                << "Invalid Character in the City Field" << endl;

    if ( errors[3] == true || errors[4] == true )
        badData << "State Field" << endl;
    if ( errors[3] == true )
        badData << '\t' << "-" << "Invalid State Code" << endl;
    if ( errors[4] == true )
        badData << '\t' << "-"
                << "First Character of the State Code not Capitalized" << endl;

    if ( errors[5] == true || errors[6] == true )
        badData << "Zip Code" << endl;
    if ( errors[5] == true )
        badData << '\t' << "-" << "Invalid 5 Digit Zip Code" << endl;
    if ( errors[6] == true )
        badData << '\t' << "-" << "Invalid 4 Digit Zip Code" << endl;

    if ( errors[7] == true || errors[8] == true || errors[9] == true )
        badData << "Birth Date" << endl;
    if ( errors[7] == true )
        badData << '\t' << "-" << "Invalid Month in Birthday" << endl;
    if ( errors[8] == true )
        badData << '\t' << "-" << "Invalid Day in Birthday" << endl;
    if ( errors[9] == true )
        badData << '\t' << "-" << "Invalid Year in Birthday" << endl;

    if ( errors[10] == true || errors[11] == true || errors[12] == true )
        badData << "License Date" << endl;
    if ( errors[10] == true )
        badData << '\t' << "-" << "Invalid Month in License Date" << endl;
    if ( errors[11] == true )
        badData << '\t' << "-" << "Invalid Day in License Date" << endl;
    if ( errors[12] == true )
        badData << '\t' << "-" << "Invalid Year in License Date" << endl;

    if ( errors[13] == true || errors[14] == true || errors[15] == true 
							|| errors[16] == true )
        badData << "Expiration Date" << endl;
    if ( errors[13] == true )
        badData << '\t' << "-" << "Invalid Month in Expiration Date" << endl;
    if ( errors[14] == true )
        badData << '\t' << "-" << "Invalid Day in Expiration Date" << endl;
    if ( errors[15] == true )
        badData << '\t' << "-" << "Invalid Year in Expiration Date" << endl;
    if ( errors[16] == true )
        badData << '\t' << "-" << "Expiration Date is not After License Date"
				<< endl;

    if ( errors[17] == true )
        badData << "Radio Class" << '\n' << '\t' << "-" 
				<< "Radio Class Code is Invalid" << endl;

    if ( errors[18] == true || errors[19] == true || errors[20] == true 
							|| errors[21] == true )
        badData << "Call Sign" << endl;
    if ( errors[18] == true )
        badData << '\t' << "-" << "Invalid First Character in Call Sign" 
				<< endl;
    if ( errors[19] == true )
        badData << '\t' << "-" 
				<< "Invalid Second, Fourth, and/or Fifth Character" << endl;
    if ( errors[20] == true )
        badData << '\t' << "-" << "Invalid Digit in Call Sign" << endl;
    if ( errors[21] == true )
        badData << '\t' << "-" << "Call Sign not Upper Case" << endl;
    badData << endl;
    badData << "---------------------------------------------" << endl;
    badData << endl;

    badData.close();
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecords function and is used as a hub
* for all the error checking that will take place. It calls individual 
* functions for checking the Name, Address, City, State, ZipCode, Birth Date, 
* License Date, Expiration Date, Class, and Call Sign.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkRecord ( Record *record, bool errors[] )
{
    checkName ( record, errors );           //Working
    checkAddress ( record, errors );        //Working
    checkCity ( record, errors );           //Working
    checkState ( record, errors );          //Working
    checkZipCode ( record, errors );        //Working
    checkBirthDate ( record, errors );      //Working
    checkLicenseDate ( record, errors );    //Working
    checkExpirationDate ( record, errors ); //Working
    checkClass ( record, errors );          //Working
    checkCallSign ( record, errors );       //Working
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* name field of the record. It checks each character stored in the name field
* to insure that is a-z, A-Z, a ".", or a space character. If it finds a 
* character that is not amongst these listed it sets error[0] equal to true 
* so that it can be output later into the bad record output file.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkName ( Record *record, bool errors[] )
{
    int temp1 = 0;
    string tempstring;
    //Checking the name for errors.
    tempstring = record[0].name;
    temp1 = tempstring.find_first_not_of
            ( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ. " );
    if ( temp1 >= 0 ) //If found a character not in this list
    {
        errors[0] = true;
    }
    temp1 = 0;
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* name field of the record. It checks each character stored in the name field 
* to insure that is a-z, A-Z, 0-9, ".", "#" or a space character. If it finds
* a character that is not amongst these listed it sets error[1] equal to true 
* so that it can be output later into the bad record output file.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkAddress ( Record *record, bool errors[] )
{
    int temp1;
    string tempstring;
    //Checking the address for errors.
    tempstring = record[0].address;
    temp1 = tempstring.find_first_not_of
		( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.# " );
    if ( temp1 > 0 )
        errors[1] = true;
    temp1 = 0;
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* name field of the record. It checks each character stored in the name field 
* to insure that is a-z, A-Z, a ".", or a space character. If it finds a 
* character that is not amongst these listed it sets error[2] equal to true 
* so that it can be output later into the bad record output file.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkCity ( Record *record, bool errors[] )
{
    int temp1;
    string tempstring;
    //Checking the city for errors.
    tempstring = record[0].city;
    temp1 = tempstring.find_first_not_of
            ( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ." );
    if ( temp1 > 0 )
        errors[2] = true;
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* state code field for a recognized state code and for all characters being
* upper case. If the state code from the record is not a recognized US state
* code it will set errors[3] to true. It will then check to see if the 
* character are upper case. If they are not then it will set errors[3] and 
* errors[4] to true.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkState ( Record *record, bool errors[] )
{
    char stateAbrev[50][3] = {"AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE",
                              "FL", "GA", "HI", "ID", "IL", "IN", "IA", "KS",
                              "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS",
                              "MO", "MT", "NE", "NV", "NH", "NJ", "NM", "NY",
                              "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC",
                              "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV",
                              "WI", "WY"
                             };

    int temp1 = 0;
    int temp2 = 0;
    int i = 0;
    string tempstring;
    string tempstring2;
    tempstring = record[0].state;
    //Preps a temporary string to be checked with recognized state codes.
    tempstring.resize ( 2 );
    tempstring2 = tempstring;
    transform ( tempstring2.begin(), tempstring2.end(), tempstring2.begin(), 
				toupper );
    //Checking for a correct abbreviations
    for ( i = 0; i < 50; i++ )
    {
        temp1 = tempstring2.compare ( stateAbrev[i] );
        if ( temp1 == 0 )
            temp2 += 1;
    }
    if ( temp2 != 1 )
        errors[3] = true;
    //Checking if the first letter is not capitalized.
    if ( tempstring[0] != tempstring2[0] )
    {
        errors[4] = true;
    }
    temp1 = 0;
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* zip code field for valid 5 digit and 4 digit zip codes. This function first
* calls the convert zip code function to decode the zip code stored in the
* binary record. It then checks to make sure the 5 digit zipcode is between
* 10000 and 99999, setting errors[5] to true if it is not. It finally checks
* if the 4 digit zip code is 0 or between 1000 and 9999. Returns to readRecord
* function.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkZipCode ( Record *record, bool errors[] )
{
    int temp1, temp2;
    convertZipCode ( temp1, temp2, record );
    //Checking 5 digit zip.
    if ( temp1 < 10000 || temp1 > 99999 )
    {
        errors[5] = true;
    }
    //Checking 4 digit zip, 0 checked first to cause short circuit.
    if ( temp2 < 1000 || temp2 > 9999 )
    {
        errors[6] = true;
    }
    if ( temp2 == 0 )
    {
        errors[6] = false;
    }
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* birth date field in the record. It calls the function converDate to convert
* the binary record entry to a usuable date. It first checks the month for 
* being between 1 and 12 inclusive. Then it calls the check day function that
* checks the number day based on what the month is. Finally it checks to see 
* if the year is between 1900 and 2013. An invalid month flags errors[7], an 
* invalid day flags errors[8], and an invalid year flags errors[9]. Returns 
* to checkRecord.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkBirthDate ( Record *record, bool errors[] )
{
    int temp1, temp2, temp3;
    convertDate ( temp1, temp2, temp3, record, 1 );
    if ( temp1 < 1 || temp1 > 12 )          //Month
        errors[7] = true;
    checkDay ( temp1, temp2, errors, 0 );   //Day
    if ( temp3 < 1900 || temp3 > 2013 )     //Year
        errors[9] = true;
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* License date field in the record. It calls the function converDate to convert
* the binary record entry to a usuable date. It first checks the month for
* being between 1 and 12 inclusive. Then it calls the check day function that
* checks the number day based on what the month is. Finally it checks to see if
* the year is between 1900 and 2013. An invalid month flags errors[10], an
* invalid day flags errors[11], and an invalid year flags errors[12]. Returns
* to checkRecord.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkLicenseDate ( Record *record, bool errors[] )
{
    int temp1, temp2, temp3;
    convertDate ( temp1, temp2, temp3, record, 2 );
    if ( temp1 < 1 || temp1 > 12 )          //Month
        errors[10] = true;
    checkDay ( temp1, temp2, errors, 1 );   //Day
    if ( temp3 < 1900 || temp3 > 2013 )     //Year
    {
        errors[12] = true;
    }
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* expiration date field in the record. It calls the function converDate to
* convert the binary record entry to a usuable date. It first checks the month
* for being between 1 and 12 inclusive. Then it calls the check day function
* that checks the number day based on what the month is. Finally it checks to
* see if the year is between 1900 and 2013. An invalid month flags errors[13],
* an invalid day flags errors[14], and an invalid year flags errors[15]. It
* also checks if the expiration date is after the license date and assigns
* errors[16] accordingly.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkExpirationDate ( Record *record, bool errors[] )
{
    int temp1, temp2, temp3;
    int tempLic1, tempLic2, tempLic3;       //Temp ints for license date
    convertDate ( temp1, temp2, temp3, record, 3 );
    if ( temp1 < 1 || temp1 > 12 )          //Month
        errors[13] = true;
    checkDay ( temp1, temp2, errors, 2 );   //Day
    if ( temp3 < 1900 || temp3 > 2013 )     //Year
    {
        errors[15] = true;
    }
    convertDate ( tempLic1, tempLic2, tempLic3, record, 2 );

    //Check that expiration is after license.
    if ( tempLic3 > temp3 )
        errors[16] = true;
    if ( tempLic3 == temp3 && tempLic1 > temp1 )
        errors[16] = true;
    if ( tempLic3 == temp3 && tempLic1 == temp1 && tempLic2 > temp2 )
        errors[16] = true;
}
/***************************************************************************//*
* @par Description:
* This function is called by multiple functions and is used to check if the
* day field in the record is valid based on what the month is. If it is Feb
* the valid days are 1-28. If it is Jan, March, May, July, Aug, Oct, Dec
* the valid days are 1-31. If it is Apr, June, Sept, or Nov the valid days are
* 1-30. If it finds that the numbered day is outside of these ranges it flags
* the field 8, 11, or 14 for errors, based on what  date is being checked.
*
* @param[in] month - an int that contains the number of the month.
* @param[in] errors - a boolean array that holds what errors have been found.
* @param[in] day - an int that contains the number of the day.
* @param[in] type - an int that determines which error fields will be marked.
*
*
******************************************************************************/
void checkDay ( int month, int day, bool errors[], int type )
{
    int temp1;
    temp1 = 8 + 3 * type;
    //February
    if ( month == 2 )
    {
        if ( day < 1 || day > 28 )
            errors[temp1] = true;
    }
    //Apr, June, Sept, Nov
    if ( month == 4 || month == 6 || month == 9 || month == 11 )
    {
        if ( day < 1 || day > 30 )
            errors[temp1] = true;
    }
    //Jan, Mar, May, July, Aug, Oct, Dec
    if ( month <= 1 || month == 3 || month == 5 || month == 7 || month == 8 
					|| month == 10 || month >= 12 )
    {
        if ( day < 1 || day > 31 )
            errors[temp1] = true;
    }

}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* radio class field. It checks if the class code is one of the recognized classes
* and sets errors[17] to true if it is not.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
*
******************************************************************************/
void checkClass ( Record *record, bool errors[] )
{
    char tempChar;
    tempChar = record[0].radioClass;
    //Checking if its one of these recognized
    if ( tempChar == 'n' || tempChar == 'N' || tempChar == 'p' || 
		 tempChar == 'P' || tempChar == 't' || tempChar == 'T' || 
		 tempChar == 'a' || tempChar == 'A' || tempChar == 'g' || 
		 tempChar == 'G' || tempChar == 'x' || tempChar == 'X' )
        errors[17] = false;
    else
        errors[17] = true;
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check the
* radio call sign. To be a valid call sign it must all uppercase letters, start
* with a K,W, or N, have a digit 0-9 in the third spot, and letters A-Z for 
* spots 2, 4 and 5. If there is an error in these fields it will set errors 
* 18, 19, 20 or 21 respectively.
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] errors - a boolean array that holds what errors have been found.
*
******************************************************************************/
void checkCallSign ( Record *record, bool errors[] )
{
    int temp1 = 0;
    int temp2 = 0;
    int temp3 = 0;
    string tempstring;
    string tempstring2;
    string tempstring3;
    string tempstring4;
    tempstring = record[0].callSign;
    tempstring.resize ( 5 );

    //Checking the First Letter
    if ( tempstring[0] == 'K' || tempstring[0] == 'W' || tempstring[0] == 'N' )
        errors[18] = false;
    else
        errors[18] = true;

    //Checking characters 2, 4, and 5
    tempstring2 = tempstring[1];
    tempstring3 = tempstring[3];
    tempstring4 = tempstring[4];
    temp1 = tempstring2.find_first_of ( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    temp2 = tempstring3.find_first_of ( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    temp3 = tempstring4.find_first_of ( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    //If all three places have a valid character
    if ( temp1 == 0 && temp2 == 0 && temp3 == 0 )
        errors[19] = false;
    else
        errors[19] = true;


    //Checking digit
    temp1 = 0;
    tempstring2 = tempstring[2];
    temp1 = tempstring2.find_first_of ( "0123456789" );
    if ( temp1 < 0 )
        errors[20] = true;

    //Checking upper/lowercase
    tempstring2 = tempstring;
    transform ( tempstring2.begin(), tempstring2.end(), tempstring2.begin(),
				toupper );
    if ( tempstring2 != tempstring )
        errors[21] = true;

}
/***************************************************************************//*
* @par Description:
* This function is called by multiple functions and is used to convert the
* binary int stored in the record into correct date using bitwise operators.
* It returns the integer value of the five digit and four digit zip code.
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] five - an int that is passed by reference to store the five 
* digit zip.
* @param[in] four - an int that is passed by reference to store the four
* digit zip.
*
*******************************************************************************/
void convertZipCode ( int &five, int &four, Record *record )
{
    unsigned int temp;
    temp = record[0].zipCode;
    four = temp & 16383;
    temp = temp >> 14;
    five = temp & 262143;
}
/***************************************************************************//*
* @par Description:
* This function is called by the readRecord function and is used to check 
* convert the integer stored in the binary file into a usuable month date and
* year. It does this by using bitwise operators to shift and filter to get the
* proper values.
*
*
* @param[in] record - a structure that holds a record and its contents.
* @param[in] month - int passed by reference that will hold converted month.
* @param[in] day - int passed by reference that will hold converted day.
* @param[in] year - int passed by reference that will hold converted year.
* @param[in] type - int used the decide which date we are converting. 
* (Birth/License).
*
******************************************************************************/
void convertDate ( int &month, int &day, int &year, Record *record, int type )
{
    unsigned int temp;
    switch ( type )
    {
        case 1: //Birth Date
        {
            temp = record[0].birthDate;
            day = temp & 63;
            temp = temp >> 6;
            month = temp & 15;
            temp = temp >> 6;
            year = temp & 2047;
            break;
        }
        case 2: //Licensed Date
        {
            temp = record[0].licensedDate;
            day = temp & 63;
            temp = temp >> 6;
            month = temp & 15;
            temp = temp >> 6;
            year = temp & 2047;
            break;
        }
        case 3: //Expiration Date
        {
            temp = record[0].expirationDate;
            day = temp & 63;
            temp = temp >> 6;
            month = temp & 15;
            temp = temp >> 6;
            year = temp & 2047;

        }
    }
}
/***************************************************************************//*
* @par Description:
* This function is called by multiple files and is used to deallocate the 
* dynamic memory acquired for record and then exit the program. This is 
* called when an error has occured.
*
* @param[in] record - a structure that holds a record and its contents.
*
******************************************************************************/
void safeExit ( Record *record )
{
    delete record;
    exit ( 1 );
}