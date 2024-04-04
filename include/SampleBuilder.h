#pragma once
#include <cmath>
#include "Chords.h"

class SampleBuilder
{
public:
	void build(Phrase phrase,std::vector<float>& samples)
	{
		//C
		std::array<float, 4> freqs_C = chord2freq(phrase.chords[0]);
		//Am
		std::array<float, 4> freqs_Am = chord2freq(phrase.chords[1]);
		//F
		std::array<float, 4> freqs_F = chord2freq(phrase.chords[2]);
		//G
		std::array<float, 4> freqs_G = chord2freq(phrase.chords[3]);

		build_samples(samples, freqs_C, 44100 * 0, 44100, 44100);
		build_samples(samples, freqs_Am, 44100 * 1, 44100, 44100);
		build_samples(samples, freqs_F, 44100 * 2, 44100, 44100);
		build_samples(samples, freqs_G, 44100 * 3, 44100, 44100);
		
	}
	
	void build_samples(std::vector<float>& samples, std::array<float,4> freqs,size_t sample_offset,size_t sample_length, size_t sample_rate)
	{

		const float PI = 3.141592f;

		float gain_bass = 1.f;
		float gain_mid = 1.f;
		float gain_fade_inout = 1.f;

		float fade_in_lenght = 0.01f;
		float fade_out_start = 0.85f;

		for (size_t index = sample_offset; index < sample_offset + sample_length; index++)
		{
			float sample_1 = gain_bass * std::sin(freqs[0] * (2.0f * PI) * (index - sample_offset) / 44100.0f);
			float sample_2 = gain_mid * std::sin(freqs[1] * (2.0f * PI) * (index - sample_offset) / 44100.0f);
			float sample_3 = gain_mid * std::sin(freqs[2] * (2.0f * PI) * (index - sample_offset) / 44100.0f);
			float sample_4 = gain_mid * std::sin(freqs[3] * (2.0f * PI) * (index - sample_offset) / 44100.0f);
			samples[index] = gain_fade_inout * (sample_1 + sample_2 + sample_3 + sample_4) / 4.0f;

			if (index - sample_offset < (float)sample_rate * fade_in_lenght)
			{
				gain_fade_inout = (float)(index - sample_offset) / ((float)sample_rate * fade_in_lenght);
				//std::cout << "F-IN " << gain_fade_inout << "\n";
			}
			else if ((index - sample_offset) > (float)sample_rate * fade_out_start)
			{
				gain_fade_inout = 1 - ((index - sample_offset) - ((float)sample_rate * fade_out_start)) / ((float)sample_rate * (1-fade_out_start));
				//std::cout << "F-OUT " << gain_fade_inout << "\n";
			}
			else
			{
				gain_fade_inout = 1;
				//std::cout << "F-SUS " << gain_fade_inout << "\n";
			}

		}
	}
	

	std::array<float, 4> chord2freq(Phrase::Chord chord)
	{
		std::cout << "========================\n";

		std::array<float, 4> freq{};
		if (chord.type == Phrase::Type::C)
		{
			switch (chord.root)
			{
			case Phrase::Root::_1:
				freq[0] = getFreq(3, 1);
				break;
			case Phrase::Root::_3:
				freq[0] = getFreq(3, 5);
				break;
			case Phrase::Root::_5:
				freq[0] = getFreq(2, 8);
				break;
			default:
				break;
			}

			switch (chord.seq)
			{
			case Phrase::Sequence::_135:
				freq[1] = getFreq(5, 1);
				freq[2] = getFreq(5, 5);
				freq[3] = getFreq(5, 8);
				break;
			case Phrase::Sequence::_351:
				freq[1] = getFreq(4, 5);
				freq[2] = getFreq(4, 8);
				freq[3] = getFreq(5, 1);
				break;
			case Phrase::Sequence::_513:
				freq[1] = getFreq(4, 8);
				freq[2] = getFreq(5, 1);
				freq[3] = getFreq(5, 5);
				break;

			default:
				break;
			}
		}
		//ACE
		else if (chord.type == Phrase::Type::Am)
		{
			switch (chord.root)
			{
			case Phrase::Root::_1:
				freq[0] = getFreq(3, 10);
				break;
			case Phrase::Root::_3:
				freq[0] = getFreq(3, 1);
				break;
			case Phrase::Root::_5:
				freq[0] = getFreq(3, 5);
				break;
			default:
				break;
			}

			switch (chord.seq)
			{
			case Phrase::Sequence::_135:
				freq[1] = getFreq(4, 10);
				freq[2] = getFreq(5, 1);
				freq[3] = getFreq(5, 5);
				break;
			case Phrase::Sequence::_351:
				freq[1] = getFreq(5, 1);
				freq[2] = getFreq(5, 5);
				freq[3] = getFreq(5, 10);
				break;
			case Phrase::Sequence::_513:
				freq[1] = getFreq(4, 5);
				freq[2] = getFreq(4, 10);
				freq[3] = getFreq(5, 1);
				break;
			default:
				break;
			}
		}

		//ACE
		else if (chord.type == Phrase::Type::F)
		{
			switch (chord.root)
			{
			case Phrase::Root::_1:
				freq[0] = getFreq(3, 6);
				break;
			case Phrase::Root::_3:
				freq[0] = getFreq(3, 10);
				break;
			case Phrase::Root::_5:
				freq[0] = getFreq(3, 1);
				break;
			default:
				break;
			}

			switch (chord.seq)
			{
			case Phrase::Sequence::_135:
				freq[1] = getFreq(4, 6);
				freq[2] = getFreq(4, 10);
				freq[3] = getFreq(5, 1);
				break;
			case Phrase::Sequence::_351:
				freq[1] = getFreq(4, 10);
				freq[2] = getFreq(5, 1);
				freq[3] = getFreq(5, 6);
				break;
			case Phrase::Sequence::_513:
				freq[1] = getFreq(5, 1);
				freq[2] = getFreq(5, 6);
				freq[3] = getFreq(5, 10);
				break;
			default:
				break;
			}
		}
		else
		{
			freq[0] = getFreq(3, 8);
			freq[1] = getFreq(4, 8);
			freq[2] = getFreq(4, 12);
			freq[3] = getFreq(5, 3);
		}
		return freq;
	}

	inline float getFreq(int32_t octave, int32_t index) noexcept
	{
		std::cout << "OCT : " << octave << " INDEX: " << index << "\n";
		return std::pow(2.0f, (octave - 1.0f)) * 55.0f * std::pow(2.0f, (index - 10.0f) / 12.0f);
	}
};