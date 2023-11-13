
#include <iostream>
#include <cassert>
#include <array>
#include <fstream>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

const int Width = 800;
const int Height = 600;

std::string ReadFile(const char* FilePath)
{
	std::string FileContents;
	if ( std::ifstream FileStream{ FilePath, std::ios::in })
	{
		//Ler dentro do FileContents o conteúdo do arquivo apontado por FilePath
		FileContents.assign(std::istreambuf_iterator<char>(FileStream), std::istreambuf_iterator<char>());
	}
	return FileContents;
}

GLuint LoadShaders(const char* VertexShaderFile, const char* FragmentShaderFile)
{
	std::string VertexShaderSource = ReadFile(VertexShaderFile);
	std::string FragmentShaderSource = ReadFile(FragmentShaderFile);

	assert(!VertexShaderSource.empty());
	assert(!FragmentShaderSource.empty());

	//Criar identificadores do Vertex e Fragment Shaders
	GLuint VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	std::cout << "Compilando: " << VertexShaderFile << std::endl;
	const char* VertexShaderSourcePtr = VertexShaderSource.c_str();
	glShaderSource(VertexShaderId, 1, &VertexShaderSourcePtr, nullptr);
	glCompileShader(VertexShaderId);

	//Verificar se a compilação do vertex shader deu certo

	std::cout << "Compilando " << FragmentShaderFile << std::endl;
	const char* FragmentShaderSourcePtr = FragmentShaderSource.c_str();
	glShaderSource(FragmentShaderId, 1, &FragmentShaderSourcePtr, nullptr);
	glCompileCommandListNV(FragmentShaderId);

	//Verificar se a compilação do fragment shader deu certo

	std::cout << "Linkando o programa" << std::endl;
	GLuint ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	glLinkProgram(ProgramId);

	//Verificar o programa
	GLint Result = GL_TRUE;
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		std::cout << "Erro ao linkar o programa" << std::endl;

		//Pegar o log para avaliar problema

		assert(false);
	}

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(VertexShaderId);
	glDeleteShader(FragmentShaderId);

	return ProgramId;
}

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

	GLuint ProgramId = LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");

	//Definir um triângulo em coordenadas normalizadas
	std::array<glm::vec3, 3> Triangle = {
		glm::vec3{ -1.0f, -1.0f, 0.0f },
		glm::vec3{  1.0f, -1.0f, 0.0f },
		glm::vec3{  0.0f,  1.0f, 0.0f }
	};

	//Model
	glm::mat4 ModelMatrix = glm::identity<glm::mat4>();

	//View Matrix
	glm::vec3 Eye{ 0, 0, 5 };
	glm::vec3 Center{ 0, 0, 0 };
	glm::vec3 Up{ 0, 1, 0 };
	glm::mat4 ViewMatrix = glm::lookAt(Eye, Center, Up);

	//Projection Matrix
	constexpr float FoV = glm::radians(45.0f);
	const float AspectRatio = Width / Height;
	const float Near = 0.001f;
	const float Far = 1000.f;
	glm::mat4 ProjectionMatrix = glm::perspective(FoV, AspectRatio, Near, Far);

	//ModelViewProjection
	glm::mat4 ModelViewProjection = ProjectionMatrix * ViewMatrix * ModelMatrix;

	//Aplicar a ModelViewProjection nos vértices do triângulo
	for (glm::vec3& Vertex : Triangle)
	{
		glm::vec4 ProjectedVertex = ModelViewProjection * glm::vec4{ Vertex, 1.0f };
		ProjectedVertex /= ProjectedVertex.w;
		Vertex = ProjectedVertex;
	}

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