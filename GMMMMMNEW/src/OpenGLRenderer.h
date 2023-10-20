#pragma once
#include "thirdParty/glcorearb.h"
#include "platform.h"


static PFNGLCREATEPROGRAMPROC glCreateProgram_ptr;
static PFNGLDRAWBUFFERPROC glDrawBuffer_ptr;
static PFNGLCREATESHADERPROC glCreateShader_ptr;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation_ptr;
static PFNGLUNIFORM1FPROC glUniform1f_ptr;
static PFNGLUNIFORM2FVPROC glUniform2fv_ptr;
static PFNGLUNIFORM3FVPROC glUniform3fv_ptr;
static PFNGLUNIFORM1IPROC glUniform1i_ptr;
static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv_ptr;
static PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor_ptr;
static PFNGLACTIVETEXTUREPROC glActiveTexture_ptr;
static PFNGLBUFFERSUBDATAPROC glBufferSubData_ptr;
static PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced_ptr;
static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer_ptr;
static PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus_ptr;
static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers_ptr;
static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D_ptr;
static PFNGLDRAWBUFFERSPROC glDrawBuffers_ptr;
static PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers_ptr;
static PFNGLBLENDFUNCIPROC glBlendFunci_ptr;
static PFNGLBLENDEQUATIONPROC glBlendEquation_ptr;
static PFNGLCLEARBUFFERFVPROC glClearBufferfv_ptr;
static PFNGLSHADERSOURCEPROC glShaderSource_ptr;
static PFNGLCOMPILESHADERPROC glCompileShader_ptr;
static PFNGLGETSHADERIVPROC glGetShaderiv_ptr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog_ptr;
static PFNGLATTACHSHADERPROC glAttachShader_ptr;
static PFNGLLINKPROGRAMPROC glLinkProgram_ptr;
static PFNGLVALIDATEPROGRAMPROC glValidateProgram_ptr;
static PFNGLGETPROGRAMIVPROC glGetProgramiv_ptr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog_ptr;
static PFNGLGENBUFFERSPROC glGenBuffers_ptr;
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays_ptr;
static PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation_ptr;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray_ptr;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray_ptr;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer_ptr;
static PFNGLBINDBUFFERPROC glBindBuffer_ptr;
static PFNGLBINDBUFFERBASEPROC glBindBufferBase_ptr;
static PFNGLBUFFERDATAPROC glBufferData_ptr;
static PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv_ptr;
static PFNGLUSEPROGRAMPROC glUseProgram_ptr;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays_ptr;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers_ptr;
static PFNGLDELETEPROGRAMPROC glDeleteProgram_ptr;
static PFNGLDETACHSHADERPROC glDetachShader_ptr;
static PFNGLDELETESHADERPROC glDeleteShader_ptr;
static PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced_ptr;
static PFNGLGENERATEMIPMAPPROC glGenerateMipmap_ptr;
static PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback_ptr;

void load_gl_functions()
{
	glCreateProgram_ptr = (PFNGLCREATEPROGRAMPROC) platform_load_gl_function(const_cast<char*>("glCreateProgram"));
	glCreateShader_ptr = (PFNGLCREATESHADERPROC) platform_load_gl_function(const_cast<char*>("glCreateShader"));
	glGetUniformLocation_ptr = (PFNGLGETUNIFORMLOCATIONPROC) platform_load_gl_function(const_cast<char*>("glGetUniformLocation"));
	glUniform1f_ptr = (PFNGLUNIFORM1FPROC) platform_load_gl_function(const_cast<char*>("glUniform1f"));
	glUniform2fv_ptr = (PFNGLUNIFORM2FVPROC) platform_load_gl_function(const_cast<char*>("glUniform2fv"));
	glUniform3fv_ptr = (PFNGLUNIFORM3FVPROC) platform_load_gl_function(const_cast<char*>("glUniform3fv"));
	glUniform1i_ptr = (PFNGLUNIFORM1IPROC) platform_load_gl_function(const_cast<char*>("glUniform1i"));
	glUniformMatrix4fv_ptr = (PFNGLUNIFORMMATRIX4FVPROC) platform_load_gl_function(const_cast<char*>("glUniformMatrix4fv"));
	glVertexAttribDivisor_ptr = (PFNGLVERTEXATTRIBDIVISORPROC) platform_load_gl_function(const_cast<char*>("glVertexAttribDivisor"));
	glActiveTexture_ptr = (PFNGLACTIVETEXTUREPROC) platform_load_gl_function(const_cast<char*>("glActiveTexture"));
	glBufferSubData_ptr = (PFNGLBUFFERSUBDATAPROC) platform_load_gl_function(const_cast<char*>("glBufferSubData"));
	glDrawArraysInstanced_ptr = (PFNGLDRAWARRAYSINSTANCEDPROC) platform_load_gl_function(const_cast<char*>("glDrawArraysInstanced"));
	glBindFramebuffer_ptr = (PFNGLBINDFRAMEBUFFERPROC) platform_load_gl_function(const_cast<char*>("glBindFramebuffer"));
	glCheckFramebufferStatus_ptr = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) platform_load_gl_function(const_cast<char*>("glCheckFramebufferStatus"));
	glGenFramebuffers_ptr = (PFNGLGENFRAMEBUFFERSPROC) platform_load_gl_function(const_cast<char*>("glGenFramebuffers"));
	glFramebufferTexture2D_ptr = (PFNGLFRAMEBUFFERTEXTURE2DPROC) platform_load_gl_function(const_cast<char*>("glFramebufferTexture2D"));
	glDrawBuffers_ptr = (PFNGLDRAWBUFFERSPROC) platform_load_gl_function(const_cast<char*>("glDrawBuffers"));
	glDeleteFramebuffers_ptr = (PFNGLDELETEFRAMEBUFFERSPROC) platform_load_gl_function(const_cast<char*>("glDeleteFramebuffers"));
	glBlendFunci_ptr = (PFNGLBLENDFUNCIPROC) platform_load_gl_function(const_cast<char*>("glBlendFunci"));
	glBlendEquation_ptr = (PFNGLBLENDEQUATIONPROC) platform_load_gl_function(const_cast<char*>("glBlendEquation"));
	glClearBufferfv_ptr = (PFNGLCLEARBUFFERFVPROC) platform_load_gl_function(const_cast<char*>("glClearBufferfv"));
	glShaderSource_ptr = (PFNGLSHADERSOURCEPROC) platform_load_gl_function(const_cast<char*>("glShaderSource"));
	glCompileShader_ptr = (PFNGLCOMPILESHADERPROC) platform_load_gl_function(const_cast<char*>("glCompileShader"));
	glGetShaderiv_ptr = (PFNGLGETSHADERIVPROC) platform_load_gl_function(const_cast<char*>("glGetShaderiv"));
	glGetShaderInfoLog_ptr = (PFNGLGETSHADERINFOLOGPROC) platform_load_gl_function(const_cast<char*>("glGetShaderInfoLog"));
	glAttachShader_ptr = (PFNGLATTACHSHADERPROC) platform_load_gl_function(const_cast<char*>("glAttachShader"));
	glLinkProgram_ptr = (PFNGLLINKPROGRAMPROC) platform_load_gl_function(const_cast<char*>("glLinkProgram"));
	glValidateProgram_ptr = (PFNGLVALIDATEPROGRAMPROC) platform_load_gl_function(const_cast<char*>("glValidateProgram"));
	glGetProgramiv_ptr = (PFNGLGETPROGRAMIVPROC) platform_load_gl_function(const_cast<char*>("glGetProgramiv"));
	glGetProgramInfoLog_ptr = (PFNGLGETPROGRAMINFOLOGPROC) platform_load_gl_function(const_cast<char*>("glGetProgramInfoLog"));
	glGenBuffers_ptr = (PFNGLGENBUFFERSPROC) platform_load_gl_function(const_cast<char*>("glGenBuffers"));
	glGenVertexArrays_ptr = (PFNGLGENVERTEXARRAYSPROC) platform_load_gl_function(const_cast<char*>("glGenVertexArrays"));
	glGetAttribLocation_ptr = (PFNGLGETATTRIBLOCATIONPROC) platform_load_gl_function(const_cast<char*>("glGetAttribLocation"));
	glBindVertexArray_ptr = (PFNGLBINDVERTEXARRAYPROC) platform_load_gl_function(const_cast<char*>("glBindVertexArray"));
	glEnableVertexAttribArray_ptr = (PFNGLENABLEVERTEXATTRIBARRAYPROC) platform_load_gl_function(const_cast<char*>("glEnableVertexAttribArray"));
	glVertexAttribPointer_ptr = (PFNGLVERTEXATTRIBPOINTERPROC) platform_load_gl_function(const_cast<char*>("glVertexAttribPointer"));
	glBindBuffer_ptr = (PFNGLBINDBUFFERPROC) platform_load_gl_function(const_cast<char*>("glBindBuffer"));
	glBindBufferBase_ptr = (PFNGLBINDBUFFERBASEPROC) platform_load_gl_function(const_cast<char*>("glBindBufferBase"));
	glBufferData_ptr = (PFNGLBUFFERDATAPROC) platform_load_gl_function(const_cast<char*>("glBufferData"));
	glGetVertexAttribPointerv_ptr = (PFNGLGETVERTEXATTRIBPOINTERVPROC) platform_load_gl_function(const_cast<char*>("glGetVertexAttribPointerv"));
	glUseProgram_ptr = (PFNGLUSEPROGRAMPROC) platform_load_gl_function(const_cast<char*>("glUseProgram"));
	glDeleteVertexArrays_ptr = (PFNGLDELETEVERTEXARRAYSPROC) platform_load_gl_function(const_cast<char*>("glDeleteVertexArrays"));
	glDeleteBuffers_ptr = (PFNGLDELETEBUFFERSPROC) platform_load_gl_function(const_cast<char*>("glDeleteBuffers"));
	glDeleteProgram_ptr = (PFNGLDELETEPROGRAMPROC) platform_load_gl_function(const_cast<char*>("glDeleteProgram"));
	glDetachShader_ptr = (PFNGLDETACHSHADERPROC) platform_load_gl_function(const_cast<char*>("glDetachShader"));
	glDeleteShader_ptr = (PFNGLDELETESHADERPROC) platform_load_gl_function(const_cast<char*>("glDeleteShader"));
	glDrawElementsInstanced_ptr = (PFNGLDRAWELEMENTSINSTANCEDPROC) platform_load_gl_function(const_cast<char*>("glDrawElementsInstanced"));
	glGenerateMipmap_ptr = (PFNGLGENERATEMIPMAPPROC) platform_load_gl_function(const_cast<char*>("glGenerateMipmap"));
	glDebugMessageCallback_ptr = (PFNGLDEBUGMESSAGECALLBACKPROC) platform_load_gl_function(const_cast<char*>("glDebugMessageCallback"));
}


GLAPI GLuint APIENTRY glCreateProgram(void)
{
	return glCreateProgram_ptr();
}

GLuint glCreateShader(GLenum shaderType)
{
	return glCreateShader_ptr(shaderType);
}

GLint glGetUniformLocation(GLuint program, const GLchar* name)
{
	return glGetUniformLocation_ptr(program, name);
}

void glUniform1f(GLint location, GLfloat v0)
{
	glUniform1f_ptr(location, v0);
}

void glUniform2fv(GLint location, GLsizei count, const GLfloat* value)
{
	glUniform2fv_ptr(location, count, value);
}

void glUniform3fv(GLint location, GLsizei count, const GLfloat* value)
{
	glUniform3fv_ptr(location, count, value);
}

void glUniform1i(GLint location, GLint v0)
{
	glUniform1i_ptr(location, v0);
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glUniformMatrix4fv_ptr(location, count, transpose, value);
}

void glVertexAttribDivisor(GLuint index, GLuint divisor)
{
	glVertexAttribDivisor_ptr(index, divisor);
}

void glActiveTexture(GLenum texture)
{
	glActiveTexture_ptr(texture);
}

void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
	glBufferSubData_ptr(target, offset, size, data);
}

void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount)
{
	glDrawArraysInstanced_ptr(mode, first, count, instanceCount);
}

void glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	glBindFramebuffer_ptr(target, framebuffer);
}

GLenum glCheckFramebufferStatus(GLenum target)
{
	return glCheckFramebufferStatus_ptr(target);
}

void glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
	glGenFramebuffers_ptr(n, framebuffers);
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	glFramebufferTexture2D_ptr(target, attachment, textarget, texture, level);
}

void glDrawBuffers(GLsizei n, const GLenum* bufs)
{
	glDrawBuffers_ptr(n, bufs);
}

void glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
	glDeleteFramebuffers_ptr(n, framebuffers);
}

void glBlendFunci(GLuint buf, GLenum src, GLenum dst)
{
	glBlendFunci_ptr(buf, src, dst);
}

void glBlendEquation(GLenum mode)
{
	glBlendEquation_ptr(mode);
}

void glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat* value)
{
	glClearBufferfv_ptr(buffer, drawbuffer, value);
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar* const* strings, const GLint* lengths)
{
	glShaderSource_ptr(shader, count, strings, lengths);
}

void glCompileShader(GLuint shader)
{
	glCompileShader_ptr(shader);
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
	glGetShaderiv_ptr(shader, pname, params);
}

void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
	glGetShaderInfoLog_ptr(shader, bufSize, length, infoLog);
}

void glAttachShader(GLuint program, GLuint shader)
{
	glAttachShader_ptr(program, shader);
}

void glLinkProgram(GLuint program)
{
	glLinkProgram_ptr(program);
}

void glValidateProgram(GLuint program)
{
	glValidateProgram_ptr(program);
}

void glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
	glGetProgramiv_ptr(program, pname, params);
}

void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
	glGetProgramInfoLog_ptr(program, bufSize, length, infoLog);
}

void glGenBuffers(GLsizei n, GLuint* buffers)
{
	glGenBuffers_ptr(n, buffers);
}

void glGenVertexArrays(GLsizei n, GLuint* arrays)
{
	glGenVertexArrays_ptr(n, arrays);
}

GLint glGetAttribLocation(GLuint program, const GLchar* name)
{
	return glGetAttribLocation_ptr(program, name);
}

void glBindVertexArray(GLuint array)
{
	glBindVertexArray_ptr(array);
}

void glEnableVertexAttribArray(GLuint index)
{
	glEnableVertexAttribArray_ptr(index);
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	glVertexAttribPointer_ptr(index, size, type, normalized, stride, pointer);
}

void glBindBuffer(GLenum target, GLuint buffer)
{
	glBindBuffer_ptr(target, buffer);
}

void glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
	glBindBufferBase_ptr(target, index, buffer);
}

void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
	glBufferData_ptr(target, size, data, usage);
}

void glGetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer)
{
	glGetVertexAttribPointerv_ptr(index, pname, pointer);
}

void glUseProgram(GLuint program)
{
	glUseProgram_ptr(program);
}

void glDeleteVertexArrays(GLsizei n, const GLuint* arrays)
{
	glDeleteVertexArrays_ptr(n, arrays);
}

void glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
	glDeleteBuffers_ptr(n, buffers);
}

void glDeleteProgram(GLuint program)
{
	glDeleteProgram_ptr(program);
}

void glDetachShader(GLuint program, GLuint shader)
{
	glDetachShader_ptr(program, shader);
}

void glDeleteShader(GLuint shader)
{
	glDeleteShader_ptr(shader);
}

void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount)
{
	glDrawElementsInstanced_ptr(mode, count, type, indices, instancecount);
}

void glGenerateMipmap(GLenum target)
{
	glGenerateMipmap_ptr(target);
}

void glDebugMessageCallback(GLDEBUGPROC callback, const void* userParam)
{
	glDebugMessageCallback_ptr(callback, userParam);
}
