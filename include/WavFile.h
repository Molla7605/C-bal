#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class WavFile
{
public:
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
private:
    uint32_t sample_rate = 44100;
    WavHeader file_header = {};
public:
    std::vector<int16_t> samples;
    WavFile() = default;

    void setSampleRate(uint32_t sample_rate)
    {
        this->sample_rate = sample_rate;
    }

    void setSamples(std::vector<int16_t>& samples)
    {
        uint32_t sample_lenght = samples.size();

        this->samples = samples;
    
        file_header.ChunkSize = 44 + sample_lenght * 2 - 8;
        file_header.ByteRate = 44100 * 1 * 16 / 8;
        file_header.BlockAlign = 1 * 16 / 8;
        file_header.SubChunk2_Size = sample_lenght * 1 * 16 / 8;

    }

    bool writeFile(const std::string& path)
    {
        std::ofstream out_file_stream(path, std::ios::binary);
        if (!out_file_stream.is_open())
        {
            return false;
        }

        out_file_stream.write(reinterpret_cast<const char*>(&file_header), sizeof(WavHeader));
        out_file_stream.write(reinterpret_cast<const char*>(&samples[0]), samples.size() * 2);
        out_file_stream.close();
        return true;
    }

    bool readFile(const std::string& path)
    {
        std::ifstream in_file_stream(path, std::ios::binary);

        if (!in_file_stream.is_open() || !in_file_stream.good()) {
            return false;
        }

        WavHeader header;
        in_file_stream.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

        sample_rate = header.SampleRate;
        uint32_t sample_lenght = header.SubChunk2_Size / 2;

        samples.resize(sample_lenght);
        in_file_stream.read(reinterpret_cast<char*>(&samples[0]), samples.size() * 2);
        return true;
    }
};

#endif // !WAV_FILE_H
