#include "filenames.h"
#include "sample.h"

const char* advent_of_code::filenames::get_filename()
{
  return advent_of_code::sample::should_use_sample() ? SAMPLE_FILENAME : INPUT_FILENAME;
}
