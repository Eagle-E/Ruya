#ifndef TEST_APPLICATION_H
#define TEST_APPLICATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <string>

#include "Application.h"
#include "Window.h"
#include "Shader.h"
#include "Object.h"
#include "Square.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "Camera.h"
#include "Cube.h"
#include "Timer.h"
#include "Icosahedron.h"
#include "Scene.h"
#include "LightSource.h"
#include "Icosphere.hpp"


using std::vector;		using std::endl;
using std::cout;		using std::cerr;
using std::string;
using glm::vec3;		using glm::vec2;		using glm::dvec2;
using ruya::Square;		using ruya::Shader;
using ruya::Mesh;		using ruya::Renderer;
using ruya::Texture;	using ruya::Camera;
using ruya::Cube;		using ruya::Timer;
using ruya::Icosahedron; using ruya::Scene;
using ruya::LightSource; using ruya::Icosphere;


namespace ruya
{


class KeyframeEditor : public Application
{
private: // VARIABLES
	Camera mCameraPov; // camera for what you see when editing
	Camera mCameraScene; // scene camera, what you see when rendering
	Timer mFrameTimer;
	Window& mWindow;
	dvec2 mOldMousePos;
	bool mAllowShadingModeChange;
	Renderer* mRenderer;
	ImGuiIO* mIo;

public: // FUNCTIONS
	/*** CONSTRUCT ***/
	KeyframeEditor(Window& window) : mWindow(window), mOldMousePos(-1.0, -1.0), mAllowShadingModeChange(true), mIo(nullptr)
	{
		// init imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		mIo = &ImGui::GetIO();
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window.get_GLFW_window(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
	}

	/*** DESTRUCT ***/
	~KeyframeEditor()
	{
		glfwTerminate(); // clean up all reasources allocated by glfw.
			
		// cleanup imgui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}



	/*** MAINLOOP ***/
	void run()
	{
		// init renderer and shaders
		Shader shaderPhongObjects("source/shaders/phong/object.vert", "source/shaders/phong/object.frag");
		Shader shaderPhongLights("source/shaders/phong/object.vert", "source/shaders/phong/light_source.frag");
		Shader shaderFlat("source/shaders/flat/flat_vert.vert", "source/shaders/flat/flat_geom.geom", "source/shaders/flat/flat_frag.frag");
		Renderer renderer(&shaderPhongObjects, &shaderPhongLights, &mWindow, &mCameraPov);
		renderer.set_flat_shader(&shaderFlat);
		mRenderer = &renderer;

		// init scene
		Scene scene;
		vector< shared_ptr<Texture>> textures;
		vector<Object*> objects;

		// textures
		{
			textures.push_back(std::make_shared<Texture>("resources/Wood049_1K-PNG/Wood049_1K_Color.png"));
			textures.push_back(std::make_shared<Texture>("resources/Leather026_1K-PNG/Leather026_1K_Color.png"));
			textures.push_back(std::make_shared<Texture>("resources/Marble023_1K-PNG/Marble023_1K_Color.png"));
			textures.push_back(std::make_shared<Texture>("resources/Metal032_1K-PNG/Metal032_1K_Color.png"));
			textures.push_back(std::make_shared<Texture>("resources/Fabric004_1K-PNG/Fabric004_1K_Color.png"));
		}

		// sample objects
		{
			int radius = 2; // radius of grid, so grid will have 2r+1 cols and rows
			float d = 7.5f;
			for (float i = -radius; i <= radius; i++)
			{
				for (float j = -radius; j <= radius; j++)
				{
					Object* newObjptr = new Icosphere(i + radius);
					float g = (i + radius) / (2 * radius) * 0.8 + 0.1; // map i and j from [-r, r] to [0.1, 0.8]
					float b = (j + radius) / (2 * radius) * 0.8 + 0.1;
					//float r = (k + radius) / (2 * radius) * 0.8 + 0.1;
					float r = (b + g) / 2.0f;
					newObjptr->set_color(vec3(r, g, b));
					newObjptr->set_position(vec3(d * i, d * j, -5.0f));
					newObjptr->set_scale(3.0f);
					objects.push_back(newObjptr);
					scene.add_object(newObjptr);
				}
			}

		}

		// load camera model
		mCameraScene.model().set_mesh(load_mesh("resources/models/camera.obj"));
		mCameraScene.model().set_color(0, 0, 0);

		// add spheres in a line
		Icosphere ico(1);
		Cube* newCubeptr = new Cube();
		Cube* floor = new Cube();
		Icosahedron* newIco = new Icosahedron();

		newCubeptr->set_position(3.0f, -1.0f, -2.0f);
		newIco->set_position(-3.0f, -1.0f, -2.0f);

		floor->set_scale(100.0f);
		floor->set_position(0, -floor->scale().y / 2 - 10.0f, 0);
		floor->set_color(0.65f, 0.85f, 0.95f);
		floor->set_material(Materials::chrome);
		//newCubeptr->set_scale(vec3(3.0f, 3.0f, 3.0f));

		//ico.set_color(vec3(1.0f, 0.5f, 0.31f));
		LightSource* light = new LightSource(vec3(1.0f, 1.0f, 1.0f));
		light->model().set_mesh(ico.mesh()); // the icosahedron obj and the lightsource will share the same mesh
		light->model().set_position(0.0f, 5.0f, 3.0f);
		light->model().set_color(vec3(1.0f, 1.0f, 1.0f));
		//light->model().set_color(vec3(1.0f, 1.0f, 1.0f));
		light->set_ambient(vec3(0.2f, 0.2f, 0.2f));
		light->set_diffuse(vec3(0.7f, 0.7f, 0.7f));
		light->set_specular(vec3(1.0f, 1.0f, 1.0f));

		scene.add_object(newCubeptr);
		scene.add_object(newIco);
		scene.add_light(light);
		//scene.add_object(floor);


		glm::vec4 bgColor(0.9f, 0.9f, 0.9f, 1.0f); // background color
		ruya::Timer timerOutput;
		timerOutput.start();

		while (!mWindow.should_close())
		{
			mFrameTimer.start();
			// change window color
			glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// feed inputs to imgui, start new frame
			//ImGui_ImplOpenGL3_NewFrame();
			//ImGui_ImplGlfw_NewFrame();
			//ImGui::NewFrame();


			// some transforming
			float xs = 0.45f; // rotation speeds
			float ys = 0.90f;
			float zs = 0.15f;

			float degrees = glm::degrees((float)glfwGetTime());
			for (Object* obj : objects)
				obj->set_rotation(vec3(xs * degrees, ys * degrees, zs * degrees));

			newCubeptr->set_rotation(vec3(xs * degrees, ys * degrees, zs * degrees));
			newIco->set_rotation(vec3(xs * degrees, ys * degrees, zs * degrees));
			mat4 rot(1.0);
			rot = glm::rotate(rot, glm::radians(zs * degrees), vec3(1.0f, 0.0f, 0.0f));
			rot = glm::rotate(rot, glm::radians(xs * degrees), vec3(0.0f, 1.0f, 0.0f));
			rot = glm::rotate(rot, glm::radians(ys * degrees), vec3(0.0f, 0.0f, 1.0f));
			vec4 pos = rot * vec4(50, 0, 0, 1);
			//light->model().set_position(vec3(pos.x, pos.y, pos.z) / pos.w);
			light->model().set_position(cos(glfwGetTime()) * 7.5f, 5.0f, 3.0f);


			// RENDER!!!
			//renderer.render_scene(scene);
			renderer.render_object(mCameraScene.model(), *light);

			// IMGUI
			// feed inputs to imgui, start new frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Editor"); // main gui widget
			{


				ImGui::Button("Hello!");
				
				
			}
			ImGui::End();
			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			// update frame => swaps buffers = starts showing newly rendered buffer
			// + checks for input events and calls handlers
			mWindow.update();

			// calc and print FPS
			{
				mFrameTimer.stop();
				double frameTime = mFrameTimer.elapsed_time_s();
				double fps = 1 / frameTime;
				//double fps = frameTime;

				if (timerOutput.elapsed_time_s() > 1.0)
				{
					std::cout << fps << " fps"
						<< "\tElapsed time: " << timerOutput.time_since_creation_s() << "s"
						<< "\tmouse pos: (" << mOldMousePos.x << "," << mOldMousePos.y << ")\n";
					timerOutput.start();
				}
			}

			poll_events();
		}
	}

	void poll_events()
	{
		GLFWwindow* glfwWindow = mWindow.get_GLFW_window();

		// move camera forward/backward/left/right perpendicular with the xz plane
		// move camera up/down along y-axis
		float moveSpeed = 6.0f; // units per second
		float dt = mFrameTimer.elapsed_time_s();

		if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			vec3 direction = mCameraPov.cam_front();
			vec2 moveDirection = glm::normalize(glm::vec2(direction.x, direction.z));
			vec3 pos = mCameraPov.position();
			pos.x += moveDirection.x * moveSpeed * dt;
			pos.z += moveDirection.y * moveSpeed * dt;
			mCameraPov.set_position(pos);
		}

		if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			vec3 direction = mCameraPov.cam_front();
			vec2 moveDirection = glm::normalize(glm::vec2(direction.x, direction.z));
			vec3 pos = mCameraPov.position();
			pos.x -= moveDirection.x * moveSpeed * dt;
			pos.z -= moveDirection.y * moveSpeed * dt;
			mCameraPov.set_position(pos);
		}

		if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			vec3 direction = mCameraPov.cam_front();
			vec2 moveDirection = glm::normalize(glm::vec2(direction.x, direction.z));
			moveDirection = vec2(moveDirection.y, -moveDirection.x); // turn clockwise 90deg from forward direction
			vec3 pos = mCameraPov.position();
			pos.x += moveDirection.x * moveSpeed * dt;
			pos.z += moveDirection.y * moveSpeed * dt;
			mCameraPov.set_position(pos);
		}

		if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			vec3 direction = mCameraPov.cam_front();
			vec2 moveDirection = glm::normalize(glm::vec2(direction.x, direction.z));
			moveDirection = vec2(-moveDirection.y, moveDirection.x); // turn clockwise 90deg from forward direction
			vec3 pos = mCameraPov.position();
			pos.x += moveDirection.x * moveSpeed * dt;
			pos.z += moveDirection.y * moveSpeed * dt;
			mCameraPov.set_position(pos);
		}

		if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			vec3 pos = mCameraPov.position();
			pos.y += moveSpeed * mFrameTimer.elapsed_time_s();
			mCameraPov.set_position(pos);
		}

		if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			vec3 pos = mCameraPov.position();
			pos.y -= moveSpeed * mFrameTimer.elapsed_time_s();
			mCameraPov.set_position(pos);
		}

		if (glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_PRESS && mAllowShadingModeChange)
		{
			if (mRenderer != nullptr)
			{
				if (mRenderer->shading_mode() == Renderer::ShadingMode::FLAT)
					mRenderer->set_shading_mode(Renderer::ShadingMode::SMOOTH);
				else
					mRenderer->set_shading_mode(Renderer::ShadingMode::FLAT);
				mAllowShadingModeChange = false;
			}
		}
		if (glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_RELEASE)
		{
			mAllowShadingModeChange = true;
		}


		// MOUSE MOVEMENT
		update_camera_look_direction();
	}

	/*
	* Look around when mouse is clicked and dragged around (while left mouse button pressed)
	*/
	void update_camera_look_direction()
	{
		if (mIo->WantCaptureMouse) // event was already consumed by ImGui widget
			return;

		int state = glfwGetMouseButton(mWindow.get_GLFW_window(), GLFW_MOUSE_BUTTON_LEFT);
		if (state == GLFW_PRESS)
		{
			dvec2 pos;
			glfwGetCursorPos(mWindow.get_GLFW_window(), &(pos.x), &(pos.y));

			if (mOldMousePos == dvec2(-1, -1))
			{
				mOldMousePos = pos;
			}
			else
			{
				// turn camera
				dvec2 deltaPos = pos - mOldMousePos;
				double turnSpeed = 0.001 * -1; // -1 for inverting look direction
				mCameraPov.update_angle(deltaPos.x * turnSpeed, deltaPos.y * turnSpeed);
				mOldMousePos = pos;
			}
		}
		else if (state == GLFW_RELEASE)
		{
			mOldMousePos = dvec2(-1, -1);
		}
	}


private:
	vector<string> split_str(string str, string delim)
	{
		vector<string> res;
		if (str.size() == 0)
			return res;

		int i = 0;
		int loc = str.find(delim, i);

		while (loc != string::npos)
		{
			res.push_back(str.substr(i, loc - i));
			if (loc != string::npos)
				i = loc + 1;
			loc = str.find(delim, i);
		}

		// the right part of the delimiter is not added yet
		// also works for the edge case of no delimiters
		res.push_back(str.substr(i, string::npos));

		return res;
	}

	/*
	* Reading model from .obj file and returning it in Mesh format
	* @assumption: each line is strictly conform the obj format
    * @assumption: the items in each line is separated by 3 spaces
	*
	*/
	shared_ptr<Mesh> load_mesh(const char* filePath)
	{
		std::ifstream objFile(filePath);
		shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		if (objFile.is_open())
		{
			string line;
			unsigned int lineNum = 1;

			int c = 0;
			while (std::getline(objFile, line))
			{
				vector<string> items = split_str(line, " ");
				if (items.size() == 0)
					continue;
				else if (items.size() != 4)
				{
					objFile.close();
					cerr << "[ERROR] Invalid line in obj file: expected 4 parts, instead got " << items.size() << endl;
					cerr << "\t\tLine number: " << lineNum << endl;
					cerr << "\t\tLine (" << line.size() << "): \"" << line << "\"" << endl;
					cerr << "\t\tFile path: \"" << filePath << "\"" << endl;
					return shared_ptr<Mesh>();
				}

				// we assume the items in each line is separated by 3 spaces
				string type = items[0];
				vec3 vertex{
					std::stof(items[1]),
					std::stof(items[2]),
					std::stof(items[3])
				};

				if (type == "v")
					mesh->vertices.push_back(vertex);
				else if (type == "f")
					mesh->faces.push_back(vertex);

				lineNum++;
			}
		}
		else
		{
			cerr << "Could not load mesh from file \"" << filePath << "\"" << endl;
		}

		objFile.close();
		return mesh;
	}






};  // END CLASS
}  // END NAMESPACE

#endif