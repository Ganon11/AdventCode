#include "stdafx.h"
#include "Filenames.h"
#include "Sample.h"

const wchar_t* AdventOfCode::Filenames::get_filename()
{
   return AdventOfCode::Sample::USE_SAMPLE ? SAMPLE_FILENAME : INPUT_FILENAME;
}
