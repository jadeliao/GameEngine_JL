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
#include "AnimationComponent.h"
#include "State.h"
#include "StateMachine.h"
#include "DirectionCondition.h"
#include "IdleCondition.h"
#include "AnimationAction.h"
#include "Transition.h"

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

	//Loop through state machine data to load
	XMLElement* stateMachineData = sceneData->FirstChildElement("StateMachine");
	AddStateMachine(stateMachineData);

	//Loop through wall data to set up scene walls
	XMLElement* wallData = sceneData->FirstChildElement("Wall");
	AddWallData(wallData);

	//Set target for AI
	SetAITarget();
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
			else if (componentType == "SpriteMesh") {
				AddComponent<MeshComponent>(componentName, nullptr, "");
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
			else if (componentType == "Animation") {

				Ref<AnimationComponent> animationComponent = std::make_shared<AnimationComponent>(nullptr);
				//Find the texture list for the animations
				const XMLElement* listElement = componentFirstElement->FirstChildElement("List");
				//Loop through all the texture under the element
				while (listElement) {
					const char* listName = listElement->FindAttribute("name")->Value();
					const XMLElement* materialElement = listElement->FirstChildElement("Material");
					std::vector <Ref<MaterialComponent>> materialList;
					while (materialElement) {
						const char* materialName = materialElement->FindAttribute("componentName")->Value();
						materialList.push_back(GetComponent<MaterialComponent>(materialName));
						materialElement = materialElement->NextSiblingElement("Material");
					}
					animationComponent->AddAnimation(listName, materialList);
					listElement = listElement->NextSiblingElement("List");
				}
				AddComponent<AnimationComponent>(componentName, animationComponent);
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
					else if (strcmp(type, "pathfollowing") == 0) {
						ai_component->addSteeringBehaviour(pathfollowing);
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
		XMLElement* componentAnimationElement = actorData->FirstChildElement("Animation");
		//if (!componentMeshElement || !componentMaterialElement || !componentShaderElement) {
		//	std::cerr << "Component Element not found\n";
		//}
		if (!componentMaterialElement || !componentShaderElement) {
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
			const char* meshName;
			if (componentMeshElement) {
				meshName = componentMeshElement->FindAttribute("componentName")->Value();
			}
			else {
				meshName = "";
			}

			const char* materialName = componentMaterialElement->FindAttribute("componentName")->Value();
			const char* shaderName = componentShaderElement->FindAttribute("componentName")->Value();
			Ref<MeshComponent> actorMesh = GetComponent<MeshComponent>(meshName);
			Ref<MaterialComponent> actorMaterial = GetComponent<MaterialComponent>(materialName);
			Ref<ShaderComponent> actorShader = GetComponent<ShaderComponent>(shaderName);

			if (!actorMaterial || !actorShader) {
				std::cerr << "Component not found\n";
			}
			//if (!actorMesh || !actorMaterial || !actorShader) {
			//	std::cerr << "Component not found\n";
			//}

			//Parent components to the actor
			if (actorMesh) actorMesh->SetParent(newActor);
			actorMaterial->SetParent(newActor);
			actorShader->SetParent(newActor);

			//Add all components to actor
			Ref<TransformComponent> transform = std::make_shared<TransformComponent>(newActor);
			if (actorMesh) newActor->AddComponent<MeshComponent>(actorMesh);
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
				//actorAI->setTarget(targetName);
				aiList[actorAI] = targetName;
				newActor->AddComponent<AIComponent>(actorAI);
			}
			//Set animation if exist
			if (componentAnimationElement) {
				const char* animationComponentName = componentAnimationElement->FindAttribute("componentName")->Value();
				Ref<AnimationComponent> actorAnimation = GetComponent<AnimationComponent>(animationComponentName);
				actorAnimation->SetParent(newActor);
				newActor->AddComponent<AnimationComponent>(actorAnimation);
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

void AssetManager::AddStateMachine(XMLElement* stateMachineData){
	while (stateMachineData) {
		//Get First child element and check if it exists;
		XMLElement* stateMachineElement = stateMachineData->FirstChildElement();
		if (!stateMachineElement) {
			std::cerr << "No First Element \n";
		}
		else {
			//Get name and information for the statemachine
			const char* stateMachineName = stateMachineData->FindAttribute("name")->Value();
			const char* initialStateName = stateMachineData->FindAttribute("initialState")->Value();
			const char* ownerName = stateMachineData->FindAttribute("owner")->Value();
			Ref<Actor> owner = GetActor(ownerName);
			//Create a structure for storing information
			Ref<StateMachineAsset> stateMachineAsset = std::make_shared<StateMachineAsset>();
			Ref<StateMachine> stateMachine = std::make_shared<StateMachine>();

			//Add Action
			XMLElement* actionData = stateMachineData->FirstChildElement("Action");
			if (actionData) {
				XMLElement* actionElement = actionData->FirstChildElement();
				while (actionElement) {
					//Get the type of the action
					std::string actionType = actionElement->Value();
					const char* actionName = actionElement->FindAttribute("name")->Value();
					if (actionType == "AnimationAction") {
						Ref<AnimationAction> animationAction = std::make_shared<AnimationAction>(owner);
						const char* animationName = actionElement->FindAttribute("animationName")->Value();
						animationAction->setAnimationName(animationName);
						stateMachineAsset->actionList[actionName] = animationAction;
					}
					actionElement = actionElement->NextSiblingElement();
				}
			}

			//Add Condition
			XMLElement* conditionData = stateMachineData->FirstChildElement("Condition");
			if (conditionData) {
				XMLElement* conditionElement = conditionData->FirstChildElement();
				while (conditionElement) {
					//Get the type of the action
					std::string conditionType = conditionElement->Value();
					const char* conditionName = conditionElement->FindAttribute("name")->Value();
					if (conditionType == "IdleCondition") {
						Ref<IdleCondition> idleCondition = std::make_shared<IdleCondition>(owner);
						stateMachineAsset->conditionList[conditionName] = idleCondition;
					}
					else if (conditionType == "DirectionCondition") {
						Ref<DirectionCondition> directionCondition = std::make_shared<DirectionCondition>(owner);
						const float min = conditionElement->FloatAttribute("min");
						const float max = conditionElement->FloatAttribute("max");
						directionCondition->setRange(min, max);
						stateMachineAsset->conditionList[conditionName] = directionCondition;
					}
					conditionElement = conditionElement->NextSiblingElement();
				}
			}
			//Create State
			XMLElement* stateData = stateMachineData->FirstChildElement("State");
			while (stateData) {
				const char* stateName = stateData->FindAttribute("name")->Value();
				const char* actionName = stateData->FindAttribute("action")->Value();
				Ref<State> state_ = std::make_shared<State>();
				Ref<Action> action_ = stateMachineAsset->getAction(actionName);
				if (action_) {
					state_->setAction(action_);
				}
				//Get the transition and store them in a temporarily list
				Ref<StateTransition> stateTransition_ = std::make_shared<StateTransition>();
				stateTransition_->state = state_;
				XMLElement* transitionNameData = stateData->FirstChildElement("Transition");
				while (transitionNameData) {
					const char* transitionName = transitionNameData->FindAttribute("name")->Value();
					stateTransition_->transitionList.push_back(transitionName);
					transitionNameData = transitionNameData->NextSiblingElement("Transition");
				}
				stateMachineAsset->stateList[stateName] = stateTransition_;
				stateData = stateData->NextSiblingElement("State");
			}

			//Create transition
			XMLElement* transitionData = stateMachineData->FirstChildElement("Transition");
			while (transitionData) {
				Ref<Transition> transition_ = nullptr;
				const char* transitionName = transitionData->FindAttribute("name")->Value();
				const char* stateName = transitionData->FindAttribute("target")->Value();
				const char* conditionName = transitionData->FindAttribute("condition")->Value();
				//Get data from the StateMachineAssets
				Ref<State> state_ = stateMachineAsset->getState(stateName);
				Ref<Condition> condition_ = stateMachineAsset->getCondition(conditionName);
				//Only add the data when its valid
				if (state_ && condition_) {
					transition_ = std::make_shared<Transition>(state_, condition_);
				}
				stateMachineAsset->transitionList[transitionName] = transition_;
				transitionData = transitionData->NextSiblingElement("Transition");
			}
			//Loop through list add transition to State
			for (auto element_ : stateMachineAsset->stateList) {
				Ref<State> state_ = element_.second->state;
				//Loop through transition list
				for (auto transitionName_ : element_.second->transitionList) {
					Ref<Transition> transition_ = stateMachineAsset->getTransition(transitionName_);
					if (transition_) {
						state_->addTransition(transition_);
					}
				}
			}
			Ref<State> initialState_ = stateMachineAsset->getState(initialStateName);
			if (initialState_) {
				stateMachine->setInitialState(initialState_);
			}
			owner->setStateMachine(stateMachine);
		}

		stateMachineData = stateMachineData->NextSiblingElement("StateMachine");

	}
}

void AssetManager::AddWallData(XMLElement* wallData){
	
	//Get labels to determine the visibility of the walls
	std::vector<int> wallLocation;
	while (wallData) {
		const int locLabel = wallData->IntAttribute("label");
		wallLocation.push_back(locLabel);
		wallData = wallData->NextSiblingElement("Wall");
	}

	//Ref<Box> wallBox_ = std::dynamic_pointer_cast<Box>(wallActor->GetComponent<CollisionComponent>()->getShape());
	//Vec3 boxSize = wallBox_->halfExtents;
	//Default size for now
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
			Vec3 pos = Vec3(x, y, 0.0f);
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

void AssetManager::SetAITarget(){
	for (auto ai : aiList) {
		Ref<Actor> actor_ = GetActor(ai.second);
		ai.first->setTarget(actor_);
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
