//
// Created by Rohun on 11/11/2023.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Header.h"
#include "Pixel.h"
using namespace std;

Header readTGAHeaderData(string filename) {
    ifstream readFile(filename, ios::binary);
    char idLength; char colorMapType; char dataTypeCode; short colorMapOrigin;
    short colorMapLength; char colorMapDepth; short xOrigin; short yOrigin;
    short width; short height; char bitsPerPixel; char imageDescriptor;
    if (!readFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        Header header;
        return header;
    } else{
        readFile.read(&idLength, sizeof(idLength));
        readFile.read(&colorMapType, sizeof(colorMapType));
        readFile.read(&dataTypeCode, sizeof(dataTypeCode));
        readFile.read(reinterpret_cast<char *>(&colorMapOrigin), sizeof(colorMapOrigin));
        readFile.read(reinterpret_cast<char *>(&colorMapLength), sizeof(colorMapLength));
        readFile.read(&colorMapDepth, sizeof(colorMapDepth));
        readFile.read(reinterpret_cast<char *>(&xOrigin), sizeof(xOrigin));
        readFile.read(reinterpret_cast<char *>(&yOrigin), sizeof(yOrigin));
        readFile.read(reinterpret_cast<char *>(&width), sizeof(width));
        readFile.read(reinterpret_cast<char *>(&height), sizeof(height));
        readFile.read(reinterpret_cast<char *>(&bitsPerPixel), sizeof(bitsPerPixel));
        readFile.read(reinterpret_cast<char *>(&imageDescriptor), sizeof(imageDescriptor));
        Header header(idLength, colorMapType, dataTypeCode, colorMapOrigin, colorMapLength, colorMapDepth, xOrigin,
                      yOrigin, width, height, bitsPerPixel, imageDescriptor);
        readFile.close();
        return header;
    }
}

vector<Pixel> readTGAPixelData(string filename) {
    vector<Pixel> pixels;
    ifstream readFile(filename, ios::binary);
    readFile.seekg(18);
    char blueVal; char greenVal; char redVal;
    Header header = readTGAHeaderData(filename);
    int pixelCount = (header.getHeight()) * (header.getWidth());
    for(int i  = 0; i < pixelCount; i++) {
        readFile.read(&blueVal, sizeof(blueVal));
        readFile.read(&greenVal, sizeof(greenVal));
        readFile.read(&redVal, sizeof(redVal));
        Pixel pixel(blueVal, greenVal, redVal);
        pixels.push_back(pixel);
    }
    readFile.close();
    return pixels;
}

void writeToTGAFile(string filenameToWrite, vector<Pixel> pixels, Header header) {
    ofstream writeFile(R"(..\output\)" + filenameToWrite, ios::binary);
    if (!writeFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
    } else {
        writeFile.write(reinterpret_cast<const char*>(&header.getIDLength()), 1);
        writeFile.write(reinterpret_cast<const char*>(&header.getColorMapType()), 1);
        writeFile.write(reinterpret_cast<const char*>(&header.getDataTypeCode()), 1);
        writeFile.write(reinterpret_cast<const char*>(&header.getColorMapOrigin()), 2);
        writeFile.write(reinterpret_cast<const char*>(&header.getColorMapLength()), 2);
        writeFile.write(reinterpret_cast<const char*>(&header.getColorMapDepth()), 1);
        writeFile.write(reinterpret_cast<const char*>(&header.getXOrigin()), 2);
        writeFile.write(reinterpret_cast<const char*>(&header.getYOrigin()), 2);
        writeFile.write(reinterpret_cast<const char*>(&header.getWidth()), 2);
        writeFile.write(reinterpret_cast<const char*>(&header.getHeight()), 2);
        writeFile.write(reinterpret_cast<const char*>(&header.getBitsPerPixel()), 1);
        writeFile.write(reinterpret_cast<const char*>(&header.getImageDescriptor()), 1);
        for (const auto &pixel : pixels) {
            writeFile.write(reinterpret_cast<const char*>(&pixel.blueVal), 1);
            writeFile.write(reinterpret_cast<const char*>(&pixel.greenVal), 1);
            writeFile.write(reinterpret_cast<const char*>(&pixel.redVal), 1);
        }
    }
    writeFile.close();
}


vector<Pixel> multiplyBlend(vector<Pixel> file1Pixels, vector<Pixel> file2Pixels) {
    vector<Pixel> newPixels;
    std::transform(file1Pixels.begin(), file1Pixels.end(), file2Pixels.begin(), std::back_inserter(newPixels),
                   [](const Pixel& pixel1, const Pixel& pixel2) {
                       unsigned char redVal = static_cast<unsigned char>((static_cast<int>(pixel1.redVal) * static_cast<int>(pixel2.redVal)) / 255.0f + 0.5f);
                       unsigned char blueVal = static_cast<unsigned char>((static_cast<int>(pixel1.blueVal) * static_cast<int>(pixel2.blueVal)) / 255.0f + 0.5f);
                       unsigned char greenVal = static_cast<unsigned char>((static_cast<int>(pixel1.greenVal) * static_cast<int>(pixel2.greenVal)) / 255.0f + 0.5f);
                       return Pixel(blueVal, greenVal, redVal);
                   });
    return newPixels;
}

vector<Pixel> subtractBlend(vector<Pixel> file1Pixels, vector<Pixel> file2Pixels) {
    vector<Pixel> newPixels;

    for(int i = 0; i < file1Pixels.size(); i++) {
        unsigned char redVal = static_cast<unsigned char>(std::clamp((((file1Pixels[i].redVal / 255.0f) - (file2Pixels[i].redVal / 255.0f)) * 255.0f) + 0.5f, 0.0f, 255.0f));
        unsigned char greenVal = static_cast<unsigned char>(std::clamp((((file1Pixels[i].greenVal / 255.0f) - (file2Pixels[i].greenVal / 255.0f)) * 255.0f) + 0.5f, 0.0f, 255.0f));
        unsigned char blueVal = static_cast<unsigned char>(std::clamp((((file1Pixels[i].blueVal / 255.0f) - (file2Pixels[i].blueVal / 255.0f)) * 255.0f) + 0.5f, 0.0f, 255.0f));

        Pixel newPixel(blueVal, greenVal, redVal);
        newPixels.push_back(newPixel);
    }

    return newPixels;
}

vector<Pixel> screenBlend(vector<Pixel> file1Pixels, vector<Pixel> file2Pixels) {
    vector<Pixel> newPixels;
    for(int i = 0; i < file1Pixels.size(); i++) {
        unsigned char redVal = static_cast<unsigned char>(std::clamp(((1 - ((1 - (file1Pixels[i].redVal / 255.0f))*(1 - (file2Pixels[i].redVal / 255.0f)))) * 255)+0.5f, 0.0f, 255.0f));
        unsigned char greenVal = static_cast<unsigned char>(std::clamp(((1 - ((1 - (file1Pixels[i].greenVal / 255.0f))*(1 - (file2Pixels[i].greenVal / 255.0f)))) * 255)+0.5f, 0.0f, 255.0f));
        unsigned char blueVal = static_cast<unsigned char>(std::clamp(((1 - ((1 - (file1Pixels[i].blueVal / 255.0f))*(1 - (file2Pixels[i].blueVal / 255.0f)))) * 255)+0.5f, 0.0f, 255.0f));
        Pixel newPixel(blueVal, greenVal, redVal);
        newPixels.push_back(newPixel);
    }
    return newPixels;
}

vector<Pixel> overlayBlend(vector<Pixel> file1Pixels, vector<Pixel> file2Pixels) {
    vector<Pixel> newPixels;
    for(int i = 0; i < file1Pixels.size(); i++) {
        unsigned char redVal = static_cast<unsigned char>(std::clamp(
                ((file2Pixels[i].redVal <= 127.5) ?
                 (2 * (file1Pixels[i].redVal / 255.0f) * (file2Pixels[i].redVal / 255.0f)) :
                 (1 - 2 * (1 - (file1Pixels[i].redVal / 255.0f)) * (1 - (file2Pixels[i].redVal / 255.0f))))
                * 255 + 0.5f,
                0.0f, 255.0f));

        unsigned char blueVal = static_cast<unsigned char>(std::clamp(
                ((file2Pixels[i].blueVal <= 127.5) ?
                 (2 * (file1Pixels[i].blueVal / 255.0f) * (file2Pixels[i].blueVal / 255.0f)) :
                 (1 - 2 * (1 - (file1Pixels[i].blueVal / 255.0f)) * (1 - (file2Pixels[i].blueVal / 255.0f))))
                * 255 + 0.5f,
                0.0f, 255.0f));

        unsigned char greenVal = static_cast<unsigned char>(std::clamp(
                ((file2Pixels[i].greenVal <= 127.5) ?
                 (2 * (file1Pixels[i].greenVal / 255.0f) * (file2Pixels[i].greenVal / 255.0f)) :
                 (1 - 2 * (1 - (file1Pixels[i].greenVal / 255.0f)) * (1 - (file2Pixels[i].greenVal / 255.0f))))
                * 255 + 0.5f,
                0.0f, 255.0f));

        Pixel newPixel(blueVal, greenVal, redVal);
        newPixels.push_back(newPixel);
    }
    return newPixels;
}

vector<Pixel> extraCredit(vector<Pixel> car, vector<Pixel> circles, vector<Pixel> pattern1, vector<Pixel> text) {

}

bool compareFiles(string filepath1, string filepath2) {
    Header headerFile1 = readTGAHeaderData(filepath1);
    Header headerFile2 = readTGAHeaderData(filepath2);
    vector<Pixel> pixelsFile1 = readTGAPixelData(filepath1);
    vector<Pixel> pixelsFile2 = readTGAPixelData(filepath2);
    bool compareHeaders = headerFile1 == headerFile2;
    if(!compareHeaders) {
        cout << "Headers not equal" << endl;
        return false;
    }
    for(int i = 0; i < pixelsFile1.size(); i++) {
        if(pixelsFile1[i].greenVal != pixelsFile2[i].greenVal) {
            cout << "Green Value for Pixel #" << i << " is not equal" << endl;
            cout << "My File Value: " << static_cast<int>(pixelsFile1[i].greenVal) <<
                 ", Value it Should Be: " << static_cast<int>(pixelsFile2[i].greenVal) << endl;
            return false;
        }
        if(pixelsFile1[i].blueVal != pixelsFile2[i].blueVal) {
            cout << "Blue Value for Pixel #" << i << " is not equal" << endl;
            cout << "My File Value: " << static_cast<int>(pixelsFile1[i].blueVal) <<
                 ", Value it Should Be: " << static_cast<int>(pixelsFile2[i].blueVal) << endl;
            return false;
        }
        if(pixelsFile1[i].redVal != pixelsFile2[i].redVal) {
            cout << "Red Value for Pixel #" << i << " is not equal" << endl;
            cout << "My File Value: " << static_cast<int>(pixelsFile1[i].redVal) <<
                 ", Value it Should Be: " << static_cast<int>(pixelsFile2[i].redVal) << endl;
            return false;
        }
    }
    return true;
}

int passedTest(bool val, int num) {
    if(val) {
        cout << "Test #" << num << "...... Passed!" << endl;
        return 1;
    } else {
        cout << "Test #" << num << "...... Failed!" << endl;
        return 0;
    }
}

int main() {
    // Part 1 - Use Multiply blending mode to combine “layer1.tga” (top layer) with “pattern1.tga” (bottom).
    vector<Pixel> part1Pixels = multiplyBlend(
            readTGAPixelData(R"(..\input\layer1.tga)"),
            readTGAPixelData(R"(..\input\pattern1.tga)"));

    Header header = readTGAHeaderData(R"(..\input\layer1.tga)");
    writeToTGAFile("part1.tga", part1Pixels, header);

    // Part 2 - Use the Subtract blending mode to combine “layer2.tga” (top layer) with “car.tga” (bottom layer).
    vector<Pixel> part2Pixels = subtractBlend(
            readTGAPixelData(R"(..\input\car.tga)"),
            readTGAPixelData(R"(..\input\layer2.tga)"));

    Header header2 = readTGAHeaderData(R"(..\input\car.tga)");
    writeToTGAFile("part2.tga", part2Pixels, header2);

    // Part 3 - Use the Multiply blending mode to combine “layer1.tga” with “pattern2.tga”, and store the
    //results temporarily. Load the image “text.tga” and, using that as the top layer, combine it with
    //the previous results of layer1/pattern2 using the Screen blending mode.
    vector<Pixel> part3Pixels1 = multiplyBlend(
            readTGAPixelData(R"(..\input\layer1.tga)"),
            readTGAPixelData(R"(..\input\pattern2.tga)"));

    vector<Pixel> part3Pixels = screenBlend(part3Pixels1,
                                            readTGAPixelData(R"(..\input\text.tga)"));

    Header header3 = readTGAHeaderData(R"(..\input\layer1.tga)");
    writeToTGAFile("part3.tga", part3Pixels, header3);

    // Part 4 - Multiply “layer2.tga” with “circles.tga”, and store it. Load “pattern2.tga” and, using that as the
    //top layer, combine it with the previous result using the Subtract blending mode.
    vector<Pixel> part4Pixels1 = multiplyBlend(readTGAPixelData(R"(..\input\layer2.tga)"),
                                               readTGAPixelData(R"(..\input\circles.tga)"));
    vector<Pixel> part4Pixels = subtractBlend(part4Pixels1,
                                              readTGAPixelData(R"(..\input\pattern2.tga)"));
    Header header4 = readTGAHeaderData(R"(..\input\pattern2.tga)");
    writeToTGAFile("part4.tga", part4Pixels, header4);

    // Part 5 - Combine “layer1.tga” (as the top layer) with “pattern1.tga” using the Overlay blending mode.
    vector<Pixel> part5Pixels = overlayBlend(
            readTGAPixelData(R"(..\input\layer1.tga)"),
            readTGAPixelData(R"(..\input\pattern1.tga)"));
    Header header5 = readTGAHeaderData(R"(..\input\layer1.tga)");
    writeToTGAFile("part5.tga", part5Pixels, header5);

    // Part 6 - Load “car.tga” and add 200 to the green channel.
    vector<Pixel> oldPixelsPart6 = readTGAPixelData(R"(..\input\car.tga)");
    vector<Pixel> part6Pixels;
    for(int i = 0; i < oldPixelsPart6.size(); i++) {
        unsigned char greenVal = std::clamp(static_cast<int>(oldPixelsPart6[i].greenVal) + 200, 0, 255);
        Pixel newPixel(oldPixelsPart6[i].blueVal, greenVal, oldPixelsPart6[i].redVal);
        part6Pixels.push_back(newPixel);
    }
    Header header6 = readTGAHeaderData(R"(..\input\car.tga)");
    writeToTGAFile("part6.tga", part6Pixels, header6);

    // Part 7 - Load “car.tga” and scale (multiply) the red channel by 4, and the blue channel by 0. This will
    //increase the intensity of any red in the image, while negating any blue it may have.
    vector<Pixel> oldPixelsPart7 = readTGAPixelData(R"(..\input\car.tga)");
    vector<Pixel> part7Pixels;
    for(int i = 0; i < oldPixelsPart7.size(); i++) {
        unsigned char redVal = std::clamp(static_cast<int>(oldPixelsPart7[i].redVal) * 4, 0, 255);
        unsigned char blueVal = std::clamp(static_cast<int>(oldPixelsPart7[i].blueVal) * 0, 0, 255);
        Pixel newPixel(blueVal, oldPixelsPart6[i].greenVal, redVal);
        part7Pixels.push_back(newPixel);
    }
    Header header7 = readTGAHeaderData(R"(..\input\car.tga)");
    writeToTGAFile("part7.tga", part7Pixels, header7);

    // Part 8 - Load “car.tga” and write each channel to a separate file: the red channel should be “part8_r.tga”,
    //the green channel should be “part8_g.tga”, and the blue channel should be “part8_b.tga”
    vector<Pixel> oldPixelsPart8 = readTGAPixelData(R"(..\input\car.tga)");
    vector<Pixel> part8PixelsR;
    vector<Pixel> part8PixelsG;
    vector<Pixel> part8PixelsB;
    for(int i = 0; i < oldPixelsPart8.size(); i++) {
        Pixel newPixelR(oldPixelsPart8[i].redVal, oldPixelsPart8[i].redVal, oldPixelsPart8[i].redVal);
        Pixel newPixelG(oldPixelsPart8[i].greenVal, oldPixelsPart8[i].greenVal, oldPixelsPart8[i].greenVal);
        Pixel newPixelB(oldPixelsPart8[i].blueVal, oldPixelsPart8[i].blueVal, oldPixelsPart8[i].blueVal);
        part8PixelsR.push_back(newPixelR);
        part8PixelsG.push_back(newPixelG);
        part8PixelsB.push_back(newPixelB);
    }
    Header header8 = readTGAHeaderData(R"(..\input\car.tga)");
    writeToTGAFile("part8_r.tga", part8PixelsR, header8);
    writeToTGAFile("part8_g.tga", part8PixelsG, header8);
    writeToTGAFile("part8_b.tga", part8PixelsB, header8);

    // Part 9 - Load “layer_red.tga”, “layer_green.tga” and “layer_blue.tga”, and combine the three files into
    //one file. The data from “layer_red.tga” is the red channel of the new image, layer_green is
    //green, and layer_blue is blue.
    vector<Pixel> oldPixelsPart9R = readTGAPixelData(R"(..\input\layer_red.tga)");
    vector<Pixel> oldPixelsPart9G = readTGAPixelData(R"(..\input\layer_green.tga)");
    vector<Pixel> oldPixelsPart9B = readTGAPixelData(R"(..\input\layer_blue.tga)");
    vector<Pixel> part9Pixels;
    for(int i = 0; i < oldPixelsPart9R.size(); i++) {
        Pixel newPixel(oldPixelsPart9B[i].blueVal, oldPixelsPart9G[i].greenVal, oldPixelsPart9R[i].redVal);
        part9Pixels.push_back(newPixel);
    }
    Header header9 = readTGAHeaderData(R"(..\input\layer_red.tga)");
    writeToTGAFile("part9.tga", part9Pixels, header9);

    // Part 10 - Load “text2.tga”, and rotate it 180 degrees, flipping it upside down. This is easier than you think!
    //Try diagramming the data of an image (such as earlier in this document). What would the data
    //look like if you flipped it? Now, how to write some code to accomplish that...?
    vector<Pixel> oldPixelsPart10 = readTGAPixelData(R"(..\input\text2.tga)");
    vector<Pixel> part10Pixels;
    for(int i = 0; i < oldPixelsPart10.size(); i++) {
        part10Pixels.push_back(*(oldPixelsPart10.rbegin() + i));
    }
    Header header10 = readTGAHeaderData(R"(..\input\text2.tga)");
    writeToTGAFile("part10.tga", part10Pixels, header10);

    bool part1 = compareFiles(R"(..\output\part1.tga)",
                              R"(..\examples\EXAMPLE_part1.tga)");
    bool part2 = compareFiles(R"(..\output\part2.tga)",
                              R"(..\examples\EXAMPLE_part2.tga)");
    bool part3 = compareFiles(R"(..\output\part3.tga)",
                              R"(..\examples\EXAMPLE_part3.tga)");
    bool part4 = compareFiles(R"(..\output\part4.tga)",
                              R"(..\examples\EXAMPLE_part4.tga)");
    bool part5 = compareFiles(R"(..\output\part5.tga)",
                              R"(..\examples\EXAMPLE_part5.tga)");
    bool part6 = compareFiles(R"(..\output\part6.tga)",
                              R"(..\examples\EXAMPLE_part6.tga)");
    bool part7 = compareFiles(R"(..\output\part7.tga)",
                              R"(..\examples\EXAMPLE_part7.tga)");

    bool part8r = compareFiles(R"(..\output\part8_r.tga)",
                               R"(..\examples\EXAMPLE_part8_r.tga)");
    bool part8g = compareFiles(R"(..\output\part8_g.tga)",
                               R"(..\examples\EXAMPLE_part8_g.tga)");
    bool part8b = compareFiles(R"(..\output\part8_b.tga)",
                               R"(..\examples\EXAMPLE_part8_b.tga)");

    bool part9 = compareFiles(R"(..\output\part9.tga)",
                              R"(..\examples\EXAMPLE_part9.tga)");
    bool part10 = compareFiles(R"(..\output\part10.tga)",
                               R"(..\examples\EXAMPLE_part10.tga)");

    int passed = 0;

    passed += passedTest(part1, 1);
    passed += passedTest(part2, 2);
    passed += passedTest(part3, 3);
    passed += passedTest(part4, 4);
    passed += passedTest(part5, 5);
    passed += passedTest(part6, 6);
    passed += passedTest(part7, 7);
    if(part8r && part8g && part8b) {
        passed += passedTest(true, 8);
    } else {
        passed += passedTest(false, 8);
    }
    passed += passedTest(part9, 9);
    passed += passedTest(part10, 10);
    cout << "Test results: " << passed << " / 10";

}

