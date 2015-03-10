/****************************************************************************
* recover.c
*
* Recovers files from a forensic image.
* Refer to the usage statement for supported file types.
*
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// create a type of unsigned integers of length of 8 bits (1 byte)
typedef uint8_t BYTE;

// define function to check file types indicated in argv.
char* checkFileType(int argc, char** argv, BYTE* block);

// define blocksize to avoid magic number
#define BLOCKSIZE 512

// enum with supported file types
typedef enum {
	PDF,
	BMP,
	JPG,
	JPG2,
	JPG3,
	PNG,
	GIF,
	GIF2
} fileType;

// if ((block[0] == 0x25) && (block[1] == 0x50) && (block[2] == 0x44) && (block[3] == 0x46))
// if ((block[0] == 0x42) && (block[1] == 0x4d))
// if ((block[0] == 0xff) && (block[1] == 0xd8) && (block[2] == 0xff) && (block[3] == 0xe0 || block[3] == 0xe1))
// if ((block[0] == 0x89) && (block[1] == 0x50) && (block[2] == 0x4e) && (block[3] == 0x47) && (block[4] == 0x0d) && (block[5] == 0x0a) && (block[6] == 0x1a) && (block[7] == 0x0a))

// Array storing all supported file type signatures.
BYTE* supportedFT[] = {	"\x25\x50\x44\x46",
			"\x42\x4d",
			"\xff\xd8\xff\xe0",
			"\xff\xd8\xff\xe1",
			"\xff\xd8\xff\xe8",
			"\x89\x50\x4e\x47\x0d\x0a\x1a\x0a",
			"\x47\x49\x46\x38\x37\x61",
			"\x47\x49\x46\x38\x39\x61"
			};

// Matches above array with lengths of signatures.
// (cannot preform strlen on signatures because may contain \x00).
int FTLens[] = { 4, 2, 4, 4, 4, 8, 6, 6};

// Converts string argument to enum filetype.
fileType convertArg(char* arg)
{
	if(strcmp(arg, "-pdf") == 0)
		return PDF;
	else if(strcmp(arg, "-bmp") == 0)
		return BMP;
	else if(strcmp(arg, "-jpg") == 0)
		return JPG;
	else if(strcmp(arg, "-png") == 0)
		return PNG;
	else if(strcmp(arg, "-gif") == 0)
		return GIF;
	// -all will return PDF because it is first case statement. -all
	// will fall through all case statements, thus checking all signatures.
	else if(strcmp(arg, "-all") == 0)
		return PDF;
	else
	{
		printf("USAGE: ./recover <imageFile> <-options>\n");
		printf(" -pdf		- Recovers pdf files\n");
		printf(" -bmp		- Recovers bmp files\n");
		printf(" -jpg		- Recovers jpg files\n");
		printf(" -png		- Recovers png files\n");
		printf(" -gif		- Recovers gif files\n");
		printf(" -all		- Recovers all supported files\n");
		exit(-1);
	}	
}

// Checks the current file for each file type listed in argv.
char* checkFileType(int argc, char** argv, BYTE* block){

	char* ret = NULL;	
	
	int i;
	// Loops through args checking each one using switch statement
	for(i = 2; i < argc; i++)
	{
		if(ret != NULL)
			break;
		int j;
		BYTE* sig;
		BYTE* sig2;
		BYTE* sig3;
		switch(convertArg(argv[i]))
		{
			case PDF: ;
				sig = supportedFT[PDF];
				for(j = 0; j < FTLens[PDF]; j++)
				{
					if(block[j] != sig[j])
					{
						ret = NULL;
						break;
					}else{
						ret = ".pdf";
					}
				}		
				// If argv is -all and current file is not a pdf (ret 
				// is still null), then fall through to next signature
				if(strcmp(argv[2], "-all") != 0 || ret != NULL)
					break;

			case BMP: ;
				sig = supportedFT[BMP];
				for(j = 0; j < FTLens[BMP]; j++)
				{
					if(block[j] != sig[j])
					{
						ret = NULL;
						break;
					}else{
						ret = ".bmp";
					}
				}
				if(strcmp(argv[2], "-all") != 0 || ret != NULL)
					break;
				
			case JPG: ;	// JPG has 3 signatures. We check each simultaneously
				sig = supportedFT[JPG];
				sig2 = supportedFT[JPG2];
				sig3 = supportedFT[JPG3];
				for(j = 0; j < FTLens[JPG]; j++)
				{
					if((block[j] != sig[j]) && (block[j] != sig2[j]) && (block[j] != sig3[j]))
					{	
						ret = NULL;
						break;
					}else{
						ret = ".jpg";
					}
				}
				if(strcmp(argv[2], "-all") != 0 || ret != NULL)
					break;
				
			case PNG: ;
				sig = supportedFT[PNG];
				for(j = 0; j < FTLens[PNG]; j++)
				{
					if(block[j] != sig[j])
					{
						ret = NULL;
						break;
					}else{
						ret = ".png";
					}
				}
				if(strcmp(argv[2], "-all") != 0 || ret != NULL)
					break;
				
			case GIF: ;
				sig = supportedFT[GIF];
				sig2 = supportedFT[GIF2];
				for(j = 0; j < FTLens[GIF]; j++)
				{
					if( (block[j] != sig[j]) && (block[j] != sig2[j]))
					{
						ret = NULL;
						break;
					}else{
						ret = ".gif";
					}
				}
				break;

			default:
				ret = NULL;
		}
	}
	return ret;
}

int main (int argc, char** argv)
{
	// Check arguments for usage
	if(argc <= 2)
	{
		printf("USAGE: ./recover <imageFile> <-options>\n");
		printf(" -pdf		- Recovers pdf files\n");
		printf(" -bmp		- Recovers bmp files\n");
		printf(" -jpg		- Recovers jpg files\n");
		printf(" -png		- Recovers png files\n");
		printf(" -gif		- Recovers gif files\n");
		printf(" -all		- Recovers all supported files\n");
		exit(-1);
	} 

    // open specified file 
	FILE* fp = fopen(argv[1], "r");
	
	// if fp is NULL (non-existent file), return 1
    if (fp == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    
    // open output file
    FILE *outfile;
    outfile = NULL;
    
    // create a variable to name the new files
    int fileNum = 0;
    
    // forever loop that will end when the file has been read
    while (1)
    {
        // create an array called block that's the size of BLOCKSIZE
        BYTE block[BLOCKSIZE] = {};
        
        // iterate over each byte in each block until you reach the end of the file
	int i;
        for (i = 0; i < BLOCKSIZE; i++)
        {
            // if you reach the end of file, close the file
            if (feof(fp))
            {
                fclose(fp);
                
                // close previously opened output file pointer if necessary
                if (outfile != NULL)
                    fclose(outfile);
                    
                return 0;
            }
            
            // read one byte at a time
            fread(&block[i], sizeof (BYTE), 1, fp);
        }

		// variable for extension
		char* ext = NULL;
        
		// get extension if indicated in argv
		ext = checkFileType(argc, argv, block);

	        // if block's first 4 bytes match those of a jpg (start of new jpg)      
		if (ext != NULL)
		{
        
	    // close previously opened output file pointer if necessary
            if (outfile != NULL)
            {
                fclose(outfile);
            }
            
            // create a constant char array for filename
            char filename[4];
            
            // name file using ###.ext format
            sprintf(filename, "%03d%s", fileNum, ext);
			printf("file %d = %s\n", fileNum, filename);
            fileNum++; 
            
            // open the output file pointer and make sure it's not NULL
            if ((outfile = fopen(filename, "w")) == NULL)
            {
                printf("Could not write file.\n");
                return 1;
            }
 
            // write the block containing the file to the outfile
            fwrite(&block[0], BLOCKSIZE * sizeof (BYTE), 1, outfile);
        }
        
        // if outfile has been opened, write the block to the outfile
        else if (outfile != NULL) 
        {
            fwrite(&block[0], BLOCKSIZE * sizeof (BYTE), 1, outfile);
        }
    }
 
    // close file
    fclose(fp);
 
    // return 0 to exit program 
    return 0;
}
