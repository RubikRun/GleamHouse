#include "GleamHouse_Application.h"

#include "GleamHouse_Scene.h"
#include "FinishedLevel_Scene.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace GleamHouse
{

	bool GleamHouse_Application::_fillLayerStack(LayerStack& layerStack)
	{
		std::shared_ptr<GleamHouse_Scene> mainScene = std::make_shared<GleamHouse_Scene>(this);
		std::shared_ptr<FinishedLevel_Scene> finishedLevelScene = std::make_shared<FinishedLevel_Scene>(this);

		finishedLevelScene->attachMainScene(mainScene.get());
		mainScene->attachFinishedLevelScene(finishedLevelScene.get());

		layerStack.pushLayer(mainScene);
		layerStack.pushLayer(finishedLevelScene);

		return true;
	}

	ApplicationProperties GleamHouse_Application::getProperties() const
	{
		ApplicationProperties props;
		props.windowProperties.width = 1600;
		props.windowProperties.height = 900;
		props.fps = 60.0;
		props.numberOfSamples = 16;
		props.windowProperties.title = getName();
		return props;
	}

} // namespace GleamHouse