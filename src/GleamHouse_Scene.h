#pragma once

#include "Floor.h"
#include "Player.h"
#include "Wall.h"

#include "Layer.h"
#include "RectangleShape.h"
#include "Camera2D.h"
#include "Torch.h"

namespace GleamHouse
{

	class FinishedLevel_Scene;

	class GleamHouse_Scene : public Pekan::Layer
	{
	public:

		GleamHouse_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;

		inline std::string getLayerName() const override { return "scene_layer"; }

		// Returns player's size in NDC space
		glm::vec2 getPlayerSizeNDC() const;

		// Attaches the "finished level" scene
		void attachFinishedLevelScene(FinishedLevel_Scene* finishedLevelScene) { m_finishedLevelScene = finishedLevelScene; }

		// Number of floor pieces
		static constexpr int FLOORS_COUNT = 21;
		// Number of torches
		static constexpr int TORCHES_COUNT = 2;

	private: /* functions */

		void createCamera();
		void updateCamera();

		void updateLights();

		void updateDistToStar();

		// Returns star's intensity based on player's current position
		float getStarIntensity();

		// Returns star's color based on player's current position
		glm::vec3 getStarColor();

	private: /* variables */

		// Background wall
		Wall m_wall;
		// Player's character
		Player m_player;
		// List of floor pieces defining the walkable area of the map
		Floor m_floors[FLOORS_COUNT];

		Torch m_torches[TORCHES_COUNT];

#if GLEAMHOUSE_WITH_DEBUG_GRAPHICS
		// A small square to mark coordinate system's center
		Pekan::Renderer2D::RectangleShape m_centerSquare;
#endif

		Pekan::Renderer2D::Camera2D_Ptr m_camera;

		float m_distToStar = -1.0f;

		bool m_hasFinished = false;

		FinishedLevel_Scene* m_finishedLevelScene = nullptr;

		float t = 0.0f;
	};

} // namespace GleamHouse