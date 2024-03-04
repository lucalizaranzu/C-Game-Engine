#pragma once

#include <sepch.h>
#include "Texture.h"
#include <stb_image.h>

std::shared_ptr<Texture2D> createTexture2D(std::string filePath);