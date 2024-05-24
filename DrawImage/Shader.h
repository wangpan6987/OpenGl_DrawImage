#pragma once	// 为了避免同一个头文件被包含（include）多次
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;
#include <GL/glew.h>


// 我们自己的着色器
class Shader
{
private:
	GLuint vertex, fragment;	// 顶点着色器 和 片元着色器 
public:
	GLuint Program;				// 着色器程序的ID

	// Constructor(着色器构造函数)
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		// 文件读取系列的变量定义
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;

		// 异常机制处理：保证ifstream对象可以抛出异常：
		vShaderFile.exceptions(ifstream::badbit);
		fShaderFile.exceptions(ifstream::badbit);

		try
		{
			// 打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;

			// 读取文件的缓冲内容到数据流中
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// 关闭文件处理器
			vShaderFile.close();
			fShaderFile.close();

			// 转换数据流到string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch (ifstream::failure e) {	// 发生异常时输出
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
		}

		/* 将 string 类型的字符串转化为 char数组 类型 */
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		/* 顶点着色器 */
		vertex = glCreateShader(GL_VERTEX_SHADER);				// 创建顶点着色器对象
		glShaderSource(vertex, 1, &vShaderCode, NULL);			// 将顶点着色器的内容传进来
		glCompileShader(vertex);								// 编译顶点着色器
		GLint flag;												// 用于判断编译是否成功
		GLchar infoLog[512];
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &flag); // 获取编译状态
		if (!flag)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}

		/* 片元着色器 */
		fragment = glCreateShader(GL_FRAGMENT_SHADER);			// 创建片元着色器对象
		glShaderSource(fragment, 1, &fShaderCode, NULL);		// 将片元着色器的内容传进来
		glCompileShader(fragment);								// 编译顶点着色器
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &flag);		// 获取编译状态
		if (!flag)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		}

		/* 着色器程序 */
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		if (!flag)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}
		// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// Deconstructor(析构函数)
	~Shader()
	{
		glDetachShader(this->Program, vertex);
		glDetachShader(this->Program, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteProgram(this->Program);
	}

	void Use()
	{
		glUseProgram(this->Program);
	}
};
