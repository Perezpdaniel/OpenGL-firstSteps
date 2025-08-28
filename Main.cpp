#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


//Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n"
"}\0";

int main() {
	//Initialize GLFW
	glfwInit();

	//Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core Profile = only modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//Vertices coordinates triangle
	/*GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,	 //Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,//Upper corner
	};*/
	//Vertices coordinates square
	GLfloat vertices[] = {
		//first square
		0.08f, 0.51f, 0.0f, //top right
		0.08f, 0.35f, 0.0f, // bottom right
		-0.08f, 0.35f, 0.0f, //bottom left
		-0.08f, 0.51f, 0.0f, //top left
		//second square
		0.58f, -0.42f, 0.0f, //top right
		0.58f, -0.58f, 0.0f, //bottom right
		0.42f, -0.58f, 0.0f, // bottom left
		0.42f, -0.42f, 0.0f, //top left
		//third square
		-0.58f, -0.42f, 0.0f, 
		-0.58f, -0.58f, 0.0f, 
		-0.42f, -0.58f, 0.0f, 
		-0.42f, -0.42f, 0.0f

	};
	GLint indices[] = {
		//first square
		0, 1, 3, //top 1 triangle
		1, 2, 3, //top 2 triangle
		//second square
		4, 5, 7,
		5, 6, 7,
		//third square
		8, 9, 11,
		9, 10, 11,

	};
	//create a GLFW object of 800 pixels 
	GLFWwindow* window = glfwCreateWindow(800, 800, "OPENGL", NULL, NULL);
	// error check if the window fails to create
	if (window == NULL) {
		std::cout << "failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Introduce the window into the current context
	glfwMakeContextCurrent(window);
	//load GLAD so it configures OpenGL
	gladLoadGL();


	//Specify the viewport of openGL in the window
	//in this case the viewport goes from x=0
	glViewport(0,0,800,800);

	//Create vertex shader Object and get reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Attach Vertex Shader source to the vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compile the vertex shader into machine code
	glCompileShader(vertexShader);

	//Create fragment shader object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Attach Fragment Shader source to the fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//compile the fragment shader into machine code
	glCompileShader(fragmentShader);

	//Create shader program object and get its reference
	GLuint shaderProgram = glCreateProgram();

	//attach the vertex and fragment shaders to the shader program 
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//wrap-up/Link all the shaders together into the sahader program
	glLinkProgram(shaderProgram);

	//delete the now useless vertex and fragment shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Create reference containers for the Vartex Array Object
	//and the Vertex Buffer Object
	GLuint VAO, VBO;

	GLuint EBO;
	glGenBuffers(1, &EBO);

	//Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	//Bind the VBO specifying its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	//Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we dont accidentally modify the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//color of the background
	glClearColor(0.200f, 0.298f, 0.298f, 1.0f);
	//Clear the back buffer and assing the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	//swap the back biffer with the front buffer
	glfwSwapBuffers(window);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//Main while loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Tell OpenGl which Shader Program we want to use
		glUseProgram(shaderProgram);
		//Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		//Draw the triangle
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		//Take care of all GLFW event
		glfwPollEvents();
	}

	//Delete all the objects we created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;	
}
