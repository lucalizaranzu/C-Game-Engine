#include <sepch.h>
#include "ModelManager.h"

#include "MasterRenderer.h"

std::shared_ptr<Model> defaultEntityModel;

void Shmingo::initModels() {

	Texture2D funnyTexture = Shmingo::createTexture2D("funnyimage.png");
	defaultEntityModel.reset(Shmingo::createModelPointer(std::bind(Shmingo::createCubeModel, vec3(0.0f, 0.0f, 0.0f), funnyTexture)));

	se_masterRenderer.declareEntityModel(Shmingo::DefaultEntity, defaultEntityModel);

}