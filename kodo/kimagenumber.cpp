/*
 * Mouspedometa
 *      Based on the original Xodometer VMS/Motif sources.
 *
 * Written by Armen Nakashian
 *            Compaq Computer Corporation
 *            Houston TX
 *            22 May 1998
 *
 * If you make improvements or enhancements to Mouspedometa, please send
 * them back to the author at any of the following addresses:
 *
 *              armen@nakashian.com
 *
 * Thanks to Mark Granoff for writing the original Xodometer, and
 * the whole KDE team for making such a nice environment to write
 * programs in.
 *
 *
 * This software is provided as is with no warranty of any kind,
 * expressed or implied. Neither Digital Equipment Corporation nor
 * Armen Nakashian will be held accountable for your use of this
 * software.
 */

#include "kimagenumber.h"

KImageNumber::KImageNumber(const TQString& font, TQWidget* parent,const char* name) :
	TQFrame(parent,name),
	m_value(0)
{
	fontPix = new TQPixmap(font);
	resize(sizeHint());
}

KImageNumber::~KImageNumber()
{
	delete fontPix;
}

void KImageNumber::paintEvent(TQPaintEvent*)
{
	int w = fontPix->width();
	int each = w/11;

	TQString data;
	data.sprintf("%06.1f", m_value);

	for(unsigned int i=0; i < data.length(); i++) {
		int wl = data.tqat(i).latin1() - '0';
		if(data.at(i) == '.')
			wl = 10;
		bitBlt(this, i*each, 0, fontPix, wl*each, 0, each, fontPix->height());
	}
}

void KImageNumber::setValue(double v)
{
	m_value = v;
	tqrepaint(false);
}

double KImageNumber::value() const
{
	return m_value;
}

TQSize KImageNumber::sizeHint() const
{
	int w = fontPix->width();
	int each = w/11;

	TQString data;
	data.sprintf("%06.1f", m_value);

	return TQSize(data.length()*each, fontPix->height());
}

#include "kimagenumber.moc"
