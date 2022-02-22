#ifndef TEXTURE_H
#define TEXTURE_H

namespace ruya
{
	
	
	class Texture
	{
	public:
		Texture();
		Texture(const char* texturePath);
		~Texture();

		int height() const { return mHeight; }
		int width() const { return mWidth; }
		int channels() const { return mChannels; }
		unsigned char* data() { return mData; }


	private:
		int mWidth;
		int mHeight;
		int mChannels; // number of color channels: rgb = 3
		unsigned char* mData;
		GLuint mTextureID;
	};


}

#endif