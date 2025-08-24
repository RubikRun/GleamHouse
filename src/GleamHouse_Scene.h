#pragma once

#include "Floor.h"
#include "Player.h"

#include "Layer.h"
#include "RectangleShape.h"
#include "Camera2D.h"

namespace GleamHouse
{

	class GleamHouse_Scene : public Pekan::Layer
	{
	public:

		GleamHouse_Scene(Pekan::PekanApplication* application) : Layer(application) {}

		bool init() override;

		void update(double deltaTime) override;

		void render() const override;

		void exit() override;

		inline std::string getLayerName() const override { return "scene_layer"; }

	private: /* functions */

		void createCamera();
		void updateCamera();

	private: /* variables */

		Floor m_floor;

		Player m_player;

#if GLEAMHOUSE_WITH_DEBUG_GRAPHICS
		// A small square to mark coordinate system's center
		Pekan::Renderer2D::RectangleShape m_centerSquare;
#endif

		Pekan::Renderer2D::Camera2D_Ptr m_camera;
	};

} // namespace GleamHouse