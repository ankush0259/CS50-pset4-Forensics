# FileCarver2.0

We have enhanced it to allow commandline arguments. The first argument is the file to read from. This can be an ISO or a dd of an image. We have also added the ability to choose what types of files to recover.

## Installation
1. Either do a git clone or use the download option on the left. 
2. Unzip the files
3. Make the file executable:

<code>chmod +x recover.o</code>

## Usage
FileCarver2.0 can reocover four different filetypes: pdf, jpg, png, and bmp. These can be set using flags in the command line.

To recover jpg files simply run the following command:

<code>./recover.o test.iso -jpg </code>

To recover multiple file types use a command similar to the one below:

<code>./recover.o test.iso -png -pdf </code>

##Recompile
Feel free to edit the recover.c file. Once you have edited the file you will need to compile it using gcc. See the command below:

<code>gcc -std=c99 recover.c -o recover.o</code>

## Contributors
This project was originally created by akush0259. It has since been upgraded by montaukk and tbristow.
