/* Author: Polina Abratenko

To use this unit test: 
    Re-build workspace to compile this test.
    Compiled binary will be outputted to this path: 
        <your workspace>/build_slf7.x86_64/sbndaq_artdaq_core/
        slf7.x86_64.e20.s112.prof/bin/
    Make sure original binary data file ("data.bin") is in the above path. 
    Run the compiled unit test with ./unitTest

Output:
    Script will output printout of data words in the structs to the terminal. 
    This script also creates a file "output.txt" with original full binary data
    converted to hex. 
*/

#include "DAPHNEFragment.hh"

#include <iostream>     // to use std::cout
#include <fstream>      // to use std::ifstream
#include <sstream>      // to use std::stringstream
#include <bit>          // to use std::byteswap
#include <cstdio>
#include <iomanip>      // for std::setw

using namespace std;
using namespace sbndaq;

void byteSwap16(uint16_t *data, size_t numElements) {
    for (size_t i = 0; i < numElements; i++) {
        data[i] = ((data[i] & 0xFF) << 8) | ((data[i] >> 8) & 0xFF);
    }
}

int main() {
    ifstream file("data.bin", ios::binary);

    if (!file.is_open()) {
        cout << "Failed to open file!" << endl;
        return 1;
    }

    // get length of file:
    file.seekg (0, file.end);
    int length = file.tellg();
    file.seekg (0, file.beg);
    //cout << "Alternative size of file: " << length << endl;
    // create a stringstream and set the format to hex
    stringstream ss;
    ss << hex;

    const int arraySize = 1028;
    int fileSize = sizeof(file);
    uint16_t data[arraySize];

    cout << "sizeof(file) = " << fileSize << endl;
    cout << "sizeof(data) = " << sizeof(data) << endl;
    file.read((char*)&data, sizeof(data));

    //int counter = 0; // counter for filled array size

    for (int i = 0; i < length; i++) {
        //cout << "Data[" << i << "]: " << data[i] << endl;
    }

    //cout << "Data: " << data << endl;
    //cout << "sizeof(data): " << sizeof(data) << endl;

    size_t numElements = sizeof(data) / sizeof(data[0]);
    //cout << "sizeof(data[0]: " << sizeof(data[0]) << endl;
    //cout << "numElements = sizeof(data) / sizeof(data[0]): " << numElements << endl;
    byteSwap16(data, numElements);

    DAPHNEData daphne;
    daphne.numFEBs = 1;
    daphne.hdr.wordCount = data[0];
    daphne.hdr.microbunch = (data[1] << 16) | (data[2] << 16) | data[3];
    daphne.hdr.timestamp = (data[4] << 16) | data[5];
    daphne.hdr.customWords = (data[6] << 16) | data[7];
    cout << "daphne.hdr.wordCount: " << hex << daphne.hdr.wordCount << endl;
    cout << "daphne.hdr.microbunch: " << hex << daphne.hdr.microbunch << endl;
    cout << "daphne.hdr.timestamp: " << hex << daphne.hdr.timestamp << endl;
    cout << "daphne.hdr.customWords: " << hex << daphne.hdr.customWords << endl;

    daphne.febhdr.wordCount = data[8];
    daphne.febhdr.statusWord1 = data[9];
    daphne.febhdr.microbunch = (data[10] << 16) | data[11];
    daphne.febhdr.emptyWord = (data[12]);
    daphne.febhdr.statusWord2 = (data[13]);
    cout << "daphne.febhdr.wordCount: " << hex << daphne.febhdr.wordCount << endl;
    cout << "daphne.febhdr.statusWord1: " << hex << daphne.febhdr.statusWord1 << endl;
    cout << "daphne.febhdr.microbunch: " << hex <<  daphne.febhdr.microbunch << endl;
    cout << "daphne.febhdr.emptyWord: " << hex << daphne.febhdr.emptyWord << endl;
    cout << "daphne.febhdr.statusWord2: " << hex << daphne.febhdr.statusWord2 << endl;

    daphne.febch.channel = data[14];
    cout << "daphne.febch.channel: " << hex << daphne.febch.channel << endl;
    for (size_t i = 15; i < 29; i++) {
        daphne.febch.waveform[i-15] = data[i];
        cout << hex << daphne.febch.waveform[i-15] << endl;
    }

    /* Note: Buggy code
    for (size_t j = 0; j < 1; j++) {// loop for number of channels
        daphne.febch.channel = data[14];
        cout << "daphne.febch.channel: " << hex << daphne.febch.channel << endl;
        for (size_t i = 15; i < 29; i++) {
            daphne.febch.waveform[i-15] = data[i];
            cout << hex << daphne.febch.waveform[i-15] << endl;
        }
        daphne.febfrg.channels[j] = daphne.febch.waveform;
        cout << "daphne.febfrg.channels[j] = daphne.febch.waveform: " << endl;
        cout << daphne.febfrg.channels[0] << endl;
    }
    */

    //cout << "newest daphne.hdr.wordCount: " << hex << daphne.hdr.wordCount << endl;
    //cout << "myArray " << hex << myArray[0] << endl;

    /*
    for (size_t i = 0; i < numElements; i++) {
        cout << hex << data[i] << endl;
    }
    */

    // byteswap
    //for (int i = 0; i < sizeof(data); i++) {
    //    std::byteswap(data[i]);
    //}

    freopen("output.txt","w",stdout);

   // int linesize = 8;

    for (size_t i = 0; i < numElements; i++) {
        if ((i % 8 == 0) && (i > 0)) {cout << endl;}
        cout << setfill('0') << setw(4) << hex << data[i] << " ";
    }

    /*
    // iterate through the binary data and convert each byte to hex
    for (int i = 0; i < sizeof(data); i++) {
        ss << static_cast<int>(data[i]) << " ";
    }
    // output the hex string
    cout << "Hex representation: " << ss.str() << endl;
    */

    file.close();

    return 0;
}