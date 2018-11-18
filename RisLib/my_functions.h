#pragma once

/*==============================================================================
My math functions
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//------------------------------------------------------------------------------
// Arithmentic

// Min,Max
double my_fmin (double aA,double aB);
double my_fmax (double aA,double aB);

int    my_imin (int aA,int aB);
int    my_imax (int aA,int aB);

// Close to
bool my_closeto (double aA,double aB,double aResolution);
bool my_closeto (double aA,double aB,int aResolution);

// Round to nearest integer
int    my_round (double aA);

// Seeds random with current time
void   my_srand();

// Return random double 0.0 <= x < 1.0
double my_frand();

// Return random double A <= x < B
double my_frand(double aA,double aB);

// Return random int 0 <= x < A
int    my_irand(int aA);

// Return random int A <= x <= B
int    my_irand(int aA,int aB);

// Return random flag with probability P
bool my_randflag(double aP);

//------------------------------------------------------------------------------
// String

// Trim CRLF from the end of a string
void my_trimCRLF(char* aString);
void my_trim_new_line(char* aString);

// Return string "true" "false" for a bool
char* my_string_from_bool(bool aValue);

// Return string "true" "false" for a bool
char* my_string_from_true(bool aValue);

// Return string "pass" "fail" for a bool
char* my_string_from_pass(bool aValue);

char* my_stringLLU(char* aString,unsigned long long aValue);

//------------------------------------------------------------------------------
// Time

// Return seconds from sec,min or hour
//    1.0 == my_time_from_string("1.0")
//    1.0 == my_time_from_string("1.0s")
//   60.0 == my_time_from_string("1.0m")
// 3600.0 == my_time_from_string("1.0h")
double my_time_from_string(char* aString);

// Return "01:02:03" from seconds
char*  my_string_from_time(double aTime, char* aBuffer);

// Return current timestamp
char*  my_timestamp(char* aBuffer);

// Convert seconds to millisecond ticks
int    my_ticks_from_time (double aTime);

//------------------------------------------------------------------------------
// Index arithmetic

// Index arithmetic for queues, wraps around
inline int my_index_add(int aIndex, int aDelta, int aSize)
{
   aIndex += aDelta;
   if (aIndex >= aSize) aIndex -= aSize;
   return aIndex;

}
inline int my_index_sub(int aIndex, int aDelta, int aSize)
{
   aIndex -= aDelta;
   if (aIndex < 0 ) aIndex += aSize;
   return aIndex;

}

//------------------------------------------------------------------------------
// Round up to 16 byte boundary

inline int my_round_upto16(int aValue)
{
   return ((aValue & 0xF) != 0) ? ((aValue & ~0xF) + 0x10) : aValue;

}

//------------------------------------------------------------------------------
// File functions

void my_strupr(char* aString);

void my_strncpy(char* aDestin,const char* aSource,unsigned int aSize);

//------------------------------------------------------------------------------
// File operations

bool my_copy_file(char* aSourcePath, char* aDestinPath);

bool my_compare_files (char* aFile1Path, char* aFile2Path, int aNumOfBytes);

//------------------------------------------------------------------------------
// Program halt

void my_halt(char* aString=0);

//******************************************************************************
//******************************************************************************
//******************************************************************************
