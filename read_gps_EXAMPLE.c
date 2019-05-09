/* 
    simple program to open and read the converted GPS coords from
    a specified file using the "open" and "read" system calls
*/

#define NUM_BYTES_TO_READ 186
#define XCOORD_START_OFFSET 20 // how many bytes into the msg is the coordinate
#define YCOORD_START_OFFSET 31
#define FILE_PATH "GPS.txt"

#include <unistd.h> // read, open
#include <fcntl.h> // open
#include <stdio.h> // printf
#include <stdlib.h> // atof
#include <string.h> // strtok

/* 
    NOTE: if this function is called and returns with 2, then
    based on our current GPS implementation (5-8-19), it should be
    called again until it returns either another error or 0.
    This is because the function was called when the GPS file
    was being refreshed and had no data.
*/

int getGPS(double* xCoord, double* yCoord) {
    int file_descriptor = open(FILE_PATH, O_RDONLY);
    // should be non-negative if successful
    if(file_descriptor < 0) {
        printf("ERROR: COULD NOT OPEN FILE");
        return 1;
    }

    char data[NUM_BYTES_TO_READ];
    // read returns the number of bytes read
    // should be non-negative if successful
    int bytes_read = read(file_descriptor, data, NUM_BYTES_TO_READ);
    if(bytes_read == 0) {
        printf("ERROR: NO DATA READ FROM FILE (EOF)");
        return 2;
    } else if(bytes_read > 0) {
        printf("%s\n", data);
    } else {
        printf("ERROR: PROBLEM READING FILE");
        return 3;
    }

    // close the file
    if(close(file_descriptor) < 0) {
        printf("ERROR: COULD NOT CLOSE FILE");
        return 4;
    }
    
    // grabs a substring from the offset until the first "," (non-inclusive)
    char* xCoord_chars = strtok(data+XCOORD_START_OFFSET,",");
    char* yCoord_chars = strtok(data+YCOORD_START_OFFSET,",");

    // convert the substrings to doubles
    *xCoord = atof(xCoord_chars);
    *yCoord = atof(yCoord_chars);

    printf("X COORD: %f \t\t Y COORD: %f\n", *xCoord, *yCoord);

    return 0;
}

int main() {
    double x = 0;
    double y = 0;
    return getGPS(&x, &y);
}