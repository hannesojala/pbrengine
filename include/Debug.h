#pragma once

#include <iostream>
#include <glad/glad.h>

// not 100% mine, from stack overflow i think, lost link a long time ago
void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
    // To shut the compiler up about unused param warnings. 
    // I do not plan to use them but they are required for this callback function.
    (void)length; (void)user_param;

	auto const src_str = [source]() {
		switch (source) {
		case GL_DEBUG_SOURCE_API:               return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY:       return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION:       return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER:             return "OTHER";
		default:                                return "OH NO!";
		}
	}();

	auto const type_str = [type]() {
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:               return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER:              return "MARKER";
		case GL_DEBUG_TYPE_OTHER:               return "OTHER";
		default:                                return "OH NO!";
		}
	}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION:    return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW:             return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM:          return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH:            return "HIGH";
		default:                                return "OH NO!";
		}
	}();

	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}