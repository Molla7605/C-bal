#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <array>

struct DataBlock
{
	uint8_t data;
	bool p;//odd
};

//C scale base Audio lock


class Phrase//in C Major scale
{
public:
	enum class Type
	{
		C,
		Am,
		F,
		G,
		None
	};
private:
	std::string Type_str[5] = { "C","Am","F","G" , "   "};

	std::string Chord_member[5] = { "CEG" , "ACE" , "FAC", "GBD", "   " };
public:
	enum class Root
	{
		_1,//000 001 010
		_3,//011 100 101
		_5 //110 111 --- 
	};

	//(000) 011 111 - 9bit --> _1 _135 / _3 _135 / _5 _351 
	enum class Sequence
	{
		_135,//CEG
		_351,//EGC
		_513//GCE
	};

	enum class Modifier
	{
		None,
		ADD9,
		_7th
	};
	
	class Chord
	{
	public:
		Type type = Type::None;
		Root root = Root::_1;
		Sequence seq = Sequence::_135;
		Modifier modifier = Modifier::None;
		Chord() = default;
		Chord(Type _type, Root _root, Sequence _seq, Modifier _modifier) : type(_type), root(_root), seq(_seq), modifier(_modifier) {}
	};

public:
	std::array<Chord,4> chords;

	Phrase() = default;

	Phrase(DataBlock& block)
	{
		buildBlock(block);
	}


	std::string getChordName(Chord chord)
	{
		std::string chord_right = "";
	
		

		switch (chord.seq)
		{
		case Sequence::_135:
			chord_right = std::string() + Chord_member[(int32_t)chord.type][0] + Chord_member[(int32_t)chord.type][1] + Chord_member[(int32_t)chord.type][2];
			break;
		case Sequence::_351:
			chord_right = std::string() + Chord_member[(int32_t)chord.type][1] + Chord_member[(int32_t)chord.type][2] + Chord_member[(int32_t)chord.type][0];
			break;
		case Sequence::_513:
			chord_right = std::string() + Chord_member[(int32_t)chord.type][2] + Chord_member[(int32_t)chord.type][0] + Chord_member[(int32_t)chord.type][1];
			break;
		default:
			break;
		}
		//return std::format("{} {} {}", chord.type, chord.type, chord.root);
		return std::format("{}/{} ({})", Type_str[(int32_t)chord.type], Chord_member[(int32_t)chord.type][(int32_t)chord.root] , chord_right);
	}


	//input : 3 bit aligned data
	Chord getChord(uint8_t data, uint8_t section_idx)
	{
		Type Chord_type = Type::None;

		switch (section_idx)
		{
		case 0:
			Chord_type = Type::C;
			break;
		case 1:
			Chord_type = Type::Am;
			break;
		case 2:
			Chord_type = Type::F;
			break;
		case 3:
			Chord_type = Type::G;
			break;
		default:
			break;
		}


		switch (data)
		{
		case 0b000:
			return { Chord_type, Root::_1,Sequence::_135, Modifier::None };
			break;
		case 0b001:
			return { Chord_type, Root::_1,Sequence::_351, Modifier::None };
			break;
		case 0b010:
			return { Chord_type, Root::_1,Sequence::_513, Modifier::None };
			break;
		case 0b011:
			return { Chord_type, Root::_3,Sequence::_135, Modifier::None };
			break;
		case 0b100:
			return { Chord_type, Root::_3,Sequence::_351, Modifier::None };
			break;
		case 0b101:
			return { Chord_type, Root::_3,Sequence::_513, Modifier::None };
			break;
		case 0b110:
			return { Chord_type, Root::_5,Sequence::_135, Modifier::None };
			break;
		case 0b111:
			return { Chord_type, Root::_5,Sequence::_351, Modifier::None };
			break;
		}
		return {};
	}




	void buildBlock(DataBlock& block)
	{

		uint8_t section_0 = (block.data & 0b00000111);
		uint8_t section_1 = (block.data & 0b00111000) >> 3;
		uint8_t section_2 = ((block.data & 0b11000000) >> 6) + (block.p ? (1 << 2) : 0);

		chords.at(0) = getChord(section_0, 0);
		chords.at(1) = getChord(section_1, 1);
		chords.at(2) = getChord(section_2, 2);
		chords.at(3) = { Type::G,Root::_1, Sequence::_135,Modifier::None };

		return;
	}

	
};