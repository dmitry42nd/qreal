#include "interpreterTest.h"

#include <src/interpreter/interpreter.h>

#include <src/textLanguage/robotsBlockParser.h>

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore::interpreter;
using namespace ::testing;

void InterpreterTest::SetUp()
{
	mQrguiFacade.reset(new QrguiFacade("unittests/basicTest.qrs"));

	mQrguiFacade->setActiveTab(qReal::Id::loadFromString(
			"qrm:/RobotsMetamodel/RobotsDiagram/RobotsDiagramNode/{f08fa823-e187-4755-87ba-e4269ae4e798}"));

	DummyBlockFactory *blocksFactory = new DummyBlockFactory(
			mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mModelManager
			, *mQrguiFacade->mainWindowInterpretersInterface().errorReporter()
			);

	mFakeConnectToRobotAction.reset(new QAction(nullptr));

	ON_CALL(mConfigurationInterfaceMock, devices(_)).WillByDefault(
			Return(QList<interpreterBase::robotModel::robotParts::Device *>())
			);
	EXPECT_CALL(mConfigurationInterfaceMock, devices(_)).Times(AtLeast(1));


	ON_CALL(mModel, needsConnection()).WillByDefault(Return(false));
	EXPECT_CALL(mModel, needsConnection()).Times(AtLeast(1));

	ON_CALL(mModel, init()).WillByDefault(Return());
	EXPECT_CALL(mModel, init()).Times(AtLeast(1));

	ON_CALL(mModel, configuration()).WillByDefault(ReturnRef(mConfigurationInterfaceMock));
	EXPECT_CALL(mModel, configuration()).Times(AtLeast(0));

	ON_CALL(mModel, mutableConfiguration()).WillByDefault(ReturnRef(mConfigurationInterfaceMock));
	EXPECT_CALL(mModel, mutableConfiguration()).Times(AtLeast(0));

	ON_CALL(mModel, connectToRobot()).WillByDefault(
			Invoke(&mModelManager, &RobotModelManagerInterfaceMock::emitConnected)
			);
	EXPECT_CALL(mModel, connectToRobot()).Times(1);

	ON_CALL(mModel, disconnectFromRobot()).WillByDefault(
			Invoke(&mModelManager, &RobotModelManagerInterfaceMock::emitDisconnected)
			);
	EXPECT_CALL(mModel, disconnectFromRobot()).Times(AtLeast(0));

	ON_CALL(mModel, configurablePorts()).WillByDefault(Return(QList<interpreterBase::robotModel::PortInfo>()));
	EXPECT_CALL(mModel, configurablePorts()).Times(AtLeast(0));

	ON_CALL(mModel, applyConfiguration()).WillByDefault(
			Invoke(&mModelManager, &RobotModelManagerInterfaceMock::emitAllDevicesConfigured)
			);
	EXPECT_CALL(mModel, applyConfiguration()).Times(1);

	ON_CALL(mModel, connectionState()).WillByDefault(Return(RobotModelInterfaceMock::connectedState));
	EXPECT_CALL(mModel, connectionState()).Times(2);


	ON_CALL(mModelManager, model()).WillByDefault(ReturnRef(mModel));
	EXPECT_CALL(mModelManager, model()).Times(AtLeast(1));

	ON_CALL(mBlocksFactoryManager, addFactory(_)).WillByDefault(Return());
	EXPECT_CALL(mBlocksFactoryManager, addFactory(_)).Times(0);

	ON_CALL(mBlocksFactoryManager, block(_)).WillByDefault(
			Invoke([=] (qReal::Id const &id) { return blocksFactory->block(id); } )
			);
	EXPECT_CALL(mBlocksFactoryManager, block(_)).Times(AtLeast(0));

	ON_CALL(mBlocksFactoryManager, providedBlocks()).WillByDefault(
			Invoke([=] { return blocksFactory->providedBlocks(); } )
			);
	EXPECT_CALL(mBlocksFactoryManager, providedBlocks()).Times(0);

	/// @todo Don't like it.
	interpreterCore::textLanguage::RobotsBlockParser parser(
			mQrguiFacade->mainWindowInterpretersInterface().errorReporter()
			);

	mInterpreter.reset(new Interpreter(
			mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mQrguiFacade->mainWindowInterpretersInterface()
			, mQrguiFacade->projectManagementInterface()
			, mBlocksFactoryManager
			, mModelManager
			, parser
			, *mFakeConnectToRobotAction
			));
}

TEST_F(InterpreterTest, interpret)
{
	EXPECT_CALL(mModel, stopRobot()).Times(1);

	mInterpreter->interpret();
}

TEST_F(InterpreterTest, stopRobot)
{
	// It shall be called directly here and in destructor of a model.
	EXPECT_CALL(mModel, stopRobot()).Times(2);

	mInterpreter->interpret();
	mInterpreter->stopRobot();
}