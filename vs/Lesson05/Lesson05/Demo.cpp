#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);
	BuildRakBuku();
	BuildDepanRakBuku();
	BuildPapan();
	BuildFloor();
	BuildColoredWall();
	BuildKaki1();
	BuildKaki2();
	BuildKaki3();
	BuildKaki4();
	BuildBuku();
	BuildCoverBuku();
	BuildKarpet();
	BuildMotifKarpet();
	BuildWindow();
	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 0.5;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);




}
void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 8.0f;
	posCamZ = 15.0f;
	viewCamX = 0.0f;
	viewCamY = 8.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.01f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}
void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}
void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}
void Demo::Update(double deltaTime) {
	angle += (float) ((deltaTime * 1.5f) / 1500);
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	DrawRakBuku();
	DrawDepanRakBuku();
	
	DrawFloor();
	DrawColoredWall();

	DrawPapan();
	DrawKaki1();
	DrawKaki2();
	DrawKaki3();
	DrawKaki4();

	DrawBuku();
	DrawCoverBuku();

	DrawKarpet();
	DrawMotifKarpet();

	DrawWindow();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildWindow() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("window.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-3.75,  5,  1, 0, 0, // 46 
3.75,  5,  1, 1, 0, // 47
3.75,  -5,  1, 1, 1, // 48
-3.75,  -5, 1, 0, 1, // 49

// right
3.75,  5,  1, 0, 0,  // 4
3.75,  5, -1, 1, 0,  // 5
3.75, -5, -1, 1, 1,  // 6
3.75, -5,  1, 0, 1,  // 7

// back
-3.75,  5,  -1, 0, 0, // 20 
3.75,  5, -1, 1,0, // 24
3.75,  -5,  -1, 1, 1, // 22
-3.75,  -5,  -1, 0, 1, // 23

// left
-3.75,  5,  -1, 0, 0, // 42
-3.75,  5,  1, 1, 0, // 43
-3.75,  -5,  1, 1, 1, // 44
-3.75,  -5, -1, 0, 1, // 45

// upper
-3.75,  5,  -1, 0, 0, // 8 
3.75,  5,  -1, 1, 0, // 9
3.75,  5, 1, 1, 1, // 40
-3.75,  5,  1, 0, 1, // 44

// bottom
-3.75, -5, 1, 0, 0,  // 0
3.75, -5, 1, 1, 0,   // 4
3.75,  -5, -1, 1, 1,   // 2
-3.75,  -5,-1, 0, 1,  // 3

	};

	unsigned int titik[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom
	};

	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(titik), titik, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawWindow()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	
	model = glm::translate(model, glm::vec3(10, 13, -19));

	model = glm::scale(model, glm::vec3(1, 1, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPapan() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("table.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-4, 3, 3, 0, 0,  // 0
		4, 3, 3, 1, 0,   // 4
		4,  2.52, 3, 1, 1,   // 2
		-4,  2.52, 3, 0, 1,  // 3
		
		// right
		4,  3,  3, 0, 0,  // 4
		4,  2.52, 3, 1, 0,  // 5
		4, 2.52, -3, 1, 1,  // 6
		4, 3,  -3, 0, 1,  // 7
		
		// back
		-4,  3,  -3, 0, 0, // 8 
		-4,  2.52,  -3, 1, 0, // 9
		4,  2.52, -3, 1, 1, // 40
		4,  3,  -3, 0, 1, // 44
						  
		// left
		-4,  3,  3, 0, 0, // 42
		-4,  2.52,  3, 1, 0, // 43
		-4,  2.52,  -3, 1, 1, // 44
		-4,  3, -3, 0, 1, // 45
						  
		// upper
		-4,  3,  -3, 1, 1, // 46 
		-4,  3,  3, 1, 0, // 47
		4,  3, -3, 0, 1, // 48
		4,  3,  3, 0, 0, // 49
						 
		// bottom
		-4,  2.52,  -3, 0, 0, // 20 
		4,  2.52, -3, 1,0, // 24
		4,  2.52,  3, 1, 1, // 22
		-4,  2.52,  3, 0, 1, // 23
	};

	unsigned int titik[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(titik), titik, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawPapan()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	
	model = glm::translate(model, glm::vec3(10, 0, -7));
	
	

	model = glm::scale(model, glm::vec3(1, 1, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::BuildKaki1() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("table.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
3.0, 2.5, 3.0, 0, 0,  // 0
4.0, 2.5, 3.0, 1, 0,   // 0.5
4.0,0.051,3.0, 1, 1,   // 2
3.0,0.051,3.0, 0, 1,  // 0.375

// right
4.0,  2.5,  3.0, 0, 0,  // 0.5
4.0,  0.051,3.0, 1, 0,  // 5
4.0,  0.051,2.0, 1, 1,  // 6
4.0,  2.5,  2.0, 0, 1,  // 7

// back
3.0, 2.5, 2.0, 0, 0, // 8 
4.0, 2.5, 2.0, 1, 0, // 9
4.0,0.051,2.0, 1, 1, // 0.50
3.0,0.051,2.0, 1, 1, // 0.50.5

// left
3.0,  2.5,  3.0, 0, 0, // 0.52
3.0,  0.051,3.0, 1, 0, // 0.50.375
3.0,  0.051,2.0, 1, 1, // 0.50.5
3.0,  2.5,  2.0, 0, 1, // 0.55

// upper
3.0,  2.5,  2.0, 0, 0, // 0.56 
4.0,  2.5, 2.0, 1,0, // 0.57
4.0,  2.5,  3.0, 1, 1, // 0.58
3.0,  2.5,  3.0, 0, 1, // 0.59

// bottom
3.0,  0.051,2.0, 0, 0, // 20 
4.0, 0.051,2.0, 1,0, // 20.5
4.0,  0.051,3.0, 1, 1, // 22
3.0,  0.051,3.0, 0, 1, // 20.375

	};

	unsigned int titik[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom
	};

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(titik), titik, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawKaki1()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	model = glm::translate(model, glm::vec3(10, 0, -7));
	
	model = glm::scale(model, glm::vec3(1, 1, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::BuildKaki2() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("table.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-4.0, 2.5, 3.0, 0, 0,  // 0 
-3.0, 2.5, 3.0, 1, 0,   // 0.5
-3.0,0.051,3.0, 1, 1,   // 2
-4.0,0.051,3.0, 0, 1,  // 0.375

// right
-3.0,  2.5,  3.0, 0, 0,  // 0.5
-3.0, 0.051, 3.0, 1, 0,  // 5
-3.0, 0.051, 2.0, 1, 1,  // 6
-3.0,  2.5,  2.0, 0, 1,  // 7

// back
-4.0, 2.5, 2.0, 0, 0, // 8 
-3.0, 2.5, 2.0, 1, 0, // 9
-3.0,0.051,2.0, 1, 1, // 0.50
-4.0,0.051,2.0, 1, 1, // 0.50.5

// left
-4.0,  2.5,  3.0, 0, 0, // 0.52
-4.0,  0.051,3.0, 1, 0, // 0.50.375
-4.0,  0.051,2.0, 1, 1, // 0.50.5
-4.0,  2.5,  2.0, 0, 1, // 0.55

// upper
-4.0,  2.5,  2.0, 0, 0, // 0.56 
-3.0,  2.5, 2.0, 1,0, // 0.57
-3.0,  2.5,  3.0, 1, 1, // 0.58
-4.0,  2.5,  3.0, 0, 1, // 0.59

// bottom
-4.0,  0.051,2.0, 0, 0, // 20 
-3.0, 0.051,2.0, 1,0, // 20.5
-3.0,  0.051,3.0, 1, 1, // 22
-4.0,  0.051,3.0, 0, 1, // 20.375

	};


	unsigned int titik[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom
	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO5);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(titik), titik, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawKaki2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	
	model = glm::translate(model, glm::vec3(10, 0, -7));
	
	
	
	model = glm::scale(model, glm::vec3(1, 1, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::BuildKaki3() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("table.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
3.0, 2.5, -2.0, 0, 0,  // 0
4.0, 2.5, -2.0, 1, 0,   // 0.5
4.0,0.051,-2.0, 1, 1,   // 2
3.0,0.051,-2.0, 0, 1,  // 0.375

// right
4.0,  2.5,  -2.0, 0, 0,  // 0.5
4.0,  0.051,-2.0, 1, 0,  // 5
4.0,  0.051,-3.0, 1, 1,  // 6
4.0,  2.5,  -3.0, 0, 1,  // 7

// back
3.0, 2.5, -3.0, 0, 0, // 8 
4.0, 2.5, -3.0, 1, 0, // 9
4.0,0.051,-3.0, 1, 1, // 0.50
3.0,0.051,-3.0, 1, 1, // 0.50.5

// left
3.0,  2.5,  -2.0, 0, 0, // 0.52
3.0,  0.051,-2.0, 1, 0, // 0.50.375
3.0,  0.051,-3.0, 1, 1, // 0.50.5
3.0,  2.5,  -3.0, 0, 1, // 0.55

// upper
3.0,  2.5,  -3.0, 0, 0, // 0.56 
4.0,  2.5, -3.0, 1,0, // 0.57
4.0,  2.5,  -2.0, 1, 1, // 0.58
3.0,  2.5,  -2.0, 0, 1, // 0.59

// bottom
3.0,  0.051,-3.0, 0, 0, // 20 
4.0, 0.051,-3.0, 1,0, // 20.5
4.0,  0.051,-2.0, 1, 1, // 22
3.0,  0.051,-2.0, 0, 1, // 20.375

	};

	unsigned int titik[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom
	};

	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(titik), titik, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawKaki3()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(10, 0, -7));

	
	model = glm::scale(model, glm::vec3(1, 1, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::BuildKaki4() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("table.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
-4.0, 2.5, -2.0, 0, 0,  // 0
-3.0, 2.5, -2.0, 1, 0,   // 0.5
-3.0,0.051,-2.0, 1, 1,   // 2
-4.0,0.051,-2.0, 0, 1,  // 0.375

// right
-3.0,  2.5,  -2.0, 0, 0,  // 0.5
-3.0,  0.051,-2.0, 1, 0,  // 5
-3.0,  0.051,-3.0, 1, 1,  // 6
-3.0,  2.5,  -3.0, 0, 1,  // 7

// back
-4.0, 2.5, -3.0, 0, 0, // 8 
-3.0, 2.5, -3.0, 1, 0, // 9
-3.0,0.051,-3.0, 1, 1, // 0.50
-4.0,0.051,-3.0, 1, 1, // 0.50.5

// left
-4.0,  2.5,  -2.0, 0, 0, // 0.52
-4.0,  0.051,-2.0, 1, 0, // 0.50.375
-4.0,  0.051,-3.0, 1, 1, // 0.50.5
-4.0,  2.5,  -3.0, 0, 1, // 0.55

// upper
-4.0,  2.5,  -3.0, 0, 0, // 0.56 
-3.0,  2.5, -3.0, 1,0, // 0.57
-3.0,  2.5,  -2.0, 1, 1, // 0.58
-4.0,  2.5,  -2.0, 0, 1, // 0.59

// bottom
-4.0,  0.051,-3.0, 0, 0, // 20 
-3.0, 0.051,-3.0, 1,0, // 20.5
-3.0,  0.051,-2.0, 1, 1, // 22
-4.0,  0.051,-2.0, 0, 1, // 20.375

	};

	unsigned int titik[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom
	};

	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(titik), titik, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void Demo::DrawKaki4()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;

	model = glm::translate(model, glm::vec3(10, 0, -7));
	model = glm::scale(model, glm::vec3(1, 1, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildBuku() {
	glGenTextures(1, &bukuTexture);
	glBindTexture(GL_TEXTURE_2D, bukuTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("samping.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, 3.1, 0.5, 0, 0,  // 0 bawah kiri 
		0.5,  3.1, 0.5, 1, 0,   // 1 bawah kanan
		0.5,  3, 0.5, 1, 1,   // 2 atas kanan
		-0.5, 3, 0.5, 0, 1,  // 3 atas kiri

		// right
		0.5,  3.1,  0.5, 0, 0,  // 4 atas
		0.5,  3.1, -0.5, 1, 0,  // 5
		0.5,  3, -0.5, 1, 1,  // 6 bawah
		0.5,  3,  0.5, 0, 1,  // 7

		// back
		-0.5,  3.1, -0.5, 0, 0, // 8 kiri
		0.5,   3.1, -0.5, 1, 0, // 9
		0.5,   3, -0.5, 1, 1, // 10
		-0.5,  3, -0.5, 0, 1, // 11 kiri

		// left
		-0.5,  3.1, -0.5, 0, 0, // 12
		-0.5,  3.1,  0.5, 1, 0, // 13
		-0.5,  3,  0.5, 1, 1, // 14
		-0.5,  3, -0.5, 0, 1, // 15

		// upper
		0.5, 3.1,  0.5, 0, 0,   // 16
		-0.5, 3.1,  0.5, 1, 0,  // 17
		-0.5, 3.1, -0.5, 1, 1,  // 18
		0.5, 3.11, -0.5, 0, 1,   // 19

		// bottom
		-0.5, 3, -0.5, 0, 0, // 20
		0.5, 3, -0.5, 1, 0,  // 21
		0.5, 3,  0.5, 1, 1,  // 22
		-0.5, 3,  0.5, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};
	glGenVertexArrays(1, &bukuVAO);
	glGenBuffers(1, &bukuVBO);
	glGenBuffers(1, &bukuEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(bukuVAO);

	glBindBuffer(GL_ARRAY_BUFFER, bukuVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bukuEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::BuildCoverBuku() {
	glGenTextures(1, &coverBukuTexture);
	glBindTexture(GL_TEXTURE_2D, coverBukuTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("covers.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
// front
		-0.5, 3.1, 0.5, 0, 0,  // 0 bawah
		0.5,  3.1, 0.5, 1, 0,   // 1
		0.5,  3, 0.5, 1, 1,   // 2 atas
		-0.5, 3, 0.5, 0, 1,  // 3

		// right
		0.5,  3.1,  0.5, 0, 0,  // 4 atas
		0.5,  3.1, -0.5, 1, 0,  // 5
		0.5,  3, -0.5, 1, 1,  // 6 bawah
		0.5,  3,  0.5, 0, 1,  // 7

		// back
		-0.5,  3.11, -0.5, 0, 0, // 8 kiri
		0.5,   3.11, -0.5, 1, 0, // 9
		0.5,   2.99, -0.5, 1, 1, // 10
		-0.5,  2.99, -0.5, 0, 1, // 11 kiri

		// left
		-0.5,  3.1, -0.5, 0, 0, // 12
		-0.5,  3.1,  0.5, 1, 0, // 13
		-0.5,  3,  0.5, 1, 1, // 14
		-0.5,  3, -0.5, 0, 1, // 15

		// upper
		0.5, 3.11,  0.5, 0, 0,   // 16
		-0.5, 3.11,  0.5, 1, 0,  // 17
		-0.5, 3.11, -0.5, 1, 1,  // 18
		0.5, 3.11, -0.5, 0, 1,   // 19

		// bottom
		-0.5, 2.99, -0.5, 0, 0, // 20
		0.5, 2.99, -0.5, 1, 0,  // 21
		0.5, 2.99,  0.5, 1, 1,  // 22
		-0.5, 2.99,  0.5, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};
	glGenVertexArrays(1, &coverBukuVAO);
	glGenBuffers(1, &coverBukuVBO);
	glGenBuffers(1, &coverBukuEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(coverBukuVAO);

	glBindBuffer(GL_ARRAY_BUFFER, coverBukuVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coverBukuEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Demo::DrawBuku()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bukuTexture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
	glBindVertexArray(bukuVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	glm::mat4 model;

	model = glm::translate(model, glm::vec3(7, -4, -5));
	model = glm::scale(model, glm::vec3(2.5, 2.5, 2.5));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawCoverBuku() {
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, coverBukuTexture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
	glBindVertexArray(coverBukuVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	glm::mat4 model;

	model = glm::translate(model, glm::vec3(7,-4,-5));
	model = glm::scale(model, glm::vec3(2.5, 2.5, 2.5));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildKarpet() {
	glGenTextures(1, &karpetTexture);
	glBindTexture(GL_TEXTURE_2D, karpetTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("karpet.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		-7, -0.5, 0.5, 0, 0,  // 0 bawah
		7, -0.5, 0.5, 1, 0,   // 1
		7,  0.05, 0.5, 1, 1,   // 2 atas
		-7,  0.05, 0.5, 0, 1,  // 3

		// right
		7,  0.05,  2.5, 0, 0,  // 4 atas
		7,  0.05, -2.5, 1, 0,  // 5
		7, -2.5, -2.5, 1, 1,  // 6 bawah
		7, -2.5,  2.5, 0, 1,  // 7

		// back
		-7, -0.5, -0.5, 0, 0, // 8 
		7,  -0.5, -0.5, 1, 0, // 9
		7,   0.05, -0.5, 1, 1, // 10
		-7,  0.05, -0.5, 0, 1, // 11

		// left
		-7, -0.5, -2.5, 0, 0, // 12
		-7, -0.5,  2.5, 1, 0, // 13
		-7,  0.05,  2.5, 1, 1, // 14
		-7,  0.05, -2.5, 0, 1, // 15

		// upper
		7, 0.05,  5, 0, 0,   // 16
		-7, 0.05,  5, 1, 0,  // 17
		-7, 0.05, -5, 1, 1,  // 18
		7, 0.05, -5, 0, 1,   // 19

		// bottom
		-7,  0.0 , -5, 0, 0, // 20
		7,  0.0 , -5, 1, 0,  // 21
		7,  0.0 ,  5, 1, 1,  // 22
		-7,  0.0 ,  5, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};
	glGenVertexArrays(1, &karpetVAO);
	glGenBuffers(1, &karpetVBO);
	glGenBuffers(1, &karpetEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(karpetVAO);

	glBindBuffer(GL_ARRAY_BUFFER, karpetVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, karpetEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::BuildMotifKarpet() {
	glGenTextures(1, &motifKarpetTexture);
	glBindTexture(GL_TEXTURE_2D, motifKarpetTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("karpet.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
// front
		-5, -0.5, 0.5, 0, 0,  // 0 bawah
		5, -0.5, 0.5, 1, 0,   // 1
		5,  0.01, 0.5, 1, 1,   // 2 atas
		-5,  0.01, 0.5, 0, 1,  // 3

		// right
		5,  0.01,  2.5, 0, 0,  // 4 atas
		5,  0.01, -2.5, 1, 0,  // 5
		5, -2.5, -2.5, 1, 1,  // 6 bawah
		5, -2.5,  2.5, 0, 1,  // 7

		// back
		-5, -0.5, -0.5, 0, 0, // 8 
		5,  -0.5, -0.5, 1, 0, // 9
		5,   0.01, -0.5, 1, 1, // 10
		-5,  0.01, -0.5, 0, 1, // 11

		// left
		-5, -0.5, -2.5, 0, 0, // 12
		-5, -0.5,  2.5, 1, 0, // 13
		-5,  0.01,  2.5, 1, 1, // 14
		-5,  0.01, -2.5, 0, 1, // 15

		// upper
		5, 0.01,  5, 0, 0,   // 16
		-5, 0.01,  5, 1, 0,  // 17
		-5, 0.01, -5, 1, 1,  // 18
		5, 0.01, -5, 0, 1,   // 19

		// bottom
		-5, -0.01, -5, 0, 0, // 20
		5, -0.01, -5, 1, 0,  // 21
		5, -0.01,  5, 1, 1,  // 22
		-5, -0.01,  5, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};
	glGenVertexArrays(1, &motifKarpetVAO);
	glGenBuffers(1, &motifKarpetVBO);
	glGenBuffers(1, &motifKarpetEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(motifKarpetVAO);

	glBindBuffer(GL_ARRAY_BUFFER, motifKarpetVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, motifKarpetEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::DrawKarpet()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, karpetTexture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
	glBindVertexArray(karpetVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	glm::mat4 model;

	model = glm::translate(model, glm::vec3(10,0, -6));
	model = glm::scale(model, glm::vec3(1, 1, 1));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawMotifKarpet() {
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, motifKarpetTexture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
	glBindVertexArray(motifKarpetVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildFloor()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("floor.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, 0.01, -50.0,  0,  0,
		 50.0, 0.01, -50.0, 15,  0,
		 50.0, 0.01,  50.0, 15, 15,
		-50.0, 0.01,  50.0,  0, 15,
	};

	GLuint titik[] = {
	0,  2,  1,  0,  3,  2 ,};

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(titik), titik, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}
void Demo::DrawFloor()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredWall()
{
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("Tembok001.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// back
		-50.0, -50.0, -20.0,  0,  0,
		 50.0, -50.0, -20.0, 5,  0,
		 50.0, 50.0,  -20.0, 5, 5,
		-50.0, 50.0,  -20.0,  0, 5,

		// right
		20.0, 50.0, 50.0,  0,  0,
		 20.0, 50.0, -50.0, 5,  0,
		 20.0, -50,  -50.0, 5, 5,
		20.0, -50,  50.0,  0, 5,


// left
-20.0, 50.0, 50.0,  0,  0,
-20.0, 50.0, -50.0, 5,  0,
-20.0, -50.0,  -50.0, 5, 5,
-20.0, -50.0,  50.0,  0, 5,

	};

	GLuint titik[] = {
	0,2,1,0,3,2,
	4,6,5,4,7,5,
	8,10,9,8,11,9};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(titik), titik, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}
void Demo::DrawColoredWall()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildRakBuku() {
	glGenTextures(1, &textureL);
	glBindTexture(GL_TEXTURE_2D, textureL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("teksturrakbuku.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		-3.0,   0.0, -3.9, 0, 0,  // 0
		-3.0,   0.0, -2.0, 1, 0,  // 1
		-3.0,   3.0, -2.0, 1, 1,  // 2
		-3.0,   3.0, -3.9, 0, 1,  // 3

		// right
		-3.0,   0.0, -2.0, 0, 0,  // 4
		-3.9,   0.0, -2.0, 1, 0,  // 5
		-3.9,   3.0, -2.0, 1, 1,  // 6
		-3.0,   3.0, -2.0, 0, 1,  // 7

		// back
		-3.9,   0.0, -3.9, 0, 0, // 8 
		-3.9,   0.0, -2.0, 1, 0, // 9
		-3.9,   3.0, -2.0, 1, 1, // 10
		-3.9,   3.0, -3.9, 0, 1, // 11

		// left
		-3.9,   0.0, -3.9, 0, 0, // 12
		-3.0,   0.0, -3.9, 1, 0, // 13
		-3.0,   3.0, -3.9, 1, 1, // 14
		-3.9,   3.0, -3.9, 0, 1, // 15

		// upper
		-3.0,   3.0, -3.9, 0, 0,   // 16
		-3.0,   3.0, -2.0, 1, 0,  // 17
		-3.9,   3.0, -2.9, 1, 1,  // 18
		-3.9,   3.0, -3.9, 0, 1,   // 19

		// bottom
		-3.0,   0.1, -3.9, 0, 0, // 20
		-3.0,   0.1, -2.0, 1, 0,  // 21
		-3.9,   0.1, -2.9, 1, 1,  // 22
		-3.9,   0.1, -3.9, 0, 1, // 23
	};
	unsigned int indices[] = {
		4,  5,  6,  4,  6, 7,   // right
		0,  1,  2,  0,  2,  3,   // front
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};
	glGenVertexArrays(1, &VAOL);
	glGenBuffers(1, &VBOL);
	glGenBuffers(1, &EBOL);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOL);

	glBindBuffer(GL_ARRAY_BUFFER, VBOL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOL);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::BuildDepanRakBuku() {
	glGenTextures(1, &DepanRakBukuTexture);
	glBindTexture(GL_TEXTURE_2D, DepanRakBukuTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("rakbukudepanfix.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	float vertices[] = {
		// format position, tex coords
		// front
		-2.95,   0.0, -3.9, 0, 0,  // 0
		-2.95,   0.0, -2.0, 1, 0,  // 1
		-2.95,   3.0, -2.0, 1, 1,  // 2
		-2.95,   3.0, -3.9, 0, 1,  // 3

		// right
		-2.99,   0.0, -2.0, 0, 0,  // 4
		-2.95,   0.0, -2.0, 1, 0,  // 5
		-2.95,   3.0, -2.0, 1, 1,  // 6
		-2.99,   3.0, -2.0, 0, 1,  // 7

		// back
		-2.95,   0.0, -3.9, 0, 0, // 8 
		-2.95,   0.0, -2.0, 1, 0, // 9
		-2.95,   3.0, -2.0, 1, 1, // 10
		-2.95,   3.0, -3.9, 0, 1, // 11

		// left
		-2.95,   0.0, -3.9, 0, 0, // 12
		-2.99,   0.0, -3.9, 1, 0, // 13
		-2.99,   3.0, -3.9, 1, 1, // 14
		-2.95,   3.0, -3.9, 0, 1, // 15

		// upper
		-2.99,   3.0, -3.9, 0, 0,   // 16
		-2.99,   3.0, -2.0, 1, 0,  // 17
		-2.95,   3.0, -2.9, 1, 1,  // 18
		-2.95,   3.0, -3.9, 0, 1,   // 19

		// bottom
		-2.95,   0.1, -3.9, 0, 0, // 20
		-2.95,   0.1, -2.0, 1, 0,  // 21
		-2.99,   0.1, -2.9, 1, 1,  // 22
		-2.99,   0.1, -3.9, 0, 1, // 23
	};
	unsigned int indices[] = {
		4,  5,  6,  4,  6, 7,   // right
		0,  1,  2,  0,  2,  3,   // front
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};
	glGenVertexArrays(1, &DepanRakBukuVAO);
	glGenBuffers(1, &DepanRakBukuVBO);
	glGenBuffers(1, &DepanRakBukuEBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(DepanRakBukuVAO);

	glBindBuffer(GL_ARRAY_BUFFER, DepanRakBukuVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DepanRakBukuEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::DrawRakBuku()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureL);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
	glBindVertexArray(VAOL); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glm::mat4 model;

	model = glm::translate(model, glm::vec3(-4, 0, -3));
	model = glm::scale(model, glm::vec3(4, 4, 4));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void Demo::DrawDepanRakBuku() {
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DepanRakBukuTexture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
	glBindVertexArray(DepanRakBukuVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glm::mat4 model;

	model = glm::translate(model, glm::vec3(-4, 0, -3));
	model = glm::scale(model, glm::vec3(4, 4, 4));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Meja Belajar dan Jendela", 900, 600, false, false);
}