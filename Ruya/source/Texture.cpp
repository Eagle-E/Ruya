#include "Texture.h"
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

ruya::Texture::Texture()
	: mWidth(0), mHeight(0), mChannels(0), mData(nullptr), mTextureID(0)
{
}

ruya::Texture::Texture(const char* texturePath)
{
	// load texture data
	stbi_set_flip_vertically_on_load(true);
	mData = stbi_load(texturePath, &mWidth, &mHeight, &mChannels, 0);

	// check if loading succeeded
	if (!mData)
	{
		std::cerr << "Error loading texture: " << stbi_failure_reason() << "\nTexture path: " << texturePath << std::endl;
		mData = nullptr;
	}

	// create opengl texture
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	GLuint sourceColorType;
	mChannels == 3 ? sourceColorType = GL_RGB : sourceColorType = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, sourceColorType, GL_UNSIGNED_BYTE, mData);
	glGenerateMipmap(GL_TEXTURE_2D);

	
	/*
		Note:	activating a texture slot then binding this texture's ID will move the texture
				data to the corresponding texture slot space in the vram.
				
				For example:
					glActiveTexture(GL_TEXTURE9);
					glBindTexture(GL_TEXTURE_2D, mTextureID);

				The above code will move the data of mTextureID to slot GL_TEXTURE9.
				We can access the texture in the fragment shader by binding a uniform
				name to the texture slot's number. So if we do shader.setInt("ourTexture", 9);
				We can use "ourTexture" to access the texture data in the fragment shader by
				declaring it like "uniform sampler2D ourTexture;".
	*/
}

ruya::Texture::~Texture()
{
	stbi_image_free(mData);
}
