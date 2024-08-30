#pragma once

#include <ShmingoCore.h>

#include "stb_image.h"
#include "Texture.h"

namespace Shmingo {

	class TextureAtlas {

	public:

		//Set uniform texture dimensions to true if all textures are the same size
		TextureAtlas(size_t width, size_t height, bool uniformTextureDimensions);

		void bind() { texture.bind(); }

		/// <summary>
		/// Adds a texture to the atlas
		/// </summary>
		/// <param name="textureID"> ID the texture will use</param>
		/// <param name="filePath"> Path of the texture</param>
		void addTexture(size_t textureID, std::string filePath);

		Shmingo::QuadTextureCoords getTextureCoords(size_t textureID);

		GLuint& getTextureID() { return texture.getTextureID(); }
		size_t getTextureCount() { return texturesInAtlas; }


	private:

		Texture2D texture;

		size_t m_Width, m_Height;

		bool m_UniformTextureDimensions;

		std::unordered_map<size_t, Shmingo::QuadTextureCoords> m_textureCoords; //TextureCoords

		size_t texturesInAtlas = 0;
	};
}