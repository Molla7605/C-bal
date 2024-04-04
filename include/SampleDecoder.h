#ifndef SAMPLE_DECODER_H
#define SAMPLE_DECODER_H

#include <vector>
#include <array>
#include <map>
#include <iostream>
#include <algorithm>
#include <kiss_fft.h>
#include <cmath>
#include <complex>
#include <deque>
#include <string>
#include "Chords.h"

struct Note
{
	int freq;
	std::string name;
};

class SampleDecoder
{
private:
	const float PI = 3.141592f;
	float* m_window = nullptr;
	std::map<int, std::string> freqPitchMap;
	std::array<int, 17> freqIndexMap;
	//std::array<std::string, 17> pitchNameMap;

public:
	SampleDecoder()
	{
		m_window = new float[16384];
		for (int count = 0; count < 16384; count++)
		{
			m_window[count] = 0.54f - 0.46f * cos((2 * PI * count) / (float)16384);
		}
		freqIndexMap[0] = 98;
		freqIndexMap[1] = 131;
		freqIndexMap[2] = 165;
		freqIndexMap[3] = 175;
		freqIndexMap[4] = 196;
		freqIndexMap[5] = 220;
		freqIndexMap[6] = 330;
		freqIndexMap[7] = 349;
		freqIndexMap[8] = 392;
		freqIndexMap[9] = 440;
		freqIndexMap[10] = 494;
		freqIndexMap[11] = 523;
		freqIndexMap[12] = 659;
		freqIndexMap[13] = 698;
		freqIndexMap[14] = 783;
		freqIndexMap[15] = 880;
		freqIndexMap[16] = 988;

		freqPitchMap.insert(std::make_pair<int,std::string>(98,std::string("2G")));
		freqPitchMap.insert(std::make_pair<int,std::string>(131,std::string("3C")));
		freqPitchMap.insert(std::make_pair<int,std::string>(165,std::string("3E")));
		freqPitchMap.insert(std::make_pair<int,std::string>(175,std::string("3F")));
		freqPitchMap.insert(std::make_pair<int,std::string>(196,std::string("3G")));
		freqPitchMap.insert(std::make_pair<int,std::string>(220,std::string("3A")));
		freqPitchMap.insert(std::make_pair<int,std::string>(330,std::string("4E")));
		freqPitchMap.insert(std::make_pair<int,std::string>(349,std::string("4F")));
		freqPitchMap.insert(std::make_pair<int,std::string>(392,std::string("4G")));
		freqPitchMap.insert(std::make_pair<int,std::string>(440,std::string("4A")));
		freqPitchMap.insert(std::make_pair<int,std::string>(494,std::string("4B")));
		freqPitchMap.insert(std::make_pair<int,std::string>(523,std::string("5C")));
		freqPitchMap.insert(std::make_pair<int,std::string>(659,std::string("5E")));
		freqPitchMap.insert(std::make_pair<int,std::string>(698,std::string("5F")));
		freqPitchMap.insert(std::make_pair<int,std::string>(783,std::string("5G")));
		freqPitchMap.insert(std::make_pair<int,std::string>(880,std::string("5A")));
		freqPitchMap.insert(std::make_pair<int,std::string>(988,std::string("5B")));
	}

	~SampleDecoder()
	{
		delete[] m_window;
	}

	void decodeSample(std::vector<Phrase>& phrases, std::vector<int16_t>& samples)
	{
		std::cout << samples.size() << "\n";
		if (samples.size() % 44100 != 0)
			return;

		std::cout << "SIZE CHECK OK\n";
		
		int chord_count = samples.size() / 44100;

		if (chord_count % 4 != 0)
			return;



		std::cout << "CHORD COUNT OK : " << chord_count << "\n";

		int phrase_count = chord_count / 4;


		std::cout << "PHRASE COUNT OK : " << phrase_count << "\n";

		kiss_fft_cfg fft_cfg = kiss_fft_alloc(16384, 0, NULL, NULL);
	
		std::complex<float>* m_complex = new std::complex<float>[16384];
		std::complex<float>* m_result  = new std::complex<float>[16384];


		for (int phrase_index = 0; phrase_index < phrase_count; phrase_index++)
		{
			for (int chord_index = 0; chord_index < 4; chord_index++)
			{
				size_t offset = (phrase_index * 44100 * 4) + (chord_index * 44100) + 10000;
				for (int index = 0; index < 16384; index++)
				{
					m_complex[index] = ((float)samples[offset + index]/(float)SHRT_MAX) * m_window[index];
				}

				kiss_fft(fft_cfg, reinterpret_cast<kiss_fft_cpx*>(m_complex), reinterpret_cast<kiss_fft_cpx*>(m_result));


				std::vector<int> freqs;
				for(int f = 0; f < 16384; f++)
				{
					float gain = std::abs(m_result[f]);
					if (gain > 850)
					{
						int length = freqs.size();
						bool flag = false;
						for (int i = 0; i < length; i++)
						{
							if (std::abs(f - freqs[i]) < 10)
							{
								flag = true;
								break;
							}
						}

						if(!flag)
							freqs.push_back(f);
					}

				}

				for (size_t index = 0; index < 4; index++)
				{
					float freq = (float)(freqs[index]) * 44100.f / 16384.f;
					int note_index = std::round(std::log2(freq / 440) * 12 + 55);
					//std::cout << note_index / 12 << " " << note_index % 12 << "\n";

					const auto lb = std::lower_bound(freqIndexMap.begin(), freqIndexMap.end(), freqs[index]);
					int ans = lb != freqIndexMap.end() ? *lb : freqIndexMap.back();
					if (lb != freqIndexMap.begin()) {
						auto prec = lb - 1;
						if (std::abs(ans - freqs[index]) > std::abs(*prec - freqs[index]))
						{
							ans = *prec;
						}
					}

					std::cout << ans << " " << freqPitchMap[ans] << "\n";



					//std::cout << ) << "\n";
				}
				std::cout << "==============\n";
			}
			std::cout << "++++++++++++++\n";
		}


		delete[] m_complex;
		delete[] m_result;
		kiss_fft_free(fft_cfg);
	}

private:
};

#endif // !SAMPLE_DECODER_H
