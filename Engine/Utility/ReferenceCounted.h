#pragma once
#include <stdint.h>

#define REFERENCE_COUNT_FUNCTIONS \
	uint16_t DecrementReferenceCount() { \
		referenceCount--; \
		return referenceCount; \
	} \
	\
	void IncrementReferenceCount() { \
			referenceCount++; \
	}

#define REFERENCE_COUNT_VARIABLES uint16_t referenceCount = 1;