// DrawImage.cpp: 定义应用程序的入口点。
//

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Shader.h"
#include <memory>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <filesystem>

using namespace std;

std::unique_ptr<Shader> shader;

GLuint VAO, VBO, EBO;
GLuint texture;

//* (样例一：木板) 编写各顶点位置  + 读取纹理 */
GLfloat vertices_1[] =
{
	//position				// color				// texture coords(纹理坐标)
	0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		// 右上顶点		编号0
	0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		// 右下顶点		编号1
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// 左下顶点		编号2
	-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f		// 左上顶点		编号3
};

//GLfloat vertices_1[] =
//{
//	//position				// color				// texture coords(纹理坐标)
//	1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 0.0f,		2.0f, 2.0f,		// 右上顶点		编号0
//	1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 0.0f,		2.0f, 0.0f,		// 右下顶点		编号1
//	-1.0f, -1.0f, 0.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// 左下顶点		编号2
//	-1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 0.0f,		0.0f, 2.0f		// 左上顶点		编号3
//};


/* 四个顶点的连接信息给出来 */
GLuint indices_1[] =
{
	0, 1, 3,		// 序号为 0、1、3 的顶点组合成一个三角形
	1, 2, 3			// 序号为 1、2、3 的顶点组合成一个三角形
};

void Init(char* path)
{
	glfwInit();
	glewInit();

	std::filesystem::path p(path);  // 获取程序的路径
	p = p.parent_path();			// 获取程序所在的目录

	string v = p.string().c_str();
	v += "\\shader_v.txt";

	string f = p.string().c_str();
	f += "\\shader_f.txt";

	shader = std::make_unique<Shader>(v.c_str(), f.c_str());

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW);

	/* 设置链接顶点属性 */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);	// 通道 0 打开
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);	// 通道 1 打开
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);	// 通道 2 打开

	/* 生成纹理 */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	
	p /= "test.jpg";				// 添加文件名
	if (!std::filesystem::exists(p)) {
		std::cout << "test.jpg is not exist" << std::endl;
		return;
	}
	
	unsigned char* data = stbi_load(p.string().c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	shader->Use();	// don't forget to activate/use the shader before setting uniforms!
	int textureLocation = glGetUniformLocation(shader->Program, "ourTexture");	// 找到着色器中 uniform 属性的名为"ourTexture"的纹理的索引
	glUniform1i(textureLocation, 0);		// 告诉 OpenGL 的着色器采样器属于哪个纹理单元	
}

void Timer(int value) {
	glutPostRedisplay(); // 标记当前窗口需要被重绘
	glutTimerFunc(1000 / 60, Timer, 0); // 重新设置定时器
}

void RenderScene(void)
{
	glClearColor(0.1f, 0.8f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 800, 600);

	/* 渲染 + 清除颜色缓冲 */
	glClearColor(0.5f, 0.8f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/* 生成纹理 */
	glActiveTexture(GL_TEXTURE0);			// 激活 0 号纹理单元。最多 16 个通道
	glBindTexture(GL_TEXTURE_2D, texture);	// 绑定这个纹理到当前激活的纹理目标	

	/*  绘制图形 */
	shader->Use();

	glBindVertexArray(VAO);									// 绑定 VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);				// 绑定 EBO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// 画两个三角形 从第0个顶点开始 一共画6次
	glBindVertexArray(0);									// 解绑定 VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);				// 解绑定 EBO

	glFlush();
}

void onWindowClose()
{
	if (shader)
		shader.reset(nullptr);

	glDeleteVertexArrays(1, &VAO);			// 释放资源	
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("绘制图像");
	glutPositionWindow(0, 0);
	glutReshapeWindow(800, 600);
	Init(argv[0]);
	glutTimerFunc(0, Timer, 0); // 设置定时器
	glutDisplayFunc(RenderScene);
	glutCloseFunc(onWindowClose);
	glutMainLoop();
	return 0;
}
