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

#include <QtCore/QScopedPointer>

#include <kitBase/kitPluginInterface.h>
#include <twoDModel/engine/twoDModelControlInterface.h>
#include <trikKit/blocks/trikV62BlocksFactory.h>
#include <trikKitInterpreterCommon/trikKitInterpreterPluginBase.h>

#include "trikV62AdditionalPreferences.h"
//#include "robotModel/real/trikV62RealRobotModel.h"
//#include "robotModel/twoD/trikV62TwoDRobotModel.h"

namespace trik {

class TrikV62KitInterpreterPlugin : public TrikKitInterpreterPluginBase
{
	Q_OBJECT
	Q_INTERFACES(kitBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "trik.TrikV62KitInterpreterPlugin")

public:
	TrikV62KitInterpreterPlugin();
//	~TrikV62KitInterpreterPlugin() override;

//	void init(const kitBase::KitPluginConfigurator &configurator) override;

//	QString kitId() const override;
//	QString friendlyKitName() const override;

//	QList<kitBase::robotModel::RobotModelInterface *> robotModels() override;
//	kitBase::robotModel::RobotModelInterface *defaultRobotModel() override;

//	kitBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
//			const kitBase::robotModel::RobotModelInterface *model) override;

//	// Transfers ownership.
//	QList<kitBase::AdditionalPreferences *> settingsWidgets() override;
//	QWidget *quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model) override;
//	QString defaultSettingsFile() const override;
//	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;
//	kitBase::DevicesConfigurationProvider * devicesConfigurationProvider() override;

//	QList<qReal::ActionInfo> customActions() override;
//	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

//private slots:
//	QWidget *produceIpAddressConfigurer();  // Transfers ownership

private:
//	TrikKitInterpreterPluginBase mPluginImpl;
//	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModel;
//	robotModel::real::RealRobotModel mRealRobotModel;
//	robotModel::twoD::TwoDRobotModel mTwoDRobotModel;

//	/// @todo Use shared pointers instead of this sh~.
//	blocks::TrikV62BlocksFactory *mBlocksFactory = nullptr;  // Transfers ownership
//	bool mOwnsBlocksFactory = true;

//	TrikAdditionalPreferences *mAdditionalPreferences = nullptr;  // Transfers ownership
//	bool mOwnsAdditionalPreferences = true;

//	kitBase::InterpreterControlInterface *mInterpreterControl;  // Does not have ownership.
//	QString mCurrentlySelectedModelName;
};

}
