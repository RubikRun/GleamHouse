#pragma once

#include "Layer.h"
#include "RenderObject.h"

namespace GleamHouse
{
	class GleamHouse_Scene;

	class FinishedLevel_Scene : public Pekan::Layer
	{
	public:

		FinishedLevel_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;

		std::string getLayerName() const override { return "finishedLevelScene_layer"; }

		// Attaches the main Gleam House scene
		void attachMainScene(GleamHouse_Scene* mainScene) { m_mainScene = mainScene; }

		// Plays the "Finished Level" animation
		void play() { m_isPlaying = true; }

		// Returns time since played
		float getTime() const { return t; }

	private: /* variables */

		// A flag indicating if "Finished Level" animation is currently playing
		bool m_isPlaying = false;

		Pekan::Graphics::RenderObject m_renderObject;

		GleamHouse_Scene* m_mainScene = nullptr;

		float t = 0.0f;
	};

} // namespace GleamHouse