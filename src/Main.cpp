#include <iostream>
#include <fstream>
#include <vector>
#include <cinttypes>
#include <cmath>
#include <bit>

#include "Chords.h"
#include "SampleBuilder.h"
#include "WavFile.h"
#include "SampleDecoder.h"

struct WavHeader {
    uint32_t ChunkID = 0x46464952;
    uint32_t ChunkSize = 0; // file size - 8
    uint32_t Format = 0x45564157;
    uint32_t SubChunk1_ID = 0x20746d66;
    uint32_t SubChunk1_Size = 16;

    uint16_t AudioFormat = 1;//PCM
    uint16_t NumChannels = 1;

    uint32_t SampleRate = 44100;
    uint32_t ByteRate = 0;//SampleRate * NumCh. * BitPerSample / 8
    uint16_t BlockAlign = 0;//NumChannels * BitsPerSample/8
    uint16_t BitPerSample = 16;
    uint32_t SubChunk2_ID = 0x61746164;
    uint32_t SubChunk2_Size = 0;// NumSamples * NumChannels * BitsPerSample / 8

};

int paritybit(unsigned char data)
{
    return ~(std::popcount(data) % 2);
}

DataBlock makeDataBlock(unsigned char data)
{
    DataBlock block = { 0, };

    block.data = data;
    block.p = paritybit(block.data);
    return block;
}

int main(int argc,char** argv) {
    /*
    if (argc == 1)
    {
        return 0;
    }

    if (strcmp(argv[1], "-v"))
    {
        std::cout << "verson : 0.0.1\n";
        return 0;
    }
    else if (!strcmp(argv[1], "-b"))
    {
        if (argc < 3)
        {
            return 0;
        }

        std::string file_path = argv[2];
    }
    else if (!strcmp(argv[1], "-d"))
    {
        if (argc < 3)
        {
            return 0;
        }

        std::string file_path = argv[2];


        WavFile file;
        if (!file.readFile(file_path))
        {
            return 0;
        }

        SampleDecoder dec;
        std::vector<Phrase> phs;
        dec.decodeSample(phs, file.samples);

        

    }
    */
    
    WavFile Ifile;
    if (!Ifile.readFile("C:\\Users\\jyc76\\cppProject\\C-bal\\out\\build\\x64-Debug\\test.wav"))
    {
        return 0;
    }

    SampleDecoder dec;
    std::vector<Phrase> phs;
    dec.decodeSample(phs, Ifile.samples);
    
    return 0;
    std::vector<DataBlock> blocks(0);
    blocks.push_back(makeDataBlock(0b11101010));
    blocks.push_back(makeDataBlock(0b01010110));
    blocks.push_back(makeDataBlock(0b01110110));
    blocks.push_back(makeDataBlock(0b01001110));

    size_t sample_lenght = 44100 * blocks.size() * 4;

    const float PI = 3.141592f;

    std::vector<int16_t> buffer(sample_lenght);


    SampleBuilder builder;

    size_t offset = 0;
    std::vector<float> f32Buffer(44100 * 4);
    for (auto& block : blocks)
    {


        Phrase ph(block);

        builder.build(ph, f32Buffer);

        float volume = 1.f;

        float delta = 0;
        for (int i = 0; i < 44100 * 4; i++)
        {
            buffer[offset + i] = static_cast<int16_t>(volume * f32Buffer[i] * SHRT_MAX);
            //buffer[i] = sin(PI * i / 180) * SHRT_MAX;
            //buffer[i] = gain * std::sin(440.f * (2.0f * PI) * i / 44100.0f) * SHRT_MAX;

          //std::cout << i << " " << (440.f * (2.0f * PI) * i / 44100.0f) << "\n";
        }
        offset += 44100 * 4;
    }

    WavFile file;
    file.setSampleRate(44100);
    file.setSamples(buffer);
    file.writeFile("test.wav");

    return 0;
}