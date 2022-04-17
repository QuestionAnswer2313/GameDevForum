#ifndef ENGINE_H
#define ENGINE_H

#include "Application/Application.h"
#include "Event/Event.h"

//Graphics
#include "Graphics/OpenGLBase.h"
#include "Graphics/Window.h"
#include "Graphics/Camera/EditorCamera.h"
#include "Graphics/Renderer.h"

//Simple ui
#include "UI/ui.h"

//ECS
#include "ECS/ECS.h"
#include "ECS/ECSTypes.h"
#include "ECS/Scene.h"
#include "ECS/Components/AllComponents.h"

//Math
#include "Math/SimpleMath.h"

//Utils
#include "Utils/SimpleStandardLibrary.h"
//#include "Utils/Reflection.h"

// Serialization / Deserialization
#include "Serializer/Serializer.h"

//Platform
#include "Platform/Linux/Linux.h"

#include "Script/Script.h"

#endif
