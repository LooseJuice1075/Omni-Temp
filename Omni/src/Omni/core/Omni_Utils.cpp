#include "ompch.h"
#include "Omni_Utils.h"
#include "renderer/EmbeddedTextures.h"
#include "renderer/Renderer.h"

namespace Omni
{
	//void* loadFileBinary(std::string filePath)
	//{
	//	// Input file pointer
	//	FILE* inFile = fopen(filePath.c_str(), "rb");

	//	// Throw error if file cannot be opened
	//	if (!inFile)
	//	{
	//		OM_CORE_ERROR("[FILE ERROR]: File {0} could not be loaded!", filePath);
	//		// Return 0 if error occurs
	//		return 0;
	//	}

	//	// Find end of file
	//	fseek(inFile, 0, SEEK_END);
	//	// Get file size
	//	long fileSize = ftell(inFile);
	//	// Reset input file pointer position
	//	fseek(inFile, 0, SEEK_SET);

	//	// Allocate memory size
	//	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	//	// Copy data from file into memory buffer
	//	fread(mem->data, 1, fileSize, inFile);
	//	// Place end of file character at the end of the buffer
	//	mem->data[mem->size - 1] = '\0';
	//	fclose(inFile);

	//	return mem->data;
	//}

	//void* loadFileBinary(std::string filePath, uint32_t* size)
	//{
	//	// Input file pointer
	//	FILE* inFile = fopen(filePath.c_str(), "rb");

	//	// Throw error if file cannot be opened
	//	if (!inFile)
	//	{
	//		OM_CORE_ERROR("[FILE ERROR]: File {0} could not be loaded!", filePath);
	//		// Return 0 if error occurs
	//		return 0;
	//	}

	//	// Find end of file
	//	fseek(inFile, 0, SEEK_END);
	//	// Get file size
	//	long fileSize = ftell(inFile);
	//	// Reset input file pointer position
	//	fseek(inFile, 0, SEEK_SET);

	//	// Allocate memory size
	//	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	//	// Copy data from file into memory buffer
	//	fread(mem->data, 1, fileSize, inFile);
	//	// Place end of file character at the end of the buffer
	//	mem->data[mem->size - 1] = '\0';
	//	fclose(inFile);

	//	*size = mem->size;

	//	return mem->data;
	//}

	std::string loadFile(std::string filePath)
	{
		std::stringstream buffer;
		std::ifstream file (filePath);
		if (file.is_open()){
			buffer << file.rdbuf();
			file.close();
		}
		else{
			OM_CORE_ERROR("[FILE ERROR]: File {0} could not be opened!", filePath);
		}

		return buffer.str();
	}

	void writeFile(std::string filePath, std::string fileContent)
	{
		std::ofstream file (filePath);
		if (file.is_open()){
			file << fileContent;
			file.close();
		}
		else{
			OM_CORE_ERROR("[FILE ERROR]: File {0} could not be opened!", filePath);
		}
	}

	uint32_t rgbaToHex(int r, int g, int b, int a)
	{
		r = std::clamp(r, 0, 255);
		g = std::clamp(g, 0, 255);
		b = std::clamp(b, 0, 255);
		a = std::clamp(a, 0, 255);
		uint32_t hexColor = (uint32_t)(r << 24) | (uint32_t)(g << 16) | (uint32_t)(b << 8) | (uint32_t)(a);

		return hexColor;
	}

	uint32_t rgbaToHex(glm::vec4 color)
	{
		int r = color.x * 255;
		int g = color.y * 255;
		int b = color.z * 255;
		int a = color.w * 255;

		r = std::clamp(r, 0, 255);
		g = std::clamp(g, 0, 255);
		b = std::clamp(b, 0, 255);
		a = std::clamp(a, 0, 255);

		uint32_t hexColor = (uint32_t)(r << 24) | (uint32_t)(g << 16) | (uint32_t)(b << 8) | (uint32_t)(a);

		return hexColor;
	}

	//uint32_t encodeNormalRgba8(float _x, float _y, float _z, float _w)
	//{
	//	const float src[] =
	//	{
	//		_x * 0.5f + 0.5f,
	//		_y * 0.5f + 0.5f,
	//		_z * 0.5f + 0.5f,
	//		_w * 0.5f + 0.5f,
	//	};
	//	uint32_t dst;
	//	bx::packRgba8(&dst, src);
	//	return dst;
	//}
}
