#pragma once

#include "PekanApplication.h"

namespace GleamHouse
{

	class GleamHouse_Application : public Pekan::PekanApplication
	{
		bool _fillLayerStack(Pekan::LayerStack& layerStack) override;
		std::string getName() const override { return "Gleam House"; }
		Pekan::ApplicationProperties getProperties() const override;
	};

} // namespace GleamHouse