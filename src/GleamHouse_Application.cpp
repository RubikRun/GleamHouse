#include "GleamHouse_Application.h"

#include "GleamHouse_Scene.h"

#include "PekanEngine.h"
using Pekan::PekanEngine;
using Pekan::ApplicationProperties;
using Pekan::WindowProperties;
using Pekan::LayerStack;

namespace GleamHouse
{

	bool GleamHouse_Application::_fillLayerStack(LayerStack& layerStack)
	{
		// Create demo scene
		std::shared_ptr<GleamHouse_Scene> demoScene = std::make_shared<GleamHouse_Scene>(this);
		// Add scene to application's layers
		layerStack.pushLayer(demoScene);

		return true;
	}

	ApplicationProperties GleamHouse_Application::getProperties() const
	{
		ApplicationProperties props;
		props.windowProperties.width = 1600;
		props.windowProperties.height = 900;
		props.numberOfSamples = 16;
		props.windowProperties.title = getName();
		return props;
	}

} // namespace GleamHouse