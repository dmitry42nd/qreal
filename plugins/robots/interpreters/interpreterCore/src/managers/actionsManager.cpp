#include "interpreterCore/managers/actionsManager.h"

#include <QtCore/QSignalMapper>

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

static const qReal::Id robotDiagramType = qReal::Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
static const qReal::Id subprogramDiagramType = qReal::Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

ActionsManager::ActionsManager(KitPluginManager &kitPluginManager, RobotModelManager &robotModelManager)
	: mKitPluginManager(kitPluginManager)
	, mRobotModelManager(robotModelManager)
	, mRunAction(QIcon(":/icons/robots_run.png"), QObject::tr("Run"), nullptr)
	, mStopRobotAction(QIcon(":/icons/robots_stop.png"), QObject::tr("Stop robot"), nullptr)
	, mConnectToRobotAction(QIcon(":/icons/robots_connect.png"), QObject::tr("Connect to robot"), nullptr)
	, mRobotSettingsAction(QIcon(":/icons/robots_settings.png"), QObject::tr("Robot settings"), nullptr)
	, mSeparator1(nullptr)
	, mSeparator2(nullptr)
{
	initKitPluginActions();

	mConnectToRobotAction.setCheckable(true);

	mSeparator1.setSeparator(true);
	mSeparator2.setSeparator(true);

	mActions
			<< &mConnectToRobotAction
			<< &mRunAction
			<< &mStopRobotAction
			<< &mRobotSettingsAction
			;
}

QList<qReal::ActionInfo> ActionsManager::actions()
{
	QList<qReal::ActionInfo> result;

	result << mPluginActionInfos;

	result
			<< qReal::ActionInfo(&mConnectToRobotAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mRunAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mStopRobotAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mSeparator1, "interpreters", "tools");

	result += mRobotModelActions.values();

	result << qReal::ActionInfo(&mSeparator2, "interpreters", "tools")
			<< qReal::ActionInfo(&mRobotSettingsAction, "interpreters", "tools")
			;

	return result;
}

QList<qReal::HotKeyActionInfo> ActionsManager::hotKeyActionInfos()
{
	mStopRobotAction.setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	mRunAction.setShortcut(QKeySequence(Qt::Key_F5));

	QList<qReal::HotKeyActionInfo> result;

	result += mPluginHotKeyActionInfos;

	result
			<< qReal::HotKeyActionInfo("Interpreter.Run", QObject::tr("Run interpreter"), &mRunAction)
			<< qReal::HotKeyActionInfo("Interpreter.Stop", QObject::tr("Stop interpreter"), &mStopRobotAction)
			;

	return result;
}

QAction &ActionsManager::runAction()
{
	return mRunAction;
}

QAction &ActionsManager::stopRobotAction()
{
	return mStopRobotAction;
}

QAction &ActionsManager::connectToRobotAction()
{
	return mConnectToRobotAction;
}

void ActionsManager::init(qReal::gui::MainWindowInterpretersInterface *mainWindowInterpretersInterface)
{
	mMainWindowInterpretersInterface = mainWindowInterpretersInterface;

	updateEnabledActions();
}

QAction &ActionsManager::robotSettingsAction()
{
	return mRobotSettingsAction;
}

void ActionsManager::onRobotModelChanged(kitBase::robotModel::RobotModelInterface &model)
{
	mConnectToRobotAction.setVisible(model.needsConnection());
	const QString currentKitId = kitIdOf(model);
	const QString switchActionName = "switchTo" + currentKitId + model.name();

	/// @todo: this stupid visibility management may show actions with custom avalability logic.
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (const qReal::ActionInfo &actionInfo : mRobotModelActions.values(kitId)) {
			if (actionInfo.isAction()) {
				actionInfo.action()->setVisible(currentKitId == kitId);
				actionInfo.action()->setChecked(actionInfo.action()->objectName() == switchActionName);
			} else {
				actionInfo.menu()->setVisible(currentKitId == kitId);
			}
		}
	}
}

void ActionsManager::onActiveTabChanged(const qReal::Id &activeTabId)
{
	const bool isDiagramTab = !activeTabId.isNull();
	mRunAction.setEnabled(isDiagramTab);
	mStopRobotAction.setEnabled(isDiagramTab);
}

void ActionsManager::onRobotModelActionChecked(QObject *robotModelObject)
{
	const auto robotModel = dynamic_cast<kitBase::robotModel::RobotModelInterface *>(robotModelObject);
	mRobotModelManager.setModel(robotModel);
	onRobotModelChanged(*robotModel);
}

QString ActionsManager::kitIdOf(kitBase::robotModel::RobotModelInterface &model) const
{
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (kitBase::KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
			if (kit->robotModels().contains(&model)) {
				return kitId;
			}
		}
	}

	/// @todo: Impossible scenario, something wrong if we get here.
	return QString();
}

void ActionsManager::updateEnabledActions()
{
	const qReal::Id &rootElementId = mMainWindowInterpretersInterface->activeDiagram();
	const bool enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;

	for (QAction * const action : mActions) {
		action->setEnabled(enabled);
	}
}

void ActionsManager::initKitPluginActions()
{
	QSignalMapper * const robotModelMapper = new QSignalMapper(this);
	connect(robotModelMapper, SIGNAL(mapped(QObject*)), this, SLOT(onRobotModelActionChecked(QObject*)));

	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (kitBase::KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			mPluginActionInfos << kitPlugin->customActions();
			for (kitBase::robotModel::RobotModelInterface * const robotModel : kitPlugin->robotModels()) {
				const QIcon &icon = kitPlugin->iconForFastSelector(*robotModel);
				if (icon.isNull()) {
					continue;
				}

				const QString &text = tr("Switch to ") + robotModel->friendlyName();
				QAction * const fastSelectionAction = new QAction(icon, text, nullptr);
				robotModelMapper->setMapping(fastSelectionAction, robotModel);
				connect(fastSelectionAction, SIGNAL(triggered()), robotModelMapper, SLOT(map()));
				fastSelectionAction->setCheckable(true);
				fastSelectionAction->setObjectName("switchTo" + kitId + robotModel->name());
				const qReal::ActionInfo actionInfo(fastSelectionAction, "interpreters", "tools");
				mRobotModelActions.insertMulti(kitId, actionInfo);
			}

			mPluginHotKeyActionInfos << kitPlugin->hotKeyActions();
		}
	}
}
