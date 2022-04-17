#include "SimpleStandardLibrary.h"

i32 GlobalSize = 0;
MemoryBlock** MemoryBlocks = NULL;
PrintAllocationSourceType PrintSourceType = PRINT_ALLOCATION_SOURCE_TYPE_NONE;
i32 AllocCalls = 0;
i32 FreeCalls  = 0;
