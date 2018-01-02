#pragma once
#include <stdint.h>

#define REFERENCE_COUNT_FUNCTIONS \
	uint16_t DecrementReferenceCount() { \
		if(referenceCount > 0){ \
			referenceCount--; \
		} \
		return referenceCount; \
	} \
	\
	void IncrementReferenceCount() { \
			referenceCount++; \
	}

#define REFERENCE_COUNT_VARIABLES uint16_t referenceCount = 1;