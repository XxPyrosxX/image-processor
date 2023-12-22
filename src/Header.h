//
// Created by Rohun on 11/11/2023.
//

// Header file guards to prevent multiple inclusion
#ifndef PROJECT_03_HEADER_H
#define PROJECT_03_HEADER_H

// Struct representing the TGA image header
struct Header {
private:
    // Private member variables representing different fields of the TGA header
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;

public:
    // Constructors for setting values
    Header();
    Header(char idLength, char colorMapType, char dataTypeCode, short colorMapOrigin, short colorMapLength,
           char colorMapDepth, short xOrigin, short yOrigin, short width, short height, char bitsPerPixel,
           char imageDescriptor);

    // Getter functions for accessing member variables (const indicates values are not changed)
    const char& getIDLength() const;
    const char& getColorMapType() const;
    const char& getDataTypeCode() const;
    const short& getColorMapOrigin() const;
    const short& getColorMapLength() const;
    const char& getColorMapDepth() const;
    const short& getXOrigin() const;
    const short& getYOrigin() const;
    const short& getWidth() const;
    const short& getHeight() const;
    const char& getBitsPerPixel() const;
    const char& getImageDescriptor() const;

    // Operator Overloading for comparing with example files
    bool operator==(const Header& other) const;
};

// End of Header file guards
#endif //PROJECT_03_HEADER_H
