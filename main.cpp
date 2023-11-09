
#include <iostream>
#include <cassert>
#include <array>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

const int Width = 800;
const int Height = 600;

int main()
{
	//Inicializar a biblioteca GLFW
	assert(glfwInit() == GLFW_TRUE);

	//Criar uma janela
	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Blue Marble", nullptr, nullptr);
	assert(Window);

	glfwMakeContextCurrent(Window);

	//Inicializar a biblioteca GLEW
	assert(glewInit() == GLEW_OK);

	//Verificar a versão do OpenGL que está sendo usado
	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);
	std::cout << "OpenGL Version: " << GLMajorVersion << "." << GLMinorVersion << std::endl;

	//Usar o glGetString() para obter informações do driver que está sendo usado
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//Definir um triângulo em coordenadas normalizadas
	std::array<glm::vec3, 3> Triangle = {
		glm::vec3{ -1.0f, -1.0f, 0.0f },
		glm::vec3{  1.0f, -1.0f, 0.0f },
		glm::vec3{  0.0f,  1.0f, 0.0f }
	};

	//Copiar os vértices do triângulo para a memória da GPU
	GLuint VertexBuffer;

	//Pedir pro OpenGL gerar o identificador do VertexBuffer
	glGenBuffers(1, &VertexBuffer);

	//Ativar o VertexBuffer para onde serão copiados os dados do triângulo
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	//Copiar os dados para a memória de vídeo
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle.data(), GL_STATIC_DRAW);

	//Definir a cor de fundo
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	//Entrar no loop de eventos da aplicação
	while (!glfwWindowShouldClose(Window))
	{
		//glClear vai limpar o framebuffer. GL_COLOR_BUFFER_BIT diz para limpar o buffer de cor. Após limpar irá preencher com a cor configurada no glClearColor
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);

		//Diz para o OpenGL que o VertexBuffer vai ser o buffer ativo no momento
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

		//Informa ao OpenGL onde, dentro do VertexBuffer se encontrarão os vértices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		//Diz ao OpenGL para desenhar o triângulo com os dados armazenados no VertexBuffer
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Reverter o estado
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(0);

		//Processar todos os eventos da fila de eventos do GLFW
		//Podem ser eventos como: teclado, mouse, gamepad...
		glfwPollEvents();

		//Enviar o conteúdo do framebuffer da janela para ser desenhado na tela
		glfwSwapBuffers(Window);
	}

	//Desalocar o VertexBuffer
	glDeleteBuffers(1, &VertexBuffer);

	//Encerrar a biblioteca GLFW
	glfwTerminate();

	return 0;
}