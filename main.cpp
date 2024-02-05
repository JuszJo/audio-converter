#include <iostream>
#include <fstream>
#include <vector>

// TODO: Make MP3 to WAV file converter

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
    char lessBytes[lessByteSize + 1];
    mp3File.read(lessBytes, lessByteSize);
    lessBytes[lessByteSize] = '\0';

     // Close the file
    mp3File.close();

    std::cout << "RIFF Byte: " << riffBytes << std::endl;
    std::cout << "LESS Byte: " << lessBytes << std::endl;

    return 1;
}