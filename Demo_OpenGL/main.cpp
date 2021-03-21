#include <GL/gl3w.h> // must be included before any other OpenGL related headers
#include <GLFW/glfw3.h>
#include <iostream>

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------
static GLuint CompileShader(GLuint type, const std::string& source);
static GLuint CreateShader (const std::string& vertexShader, const std::string& fragmentShader);

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
GLFWwindow* g_window;
GLuint g_vertexArrayObject;
GLuint g_vertexBuffer;
GLuint g_indexBuffer;
GLuint g_shader;

//-----------------------------------------------------------------------------
// Shaders
//-----------------------------------------------------------------------------
std::string g_vertexShader =
	"#version 330 core\n"
	"\n"
	"layout(location = 0) in vec4 position;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_Position = position;\n"
	"}\n";

std::string g_fragmentShader =
	"#version 330 core\n"
	"\n"
	"layout(location = 0) out vec4 color;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	color = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n";

//-----------------------------------------------------------------------------
// Vertex Data and Index Data
//-----------------------------------------------------------------------------
float g_positions[6] = {
	-0.5f, -0.5f,
	 0.0f,  0.5f,
	 0.5f, -0.5f
};

unsigned short g_indices[3] = { 0, 1, 2 };

int main()
{
	//-----------------------------------------------------------------------------
	// Initialize GLFW Windowing Library
	//-----------------------------------------------------------------------------
	if (!glfwInit())
		return -1;

	//-----------------------------------------------------------------------------
	// Create Window
	//-----------------------------------------------------------------------------
	g_window = glfwCreateWindow(640, 480, "Demo - OpenGL", nullptr, nullptr);

	if (!g_window)
	{
		glfwTerminate();
		return -1;
	}

	//-----------------------------------------------------------------------------
	// Make the window's context current
	//-----------------------------------------------------------------------------
	glfwMakeContextCurrent(g_window);

	//-----------------------------------------------------------------------------
	// Initialize gl3w OpenGL Loading Library
	//-----------------------------------------------------------------------------
	if (gl3wInit() != 0)
		return -1;
	std::cout << glGetString(GL_VERSION) << std::endl;

	//-----------------------------------------------------------------------------
	// Create and Bind Vertex Array Object
	//-----------------------------------------------------------------------------
	glGenVertexArrays(1, &g_vertexArrayObject);
	glBindVertexArray(g_vertexArrayObject);

	//-----------------------------------------------------------------------------
	// Create and Bind Vertex Buffer
	//-----------------------------------------------------------------------------
	glGenBuffers(1, &g_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), g_positions, GL_STATIC_DRAW);

	//-----------------------------------------------------------------------------
	// Create and Bind Index Buffer
	//-----------------------------------------------------------------------------
	glGenBuffers(1, &g_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned short), g_indices, GL_STATIC_DRAW);

	//-----------------------------------------------------------------------------
	// Configure Vertex Attributes
	//-----------------------------------------------------------------------------
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//-----------------------------------------------------------------------------
	// Create Shader
	//-----------------------------------------------------------------------------
	g_shader = CreateShader(g_vertexShader, g_fragmentShader);
	glUseProgram(g_shader);

	
	//-----------------------------------------------------------------------------
	// Unbind Vertex Array Object, Vertex Buffer Object, and Index Buffer Object
	// 	   - Make sure you unbind VAO before VBO and IBO
	//-----------------------------------------------------------------------------
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	//-----------------------------------------------------------------------------
	// Main loop
	//-----------------------------------------------------------------------------
	while (!glfwWindowShouldClose(g_window))
	{
		
		// clear render target
		glClear(GL_COLOR_BUFFER_BIT);

		// bind the Vertex Array Object to be drawn
		glBindVertexArray(g_vertexArrayObject);

		// draw
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

		// swap front and back buffers
		glfwSwapBuffers(g_window);

		// poll for and process events
		glfwPollEvents();
	}

	//-----------------------------------------------------------------------------
	// Clean up
	//-----------------------------------------------------------------------------
	glDeleteProgram(g_shader);
	glfwTerminate();
}

static GLuint CompileShader(GLuint type, const std::string& source)
{
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}