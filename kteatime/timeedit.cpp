/* -------------------------------------------------------------

   timeedit.cpp

   (C) 2003 by Daniel Teske (teske@bigfoot.com)

 ------------------------------------------------------------- */

#include <klocale.h>
#include <tqlabel.h>
#include <tqlayout.h>

#include "timeedit.h"
#include "timeedit.moc"

WrappingSpinBox::WrappingSpinBox(int minValue, int maxValue, int step, TQWidget *parent, const char *name)
	: TQSpinBox(minValue, maxValue, step, parent, name)
{
}

WrappingSpinBox::~WrappingSpinBox()
{
}


/** Overloaded TQSpinBox method */
void WrappingSpinBox::stepUp()
{
	bool wrap = false;
	if (value() == 59)
		wrap = true;
	if (wrap)
		emit wrapUp();              // must wrap first (to avoid double-step-up)
	TQSpinBox::stepUp();
}

/** Overloaded TQSpinBox method */
void WrappingSpinBox::stepDown()
{
	bool wrap = false;
	if (value() == 0)
		wrap = true;
	TQSpinBox::stepDown();
	if (wrap)
		emit wrapDown();
}


// -------------------------------------------------------------------------


TimeEdit::TimeEdit(TQWidget* parent, const char* name)
    : TQWidget(parent, name)
{
	tqlayout = new TQHBoxLayout(this);
	minuteBox = new TQSpinBox(0, 300, 1, this);
//	minuteBox->setFixedSize(minuteBox->sizeHint());

	TQLabel* min = new TQLabel(i18n(" min"), this);
	min->setFixedSize(min->sizeHint());
	secondBox = new WrappingSpinBox(0, 59, 1, this);
	secondBox->setWrapping(true);
//	secondBox->setFixedSize(secondBox->sizeHint());

	TQLabel* sec = new TQLabel(i18n(" sec"),this);
	sec->setFixedSize(sec->sizeHint());

	tqlayout->addWidget(minuteBox);
	tqlayout->addWidget(min);

	tqlayout->addWidget(secondBox);
	tqlayout->addWidget(sec);

	connect(minuteBox, TQT_SIGNAL(valueChanged(int)), TQT_SLOT(spinBoxValueChanged(int)) );
	connect(secondBox, TQT_SIGNAL(valueChanged(int)), TQT_SLOT(spinBoxValueChanged(int)) );
	connect(secondBox, TQT_SIGNAL(wrapUp()), TQT_SLOT(wrappedUp()));
	connect(secondBox, TQT_SIGNAL(wrapDown()), TQT_SLOT(wrappedDown()));
}

TimeEdit::~TimeEdit()
{
}

/** Set to specified number of seconds. */
void TimeEdit::setValue(int val)
{
	if (val < 0)
		return;

	// block signals to avoid receiption of valueChanged()
	// between changing of minutes and seconds
	secondBox->blockSignals(true);
	minuteBox->blockSignals(true);

	secondBox->setValue(val % 60);
	minuteBox->setValue(val / 60);

	secondBox->blockSignals(false);
	minuteBox->blockSignals(false);

	emit valueChanged(value());
}

/** Return current value in seconds. */
int TimeEdit::value()
{
	return minuteBox->value()*60 + secondBox->value();
}

/** SLOT: Handle wrap-up of seconds-box */
void TimeEdit::wrappedUp()
{
	if (minuteBox->value() != minuteBox->maxValue()) {
		minuteBox->stepUp();
	} else {
		secondBox->setValue(58);    // ugly: must cater for wrapping-first
	}
}

/** SLOT: Handle wrap-down of seconds-box */
void TimeEdit::wrappedDown()
{
	// well, the "if" should always be true
	if (minuteBox->value() != minuteBox->minValue()) {
		minuteBox->stepDown();
	} else {
		secondBox->setValue(0);
	}
}

/** SLOT: Handle any change in minutes of seconds */
void TimeEdit::spinBoxValueChanged(int)
{
	if (value() == 0) {
		secondBox->stepUp();        // this will give another spinBoxValueChanged() invocation
		return;
	}

	emit valueChanged(value());
}

/** TQT_SLOT (overloading TQSpinBox): set focus */
void TimeEdit::setFocus()
{
	minuteBox->setFocus();
}
