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

#include "generator.h"

#include <QtCore/QDir>

#include "concreteGenerator.h"
#include "generatorUtils/defs.h"

using namespace constraints::generator;

Generator::Generator()
{
}

Generator::~Generator()
{
}

void Generator::init(
		const qReal::LogicalModelAssistInterface &logicalModel
		, qReal::ErrorReporterInterface &errorReporter)
{
	mLogicalModel = &logicalModel;
	mErrorReporter = &errorReporter;
}

bool Generator::isCorrectName(const QString &name) const
{
	QRegExp pattern;
	pattern.setPattern("[A-Za-z]+([A-Za-z_0-9]*)");

	return (pattern.exactMatch(name));
}

void Generator::generate(const qReal::Id &metamodel)
{
	mErrorReporter->clear();
	mErrorReporter->clearErrors();

	mBuildConfiguration = mLogicalModel->propertyByRoleName(metamodel, "buildConfiguration").toString();

	QString outputDirPath = mLogicalModel->propertyByRoleName(metamodel, "outputDirPath").toString();

	if (outputDirPath.isEmpty()) {
		mErrorReporter->addCritical(tr("OutputDirPath of constraints model not found"), metamodel);
		return;
	}

	qDebug() << "generate : " << outputDirPath;

	const QString constraintMetamodelName = mLogicalModel->logicalRepoApi().name(metamodel);

	if (constraintMetamodelName.isEmpty() || (constraintMetamodelName.compare("(Metamodel Constraints)") == 0) ) {
		mErrorReporter->addCritical(tr("Name of constraints model not found.\n"), metamodel);
		return;
	} else if (!isCorrectName(constraintMetamodelName)) {
		mErrorReporter->addCritical(tr("Name of constraints model is not correct.\n"), metamodel);
		return;
	}

	QString metamodelName = mLogicalModel->propertyByRoleName(metamodel, "metamodelName").toString();

	if (metamodelName.isEmpty()) {
		mErrorReporter->addCritical(tr("MetamodelName of constraints model not found"), metamodel);
		return;
	}

	if ((metamodelName.compare("all", Qt::CaseInsensitive) == 0)
			|| (metamodelName.compare(keywordForAllMetamodels, Qt::CaseInsensitive) == 0))
	{
		metamodelName = keywordForAllMetamodels;
	}

	const QString pathToQReal = mLogicalModel->propertyByRoleName(metamodel, "pathToQReal").toString();

	if (pathToQReal.isEmpty()) {
		mErrorReporter->addCritical(tr("PathToQReal of constraints model not found"), metamodel);
		return;
	}

	outputDirPath.replace("\\", "/");

	ConcreteGenerator generator(templateDir, outputDirPath
			, pathToQReal, *mLogicalModel, *mErrorReporter
			, metamodelName, constraintMetamodelName);

	generator.generate();

	mConstraintModelFullName = generator.constraintModelFullName();
	mConstraintConstraintsModelName = generator.constraintConstraintsModelName();
	mConstraintNormalizerConstraintsModelName = generator.normalizedConstraintConstraintsModelName();
	mConstraintModelId = generator.constraintModelId();
}

QString Generator::constraintModelFullName() const
{
	return mConstraintModelFullName;
}

QString Generator::constraintConstraintsModelName() const
{
	return mConstraintConstraintsModelName;
}

QString Generator::constraintNormalizedConstraintsModelName() const
{
	return mConstraintNormalizerConstraintsModelName;
}

QString Generator::constraintModelId() const
{
	return mConstraintModelId;
}

QString Generator::buildConfiguration() const
{
	return mBuildConfiguration;
}