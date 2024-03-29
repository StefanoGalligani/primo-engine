#pragma once

#include <sgModel.h>
#include <GL/glew.h>
#include <chrono>
#include <fstream>
#include <iostream>

namespace sg {

    GLuint CompileShader(const char* source, GLuint shaderType) {
        GLuint shaderID;
        std::ifstream shaderStream(source, std::ios::in);
        if (!shaderStream.is_open()) {
            std::cout << "ERROR: Cannot open file." << std::endl;
            return false;
        }

        std::string shaderSourceCode((std::istreambuf_iterator<char>(shaderStream)), std::istreambuf_iterator<char>());
        shaderStream.close();

        shaderID = glCreateShader(shaderType);
        char const *shaderData = shaderSourceCode.data();
        glShaderSource(shaderID, 1, &shaderData, nullptr);
        glCompileShader(shaderID);

        GLint result = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

        int infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            std::vector<char> compilerMessage(infoLogLength);
            glGetShaderInfoLog(shaderID, infoLogLength, nullptr, compilerMessage.data());
            if (!result) {
                std::cout << "ERROR: Cannot compile shader " << source << " ; " << compilerMessage.data() << std::endl;
                glDeleteShader(shaderID);
                return false;
            }
        }

        if (result) {
            GLint stype;
            glGetShaderiv(shaderID, GL_SHADER_TYPE, &stype);
            if (stype != (GLint)shaderType) {
                std::cout << "ERROR: Incorrect shader type." << std::endl;
                return false;
            }
        }
        return shaderID;
    }

    GLuint CreateProgram(const char* vsSource, const char* fsSource) {
        GLuint vsID = CompileShader(vsSource, GL_VERTEX_SHADER);
        GLuint fsID = CompileShader(fsSource, GL_FRAGMENT_SHADER);

        GLuint programID = glCreateProgram();
        glAttachShader(programID, vsID);
        glAttachShader(programID, fsID);

        glLinkProgram(programID);
        GLint result = GL_FALSE;
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        int infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            std::vector<char> compilerMessage(infoLogLength);
            glGetProgramInfoLog(programID, infoLogLength, nullptr, compilerMessage.data());
            std::cout << "ERROR: " << compilerMessage.data() << std::endl;
        }

        glUseProgram(programID);

        glDeleteShader(vsID);
        glDeleteShader(fsID);

        return programID;
    }

    GLuint CreateProgram(const char* vsSource, const char* fsSource, const char* gsSource) {
        GLuint vsID = CompileShader(vsSource, GL_VERTEX_SHADER);
        GLuint fsID = CompileShader(fsSource, GL_FRAGMENT_SHADER);
        GLuint gsID = CompileShader(gsSource, GL_GEOMETRY_SHADER);

        GLuint programID = glCreateProgram();
        glAttachShader(programID, vsID);
        glAttachShader(programID, fsID);
        glAttachShader(programID, gsID);

        glLinkProgram(programID);
        GLint result = GL_FALSE;
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        int infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            std::vector<char> compilerMessage(infoLogLength);
            glGetProgramInfoLog(programID, infoLogLength, nullptr, compilerMessage.data());
            std::cout << "ERROR: " << compilerMessage.data() << std::endl;
        }

        glUseProgram(programID);

        glDeleteShader(vsID);
        glDeleteShader(fsID);
        glDeleteShader(gsID);

        return programID;
    }

    GLuint CreateProgram(const char* vsSource, const char* fsSource, const char* tcsSource, const char* tesSource) {
        GLuint vsID = CompileShader(vsSource, GL_VERTEX_SHADER);
        GLuint fsID = CompileShader(fsSource, GL_FRAGMENT_SHADER);
        GLuint tcsID = CompileShader(tcsSource, GL_TESS_CONTROL_SHADER);
        GLuint tesID = CompileShader(tesSource, GL_TESS_EVALUATION_SHADER);

        GLuint programID = glCreateProgram();
        glAttachShader(programID, vsID);
        glAttachShader(programID, fsID);
        glAttachShader(programID, tcsID);
        glAttachShader(programID, tesID);

        glLinkProgram(programID);
        GLint result = GL_FALSE;
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        int infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            std::vector<char> compilerMessage(infoLogLength);
            glGetProgramInfoLog(programID, infoLogLength, nullptr, compilerMessage.data());
            std::cout << "ERROR: " << compilerMessage.data() << std::endl;
        }

        glUseProgram(programID);

        glDeleteShader(vsID);
        glDeleteShader(fsID);
        glDeleteShader(tcsID);
        glDeleteShader(tesID);

        return programID;
    }

    GLuint CreateProgram(const char* vsSource, const char* fsSource, const char* tcsSource, const char* tesSource, const char* gsSource) {
        GLuint vsID = CompileShader(vsSource, GL_VERTEX_SHADER);
        GLuint fsID = CompileShader(fsSource, GL_FRAGMENT_SHADER);
        GLuint tcsID = CompileShader(tcsSource, GL_TESS_CONTROL_SHADER);
        GLuint tesID = CompileShader(tesSource, GL_TESS_EVALUATION_SHADER);
        GLuint gsID = CompileShader(gsSource, GL_GEOMETRY_SHADER);

        GLuint programID = glCreateProgram();
        glAttachShader(programID, vsID);
        glAttachShader(programID, fsID);
        glAttachShader(programID, tcsID);
        glAttachShader(programID, tesID);
        glAttachShader(programID, gsID);

        glLinkProgram(programID);
        GLint result = GL_FALSE;
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        int infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 1) {
            std::vector<char> compilerMessage(infoLogLength);
            glGetProgramInfoLog(programID, infoLogLength, nullptr, compilerMessage.data());
            std::cout << "ERROR: " << compilerMessage.data() << std::endl;
        }

        glUseProgram(programID);

        glDeleteShader(vsID);
        glDeleteShader(fsID);
        glDeleteShader(tcsID);
        glDeleteShader(tesID);
        glDeleteShader(gsID);

        return programID;
    }

    void UpdateLightPos(glm::vec3 lightPos, glm::mat4 mv, GLuint program) {
        glm::vec3 lightPosCy = glm::vec3(mv * glm::vec4(lightPos.x, lightPos.y, lightPos.z, 0));
        if (program > 0) {
            glUseProgram(program);
            glUniform3f(glGetUniformLocation(program, "lightPos"), lightPosCy.x, lightPosCy.y, lightPosCy.z);
        }
    }

    double getCurrentTimeMillis() {
        return std::chrono::system_clock::now().time_since_epoch().count() / 1000;
    }

    void SetMatrix(glm::mat4 matrix, GLuint program, char const* name) {
        GLint location = glGetUniformLocation(program, name);
        glUseProgram(program);
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
    }

    void SetMatrix(glm::mat3 matrix, GLuint program, char const* name) {
        GLint location = glGetUniformLocation(program, name);
        glUseProgram(program);
        glUniformMatrix3fv(location, 1, false, glm::value_ptr(matrix));
    }
}