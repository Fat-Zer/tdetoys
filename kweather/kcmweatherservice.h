/*                                                                      
    This file is part of KWeather.                                  
    Copyright (c) 2003 Tobias Koenig <tokoe@kde.org>
                                                                        
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or   
    (at your option) any later version.                                 
                                                                        
    This program is distributed in the hope that it will be useful,     
    but WITHOUT ANY WARRANTY; without even the implied warranty of      
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        
    GNU General Public License for more details.                        
                                                                        
    You should have received a copy of the GNU General Public License   
    along with this program; if not, write to the Free Software         
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.           
                                                                        
    As a special exception, permission is given to link this program    
    with any edition of TQt, and distribute the resulting executable,    
    without including the source code for TQt in the source distribution.
*/                                                                      

#ifndef KCMWEATHERSERVICE_H
#define KCMWEATHERSERVICE_H

#include <tdecmodule.h>

class ServiceConfigWidget;

class KCMWeatherService : public TDECModule
{
  Q_OBJECT
  

  public:
    KCMWeatherService( TQWidget *parent = 0, const char *name = 0 );

    virtual void load();
    virtual void save();
    virtual void defaults();

  private:
    ServiceConfigWidget *mWidget;
};

#endif
