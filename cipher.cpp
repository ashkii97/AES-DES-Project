#include <string>
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/**
 *  @param inFile - the popinter to the input file
 *  @param outFile - the pointer to the output file
 *  @param bolockSize - the pointer to the block size
 *  @param cipher - the cipher class to use for encryption
 */
void encryptFile(FILE* inFile, FILE* outFile, const int& blockSize, CipherInterface* cipher)
{
	
	// How many bytes were actually read from the file
	int numRead = -1;
	
	// The buffer to store the read data
	unsigned char buffer[blockSize];

	// THe pointer to the cipher block
	unsigned char* cipherPtr = NULL;

	// Keep processing until you reach the end of file
	while(!feof(inFile))
	{
		// Read 1 * blockSize bytes from the file
		// Save thbe number of bytes actually read from the file
		numRead = fread(buffer, 1, blockSize, inFile);
	
		// If we are already at the end of file and have read nothing, then do not encrypt	
		if(numRead != 0)
		{

			// We nedd to pad the incomplete block
			if(numRead < blockSize)
			{
				/// Pad the remaining bytes
				memset(buffer + numRead, 'x', blockSize - numRead);
			}

			// Encrypt!
			cipherPtr = cipher->encrypt(buffer);

			// Write the data to the file
			fwrite(cipherPtr, 1, blockSize, outFile	);

			// Free the memory
			delete[] cipherPtr;
		}
	}
}	

void decryptFile(FILE* inFile, FILE* outFile, const int& blockSize, CipherInterface* cipher)
{
	
	// How many bytes were actually read from the file
	int numRead = -1;
	
	// The buffer to store the read data
	unsigned char buffer[blockSize];

	// THe pointer to the cipher block
	unsigned char* cipherPtr = NULL;

	// Keep processing until you reach the end of file
	while(!feof(inFile))
	{
		// Read 1 * blockSize bytes from the file
		// Save thbe number of bytes actually read from the file
		numRead = fread(buffer, 1, blockSize, inFile);
	
		// If we are already at the end of file and have read nothing, then do not encrypt	
		if(numRead != 0)
		{
			// Encrypt!
			cipherPtr = cipher->decrypt(buffer);

			// Write the data to the file
			fwrite(cipherPtr, 1, blockSize, outFile	);

			// Free the memory
			delete[] cipherPtr;
		}
	}
}	

int main(int argc, char** argv)
{
	/**
	 * TODO: Replace the code below	with your code which can SWITCH
	 * between DES and AES and encrypt files. DO NOT FORGET TO PAD
	 * THE LAST BLOCK IF NECESSARY.
	 *
	 * NOTE: due to the incomplete skeleton, the code may crash or
	 * misbehave.
	 */
	
	string cipherName = argv[1];
	unsigned char* key = (unsigned char*)argv[2];
	string encOrDec = argv[3];
	string inFile = argv[4];
	string outFile = argv[5];
	
	// Open the input file
	FILE* inFilePtr = fopen(inFile.c_str(), "r");

	// Open the outpuiit file
	FILE* outFilePtr = fopen(outFile.c_str(), "w");

	// Check if the input file was successfully open
	if(!inFilePtr)
	{	
		perror("infile fopen():");
		exit(1);
	}	

	/* Create an i stance of the DES cipher */	

	// Add the sanity check for the outpuit file
	if (!outFilePtr)
	{
		perror("outfile fopen():");
		exit(1);
	}

	CipherInterface* cipher = NULL; 
	
	if (cipherName == "AES")
		cipher = new AES();
	else if (cipherName == "DES")
		cipher = new DES();
	
	/* Error checks */
	if(!cipher)
	{
		fprintf(stderr, "ERROR [%s %s %d]: could not allocate memory\n",	
		__FILE__, __FUNCTION__, __LINE__);
		exit(-1);
	}

	/* Set the encryption key
	 * A valid key comprises 16 hexidecimal
	 * characters. Below is one example.
	 * Your program should take input from
	 * command line.
	 */
	cipher->setKey(key);
	
	unsigned char* block = new unsigned char;
	unsigned char* textBuffer = new unsigned char[17];

	if (cipherName == "AES")
	{
		if (encOrDec == "ENC")
		{
			encryptFile(inFilePtr, outFilePtr, 16, cipher);
		}
		else if (encOrDec == "DEC")
		{
			decryptFile(inFilePtr, outFilePtr, 16, cipher);
		}
	}
	else if (cipherName == "DES")
	{
		if (encOrDec == "ENC")
		{
			// Encrypt using DES
			encryptFile(inFilePtr, outFilePtr, 8, cipher);


		}
		else if (encOrDec == "DEC")
		{
			decryptFile(inFilePtr, outFilePtr, 8, cipher);
		}
	}

	fclose(inFilePtr);
	fclose(outFilePtr);

	return 0;
}
