//
// Created by Rohun on 11/11/2023.
//

// Header file guards to prevent multiple inclusion
#ifndef PROJECT_03_PIXEL_H
#define PROJECT_03_PIXEL_H

// Struct representing a pixel in a TGA image
struct Pixel {
public:
    // Public member variables representing the blue, green, and red values of a pixel
    unsigned char blueVal;
    unsigned char greenVal;
    unsigned char redVal;

    // Constructor for initializing pixel values
    Pixel(unsigned char blueVal, unsigned char greenVal, unsigned char redVal);
};

// End of Pixel file guards
#endif //PROJECT_03_PIXEL_H