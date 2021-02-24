#pragma once
#include "UISystem.h"
#include "IDTranslationSystem.h"
#include "NetworkSystem.h"
#include "MovementSystem.h"

namespace SceneSystems {
	class SceneSystems {
	public:
		UI::UISystem uiSystem;
		TranslationSystem::IDTranslation translationSystem;
		NetworkSystem::NetworkHandler netSystem;
		MovementSystem::MovementSystem movSystem;
	};
}