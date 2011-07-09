/*
 *   kmoon - a moon phase indicator
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

#ifndef KMOONDLG
#define KMOONDLG

#include <kdialogbase.h>

class TQSlider;
class MoonWidget;
class TQPushButton;

class KMoonDlg : public KDialogBase {
  Q_OBJECT
  TQ_OBJECT
    
 public:
  KMoonDlg(int angle, bool north, bool tqmask, TQWidget *tqparent, const char *name);
  int getAngle() const { return angle; }
  bool getNorthHemi() const { return north; }
  bool getMask() const { return tqmask; }
  
 private:
  TQSlider *slider;
  MoonWidget *moon;
  int angle;
  bool north;
  bool tqmask;
  TQPushButton *hemitoggle;
  TQPushButton *masktoggle;
  
 private slots:
  void help();
  void angleChanged(int);
  void toggleHemi();
  void toggleMask();
};

#endif
