#include "stdafx.h"
#include "Sample.h"

namespace {
static bool SHOULD_USE_SAMPLE = false;
}

bool AdventOfCode::Sample::should_use_sample()
{
  return ::SHOULD_USE_SAMPLE;
}

void AdventOfCode::Sample::set_use_sample(const bool should_use_sample) {
  ::SHOULD_USE_SAMPLE = should_use_sample;
}
