#pragma once

class Model3D {
protected:

	GLuint VAO, VBO;

	glm::vec3 modelPos;
	glm::vec3 modelScale;
	glm::vec3 modelTheta;
	glm::vec3 modelRX;
	glm::vec3 modelRY;
	glm::vec3 modelRZ;

	float modelScaleMod;

	int modelType;

	GLuint texture;

	std::vector<GLfloat> fullVertexData;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t>material;
	tinyobj::attrib_t attributes;
	glm::mat4 identity_matrix;
	glm::mat4 transformation_matrix;

	const char* image;

	const char* object;

public:
	Model3D(glm::vec3 pos, const char* image, const char* object, glm::vec3 scale, int i) {
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		this->modelPos = pos;
		this->modelScale = scale;
		this->modelScaleMod = 1.f;

		this->modelTheta = glm::vec3(0.f, 0.f, 0.f);
		this->modelRX = glm::vec3(1.f, 0.f, 0.f);
		this->modelRY = glm::vec3(0.0f, 1.f, 0.f);
		this->modelRZ = glm::vec3(0.f, 0.f, 1.f);

		this->identity_matrix = glm::mat4(1.0f);

		this->texture = NULL;
		this->transformation_matrix = {};

		this->image = image;
		this->object = object;

		this->modelType = i;
	}

public:
	void spawn() {
		this->loadTexture();

		this->loadObject();

		this->buildVertexData();

		this->loadVertexData();
	}

	void update(GLuint shaderProg) {
		this->transformationMatrix(shaderProg);
	}

	void draw() {
		glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
	}

	void setPosition(glm::vec3 pos) {
		this->modelPos = pos;
	}

	void setYTheta(float y) {
		this->modelTheta.y = y;
	}

	glm::vec3 getPosition() {
		return this->modelPos;
	}

	glm::vec3 getThetas() {
		return this->modelTheta;
	}

	GLuint getVAO() {
		return this->VAO;
	}

	GLuint getTexture() {
		return this->texture;
	}

	int getModelType() {
		return this->modelType;
	}

private:
	virtual void loadTexture() {
		int img_width, img_height, colorChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* tex_bytes = stbi_load(this->image,
			&img_width,
			&img_height,
			&colorChannels,
			0);

		glGenTextures(1, &this->texture);

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, this->texture);

		glTexImage2D(GL_TEXTURE_2D,
			0,             //texture 0
			GL_RGBA,       //target color format of the texture // change to RGBA for png, RGB for jpg
			img_width,   //texture width
			img_height, //texture height
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			tex_bytes);       //loaded texture in bytes

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(tex_bytes);
		glEnable(GL_DEPTH_TEST);
	}

	void loadObject() {
		std::string path = this->object;
		std::string warning, error;

		bool success = tinyobj::LoadObj(
			&this->attributes,
			&this->shapes,
			&this->material,
			&warning,
			&error,
			path.c_str()
		);
	}

	virtual void buildVertexData() {
		for (int i = 0; i < this->shapes[0].mesh.indices.size(); i++) {
			tinyobj::index_t vData = this->shapes[0].mesh.indices[i];

			this->fullVertexData.push_back(this->attributes.vertices[(vData.vertex_index * 3)]);
			this->fullVertexData.push_back(this->attributes.vertices[(vData.vertex_index * 3) + 1]);
			this->fullVertexData.push_back(this->attributes.vertices[(vData.vertex_index * 3) + 2]);

			this->fullVertexData.push_back(this->attributes.normals[(vData.normal_index * 3)]);
			this->fullVertexData.push_back(this->attributes.normals[(vData.normal_index * 3) + 1]);
			this->fullVertexData.push_back(this->attributes.normals[(vData.normal_index * 3) + 2]);

			this->fullVertexData.push_back(this->attributes.texcoords[(vData.texcoord_index * 2)]);
			this->fullVertexData.push_back(this->attributes.texcoords[(vData.texcoord_index * 2) + 1]);
		}

	}

	virtual void loadVertexData() {
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(GLfloat) * this->fullVertexData.size(),
			this->fullVertexData.data(),
			GL_DYNAMIC_DRAW);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(float),
			(void*)0
		);

		glEnableVertexAttribArray(0);

		GLintptr normPtr = 3 * sizeof(float);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(float),
			(void*)normPtr
		);

		glEnableVertexAttribArray(1);

		GLintptr uvPtr = 6 * sizeof(float);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(float),
			(void*)uvPtr
		);

		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

private:
	void transformationMatrix(GLuint shaderProg) {

		this->transformation_matrix = glm::translate(this->identity_matrix,
			this->modelPos);

		this->transformation_matrix = glm::scale(this->transformation_matrix,
			this->modelScale);

		this->transformation_matrix = glm::rotate(this->transformation_matrix,
			glm::radians(this->modelTheta.y),
			this->modelRY);

		this->transformation_matrix = glm::rotate(this->transformation_matrix,
			glm::radians(this->modelTheta.x),
			this->modelRX);

		this->transformation_matrix = glm::rotate(this->transformation_matrix,
			glm::radians(this->modelTheta.z),
			this->modelRZ);

		glUseProgram(shaderProg);
		unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");

		glUniformMatrix4fv(transformLoc,
			1,
			GL_FALSE,
			glm::value_ptr(this->transformation_matrix));
	}
};