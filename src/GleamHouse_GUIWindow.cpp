#include "GleamHouse_GUIWindow.h"

#include "PekanLogger.h"

using namespace Pekan;
using namespace Pekan::GUI;

namespace Demo
{

	bool GleamHouse_GUIWindow::init()
	{
		gui.textWidget_backgroundColor->create(this, "Background Color");
		gui.colorEditWidget->create(this, { 0.1f, 0.1f, 0.1f });

		return true;
	}

	glm::vec4 GleamHouse_GUIWindow::getBackgroundColor() const
	{
		PK_ASSERT_QUICK(gui.colorEditWidget != nullptr);
		return glm::vec4(gui.colorEditWidget->getValue(), 1.0f);
	}

	GUIWindowProperties GleamHouse_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 200, 120 };
		props.name = "GleamHouse";
		return props;
	}

} // namespace Demo