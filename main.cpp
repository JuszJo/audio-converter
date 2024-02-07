#include <iostream>
#include <fstream>
#include <vector>

// TODO: Make MP3 to WAV file converter

template <typename T>

char byteToBinary(T byte) {
    char binary[9];

    for (int i = 7; i >= 0; --i) {
        binary[7 - i] = ((byte >> i) & 1) ? '1' : '0';
    }

    binary[8] = '\0';

    std::cout << "Binary: " << binary << std::endl;

    return *binary;
}

namespace LittleEndian {
    int byteToInteger(char bytes[], int size) {
        int value;

        for(int i = 0; i < size; ++i) {
            value |= (unsigned char)bytes[i] << (i * 8);
        }

        return value;
    }
}

int parseHeaders(const int byteSize, std::ifstream& file) {
    char* buffer = new char[byteSize];
    char newBuffer = *buffer;
    delete[] buffer;
    file.read(&newBuffer, byteSize);
    return LittleEndian::byteToInteger(&newBuffer, byteSize);
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
    int lessBytes = parseHeaders(lessByteSize, mp3File);

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
    int wavSectionValue = parseHeaders(wavSectionChunkByteSize, mp3File);

    // WAV type format, PCM = 1, others = some form of compression
    const int wavTypeFormatByteSize = 2;
    int wavTypeFormatValue = parseHeaders(wavTypeFormatByteSize, mp3File);

    // Number of channels
    // 0x01 for mono, 0x02 for stereo
    const int channelsByteSize = 2;
    int channelValue = parseHeaders(channelsByteSize, mp3File);

    // Sample Frequency (Hertz)
    const int sampleFreqByteSize = 4;
    int sampleFreq = parseHeaders(sampleFreqByteSize, mp3File);

    // Byte Rate
    const int rateSize = 4;
    int byteRateValue = parseHeaders(rateSize, mp3File);

    // Block alignment
    const int blockByteSize = 2;
    int blockAlignValue = parseHeaders(blockByteSize, mp3File);

    // Bits per sample
    const int bitSampleByteSize = 2;
    int bitsPerSampleValue = parseHeaders(bitSampleByteSize, mp3File);

    // Skip next 34 bytes (ADLT)
    mp3File.seekg(34, std::ios::cur);

    // Data description header
    const int dataDescByteSize = 4;
    char dataDescBytes[dataDescByteSize + 1];
    mp3File.read(dataDescBytes, dataDescByteSize);
    dataDescBytes[dataDescByteSize] = '\0';

    // Data Size
    const int dataByteSize = 4;
    int dataSizeValue = parseHeaders(dataByteSize, mp3File);

    // Close the file
    mp3File.close();

    std::cout << "RIFF Bytes: " << riffBytes << std::endl;
    std::cout << "LESS Bytes: " << lessBytes << std::endl;
    std::cout << "WAV Bytes: " << wavBytes << std::endl;
    std::cout << "FMT Bytes: " << fmtBytes << std::endl;
    std::cout << "WAV Section Chunk Bytes: " << wavSectionValue << std::endl;
    std::cout << "WAV Type Format Bytes: " << wavTypeFormatValue << std::endl;
    std::cout << "Channel Value: " << channelValue << std::endl;
    std::cout << "Sample Frequency Value: " << sampleFreq << std::endl;
    std::cout << "Byte Rate Value: " << byteRateValue << std::endl;
    std::cout << "Block Alignment Value: " << blockAlignValue << std::endl;
    std::cout << "Bits Per Sample Value: " << bitsPerSampleValue << std::endl;
    std::cout << "Data Description Bytes: " << dataDescBytes << std::endl;
    std::cout << "Data Size Bytes: " << dataSizeValue << std::endl;

    return 1;
}