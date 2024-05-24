#pragma once	// Ϊ�˱���ͬһ��ͷ�ļ���������include�����
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;
#include <GL/glew.h>


// �����Լ�����ɫ��
class Shader
{
private:
	GLuint vertex, fragment;	// ������ɫ�� �� ƬԪ��ɫ�� 
public:
	GLuint Program;				// ��ɫ�������ID

	// Constructor(��ɫ�����캯��)
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		// �ļ���ȡϵ�еı�������
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;

		// �쳣���ƴ�����֤ifstream��������׳��쳣��
		vShaderFile.exceptions(ifstream::badbit);
		fShaderFile.exceptions(ifstream::badbit);

		try
		{
			// ���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;

			// ��ȡ�ļ��Ļ������ݵ���������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// �ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();

			// ת����������string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch (ifstream::failure e) {	// �����쳣ʱ���
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
		}

		/* �� string ���͵��ַ���ת��Ϊ char���� ���� */
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		/* ������ɫ�� */
		vertex = glCreateShader(GL_VERTEX_SHADER);				// ����������ɫ������
		glShaderSource(vertex, 1, &vShaderCode, NULL);			// ��������ɫ�������ݴ�����
		glCompileShader(vertex);								// ���붥����ɫ��
		GLint flag;												// �����жϱ����Ƿ�ɹ�
		GLchar infoLog[512];
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &flag); // ��ȡ����״̬
		if (!flag)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}

		/* ƬԪ��ɫ�� */
		fragment = glCreateShader(GL_FRAGMENT_SHADER);			// ����ƬԪ��ɫ������
		glShaderSource(fragment, 1, &fShaderCode, NULL);		// ��ƬԪ��ɫ�������ݴ�����
		glCompileShader(fragment);								// ���붥����ɫ��
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &flag);		// ��ȡ����״̬
		if (!flag)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		}

		/* ��ɫ������ */
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		if (!flag)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}
		// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// Deconstructor(��������)
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
