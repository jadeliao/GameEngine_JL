#include "AssetManager.h"
#include <fstream>
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "TransformComponent.h"
#include "BodyComponent.h"
#include "AIComponent.h"
#include "CollisionComponent.h"
#include "Node.h"

using namespace tinyxml2;

AssetManager::AssetManager(){
	ReadManiFest();
	OnCreate();
}

AssetManager::AssetManager(const char* scene_): scene(scene_) {
	ReadManiFest();
	OnCreate();
}

AssetManager::~AssetManager(){
	OnDestroy();
}


void AssetManager::ReadManiFest() {
	//Read document with the given name
	char sceneFile[20];
	strcpy_s(sceneFile, scene);
	strcat_s(sceneFile, ".xml");
	doc.LoadFile(sceneFile);
	bool status = doc.Error();
	if (status) {
		std::cout << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
		return;
	}
	//Loop through document to read data
	XMLElement* rootData = doc.RootElement();
	XMLElement* sceneData = rootData->FirstChildElement(scene);
	//Loop through component data to load
	XMLElement* componentData = sceneData->FirstChildElement("Component");
	AddComponentData(componentData);

	//Loop through actor data to load
	XMLElement* actorData = sceneData->FirstChildElement("Actor");
	AddActorData(actorData);

	//Loop through wall data to set up scene walls
	XMLElement* wallData = sceneData->FirstChildElement("Wall");
	AddWallData(wallData);
}

void AssetManager::AddComponentData(XMLElement* componentData) {
	while (componentData) {
		//Get First child element and check if it exists;
		XMLElement* componentFirstElement = componentData->FirstChildElement();
		if (!componentFirstElement) {
			std::cerr << "No First Element \n";
		}
		else {
			//Get file name and add components			
			std::string componentType = componentFirstElement->Value();
			const char* componentName = componentData->FindAttribute("name")->Value();
			//Create component accordingly and add to asset manager
			if (componentType == "Mesh") {
				const XMLAttribute* filenameAttribute = componentFirstElement->FindAttribute("filename");
				if (!filenameAttribute) {
					std::cerr << "No filename Attribute \n";
				}
				else {
					const char* filename = componentFirstElement->FindAttribute("filename")->Value();
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
			else if (componentType == "Body") {
				std::string bodyType_ = componentFirstElement->FindAttribute("type")->Value();
				const float posx = componentFirstElement->FloatAttribute("posx");
				const float posy = componentFirstElement->FloatAttribute("posy");
				const float posz = componentFirstElement->FloatAttribute("posz");
				const float velx = componentFirstElement->FloatAttribute("velx");
				const float vely = componentFirstElement->FloatAttribute("vely");
				const float velz = componentFirstElement->FloatAttribute("velz");
				const float accelx = componentFirstElement->FloatAttribute("accelx");
				const float accely = componentFirstElement->FloatAttribute("accely");
				const float accelz = componentFirstElement->FloatAttribute("accelz");
				const float mass = componentFirstElement->FloatAttribute("mass");
				const float radius = componentFirstElement->FloatAttribute("radius");
				const float orientation = componentFirstElement->FloatAttribute("orientation");
				const float rotation = componentFirstElement->FloatAttribute("rotation");
				const float angular = componentFirstElement->FloatAttribute("angular");
				const float maxSpeed = componentFirstElement->FloatAttribute("maxSpeed");
				const float maxAccel = componentFirstElement->FloatAttribute("maxAccel");
				const float maxRotation = componentFirstElement->FloatAttribute("maxRotation");
				const float maxAngular = componentFirstElement->FloatAttribute("maxAngular");
				//Setup body type
				enum BodyType bodyType = Base;
				if (bodyType_ == "Kinematic") {
					bodyType = Kinematic;
				}
				AddComponent<BodyComponent>(componentName, nullptr, bodyType,
					Vec3(posx, posy, posz), Vec3(velx, vely, velz), Vec3(accelx, accely, accelz),
					mass, radius, orientation, rotation, angular,
					maxSpeed, maxAccel, maxRotation, maxAngular);
			}
			else if (componentType == "Collision") {
				std::string collisionType = componentFirstElement->FindAttribute("shape")->Value();
				Ref<GEOMETRY::Shape> shape_;
				if (collisionType == "sphere") {
					const float posx = componentFirstElement->FloatAttribute("posx");
					const float posy = componentFirstElement->FloatAttribute("posy");
					const float posz = componentFirstElement->FloatAttribute("posz");
					const float radius = componentFirstElement->FloatAttribute("radius");
					shape_ = std::make_shared<GEOMETRY::Sphere>(posx, posy, posz, radius);
				}
				else if (collisionType == "cylinder") {
					const float posxA = componentFirstElement->FloatAttribute("posxA");
					const float posyA = componentFirstElement->FloatAttribute("posyA");
					const float poszA = componentFirstElement->FloatAttribute("poszA");
					const float posxB = componentFirstElement->FloatAttribute("posxB");
					const float posyB = componentFirstElement->FloatAttribute("posyB");
					const float poszB = componentFirstElement->FloatAttribute("poszB");
					const float radius = componentFirstElement->FloatAttribute("radius");
					shape_ = std::make_shared<GEOMETRY::Cylinder>(radius, 
						Vec3(posxA, posyA, poszA), Vec3(posxB, posyB, poszB));
				}
				else if (collisionType == "capsule") {
					const float posxA = componentFirstElement->FloatAttribute("posxA");
					const float posyA = componentFirstElement->FloatAttribute("posyA");
					const float poszA = componentFirstElement->FloatAttribute("poszA");
					const float posxB = componentFirstElement->FloatAttribute("posxB");
					const float posyB = componentFirstElement->FloatAttribute("posyB");
					const float poszB = componentFirstElement->FloatAttribute("poszB");
					const float radius = componentFirstElement->FloatAttribute("radius");
					shape_ = std::make_shared<GEOMETRY::Capsule>(radius,
						Vec3(posxA, posyA, poszA), Vec3(posxB, posyB, poszB));
				}
				else if (collisionType == "box") {
					const float posx = componentFirstElement->FloatAttribute("posx");
					const float posy = componentFirstElement->FloatAttribute("posy");
					const float posz = componentFirstElement->FloatAttribute("posz");
					const float halfExtentX = componentFirstElement->FloatAttribute("halfExtentX");
					const float halfExtentY = componentFirstElement->FloatAttribute("halfExtentY");
					const float halfExtentZ = componentFirstElement->FloatAttribute("halfExtentZ");
					shape_ = std::make_shared<GEOMETRY::Box>(Vec3(posx, posy, posz), 
						Vec3(halfExtentX, halfExtentY, halfExtentZ), Quaternion());

				}
				else if (collisionType == "plane") {
					const float posx = componentFirstElement->FloatAttribute("posx");
					const float posy = componentFirstElement->FloatAttribute("posy");
					const float posz = componentFirstElement->FloatAttribute("posz");
					const float d = componentFirstElement->FloatAttribute("d");
					shape_ = std::make_shared<GEOMETRY::Plane>(Vec3(posx, posy, posz), d);

				}
				AddComponent<CollisionComponent>(componentName, nullptr, collisionType, shape_);
			}
			else if (componentType == "AI") {
				XMLElement* AIData = componentData->FirstChildElement("Steering");
				AddComponent<AIComponent>(componentName, nullptr, nullptr);
				Ref<AIComponent> ai_component = GetComponent<AIComponent>(componentName);
				//Add all the steering behaviours
				while (AIData) {
					const char* type = AIData->FindAttribute("type")->Value();
					if (strcmp(type, "seeking") == 0) {
						ai_component->addSteeringBehaviour(seeking);
					}
					else if (strcmp(type, "aligning") == 0) {
						ai_component->addSteeringBehaviour(aligning);
					}
					else if (strcmp(type, "looking") == 0) {
						ai_component->addSteeringBehaviour(looking);
					}
					else {
						std::cerr << "Steering type not supported\n";
					}
					AIData = AIData->NextSiblingElement("Steering");
				}
			}
		}
		componentData = componentData->NextSiblingElement("Component");
	}
}

void AssetManager::AddActorData(XMLElement* actorData) {
	while (actorData) {
		//Get First child element and check if it exists;
		XMLElement* componentParentElement = actorData->FirstChildElement("Parent");
		XMLElement* componentMeshElement = actorData->FirstChildElement("Mesh");
		XMLElement* componentMaterialElement = actorData->FirstChildElement("Material");
		XMLElement* componentShaderElement = actorData->FirstChildElement("Shader");
		XMLElement* componentBodyElement = actorData->FirstChildElement("Body");
		XMLElement* componentCollisionElement = actorData->FirstChildElement("Collision");
		XMLElement* componentAIElement = actorData->FirstChildElement("AI");
		if (!componentMeshElement || !componentMaterialElement || !componentShaderElement) {
			std::cerr << "Component Element not found\n";
		}
		else {

			Ref<Actor> newActor;
			//Get Actor name
			std::string actorName = actorData->FindAttribute("name")->Value();

			//Check actor data
			std::string parentName = componentParentElement->FindAttribute("componentName")->Value();
			if (actorName == "Wall") {
				newActor = std::make_shared<WallActor>(nullptr);
			}
			else if (parentName == "None") {
				newActor = std::make_shared<Actor>(nullptr);
			}
			else {
				newActor = std::make_shared<Actor>(GetComponent<Actor>(parentName.c_str()));
			}
			//Get components to add to actor
			const char* meshName = componentMeshElement->FindAttribute("componentName")->Value();
			const char* materialName = componentMaterialElement->FindAttribute("componentName")->Value();
			const char* shaderName = componentShaderElement->FindAttribute("componentName")->Value();
			Ref<MeshComponent> actorMesh = GetComponent<MeshComponent>(meshName);
			Ref<MaterialComponent> actorMaterial = GetComponent<MaterialComponent>(materialName);
			Ref<ShaderComponent> actorShader = GetComponent<ShaderComponent>(shaderName);

			if (!actorMesh || !actorMaterial || !actorShader) {
				std::cerr << "Component not found\n";
			}

			//Parent components to the actor
			actorMesh->SetParent(newActor);
			actorMaterial->SetParent(newActor);
			actorShader->SetParent(newActor);

			//Add all components to actor
			Ref<TransformComponent> transform = std::make_shared<TransformComponent>(newActor);
			newActor->AddComponent<MeshComponent>(actorMesh);
			newActor->AddComponent<MaterialComponent>(actorMaterial);
			newActor->AddComponent<ShaderComponent>(actorShader);
			newActor->AddComponent<TransformComponent>(transform);

			//Set body if exist
			if (componentBodyElement) {
				const char* bodyComponentName = componentBodyElement->FindAttribute("componentName")->Value();
				Ref<BodyComponent> actorBody = GetComponent<BodyComponent>(bodyComponentName);
				actorBody->SetParent(newActor);
				newActor->AddComponent<BodyComponent>(actorBody);
			}

			//Set collision if exist
			if (componentCollisionElement) {
				const char* collisionName = componentCollisionElement->FindAttribute("componentName")->Value();
				Ref<CollisionComponent> actorCollision = GetComponent<CollisionComponent>(collisionName);
				actorCollision->SetParent(newActor);
				newActor->AddComponent<CollisionComponent>(actorCollision);
			}

			//Set AI if exist
			if (componentAIElement) {
				const char* aiComponentName = componentAIElement->FindAttribute("componentName")->Value();
				const char* targetName = componentAIElement->FindAttribute("targetName")->Value();
				Ref<AIComponent> actorAI = GetComponent<AIComponent>(aiComponentName);
				actorAI->SetParent(newActor);
				actorAI->setTarget(targetName);
				newActor->AddComponent<AIComponent>(actorAI);
			}

			if (actorName == "Wall") {
				wallActor = std::dynamic_pointer_cast<WallActor>(newActor);
			}
			else {
				AddActor(actorName.c_str(), newActor);
			}

		}
		actorData = actorData->NextSiblingElement("Actor");
	}
}

void AssetManager::AddWallData(XMLElement* wallData){
	
	//Get labels to determine the visibility of the walls
	std::vector<int> wallLocation;
	while (wallData) {
		const float locLabel = wallData->IntAttribute("label");
		wallLocation.push_back(locLabel);
		wallData = wallData->NextSiblingElement("Wall");
	}

	float maxRow = 15.0f;
	float maxColumn = 8.5f;
	float row_start = -14.0f;
	float column_start = -8.0f;
	float tileWidth = 2.0f;
	float tileHeight = 2.0f;

	int rowNum = ceil(maxRow * 2.0f / tileWidth);
	int columnNum = ceil(maxColumn * 2.0f / tileHeight);

	int j = 0;
	int label = 0;
	//Resize to avoid exception thrown
	wallList.resize(rowNum);
	//Loop through to implement graph
	for (float y = column_start; y < maxColumn; y += tileHeight) {
		int i = 0;
		for (float x = row_start; x < maxRow; x += tileWidth) {
			wallList[i].resize(columnNum);
			//Resize to avoid exception thrown
			Vec3 pos = Vec3(x, y, 0.5f);
			//Use copy contructor to setup wall with different transform component
			Ref<WallActor> wall_ = std::make_shared<WallActor>(*(wallActor.get()));
			wall_->AddComponent<TransformComponent>(wall_, pos, Quaternion());

			if (std::find(wallLocation.begin(), wallLocation.end(), label) != wallLocation.end()) {
				wall_->setVisible(true);
			}
			
			//Create node
			Ref<Node> node_ = std::make_shared<Node>(label, pos);
			node_->setTile(wall_);
			wallList[i][j] = node_;
			i++;
			label++;
		}
		j++;
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
	actorList.clear();
}
