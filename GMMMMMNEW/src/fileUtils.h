#pragma once
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <iostream>

#define b8 char
#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long) 1024 * x)
#define MB(x) ((unsigned long long) 1024 * KB(x))
#define GB(x) ((unsigned long long) 1024 * MB(x))







struct BumpAllocator {
	size_t capacity;
	size_t used;
	char* memory;
};

BumpAllocator makeBumpAllocator(size_t size)
{
	BumpAllocator bump = {};

	bump.memory = (char*)malloc(size);

	if (bump.memory)
	{
		bump.capacity = size;
		memset(bump.memory, 0, size);
	}
	else 
	{
		assert(false && "failed to allocate memory");
	}
	return bump;
}


char* bumpAlloc(BumpAllocator* allocator, size_t size)
{
	char* result = nullptr;

	size_t alignedSize = ( size + 7 ) & ~ 7;

	if (allocator->used + alignedSize <= allocator->capacity)
	{
		result = allocator->memory + allocator->used;
		allocator->used += alignedSize; 
	}
	else
	{
		assert(false && "allocator full");
	}
	return result;
}


long long getTimeStamp(const char* file)
{
	struct stat fileStat = {};
	stat(file, &fileStat);
	return fileStat.st_mtime;
}


bool file_exists(char* filePath)
{
	assert(filePath && "no filepath");

	auto file = fopen(filePath, "rb");
	if (!file)
	{
		return false;
	}
	fclose(file);

	return true;
}


long getFileSize(char* filePath)
{
	assert(filePath && "no filepath");

	long fileSize = 0;
	auto file = fopen(filePath, "rb");
	if (!file)
	{
		
		return 0;
	}
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	return fileSize;
}


char* readFile(char* filePath, int* fileSize, char* buffer)
{
	assert(filePath && "no filePath");
	assert(fileSize && "no fileSize");

	*fileSize = 0;
	auto file = fopen(filePath, "rb");
	if (!file)
	{
		return nullptr;
	}

	fseek(file, 0, SEEK_END);
	*fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (*fileSize <= 0)
	{
		std::cerr << "failed to determine file size" << std::endl;
	}


	memset(buffer, 0, *fileSize);
	fread(buffer, sizeof(char),*fileSize,file);
	
	fclose(file);

	return buffer;
}


char* readFile(char* filePath, int* fileSize, BumpAllocator * bumpAllocator)
{
	char* file = nullptr;
	long fileSize2 = getFileSize(filePath);

	if (fileSize2)
	{
		char* buffer = bumpAlloc(bumpAllocator, fileSize2+1);

		file = readFile(filePath, fileSize, buffer);
	}

	return file;
}


void writeFile(char* filePath, char* buffer, int size)
{
	assert(filePath && "no filepath");

	auto file = fopen(filePath, "wb");
	if (!file)
	{
		return;
	}

	fwrite(buffer, sizeof(char), size, file);
	fclose(file);
}


bool copyFile(char* fileName, char* outputName, char* buffer)
{
	int fileSize = 0;
	char* data = readFile(fileName, &fileSize, buffer);

	auto outputFile = fopen(outputName, "wb");
	if (!outputFile)
	{
		assert("failed to open output file at bool copyFile()");
		return false;
	}

	int result = fwrite(data, sizeof(char), fileSize, outputFile);
	if (!result)
	{
		return false;
	}

	fclose(outputFile);

	return true;
}


bool copyFile(char* fileName, char* outputName, BumpAllocator* bumpAllocator)
{
	char* file = 0;
	long fileSize = getFileSize(fileName);

	if (fileSize)
	{
		char* buffer = bumpAlloc(bumpAllocator, fileSize);

		return copyFile(fileName, outputName, buffer);
	}
	return false;
}


//MATHFUNCTIONS____________________________________________________________________


struct Vec2
{
	float x;
	float y;

	Vec2 operator/ (float scalar)
	{
		return{ x / scalar, y / scalar };
	}

	Vec2 operator- (Vec2 other)
	{
		return { x - other.x, y - other.y };
	}
};

struct iVec2
{
	int x;
	int y;

	iVec2 operator- (iVec2 other)
	{
		return { x - other.x, y - other.y };
	}
};

struct Vec4
{
	union {
		float values[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
	};

	float& operator[](int idx)
	{
		return values[idx];
	}

	Vec4() = default;

	bool operator==(const Vec4& other) const
	{
		return false;
	}
};


struct Mat4{

	union {
		Vec4 values[4];
		struct {
			float ax;
			float bx;
			float cx;
			float dx;

			float ay;
			float by;
			float cy;
			float dy;

			float az;
			float bz;
			float cz;
			float dz;

			float aw;
			float bw;
			float cw;
			float dw;
		};
	};

	Vec4& operator[](int col)
	{
		return values[col];
	}

};

Mat4 orthographicProjection(float left, float right, float top, float bottom)
{
	Mat4 resultMat = {};
	resultMat.aw = -(right + left) / (right - left);
	resultMat.bw = (top + bottom) / (top - bottom);
	resultMat.cw = 0.0f;
	resultMat[0][0] = 2.0f / (right - left);
	resultMat[1][1] = 2.0f / (top - bottom);
	resultMat[2][2] = 1.0f / (1.0f - 0.0f);
	resultMat[3][3] = 1.0f;

	return resultMat;
 }

Vec2 vec_2(iVec2 vec)
{
	return Vec2{ (float)vec.x, (float)vec.y };
}
template<typename T, int N>
struct Array
{
	static constexpr int maxElements = N;
	int count = 0;
	T elements[N];

	T& operator[](int idx)
	{
		assert(idx >= 0 && "idx negative!");
		assert(idx < count && "Idx out of bounds!");
		return elements[idx];
	}

	int add(T element)
	{
		elements[count] = element;
		return count++;
	}

	void remove_idx_and_swap(int idx)
	{
		assert(idx >= 0 && "idx negative!");
		assert(idx < count && "idx out of bounds!");
		elements[idx] = elements[--count];
	}

	void clear()
	{
		count = 0;
	}

	bool is_full()
	{
		return count == N;
	}
};
