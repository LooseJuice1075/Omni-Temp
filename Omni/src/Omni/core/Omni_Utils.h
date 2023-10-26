#pragma once

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include <glm.hpp>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <algorithm>

namespace Omni
{
	/// <summary>
	/// Loads raw binary data from specified file
	/// </summary>
	void* loadFileBinary(std::string filePath);

	/// <summary>
	/// Loads raw binary data from specified file
	/// </summary>
	void* loadFileBinary(std::string filePath, uint32_t* size);

	std::string loadFile(std::string filePath);

	void writeFile(std::string filePath, std::string fileContent);

	/// <summary>
	/// All colors are clamped between 0 and 255
	/// </summary>
	uint32_t rgbaToHex(int r, int g, int b, int a);
	uint32_t rgbaToHex(glm::vec4 color);
	//uint32_t encodeNormalRgba8(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f);
}
