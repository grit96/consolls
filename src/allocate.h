#pragma once

#include <malloc.h>
#include <cstring>
#include "types.h"


namespace Allocate
{

template<typename X>
X *
allocate(u32 n_elements)
{
  u32 bytes_size = sizeof(X) * n_elements;
  X *result = (X *)malloc(bytes_size);
  memset(result, 0, bytes_size);

  return result;
}


inline void
unallocate(void *ptr) { free(ptr); }


} // namespace Allocate