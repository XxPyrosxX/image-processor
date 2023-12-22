# Image Processor

## Overview
The Image Processor is a program designed to read, process, and write TGA (Truevision Graphics Adapter) image files.  
It offers various functionalities to manipulate image data, such as loading TGA files, applying blending modes, adjusting color channels, and performing image transformations.  

## Reading Binary Data
The program operates on binary file operations, reading bytes from TGA files and storing them in memory without any conversion or interpretation.  
The file format details, especially the header structure, are crucial for proper reading and processing of TGA files.   

## Viewing TGA Files
Since some operating systems may not natively support TGA file viewing, users are encouraged to install external tools like Photoshop, GIMP, or TGAViewer to visualize the processed images.  

## File Format Description
The TGA file format is relatively simple, consisting of a header (18 bytes) followed by image data.    
For this project, 24-bit true color, uncompressed images are used. The header contains information about the image, such as width, height, and pixel depth.   

## What's in a Pixel?
Pixels in TGA files represent color components (red, green, blue), and each component is an unsigned char with values ranging from 0 to 255.   
The assignment emphasizes the importance of understanding the storage order of colors in TGA files, where the colors are stored in reverse order (blue, green, red).  

## Storage
Storing TGA files involves saving the header (18 bytes) and pixel data. 
The header contains essential information about the image, even if not all values are used.

## Writing a File
The process of writing a TGA file is straightforward: first, write the header, then write the image data. 

## Image Manipulations
The project introduces various image manipulation techniques, focusing on blending modes like Multiply, Subtract, Screen, and Overlay.   
Additionally, users can modify individual color channels by adding or scaling values.  

## Tasks
The project is divided into 10 tasks, each involving loading one or more files, performing specific operations, and writing the results to new TGA files in the "output" folder. 
The "examples" folder provides completed versions for testing against the generated files.

## Testing Your Files
Testing is essential to verify the correctness of the implemented functionalities. 
The "examples" folder can be used to compare against the generated output. 
Every byte in the header and every pixel must match exactly for the test to be successful.
