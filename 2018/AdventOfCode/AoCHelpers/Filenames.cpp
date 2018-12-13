#include "stdafx.h"
#include "Filenames.h"
#include "Sample.h"

const wchar_t* advent_of_code::Filenames::get_filename()
{
  return advent_of_code::sample::should_use_sample() ? SAMPLE_FILENAME : INPUT_FILENAME;
}
