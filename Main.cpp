// Execution command on MacOS
// /usr/bin/clang++ -std=c++17 -I/Users/sudhanshu/Documents/C++_Projects/open-gl-main/OpenGLProject/Libraries/include -g /Users/sudhanshu/Documents/C++_Projects/open-gl-main/OpenGLProject/**.cpp /Users/sudhanshu/Documents/C++_Projects/open-gl-main/OpenGLProject/glad.cc -o /Users/sudhanshu/Documents/C++_Projects/open-gl-main/OpenGLProject/Main -L/Users/sudhanshu/Documents/C++_Projects/open-gl-main/OpenGLProject/Libraries/lib -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit && ./Main

// GLM Reference: https://openframeworks.cc//documentation/glm/detail_func_geometric/#!show_glm::cross
#include "Primitives/Utils.h"
#include "Model.h"
#include "Primitives/primitives.h"
#include "Line.h"
#include "Physics/physics.h"

#include "Box.h"
#include "BVH.h"

#include "Primitives/Mathematics.h"
#include "Boid.h"
#include <cmath>

#define WIDTH 480
#define HEIGHT 480

#define SHOWSCALE 1.0f

#define ORBITRAD 2.5
const int BOIDCOUNT = 10;

static void glfwError(int id, const char *description)
{
	std::cout << description << std::endl;
}

void CleanUp();
void render();


std::vector<Vertex> plane_vertices =
{ //			COORDINATES				/			NORMALS				/			COLORS					/		TEXTURE COORDINATES
	Vertex{ glm::vec3(-1.0f, 1.0f,  0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec3(1.0f, 1.0f, 0.0f),		glm::vec2(0.0f, 0.0f) },
	Vertex{ glm::vec3(-1.0f, -1.0f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec3(0.0f, 1.0f, 1.0f),		glm::vec2(0.0f, 1.0f) },
	Vertex{ glm::vec3(1.0f, -1.0f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec3(1.0f, 0.0f, 1.0f),		glm::vec2(1.0f, 1.0f) },
	Vertex{ glm::vec3(1.0f, 1.0f,  0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec3(1.0f, 0.5f, 0.4f),		glm::vec2(1.0f, 0.0f) }
};

std::vector<GLuint> plane_indices =
{
	0, 3, 2,	
	0, 2, 1	
};

std::vector<Vertex> triangle_vertices =
{ //			COORDINATES													/			NORMALS				/			COLORS					/		TEXTURE COORDINATES
	Vertex{ glm::vec3(-0.5f,	0.0f,			0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec3(1.0f, 1.0f, 0.0f),		glm::vec2(0.0f, 0.0f) },
	Vertex{ glm::vec3(0.5f,		0.0,			0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec3(0.0f, 1.0f, 1.0f),		glm::vec2(0.0f, 1.0f) },
	Vertex{ glm::vec3(0.0f,		-0.5f,		0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec3(1.0f, 0.0f, 1.0f),		glm::vec2(1.0f, 1.0f) },
};

std::vector<GLuint> triangle_indices =
{	
	0, 2, 1	
};


int main()
{
	glfwSetErrorCallback(&glfwError);
	// Init GLFW
	glfwInit();

	// Tell GLFW which version we are using
	// For Windows/Linux
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	// //glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// For MacOS
	// https://www.glfw.org/faq.html#14---what-platforms-are-supported-by-glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window object
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Open GL Project", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to initialize GLFW windows!\n";
		glfwTerminate();
		return -1;
	}
	std::cout << "GLFW Window created...\n\n";
	// Use created GLFW window
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();
	std::cout << "GLAD LOADED...\n\n";

	srand(time(NULL));

	//glViewport(0, 0, WIDTH, HEIGHT);
	//glfwSetWindowPos(window, 100, 100);

	Shader shaderProgram("default.vert", "default.frag");

	Shader lightShader("default.vert", "lightnormal.frag");

	// Create camera
	Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 5.0f));

	glm::vec4 lightColor = glm::vec4(188 / 255.0f, 206 / 255.0f, 248 / 255.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, -0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);

	lightModel = glm::translate(lightModel, lightPos);

	
	
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	lightShader.Activate();
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(lightShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	// Enables the depth buffer
	glEnable(GL_DEPTH_TEST);

	
	//std::cout << "Loading Model..\n\n";
	//Model model("Models/sphere.gltf", glm::vec3(139.0f / 255.0f, 0.0f / 255.0f, 0));
	Model model("Models/sphere.gltf", glm::vec3(0, 0, 0));
	Model monkeyModel("Models/suzanne.gltf", glm::vec3(152.0f / 255.0f, 168.0f / 255.0f, 248 / 255.0f));
	//std::cout << "Model Loaded\n\n";
	model.meshes[0].scale = glm::vec3(0.02, 0.02, 0.02);
	monkeyModel.meshes[0].scale = glm::vec3(0.95, 0.95, 0.95);
	//monkeyModel.meshes[0].rotation = glm::quat(glm::vec3(45.0f, 45.0f, 0.0f));
	BVH bvh(monkeyModel);
	
	int boxIndex = 0;
	
	std::vector<Boid> boids;
	std::vector<Line> lines;
	for(int i = 0 ; i < BOIDCOUNT; ++i)
	{
		glm::vec3 startPoint = Mathematics::GetRandomPointOnSphere(ORBITRAD);
		glm::vec3 endPoint = model.meshes[0].translation;
		boids.push_back(Boid(startPoint));
		glm::vec3 c(0, 0, 0);
		lines.push_back(Line(startPoint, endPoint, c));
	}
	


	std::vector<Texture> ts;
	
	Mesh plane(plane_vertices, plane_indices, ts);
	//BVH bvh(plane);
	//Mesh triangleMesh(triangle_vertices, triangle_indices, ts);

	// triangleMesh.matrix = glm::mat4(1.0f);
	// triangleMesh.translation = glm::vec3(0.0f, -1.0f, 0.0f);
	// triangleMesh.rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	// triangleMesh.scale = glm::vec3(2.0f, 2.0f, 2.0f);

	//std::vector<glm::vec3> triCoord = monkeyModel.meshes[0].GetTransformedCoordinates(630);


	glm::vec3 iPoint;

	glm::vec3 bStart = glm::vec3(-0.65, -0.49, -0.42);
	glm::vec3 bEnd = glm::vec3(0, 0.49, 0.42);
	glm::vec3 bColor = glm::vec3(0, 1, 0);
	//Box b(bStart, bEnd, bColor);

	glm::vec3 boxnPoint;
	glm::vec3 boxfPoint;
	

	// Only close when user decides to close the window
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(245 / 255.0f, 245 / 255.0f, 245 / 255.0f, 1.0f);
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Clear the depth buffer and the color buffer bit
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for(int i = 0; i < BOIDCOUNT; ++i)
		{
			boids[i].Draw(shaderProgram, camera, model.meshes[0]);
			//newSPos.x = Utils::map(xpos, 0, WIDTH, -2, 2);
			//newSPos.y = Utils::map(ypos, 0, HEIGHT, 2, -2);
			//glm::vec3 newEPos = line.getEndPosition();
			//newEPos.x = newSPos.x;
			//newEPos.y = newSPos.y;
			lines[i].setStartPosition(boids[i].GetPosition());
			lines[i].setEndPosition(model.meshes[0].translation);
			glm::vec3 dir = glm::normalize(lines[i].getEndPosition() - lines[i].getStartPosition());

			if(bvh.findIntersectionPoint(shaderProgram, camera, lines[i].getStartPosition(), dir, iPoint))
			{
				lines[i].setEndPosition(iPoint);
				lines[i].Draw(shaderProgram, camera);
				//std::cout << "\n\nI Point: \n";
				//Utils::showGlmVec3(iPoint);
				model.meshes[0].DrawInstance
				(
					shaderProgram, 
					camera,
					model.meshes[0].matrix,
					iPoint,
					model.meshes[0].rotation,
					model.meshes[0].scale
				);
				//exit(1);
			}
			else
			{
				//std::cout << "\nNo I Point";
			}

		}
		//boid.Update();
		
		camera.Inputs(window);
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
		//model.Draw(shaderProgram, camera);
		
		//plane.Draw(shaderProgram, camera);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		//std::cout << "Cursor position: " << xpos << ", " << ypos << std::endl;

	
		
		//triangleMesh.Draw(shaderProgram, camera);

		

	
		// if(Physics::RayIntersectsTriangle(newSPos, dir, triCoord, iPoint))
		// {
		// 	model.meshes[0].translation = iPoint;
		// 	model.Draw(shaderProgram, camera);
		// }
		// if(b.IntersectsRay(newSPos, dir, 0, 100, boxnPoint, boxfPoint))
		// {
		// 	model.meshes[0].DrawInstance
		// 	(
		// 		shaderProgram, 
		// 		camera,
		// 		model.meshes[0].matrix,
		// 		boxnPoint,
		// 		model.meshes[0].rotation,
		// 		model.meshes[0].scale
		// 	);
		// 	//std::cout << "Near Point I: \n";
		// 	//showGlmVec3(boxnPoint);
		// 	model.meshes[0].DrawInstance
		// 	(
		// 		shaderProgram, 
		// 		camera,
		// 		model.meshes[0].matrix,
		// 		boxfPoint,
		// 		model.meshes[0].rotation,
		// 		model.meshes[0].scale
		// 	);
		// }

		monkeyModel.Draw(lightShader, camera);
		//b.DrawWireFrame(shaderProgram, camera);

		bvh.showBoxes(shaderProgram, camera, 5);
		
	
	

		glfwSwapBuffers(window);
		// Process all the events
		glfwPollEvents(); // If not done window will go to not responding

	}

	// Delete created objects
	
	shaderProgram.Delete();
	lightShader.Delete();

	// Destroy Window
	glfwDestroyWindow(window);
	// End GLFW
	glfwTerminate();
	// std::cin.get();
	return 0;
}


void CleanUp()
{

}

void render()
{

}