// Written by Alex Nye dba Preposterous Calculations. Released under the MIT license.
#pragma once


// Iterates over each element of an array, starting at the given index, and wrapping around.
#define CIRCULATE(StartIndex, Array, ConditionAndEffect) \
	if(Array.Num()) \
	for ( \
		uint8 i = (StartIndex) % Array.Num(), \
		NumSteps = 0; \
		\
		NumSteps < Array.Num(); \
		\
		i = (i + 1) % Array.Num(), \
		NumSteps++ \
	) { \
		{ ConditionAndEffect } \
	}