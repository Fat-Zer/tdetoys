/*
 *   kmoon - a moon phase indicator
 *   $Id$
 *   Copyright (C) 1998,2000  Stephan Kulow
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <tqbitmap.h>
#include <tqtooltip.h>
#include <tqpainter.h>
#include <tqpopupmenu.h>
#include <tqcolor.h>

#include <dcopclient.h>
#include <kdebug.h>
#include <kapplication.h>
#include <kwin.h>
#include <kstartupinfo.h>
#include <kmessagebox.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kcmdlineargs.h>
#include <kiconloader.h>
#include <kiconeffect.h>
#include <kconfig.h>

#include "kmoondlg.h"
#include "kmoonwidget.h"
#include <math.h>

extern double moonphasebylunation(int lun, int phi);
extern time_t JDtoDate(double jd, struct tm *event_date);

MoonWidget::MoonWidget(TQWidget *tqparent, const char *name)
  : TQWidget(tqparent, name)
{
    struct tm * t;
    time_t clock;

    counter = -1;
    KConfig *config = KGlobal::config();
    config->setGroup("General");
    _angle = config->readNumEntry("Rotation", 0);
    _north = config->readBoolEntry("Northern", true);
    _tqmask = config->readBoolEntry("Mask", true);
    old_angle = old_w = old_h = old_counter = -1;
    old_north = false;
    old_tqmask = false;
    startTimer(1000 * 60 * 20);

    time(&clock);
    t = gmtime(&clock);
    // kdDebug() << "time " << t->tm_isdst << " " << timezone << " " << daylight << endl ;
    calctqStatus(mktime(t));
}

MoonWidget::~MoonWidget()
{
}

void MoonWidget::calctqStatus( time_t time )
{
    uint lun = 0;
    time_t last_new = 0;
    time_t next_new = 0;

    do {
        double JDE = moonphasebylunation(lun, 0);
        last_new = next_new;
        next_new = JDtoDate(JDE, 0);
        lun++;
    } while (next_new < time);

    lun -= 2;

    TQDateTime ln;
    ln.setTime_t( last_new );
    kdDebug() << KGlobal::locale()->formatDateTime( ln ) << endl;

    time_t first_quarter = JDtoDate( moonphasebylunation( lun, 1 ), 0 );
    TQDateTime fq;
    fq.setTime_t( first_quarter );
    kdDebug() << KGlobal::locale()->formatDateTime( fq ) << endl;

    time_t full_moon = JDtoDate( moonphasebylunation( lun, 2 ), 0 );
    TQDateTime fm;
    fm.setTime_t( full_moon );
    kdDebug() << KGlobal::locale()->formatDateTime( fm ) << endl;

    time_t third_quarter = JDtoDate( moonphasebylunation( lun, 3 ), 0 );
    TQDateTime tq;
    tq.setTime_t( third_quarter );
    kdDebug() << KGlobal::locale()->formatDateTime( tq ) << endl;

    TQDateTime nn;
    nn.setTime_t( next_new );
    kdDebug() << KGlobal::locale()->formatDateTime( nn ) << endl;

    TQDateTime now;
    now.setTime_t( time );
    kdDebug() << KGlobal::locale()->formatDateTime( now ) << endl;

    counter = ln.daysTo( now );
    kdDebug() << "counter " << counter << " " << fm.daysTo( now ) << endl;

    if ( fm.daysTo( now ) == 0 ) {
        counter = 14;
        tooltip = i18n( "Full Moon" );
        return;
    } else if ( counter <= 15 && counter >= 13 ) {
        counter = 14 + fm.daysTo( now );
        kdDebug() << "around full moon " << counter << endl;
    }

    int diff = fq.daysTo( now );
    if ( diff  == 0 )
        counter = 7;
    else if ( counter <= 8 && counter >= 6 ) {
        counter = 7 + diff;
         kdDebug() << "around first quarter " << counter << endl;
    }

    diff = ln.daysTo( now );
    if ( diff == 0 )
        counter = 0;
    else if ( counter <= 1 || counter >= 28 )
    {
        counter = ( 29 + diff ) % 29;
        diff = -nn.daysTo( now );
        if ( diff == 0 )
            counter = 0;
        else if ( diff < 3 )
            counter = 29 - diff;
        kdDebug() << "around new " << counter << " " << diff << endl;
    }

    if ( tq.daysTo( now ) == 0 )
        counter = 21;
    else if ( counter <= 22 && counter >= 20 )
    {
        counter = 21 + tq.daysTo( now );
        kdDebug() << "around third quarter " << counter << endl;
    }

    kdDebug() << "counter " << counter << endl;

    assert (counter >= 0 && counter < 29);

    switch (counter) {
    case 0:
        tooltip = i18n("New Moon");
        return;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        tooltip = i18n("Waxing Crescent (New Moon was yesterday)", "Waxing Crescent (%n days since New Moon)", counter );
        break;
    case 7:
        tooltip = i18n("First Quarter");
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        tooltip = i18n( "Waxing Gibbous (Tomorrow is Full Moon)", "Waxing Gibbous (%n days to Full Moon)", -fm.daysTo( now ) );
        break;
    case 14:
        assert( false );
        break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        tooltip = i18n("Waning Gibbous (Yesterday was Full Moon)", "Waning Gibbous (%n days since Full Moon)", fm.daysTo( now ) );
        break;
    case 21:
        tooltip = i18n("Last Quarter");
        break;
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
        kdDebug() << "nn.days " << ln.daysTo( now ) << " " << nn.daysTo( now ) << endl;
        tooltip = i18n("Waning Crescent (Tomorrow is New Moon)", "Waning Crescent (%n days to New Moon)", -nn.daysTo( now ) );
        break;
    default:
        kdFatal() << "coolo can't count\n";
    }
    
    renderGraphic();
    tqrepaint();
}

TQImage MoonWidget::loadMoon(int index)
{
    if (index == 0) // the new moon has the wrong filename
        index = 29;
    TQString filename = TQString("kmoon/pics/moon%1.png").tqarg(index);
    TQString path = locate("data", filename);
    if (path.isNull())
        kdFatal() << "cound't tqfind " << filename << ". Exiting.\n";
    TQImage image(path);
    KIconEffect iconeffect;
    image=iconeffect.apply(image, KIcon::Panel, KIcon::DefaultState);
    return image;
}

void MoonWidget::setAngle(int value)
{
    _angle = value;
    renderGraphic();
    tqrepaint();
}

void MoonWidget::setNorthHemi(bool n)
{
    _north = n;
    renderGraphic();
    tqrepaint();
}

void MoonWidget::setMask(bool value)
{
    _tqmask = value;
    renderGraphic();
    tqrepaint();
}

void MoonWidget::paintEvent(TQPaintEvent *)
{
    bitBlt(this, 0, 0, &pixmap, 0, 0);
}

void MoonWidget::resizeEvent(TQResizeEvent *)
{
    renderGraphic();
    tqrepaint();
}

void MoonWidget::renderGraphic()
{
  if (old_counter == counter && old_w == width() && old_h == height() && 
      old_angle == _angle && old_north == _north)
    return;
  old_counter = counter;
  old_w = width();
  old_h = height();
  old_north = _north;
  TQImage im = loadMoon(counter);
  assert(!im.isNull());
  im = im.convertDepth(32, 0);
  assert(!im.isNull());

  int mw = TQMIN(width(), height());
  TQImage dest;

  if (TQPixmap::defaultDepth() > 8) {

    if (fabs(_angle)!=0) { // nothing to rotate
      //We expand the image 2x before rotating, rotate it, and then average out
      //the pixel for better quality
      int dmw = 2 * mw;
      if (!pixmap.convertFromImage(im.smoothScale(dmw, dmw), 0)) {
	return;
      }
      TQWMatrix m;
      m.rotate(_angle);
      TQPixmap rotated = pixmap.xForm(m);
      
      //Copy the relevant part back to the pixmap
      TQRegion r(TQRect(0, 0, dmw, dmw), TQRegion::Ellipse);
      TQPainter p;
      p.begin(&pixmap);
      p.fillRect(0, 0, dmw, dmw, TQt::black);
      p.setClipRegion(r);
      p.drawPixmap(0, 0, rotated, (rotated.width() - dmw) / 2, 
		   (rotated.height() - dmw) / 2,
		   dmw, dmw);
      p.end();
      
      //Shrink down to the proper size, averaging from the source
      im = pixmap.convertToImage();
      dest = im.copy(0, 0, mw, mw);
      for (int y = 0; y < mw; y++) {
	TQRgb *destline = (TQRgb*)dest.scanLine(y);
	TQRgb *sourceline1 = (TQRgb*)im.scanLine(2*y);
	TQRgb *sourceline2 = (TQRgb*)im.scanLine(2*y + 1);
	for (int x = 0; x < mw; x++) {
	  int r = tqRed(sourceline1[2*x]) + tqRed(sourceline1[2*x+1]);
	  r = r + tqRed(sourceline2[2*x]) + tqRed(sourceline2[2*x+1]);
	  int g = tqGreen(sourceline1[2*x]) + tqGreen(sourceline1[2*x+1]);
	  g = g + tqGreen(sourceline2[2*x]) + tqGreen(sourceline2[2*x+1]);
	  int b = tqBlue(sourceline1[2*x]) + tqBlue(sourceline1[2*x+1]);
	  b = b + tqBlue(sourceline2[2*x]) + tqBlue(sourceline2[2*x+1]);
	  destline[x] = tqRgb(tqRound(r / 4), tqRound(g / 4),
			     tqRound(b / 4));
	}
      }
    } else {
      dest = im.smoothScale(mw, mw).convertDepth(32);
    }
    if (_tqmask) {
      // generate alpha-channel
      int dmw = mw*2;
      TQBitmap dMask(dmw, dmw);
      TQRegion r(TQRect(0, 0, dmw, dmw), TQRegion::Ellipse);
      TQPainter p;
      p.begin(&dMask);
      p.fillRect(0, 0, dmw, dmw, TQt::white);
      p.setClipRegion(r);
      p.fillRect(0, 0, dmw, dmw, TQt::black);
      p.end();
      TQImage Mask2 = TQImage(dMask.convertToImage()).convertDepth(32).smoothScale(mw, mw);
      dest.setAlphaBuffer(true);
      for (int y = 0; y < mw; y++) {
	TQRgb *destline = (TQRgb*)dest.scanLine(y);
	TQRgb *sourceline = (TQRgb*)Mask2.scanLine(y);
	for (int x = 0; x < mw; x++) {
	  destline[x] = (destline[x] & TQRGB_MASK)|(tqRed(sourceline[x]) << 24);
	}
      }
    }
  } else {
    dest = im.smoothScale(mw, mw);
  }

  if (!_north)
    dest = dest.mirror(true, true);

  if (!pixmap.convertFromImage(dest, 0)) {
    return;
  }
    
  TQToolTip::remove(this);

  TQToolTip::add(this, tooltip);
}


#include "kmoonwidget.moc"
