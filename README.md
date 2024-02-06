# Multithreaded Matrix Multiplication  

Is a program written in c++, which uses multithreading to solve matrix multiplication faster.  

## Usage  

Clone this git repository and provide two matrixes in .csv format, their filenames by default should be matrix1.txt and matrix2.txt. Now run MMM.exe, you will be prompted to enter config, I highly advise you to do so and change the number of threads, this program will spawn to fit your needs. After changing configuration, or deciding, that default settings are good enough, you may exit the configuration mode. Calculation should start immediatly and based on the size of your input and computing power of your machine, you should soon see a file called output.txt. In that file you can find the result once again in csv format.  

### RandomMatrixGen.py  

If you have python installed, you can use this small program to create matrixes automaticly for you, just be aware that it could take a few seconds for large matrixes.  

## TODO  

Create struct for result matrix. Proper error handling for invalid inputs and especially invalid matrixes.  
