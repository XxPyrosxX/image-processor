//
// Created by Rohun on 11/11/2023.
//

#include "Header.h"

// Constructor with parameters to initialize the Header object
Header::Header(char idLength, char colorMapType, char dataTypeCode, short colorMapOrigin, short colorMapLength,
               char colorMapDepth, short xOrigin, short yOrigin, short width, short height, char bitsPerPixel,
               char imageDescriptor) {
    // Initialize member variables with provided values
    this->idLength = idLength;
    this->colorMapType = colorMapType;
    this->dataTypeCode = dataTypeCode;
    this->colorMapOrigin = colorMapOrigin;
    this->colorMapLength = colorMapLength;
    this->colorMapDepth = colorMapDepth;
    this->xOrigin = xOrigin;
    this->yOrigin = yOrigin;
    this->width = width;
    this->height = height;
    this->bitsPerPixel = bitsPerPixel;
    this->imageDescriptor = imageDescriptor;
}

// Default constructor for Header
Header::Header() {
    // Initialize member variables with default values
    idLength = 0;
    colorMapType = 0;
    dataTypeCode = 0;
    colorMapOrigin = 0;
    colorMapLength = 0;
    colorMapDepth = 0;
    xOrigin = 0;
    yOrigin = 0;
    width = 0;
    height = 0;
    bitsPerPixel = 0;
    imageDescriptor = 0;
}

// Overload the == operator
bool Header::operator==(const Header& other) const {
    return idLength == other.idLength &&
           colorMapType == other.colorMapType &&
           dataTypeCode == other.dataTypeCode &&
           colorMapOrigin == other.colorMapOrigin &&
           colorMapLength == other.colorMapLength &&
           colorMapDepth == other.colorMapDepth &&
           xOrigin == other.xOrigin &&
           yOrigin == other.yOrigin &&
           width == other.width &&
           height == other.height &&
           bitsPerPixel == other.bitsPerPixel &&
           imageDescriptor == other.imageDescriptor;
}

// Getter functions for each member variable

// Provide access to idLength
const char& Header::getIDLength() const {
    return this->idLength;
}

// Provide access to colorMapType
const char& Header::getColorMapType() const {
    return this->colorMapType;
}

// Provide access to dataTypeCode
const char& Header::getDataTypeCode() const {
    return this->dataTypeCode;
}

// Provide access to colorMapOrigin
const short& Header::getColorMapOrigin() const {
    return this->colorMapOrigin;
}

// Provide access to colorMapLength
const short& Header::getColorMapLength() const {
    return this->colorMapLength;
}

// Provide access to colorMapDepth
const char& Header::getColorMapDepth() const {
    return this->colorMapDepth;
}

// Provide access to xOrigin
const short& Header::getXOrigin() const {
    return this->xOrigin;
}

// Provide access to yOrigin
const short& Header::getYOrigin() const {
    return this->yOrigin;
}

// Provide access to width
const short& Header::getWidth() const {
    return this->width;
}

// Provide access to height
const short& Header::getHeight() const {
    return this->height;
}

// Provide access to bitsPerPixel
const char& Header::getBitsPerPixel() const {
    return this->bitsPerPixel;
}

// Provide access to imageDescriptor
const char& Header::getImageDescriptor() const {
    return this->imageDescriptor;
}