#pragma once

#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include "nxtInputDevice.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class LightSensor : public interpreterBase::robotModel::robotParts::LightSensor
{
	Q_OBJECT

public:
	LightSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;
	void doConfiguration() override;

private slots:
	void sensorSpecificProcessResponse(const QByteArray &reading);

private:
	NxtInputDevice mImplementation;
};

}
}
}
}
