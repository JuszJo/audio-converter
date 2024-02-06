#include <iostream>
#include <fstream>
#include <vector>

// TODO: Make MP3 to WAV file converter

char byteToBinary(char byte) {
    std::cout << "Byte to convert: " << byte << std::endl;
    char binary[9];

    for (int i = 7; i >= 0; --i) {
        binary[7 - i] += ((byte >> i) & 1) ? '1' : '0';
    }

    binary[8] = '\0';

    std::cout << "Binary: " << binary << std::endl;

    return *binary;
}

void intToBinary(int value) {
    std::cout << "Byte to convert: " << value << std::endl;
    char binary[9];

    for (int i = 7; i >= 0; --i) {
        binary[7 - i] += ((value >> i) & 1) ? '1' : '0';
    }

    binary[8] = '\0';

    std::cout << "Binary: " << binary << std::endl;
}

int main() {
    // const wchar_t* audioName = L"Boa.mp3";
    const wchar_t* audioName = L"Duvet.wav";

    // open the mp3 file in binary mode
    std::ifstream mp3File(audioName, std::ios::binary);

    if(!mp3File.is_open()) {
        std::cerr << "Error: Could not open the MP3 file." << std::endl;
        return 1;
    }

    // RIFF
    const int riffByteSize = 4;
    char riffBytes[riffByteSize + 1];
    mp3File.read(riffBytes, riffByteSize);
    riffBytes[riffByteSize] = '\0';

    // LESS
    const int lessByteSize = 4;
    char buffer[lessByteSize];
    int lessBytes;
    mp3File.read(buffer, lessByteSize);
    for(int i = 0; i < lessByteSize; ++i) {
        lessBytes |= ((unsigned char)buffer[i] << (i * 8));
    }

    // WAV
    const int wavByteSize = 4;
    char wavBytes[wavByteSize + 1];
    mp3File.read(wavBytes, wavByteSize);
    wavBytes[wavByteSize] = '\0';

    // FMT
    const int fmtByteSize = 4;
    char fmtBytes[fmtByteSize + 1];
    mp3File.read(fmtBytes, fmtByteSize);
    fmtBytes[fmtByteSize] = '\0';

    // WAV section chunk
    const int wavSectionChunkByteSize = 4;
    char wavSectionChunkBytes[wavSectionChunkByteSize + 1];
    mp3File.read(wavSectionChunkBytes, wavSectionChunkByteSize);
    wavSectionChunkBytes[wavSectionChunkByteSize] = '\0';
    int wavSectionValue;
    for(char byte : wavSectionChunkBytes) {
        if(byte == '\0') continue;
        // perform bitmasking to only use lower 8 bits and use
        // OR operator to add the resulting bits
        wavSectionValue = (wavSectionValue << 8) | (byte & 0xFF);
    }

     // Close the file
    mp3File.close();

    std::cout << "RIFF Bytes: " << riffBytes << std::endl;
    std::cout << "LESS Bytes: " << lessBytes << std::endl;
    std::cout << "WAV Bytes: " << wavBytes << std::endl;
    std::cout << "FMT Bytes: " << fmtBytes << std::endl;
    std::cout << "WAV Section Chunk Bytes: " << wavSectionValue << std::endl;

    return 1;
}