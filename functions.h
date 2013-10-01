#pragma pack(1)
/***************************************************************************//*
* @file functions.h
 *****************************************************************************/
/***************************************************************************//*
Author: Dana Jensen
Date: Oct 19, 2011
Teacher: Roger Schrader
Program Description
 *****************************************************************************/

#ifndef __FUNCTIONS__H__
#define __FUNCTIONS__H__

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
/*!
 * @brief structure that follows is used to store the contents of a record.
 */
struct Record
{
    char name[30];              /*!<Name of the Person */
    char address[30];           /*!<Address of the Person */
    char city[28];              /*!<Name of City Person Resides in */
    char state[2];              /*!<State Code where Person Resides */
    unsigned int zipCode;       /*!<ZipCode of where Person Resides */
    unsigned int birthDate;     /*!<Day Person was born */
    unsigned int licensedDate;  /*!<Day Person got License */
    unsigned int expirationDate;/*!<Day the License Expires */
    char radioClass;            /*!<RadioClass of the License */
    char callSign[5];           /*!<Callsign for the Person */
};

void readRecords ( char *argv1[] );
void checkRecord ( Record *record, bool errors[] );
void checkName ( Record *record, bool errors[] );
void checkAddress ( Record *record, bool errors[] );
void checkCity ( Record *record, bool errors[] );
void checkState ( Record *record, bool errors[] );
void checkZipCode ( Record *record, bool errors[] );
void checkBirthDate ( Record *record, bool errors[] );
void checkLicenseDate ( Record *record, bool errors[] );
void checkExpirationDate ( Record *record, bool errors[] );
void checkDay ( int month, int day, bool errors[], int type );
void checkClass ( Record *record, bool errors[] );
void checkCallSign ( Record *record, bool errors[] );
void saveGoodRecord ( Record *record, char *argv1[] );
void saveBadRecord ( Record *record, char *argv1[], bool errors[] );
void safeExit ( Record *record );
void convertZipCode ( int &five, int &four, Record *record );
void convertDate ( int &month, int &day, int &year, Record *record, int type );

#endif