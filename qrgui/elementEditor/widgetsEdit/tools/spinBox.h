#pragma once

#include <QtGui/QSpinBox>

#include "abstractSpinBox.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class SpinBox : public AbstractSpinBox
{
	Q_OBJECT

public:
	SpinBox(ToolController *controller);

private slots:
	void valueChanged(int i);

private:
	QSpinBox *mSpinBox;

};

}
}