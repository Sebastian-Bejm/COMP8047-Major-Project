#include "Renderer.h"

Renderer* Renderer::renderer = nullptr;

GLFWwindow* window;

Renderer* Renderer::GetInstance() {
	if (renderer == nullptr) {
		renderer = new Renderer();
	}
	return renderer;
}

int Renderer::Init(glm::vec4 backgroundColour, int windowWidth, int windowHeight) {
	this->backgroundColour = backgroundColour;

	// Init GLFW
	glfwInit();

	// Tell GLFW what version we are using (OpenGL 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Get our monitor information
	/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* monitorInfo = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, monitorInfo->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, monitorInfo->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, monitorInfo->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, monitorInfo->refreshRate);*/

	// Setup our window for OpenGL
	window = glfwCreateWindow(windowWidth, windowHeight, "COMP8047 Major Project", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	glViewport(0, 0, windowWidth, windowHeight);

	// Enable blending for 2D text
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Enable the depth buffer for 3D objects
	glEnable(GL_DEPTH_TEST);

	// Prepare the buffers needed to render Vertex of objects
	PrepareGLBuffers();
	// Prepare the textures needed for the scene to be rendered
	LoadTextures();
	// Load the freetype library font
	LoadFreetype();

	return 0;
}

// https://stackoverflow.com/questions/40652905/render-one-vao-containing-two-vbos
// Prepares the OpenGL VAO, VBO, and EBO for later usage
void Renderer::PrepareGLBuffers() {
	// Get shape vertices and indices
	// All shapes used will be cubes unless this changes next month
	ShapeDetails shapeDetails;
	Shape shape = shapeDetails.GetShape(ShapeType::CUBE);

	this->vertices = shape.vertices;
	this->indices = shape.indices;

	vao.InitVAO();
	vao.Bind();
	// Generates Vertex Buffer object and links it to vertices
	VBO vbo = VBO(vertices);
	// Generates Element Buffer object and links it to indices
	EBO ebo = EBO(indices);

	// Links the attributes to the shader based on the layout
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // coordinates
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // color
	vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // texture

	// Unbind to prevent modification
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

}

void Renderer::LoadTextures() {
	// Order image according to enum in GameObject
	std::vector<std::string> images = { "crate.jpg", "brick.png" };

	for (std::string image : images) {
		GLenum format = NULL;
		std::string ext = GetTextureFileExtension(image);

		if (ext == "png") {
			format = GL_RGBA;
		}
		else if (ext == "jpg") {
			format = GL_RGB;
		}
		// Create a new texture and store it for later use
		Texture texture(image.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, format, GL_UNSIGNED_BYTE);
		textures.push_back(texture);
	}

}

std::string Renderer::GetTextureFileExtension(const std::string& textureFile) {
	size_t i = textureFile.rfind('.', textureFile.length());
	if (i != std::string::npos) {
		return (textureFile.substr(i + 1, textureFile.length() - i));
	}
	return "";
}

// Load the Freetype library and the font used
void Renderer::LoadFreetype() {
	// init the library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	// load font file as a texture
	FT_Face face;
	if (FT_New_Face(ft, "arial.ttf", 0, &face))
	{
		std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load all the characters from the font file
	for (unsigned char c = 0; c < 128; c++) {
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
			continue;
		}

		// generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		int height = face->glyph->bitmap.rows;
		int width = face->glyph->bitmap.width;
		unsigned char* &data = face->glyph->bitmap.buffer;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			width,
			height,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			data
		);

		// set the texture parameters to the currently bound texture font
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		FT_Int left = face->glyph->bitmap_left;
		FT_Int top = face->glyph->bitmap_top;
		unsigned int advance = (unsigned int)face->glyph->advance.x;

		Character character = {
			texture,
			glm::ivec2(width, height),
			glm::ivec2(left, top),
			advance
		};

		characters.insert(std::pair<char, Character>(c, character));
	}

	// clean up freetype
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

}

void Renderer::RenderText() {

}

int Renderer::Update(ObjectTracker* tracker) {
	// This fixes the fast movement over time on the machine
	float deltaTime = Time::GetInstance().DeltaTime();

	// Specify the color of the background (silver)
	glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);

	// Clean the back buffer and assign the new color to it, and depth buffer for correct 3D rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.ProcessInput(window, deltaTime);
	//camera.SetPerspectiveMatrix(45.0f, 0.1f, 100.0f); // 0.1f, 100.0f
	camera.SetOrthoMatrix(-8.0f, 11.0f, -11.0f, 8.0f, 0.1f, 100.0f);

	// Draw the game objects here with a reference to the camera
	std::vector<GameObject> objects = tracker->GetAllObjects();
	for (int i = 0; i < objects.size(); i++) {

		objects[i].GetShader().Activate();
		vao.Bind();

		int textureID = objects[i].GetTextureID();
		textures[textureID].TexUnit(objects[i].GetShader(), "tex0", 0);
		textures[textureID].Bind();

		objects[i].Draw(camera);

		// Draw the actual Mesh
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	glfwSwapBuffers(window);

	return 0;
}

int Renderer::Teardown() {
	// Delete the VAO
	vao.Delete();
	
	// Delete the textures
	for (Texture tex : textures) {
		tex.Delete();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void Renderer::SetCamera(Camera& camera) {
	this->camera = camera;
}

GLFWwindow* Renderer::SetupGLFW() {
	// Setup GLFW
	// Get the main monitor for the screen size
	/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* monitorInfo = glfwGetVideoMode(monitor);
	GLFWmonitor* fullScreenMonitor;

	glfwWindowHint(GLFW_RED_BITS, monitorInfo->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, monitorInfo->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, monitorInfo->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, monitorInfo->refreshRate);

	// Get the width and height of our monitor
	// We want to make it full screen
	windowWidth = monitorInfo->width;
	windowHeight = monitorInfo->height;
	fullScreenMonitor = monitor;

	// Create window with reference to the monitor in full size
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "COMP8047 Major Project", fullScreenMonitor, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create window" << std::endl;
		return NULL;
	}

	glfwMakeContextCurrent(window);

	delete monitor;
	delete monitorInfo;*/

	return window;
}
