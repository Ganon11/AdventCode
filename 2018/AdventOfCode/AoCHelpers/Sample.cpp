#include "stdafx.h"
#include "Sample.h"

namespace {
static bool SHOULD_USE_SAMPLE = false;
}

bool advent_of_code::sample::should_use_sample()
{
  return ::SHOULD_USE_SAMPLE;
}

void advent_of_code::sample::set_use_sample(const bool should_use_sample) {
  ::SHOULD_USE_SAMPLE = should_use_sample;
}
