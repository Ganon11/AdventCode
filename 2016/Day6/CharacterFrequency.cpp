#include "stdafx.h"
#include "CharacterFrequency.h"

CharacterFrequency::CharacterFrequency(const wchar_t c) : character{ c }, frequency{ 1 } {}

bool CharacterFrequency::operator==(const CharacterFrequency& rhs) {
   return character == rhs.character;
}

bool CharacterFrequency::operator==(const wchar_t c) {
   return character == c;
}
