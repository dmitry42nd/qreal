/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <trikKit/robotModel/parts/trikDisplay.h>
#include <utils/tcpRobotCommunicator.h>

#include "trikKitInterpreterCommon/declSpec.h"

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT Display : public robotModel::parts::TrikDisplay
{
	Q_OBJECT

public:
	Display(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void drawSmile(bool sad) override;
	/// @todo: color enum here?
	void setBackground(const QColor &color, bool redraw) override;
	void clearScreen(bool redraw) override;
	void printText(int x, int y, const QString &text, bool redraw) override;

	void setPainterColor(const QColor &color) override;
	void setPainterWidth(int penWidth) override;
	void drawPixel(int x, int y, bool redraw) override;
	void drawLine(int x1, int y1, int x2, int y2, bool redraw) override;
	void drawRect(int x, int y, int width, int height, bool redraw)  override;
	void drawEllipse(int x, int y, int width, int height, bool redraw) override;
	void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle, bool redraw)  override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
