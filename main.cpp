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
    mp3File.read(buffer, lessByteSize);
    int lessBytes = LittleEndian::byteToInteger(buffer, lessByteSize);

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
    char wavSectionChunkBytes[wavSectionChunkByteSize];
    mp3File.read(wavSectionChunkBytes, wavSectionChunkByteSize);
    int wavSectionValue = LittleEndian::byteToInteger(wavSectionChunkBytes, wavSectionChunkByteSize);

    // WAV type format, PCM = 1, others = some form of compression
    const int wavTypeFormatByteSize = 2;
    char wavTypeFormatBytes[wavTypeFormatByteSize];
    mp3File.read(wavTypeFormatBytes, wavTypeFormatByteSize);
    int wavTypeFormatValue = LittleEndian::byteToInteger(wavTypeFormatBytes, wavTypeFormatByteSize);

    // Number of channels
    // 0x01 for mono, 0x02 for stereo
    const int channelsByteSize = 2;
    char channelsBytes[channelsByteSize];
    mp3File.read(channelsBytes, channelsByteSize);
    int channelValue = LittleEndian::byteToInteger(channelsBytes, channelsByteSize);

    // Sample Frequency (Hertz)
    const int sampleFreqByteSize = 4;
    char sampleFreqBytes[sampleFreqByteSize];
    mp3File.read(sampleFreqBytes, sampleFreqByteSize);
    int sampleFreq = LittleEndian::byteToInteger(sampleFreqBytes, sampleFreqByteSize);

    // Byte Rate
    const int rateSize = 4;
    char rateBytes[rateSize];
    mp3File.read(rateBytes, rateSize);
    int byteRateValue = LittleEndian::byteToInteger(rateBytes, rateSize);

    // Block alignment
    const int blockByteSize = 2;
    char blockBytes[blockByteSize];
    mp3File.read(blockBytes, blockByteSize);
    int blockAlignValue = LittleEndian::byteToInteger(blockBytes, blockByteSize);

    // Bits per sample
    const int bitSampleByteSize = 2;
    char bitSampleBytes[bitSampleByteSize];
    mp3File.read(bitSampleBytes, bitSampleByteSize);
    int bitsPerSampleValue = LittleEndian::byteToInteger(bitSampleBytes, bitSampleByteSize);

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

    return 1;
}