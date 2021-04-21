#pragma once

//-----------------------------------------------------------------------------
// mvUtilities
//
//     - This file contains typically platform specific functions. May need
//       to rename to a more appropriate name.
//     
//-----------------------------------------------------------------------------

#include <vector>
#include <string>

namespace Marvel {

	// new
	void* LoadTextureFromFile(const char* filename, int& width, int& height);
	void* LoadTextureFromArray(unsigned width, unsigned height, float* data);
	void FreeTexture(void* texture);

}