//This Code belongs to Kerem Günay
//Built using C++20 & OpenGL4.1 & SDL2.30.8
//SiegeCraft created with SiegeEngine

//Third Party Libraries
#include <glad/glad.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//Custom Header Files
#include "camera.hpp"
#include "animation.hpp"

//C++ Standard Template Libraries (STLs)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//Structs
struct App {
	int mScreenWidth = 1366;
	int mScreenHeight = 768;
	SDL_Window* mAppWindow = nullptr;
	SDL_GLContext mOpenGLContext = nullptr;
	bool mQuit = false;
	GLuint mGraphicsPipelineShaderProgram = 0;
	camera mCamera;
	AnimationSystem animationSystem;
	void updateDeltaTime();
	static void calculateFrameUV(int frames, int index, int framesPerRow, int sheetWidth, int sheetHeight,
							     float& uMin, float& vMin, float& uMax, float& vMax);
	Uint32 lastTicks = 0;
	float deltaTime = 0.0f;
	int currentAnimationIndex = 0;
};
struct Transform {
	float x, y, z;
};
struct Mesh3D {
	GLuint mVertexArrayObject = 0;
	GLuint mVertexBufferObject = 0;
	GLuint mIndexBufferObject = 0;
	GLuint mPipeline = 0;
	GLuint mTexture = 0;
	Transform mTransform;
	float m_uOffset = 0.0f;
	float m_uOffset2 = 0.0f;
	float m_uOffset3 = 0.0f;
};

//	Global Declerations		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
App gApp; 
Mesh3D gMainChar;
Mesh3D gGround;
Mesh3D gMesh1;
Mesh3D gMesh2;
GLint maxTextureSize;
void App::calculateFrameUV(int frames, int index, int framesPerRow, int sheetWidth, int sheetHeight,
						   float& uMin, float& vMin, float& uMax, float& vMax) {
	int framesPerColumn = frames / framesPerRow;
	int frameWidth = sheetWidth / ((frames + framesPerColumn - 1) / framesPerColumn); // Total columns
	int frameHeight = sheetHeight / framesPerColumn;

	int column = index / framesPerColumn;
	int row = index % framesPerColumn;

	int x = column * frameWidth;
	int y = row * frameHeight;

	uMin = static_cast<float>(x) / sheetWidth;
	vMin = static_cast<float>(y) / sheetHeight;
	uMax = static_cast<float>(x + frameWidth) / sheetWidth;
	vMax = static_cast<float>(y + frameHeight) / sheetHeight;
}
void App::updateDeltaTime() {
	Uint32 currentTicks = SDL_GetTicks(); // How many ticks since SDL started
	deltaTime = (currentTicks - lastTicks) / 1000.0f;  //milisec to sec conversion
	lastTicks = currentTicks;
}
//Mesh Data is described below.
void MeshCreateMainChar(Mesh3D* mesh) {
	//Add the spritesheet to the RAM
	stbi_set_flip_vertically_on_load(true);
	int widthImg, heightImg, numColChanel;
	unsigned char* bytes = stbi_load("sprites/anim1/spritesheet.png", &widthImg, &heightImg, &numColChanel, 0);
	//Calculate the correct UVs
	float uMin = 0.0, uMax = 0.0, vMin = 0.0, vMax = 0.0;

	//Below data is set on CPU
	const std::vector<GLfloat> vertexData{
		//A Quad(Quadrilateral)(4vertices,4sides,4angles)(winding order - right hand rule)
		//1st Triangle
		//v0
		-0.2f, -0.2f, 0.0f, //v11 position - named v0
		1.0f, 0.0f, 0.0f, //v11 color
		uMin, vMin, //Texture Coords
		//v1
		0.2f, -0.2f, 0.0f, //v12 position - named v1
		0.0f, 1.0f, 0.0f, //v12 color
		uMax, vMin, //Texture Coords
		//v2
		-0.2, 0.2f, 0.0f, //v13 position - named v2
		0.0f, 0.0f, 1.0f, //v13 color
		uMax, vMax, //Texture Coords
		//2nd triangle (also have v1 &v2)
		//v3
		0.2f, 0.2f, 0.0f, //v22 position - named v3
		1.0f, 0.0f, 0.0f, //v22 color
		uMin, vMax //Texture Coords
	};
	//Below data is set up on GPU
	gApp.animationSystem.addAnimation(Animation(0, 21, 384, 1188, 2, 1.5f / 22.0f)); //Running index0
	gApp.animationSystem.addAnimation(Animation(0, 21, 384, 1188, 2, 1.0f / 6.0f)); //Sprinting index1
	gApp.currentAnimationIndex = 0;
	Animation& currentAnimation = gApp.animationSystem.getAnimation(gApp.currentAnimationIndex);
	
	glGenVertexArrays(1, &mesh->mVertexArrayObject);
	glBindVertexArray(mesh->mVertexArrayObject);

	glGenBuffers(1, &mesh->mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER,
		vertexData.size() * sizeof(GLfloat),
		vertexData.data(),
		GL_STATIC_DRAW);

	const std::vector<GLuint> indexBufferData{ 2,0,1,3,2,1 };
	glGenBuffers(1, &mesh->mIndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexBufferData.size() * sizeof(GLuint),
		indexBufferData.data(),
		GL_STATIC_DRAW);
	//position information
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,   //index of AttribArray
		3, //number of components
		GL_FLOAT, //data type
		false, //is the data normalized
		sizeof(GLfloat) * 8, //stride
		(void*)0); //offset
	//color information
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	//texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 6));

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//texture setup
	glGenTextures(1, &gMainChar.mTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gMainChar.mTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texture loading
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void MeshCreateStatic(Mesh3D* mesh) {
	//Below data is set on CPU
	const std::vector<GLfloat> vertexData{
		//A Quad(Quadrilateral)(4vertices,4sides,4angles)(winding order - right hand rule)
		//1st Triangle
		//v0
		-0.2f, -0.2f, 0.0f, //v11 position - named v0
		1.0f, 0.0f, 0.0f, //v11 color
		0.0f, 0.0f, //Texture Coords
		//v1
		0.2f, -0.2f, 0.0f, //v12 position - named v1
		0.0f, 1.0f, 0.0f, //v12 color
		1.0f, 0.0f, //Texture Coords
		//v2
		-0.2, 0.2f, 0.0f, //v13 position - named v2
		0.0f, 0.0f, 1.0f, //v13 color
		0.0f, 1.0f, //Texture Coords
		//2nd triangle (also have v1 &v2)
		//v3
		0.2f, 0.2f, 0.0f, //v22 position - named v3
		1.0f, 0.0f, 0.0f, //v22 color
		1.0f, 1.0f //Texture Coords
	};
	//Below data is set up on GPU
	glGenVertexArrays(1, &mesh->mVertexArrayObject);
	glBindVertexArray(mesh->mVertexArrayObject);

	glGenBuffers(1, &mesh->mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER,
		vertexData.size() * sizeof(GLfloat),
		vertexData.data(),
		GL_STATIC_DRAW);

	const std::vector<GLuint> indexBufferData{ 2,0,1,3,2,1 };
	glGenBuffers(1, &mesh->mIndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexBufferData.size() * sizeof(GLuint),
		indexBufferData.data(),
		GL_STATIC_DRAW);
	//position information
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,   //index of AttribArray
		3, //number of components
		GL_FLOAT, //data type
		false, //is the data normalized
		sizeof(GLfloat) * 8, //stride
		(void*)0); //offset
	//color information
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
	//texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 6));

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//texture setup
	int widthImg, heightImg, numColChanel;
	unsigned char* bytes = stbi_load("textures/grass/grass2.png", &widthImg, &heightImg, &numColChanel, 0);
	glGenTextures(1, &gGround.mTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gGround.mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texture loading
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void MeshCreateGround(Mesh3D* mesh) {
	//Below data is set on CPU
	const std::vector<GLfloat> vertexData{
		//A Quad(Quadrilateral)(4vertices,4sides,4angles)(winding order - right hand rule)
		//1st Triangle
		//v0
		-5.0f, -5.0f, 0.0f, //v11 position - named v0
		1.0f, 0.0f, 0.0f, //v11 color
		0.0f, 0.0f, //Texture Coords
		//v1
		5.0f, -5.0f, 0.0f, //v12 position - named v1
		0.0f, 1.0f, 0.0f, //v12 color
		5.0f, 0.0f,  //Texture Coords
		//v2
		-5.0f, 5.0f, 0.0f, //v13 position - named v2
		0.0f, 0.0f, 1.0f, //v13 color
		0.0f, 5.0f, //Texture Coords
		//2nd triangle (also have v1 &v2)
		//v3
		5.0f, 5.0f, 0.0f, //v22 position - named v3
		1.0f, 0.0f, 0.0f, //v22 color
		5.0f, 5.0f //Texture Coords
	};
	//Below data is set up on GPU
	glGenVertexArrays(1, &mesh->mVertexArrayObject);
	glBindVertexArray(mesh->mVertexArrayObject);

	glGenBuffers(1, &mesh->mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER,
		vertexData.size() * sizeof(GLfloat),
		vertexData.data(),
		GL_STATIC_DRAW);

	const std::vector<GLuint> indexBufferData{ 2,0,1,3,2,1 };
	glGenBuffers(1, &mesh->mIndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexBufferData.size() * sizeof(GLuint),
		indexBufferData.data(),
		GL_STATIC_DRAW);
	//position information
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,   //index of AttribArray
		3, //number of components
		GL_FLOAT, //data type
		false, //is the data normalized
		sizeof(GLfloat) * 8, //stride
		(void*)0); //offset
	//color information
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 3));
	//texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 6));

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//texture setup
	int widthImg, heightImg, numColChanel;
	unsigned char* bytes = stbi_load("textures/grass/grass2.png", &widthImg, &heightImg, &numColChanel, 0);
	glGenTextures(1, &gGround.mTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gGround.mTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texture loading
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}
//Erases a mesh from GPU's memory
void MeshDelete(Mesh3D* mesh) {
	glDeleteBuffers(1, &mesh->mVertexBufferObject);
	glDeleteVertexArrays(1, &mesh->mVertexArrayObject);
}
//Mesh Set Pipeline Function
void MeshSetPipeline(Mesh3D* mesh, GLuint pipeline) {
	//graphics pipeline setup
	mesh->mPipeline = pipeline;
}
//A function that returns a uniform's location
int FindUniformLocation(GLuint pipeline, const GLchar* name) {
	GLint location = glGetUniformLocation(pipeline, name);
	if (location <  0) {
		std::cerr << "Could not find " << name << std::endl;
	}
	return location;
}
//DrawMesh Function
//We choose which pipeline to use draw our mesh per each mesh.
void MeshDrawStatic(Mesh3D* mesh) {
	if (mesh == nullptr) {
		return;
	}
	glUseProgram(mesh->mPipeline);
	//This process translates our object to the world space using transform matrix
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(mesh->mTransform.x,
																	mesh->mTransform.y,
																	mesh->mTransform.z));
	GLint u_ModelMatrixLocation = FindUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_ModelMatrix");
	glUniformMatrix4fv(u_ModelMatrixLocation, 1, false, &translate[0][0]);
	
	//Camera matrix
	glm::mat4 view = gApp.mCamera.GetViewMatrix();
	//Retrieve the location of our view matrix
	GLint u_ViewMatrixLocation = FindUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_ViewMatrix");
	glUniformMatrix4fv(u_ViewMatrixLocation, 1, false, &view[0][0]);
	
	//This process creates perspective and recalibrates our perception of the world using perspective matrix
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
		(float)gApp.mScreenWidth / (float)gApp.mScreenHeight,
		0.1f, 10.0f);
	//Retrieve the location of our perspective matrix
	GLint u_PerspectiveLocation = FindUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_Perspective");
	glUniformMatrix4fv(u_PerspectiveLocation, 1, false, &perspective[0][0]);
	
	//Retrieve the location of texture coords
	GLuint u_texCoordsLocation = FindUniformLocation(gApp.mGraphicsPipelineShaderProgram, "tex0");
	glUniform1i(u_texCoordsLocation, 0);

	glUseProgram(mesh->mPipeline);
	glBindVertexArray(mesh->mVertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, gGround.mTexture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
}
void MeshDrawMainChar(Mesh3D* mesh) {
	if (mesh == nullptr) {
		return;
	}
	gApp.updateDeltaTime();
	gApp.animationSystem.update(gApp.deltaTime);
	Animation& currentAnimation = gApp.animationSystem.getAnimation(gApp.currentAnimationIndex);
	int frameIndex = currentAnimation.currentFrame;
	
	glUseProgram(mesh->mPipeline);
	//This process translates our object to the world space using transform matrix
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(gMesh1.m_uOffset2,
																	gMesh1.m_uOffset,
																	0.0f));
	GLint u_ModelMatrixLocation = FindUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_ModelMatrix");
	glUniformMatrix4fv(u_ModelMatrixLocation, 1, false, &translate[0][0]);

	//Camera matrix
	glm::mat4 view = gApp.mCamera.GetViewMatrix();
	//Retrieve the location of our view matrix
	GLint u_ViewMatrixLocation = FindUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_ViewMatrix");
	glUniformMatrix4fv(u_ViewMatrixLocation, 1, false, &view[0][0]);

	//This process creates perspective and recalibrates our perception of the world using perspective matrix
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
		(float)gApp.mScreenWidth / (float)gApp.mScreenHeight,
		0.1f, 10.0f);
	//Retrieve the location of our perspective matrix
	GLint u_PerspectiveLocation = FindUniformLocation(gApp.mGraphicsPipelineShaderProgram, "u_Perspective");

	//Retrieve the location of texture coords
	GLuint u_texCoordsLocation = FindUniformLocation(gApp.mGraphicsPipelineShaderProgram, "tex0");
	glUniform1i(u_texCoordsLocation, 0);
	glUniformMatrix4fv(u_PerspectiveLocation, 1, false, &perspective[0][0]);

	float uMin = 0.0, uMax = 0.0, vMin = 0.0, vMax = 0.0;
	App::calculateFrameUV(22, frameIndex, 2, 384, 1188, uMin, vMin, uMax, vMax);
	
	//We must update the vertices per each frame now due to dynamic texture
	const std::vector<GLfloat> vertexData{
		//A Quad(Quadrilateral)(4vertices,4sides,4angles)(winding order - right hand rule)
		//1st Triangle
		//v0
		-0.2f, -0.2f, 0.0f, //v11 position - named v0
		1.0f, 0.0f, 0.0f, //v11 color
		uMin, vMin, //Texture Coords
		//v1
		0.2f, -0.2f, 0.0f, //v12 position - named v1
		0.0f, 1.0f, 0.0f, //v12 color
		uMax, vMin, //Texture Coords
		//v2
		-0.2, 0.2f, 0.0f, //v13 position - named v2
		0.0f, 0.0f, 1.0f, //v13 color
		uMin, vMax, //Texture Coords
		//2nd triangle (also have v1 &v2)
		//v3
		0.2f, 0.2f, 0.0f, //v22 position - named v3
		1.0f, 0.0f, 0.0f, //v22 color
		uMax, vMax //Texture Coords
	};
	glBindBuffer(GL_ARRAY_BUFFER, gMainChar.mVertexBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(GLfloat), vertexData.data());

	glUseProgram(mesh->mPipeline);
	glBindVertexArray(mesh->mVertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, gMainChar.mTexture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
}
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//vvvvvvvvvvvvvv Error Handling Routines vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
static void GLClearAllErrors() {
	while (glGetError() != GL_NO_ERROR) {
	}
}
static bool GLCheckErrorStatus(const char* function, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "OpenGL Error:" << error 
			<< "\t Line:" << line 
			<< "\t Function:" << function << std::endl;
		return true;
	}
	return false;
}
#define GLCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x, __LINE__);
//^^^^^^^^^^^^^^ Error Handling Routines ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//vvvvvvvvvvvvvvvvvvvvvvv	 SHADERS	vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
std::string LoadShaderAsString(const std::string& filename) {
	std::string result = "";
	std::string line = "";
	std::ifstream myFile(filename.c_str()); //filename.c_str() function reads file as a string
	if (myFile.is_open()) {
		while (std::getline(myFile, line)) {
			result += line + '\n';
		}
		myFile.close();
	}
	return result;
}
GLuint CompileShader(GLuint type, const std::string& source) {
	GLuint shaderObject;
	if (type == GL_VERTEX_SHADER) {
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == GL_FRAGMENT_SHADER) {
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}
	const char* src = source.c_str();
	glShaderSource(shaderObject, 1, &src, nullptr);
	glCompileShader(shaderObject);
	return shaderObject;
}
GLuint CreateShaderProgram(const std::string& vertexshadersource,
	const std::string& fragmentshadersource)
{
	GLuint programObject = glCreateProgram();
	GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
	GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);
	glAttachShader(programObject, myVertexShader);
	glAttachShader(programObject, myFragmentShader);
	glLinkProgram(programObject);
	//Validate our program
	glValidateProgram(programObject);
	return programObject;
}
void CreateGraphicsPipeline() {
	std::string vertexShaderSource = LoadShaderAsString("./shaders/vss.glsl");
	std::string fragmentShaderSource = LoadShaderAsString("./shaders/fss.glsl");
	gApp.mGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^	SHADERS    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//Initialization and Window Creation
void GetOpenGLVersionInfo() {
	std::cout << "Vendor:" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer:" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version:" << glGetString(GL_VERSION) << std::endl;
	std::cout << "Shading Language:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
void Initialize_Program() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cout << "ERROR! SDL could not initialize video & timer subsystems" << std::endl;
		exit(1);
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	gApp.mAppWindow = SDL_CreateWindow("SiegeCraft",
		50, 30, gApp.mScreenWidth, gApp.mScreenHeight,
		SDL_WINDOW_OPENGL);
	if (gApp.mAppWindow == nullptr) {
		std::cout << "ERROR! Window cannot be created" << std::endl;
		exit(1);
	}
	gApp.mOpenGLContext = SDL_GL_CreateContext(gApp.mAppWindow);
	if (gApp.mOpenGLContext == nullptr) {
		std::cout << "ERROR! Context does not apply" << std::endl;
		exit(1);
	}
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cout << "ERROR! Glad was not initialized" << std::endl;
		exit(1);
	}
	GetOpenGLVersionInfo();

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	printf("Max texture size: %d x %d\n", maxTextureSize, maxTextureSize);
}
//Input Function
void Input() {
	float speed = 0.2f;
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			std::cout << "\nGoodbye" << std::endl;
			gApp.mQuit = true;
			break;
		case SDL_MOUSEWHEEL:
			gMesh1.m_uOffset3 += e.wheel.y * 0.15f;
			gApp.mCamera.Zoom(gMesh1.m_uOffset3);
			std::cout << "g_uOffset3 is: " << gMesh1.m_uOffset3 << std::endl;
		}
	}
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if(state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
		gMesh1.m_uOffset += 0.0002f;
		gApp.mCamera.Ydir(gMesh1.m_uOffset);
		std::cout << "uOffset is:" << gMesh1.m_uOffset << std::endl;
	}
	if(state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
		gMesh1.m_uOffset -= 0.0002f;
		gApp.mCamera.Ydir(gMesh1.m_uOffset);
		std::cout << "uOffset is:" << gMesh1.m_uOffset << std::endl;
	}
	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
		gMesh1.m_uOffset2 -= 0.0002f;
		gApp.mCamera.Xdir(gMesh1.m_uOffset2);
		std::cout << "uOffset2 is:" << gMesh1.m_uOffset2 << std::endl;
	}
	if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
		gMesh1.m_uOffset2 += 0.0002f;
		gApp.mCamera.Xdir(gMesh1.m_uOffset2);
		std::cout << "uOffset2 is:" << gMesh1.m_uOffset2 << std::endl;
	}
	if (state[SDL_SCANCODE_KP_PLUS]) {
		gMesh1.m_uOffset3 += 0.0015f;
		gApp.mCamera.Zoom(gMesh1.m_uOffset3);
		std::cout << "uOffset3 is:" << gMesh1.m_uOffset3 << std::endl;
	}
	if (state[SDL_SCANCODE_KP_MINUS]) {
		gMesh1.m_uOffset3 -= 0.0015f;
		gApp.mCamera.Zoom(gMesh1.m_uOffset3);
		std::cout << "uOffset3 is:" << gMesh1.m_uOffset3 << std::endl;
	}
}
//Mainloop Function
void MainLoop() {
	while (!gApp.mQuit) {
		//SDL Ticks are used for time
		//Later on we will use gameRunning boolean to better control the ticks
		Input();
		//Let's initialize clear color
		glViewport(0, 0, gApp.mScreenWidth, gApp.mScreenHeight);
		//Let's set the background color
		glClearColor(0.569f, 0.412f, 0.243f, 1.0f);
		//Clear color buffer and depth buffer
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		//Use our shader
		MeshDrawStatic(&gGround);	
		MeshDrawStatic(&gMesh1);
		MeshDrawStatic(&gMesh2);
		MeshDrawMainChar(&gMainChar);

		SDL_GL_SwapWindow(gApp.mAppWindow); //updates screen
	}
}
//CleanUp Function
void CleanUp() {
	SDL_DestroyWindow(gApp.mAppWindow);
	gApp.mAppWindow = nullptr;
	MeshDelete(&gMesh1);
	glDeleteTextures(1, &gGround.mTexture);
	glDeleteTextures(1, &gMainChar.mTexture);
	glDeleteProgram(gApp.mGraphicsPipelineShaderProgram);
	SDL_Quit();
}

//vvvvvvvvvvvvvvvvv		main function	vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int main(int argc, char* args[]) {

	//Setup the graphics program
	Initialize_Program();

	//Setup the Geometry
	MeshCreateGround(&gGround);
	MeshCreateMainChar(&gMainChar);
	MeshCreateStatic(&gMesh1);
	gMesh1.mTransform.x = 2.0f;
	gMesh1.mTransform.y = 4.0f;
	gMesh1.mTransform.z = 0.0f;
	MeshCreateStatic(&gMesh2);
	gMesh2.mTransform.x = 1.0f;
	gMesh2.mTransform.y = -1.0f;
	gMesh2.mTransform.z = 0.0f;

	//Create our graphics pipeline, vertexshader and fragmentshader
	CreateGraphicsPipeline();

	//For each of our meshes, set them to a pipeline
	MeshSetPipeline(&gGround, gApp.mGraphicsPipelineShaderProgram);
	MeshSetPipeline(&gMesh1, gApp.mGraphicsPipelineShaderProgram);
	MeshSetPipeline(&gMesh2, gApp.mGraphicsPipelineShaderProgram);
	MeshSetPipeline(&gMainChar, gApp.mGraphicsPipelineShaderProgram);

	//Call the main application loop
	MainLoop();

	//Call the cleanup function when our program terminates
	CleanUp();

	return 0;
}