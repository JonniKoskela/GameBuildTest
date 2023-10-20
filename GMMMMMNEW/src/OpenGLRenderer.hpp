#pragma once
#include "OpenGLRenderer.h"
#include "fileUtils.h"
#define STB_IMAGE_IMPLEMENTATION    
#include "stb/stb_image.h"
#include "input.h"
#include "renderInterface.h"
#ifndef GLHDR
#define GLHDR
#include <gl/GL.h>
#endif
// GLCONSTANTS_______________________________________________________

const char* TEXTURE_PATH = "assets/textures/textureAtlas.png";


// GLSTRUCTS_________________________________________________________

struct GLContext
{
	GLuint programID;
	GLuint textureID;
	GLuint transformSBOID;
	GLuint screenSizeID;
	GLuint orthoProjectionID;
};

// GLGLOBALS_________________________________________________________

static GLContext glContext;

//GLFUNCTIONS________________________________________________________

static void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* user)
{
	if (severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH)
	{
		std::cerr << message << "\n";
	}
	else {
		std::cerr << message << "\n";
	}
}




bool gl_init(BumpAllocator* transientStorage)
{
	load_gl_functions();

	glDebugMessageCallback(&gl_debug_callback, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEBUG_OUTPUT);

	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);

	int fileSize = 0;
	char* vertShader = readFile(const_cast<char*>("assets/shaders/quad.vert"), &fileSize, transientStorage);
	char* fragShader = readFile(const_cast<char*>("assets/shaders/quad.frag"), &fileSize, transientStorage);

	if (!vertShader || !fragShader)
	{
		std::cerr << "faield to load shaders";
		return false;
	}
	glShaderSource(vertShaderID, 1, &vertShader, 0);
	glShaderSource(fragShaderID, 1, &fragShader, 0);

	glCompileShader(vertShaderID);
	glCompileShader(fragShaderID);

	{
		int success;
		char shaderLog[2048] = {};

		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertShaderID, 2048, 0, shaderLog);
			std::cerr << "failed to compile vShader" << "\n";
		}
	}
	{
		int success;
		char shaderLog[2048] = {};

		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShaderID, 2048, 0, shaderLog);
			std::cerr << "failed to compile shaders" << "\n";
		}
	}
	glContext.programID = glCreateProgram();
	glAttachShader(glContext.programID, vertShaderID);
	glAttachShader(glContext.programID, fragShaderID);
	glLinkProgram(glContext.programID);

	{
		int programSuccess;
		char programInfoLog[512];
		glGetProgramiv(glContext.programID, GL_LINK_STATUS, &programSuccess);

		if (!programSuccess)
		{
			glGetProgramInfoLog(glContext.programID, 512, 0, programInfoLog);

			std::cerr << "failed to link program" << "\n";
			return false;
		}
	}

	glDetachShader(glContext.programID, vertShaderID);
	glDetachShader(glContext.programID, fragShaderID);


	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);


	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	{
		int width, height, channels;
		char* data = (char*)stbi_load(TEXTURE_PATH, &width, &height, &channels, 4);
		assert(data && "stbi_load failure");

		glGenTextures(1, &glContext.textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glContext.textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}



	//transform storage buffer
	{
	glGenBuffers(1, &glContext.transformSBOID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glContext.transformSBOID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Transform) * renderData->transforms.maxElements, renderData->transforms.elements, GL_DYNAMIC_DRAW);
	}


	// uniforms
	{
		glContext.orthoProjectionID = glGetUniformLocation(glContext.programID, "orthoProjection");
		glContext.screenSizeID = glGetUniformLocation(glContext.programID, "screenSize");
	}


	glEnable(GL_FRAMEBUFFER_SRGB);
	glDisable(0x809D);
	glDepthFunc(GL_GREATER);
	glUseProgram(glContext.programID);

	return true;
}

void gl_render()
{
	glViewport(0, 0, input->screenSize.x, input->screenSize.y);
	glClearColor(119.0f / 255.0f, 33.0f / 255.0f, 111.0f / 255.0f, 1.0f);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Vec2 screenSize = { (float)input->screenSize.y, (float)input->screenSize.y };
	glUniform2fv(glContext.screenSizeID, 1, &screenSize.x);



	OrtographicCamera2D camera = renderData->gameCamera;

	Mat4 orthoProjection = orthographicProjection(
		camera.position.x - camera.dimensions.x / 2.0f,
		camera.position.x + camera.dimensions.x/ 2.0f,
		camera.position.y - camera.dimensions.y/ 2.0f,
		camera.position.y + camera.dimensions.y / 2.0f);

	glUniformMatrix4fv(glContext.orthoProjectionID, 1, GL_FALSE, &orthoProjection.ax);
	{
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Transform) * renderData->transforms.maxElements, renderData->transforms.elements);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderData->transforms.count);

		renderData->transforms.clear();
	}
}