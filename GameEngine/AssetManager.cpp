#include "AssetManager.h"
#include <fstream>
#include "tinyxml2.h"
#include "Actor.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"

using namespace tinyxml2;

AssetManager::AssetManager(){
	ReadManiFest();
	OnCreate();
}

AssetManager::~AssetManager(){
	//OnDestroy();
}


void AssetManager::ReadManiFest() {
	//Read document
	doc.LoadFile("Scene1.xml");
	bool status = doc.Error();
	if (status) {
		std::cout << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
		return;
	}
	//Loop through the document to read data
	XMLElement* rootData = doc.RootElement();
	XMLElement* sceneData = rootData->FirstChildElement("Scene1");
	//While exists a component data
	XMLElement* componentData = sceneData->FirstChildElement("Component");
	while (componentData) {
		//Get First child element and check if it exists;
		XMLElement* componentFirstElement = componentData->FirstChildElement();
		if (!componentFirstElement) {
			std::cerr << "No First Element \n";
		}
		else {
			//Get file name and add components
			std::string componentType = componentFirstElement->Value();
			const char *componentName = componentData->FindAttribute("name")->Value();
			//Create component accordingly and add to asset manager
			if (componentType == "Mesh") {
				const XMLAttribute* filenameAttribute = componentFirstElement->FindAttribute("filename");
				if (!filenameAttribute) {
					std::cerr << "No filename Attribute \n";
				}
				else {
					const char *filename = componentFirstElement->FindAttribute("filename")->Value();
					AddComponent<MeshComponent>(componentName, nullptr, filename);
				}
			}
			else if (componentType == "Material") {
				const XMLAttribute* filenameAttribute = componentFirstElement->FindAttribute("filename");
				if (!componentFirstElement->FindAttribute("filename")) {
					std::cerr << "No filename Attribute \n";
				}
				else {
					const char* filename = componentFirstElement->FindAttribute("filename")->Value();
					AddComponent<MaterialComponent>(componentName, nullptr, filename);
				}
			}
			else if (componentType == "Shader") {
				const XMLAttribute* vertFilenameAttribute = componentFirstElement->FindAttribute("vertFilename");
				const XMLAttribute* fragFilenameAttribute = componentFirstElement->FindAttribute("fragFilename");
				if ((!vertFilenameAttribute) || (!fragFilenameAttribute)) {
					std::cerr << "No vertFilename or fragFilename Attribute \n";
				}
				else {
					const char* vertFile = vertFilenameAttribute->Value();
					const char* fragFile = fragFilenameAttribute->Value();
					AddComponent<ShaderComponent>(componentName, nullptr, vertFile, fragFile);
				}
			}
		}
		componentData = componentData->NextSiblingElement("Component");
	}
	//Loop through actor data to load
	XMLElement* actorData = sceneData->FirstChildElement("Actor");
	while (actorData) {
		//Get First child element and check if it exists;
		XMLElement* componentParentElement = actorData->FirstChildElement("Parent");
		XMLElement* componentMeshElement = actorData->FirstChildElement("Mesh");
		XMLElement* componentMaterialElement = actorData->FirstChildElement("Material");
		XMLElement* componentShaderElement = actorData->FirstChildElement("Shader");
		if (!componentMeshElement || !componentMaterialElement || !componentShaderElement) {
			std::cerr << "Component Element not found\n";
		}
		else {

			Ref<Actor> newActor;
			const char* actorName = actorData->FindAttribute("name")->Value();
			std::string parentName = componentParentElement->FindAttribute("componentName")->Value();
			if (parentName == "None") {
				newActor = std::make_shared<Actor>(nullptr);
			}
			else {
				newActor = std::make_shared<Actor>(GetComponent<Actor>(parentName.c_str()));
			}
			const char* meshName = componentMeshElement->FindAttribute("componentName")->Value();
			const char* materialName = componentMaterialElement->FindAttribute("componentName")->Value();
			const char* shaderName = componentShaderElement->FindAttribute("componentName")->Value();
			Ref<MeshComponent> actorMesh = GetComponent<MeshComponent>(meshName);
			Ref<MaterialComponent> actorMaterial = GetComponent<MaterialComponent>(materialName);
			Ref<ShaderComponent> actorShader = GetComponent<ShaderComponent>(shaderName);
			if (!actorMesh || !actorMaterial || !actorShader) {
				std::cerr << "Component not found\n";
			}
			newActor->AddComponent<MeshComponent>(actorMesh);
			newActor->AddComponent<MaterialComponent>(actorMaterial);
			newActor->AddComponent<ShaderComponent>(actorShader);
			AddComponent<Actor>(actorName, newActor);
		}
		actorData = actorData->NextSiblingElement("Actor");
	}
}

bool AssetManager::OnCreate(){
	for (std::pair<const char*, Ref<Component>> c : componentCatalog) {
		if (c.second->OnCreate() == false) {
			return false;
		}
	}
	return true;
}

void AssetManager::OnDestroy(){
}

void AssetManager::RemoveAllComponents(){

	componentCatalog.clear();
}
