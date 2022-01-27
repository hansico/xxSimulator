/***************************************************************************
**                                                                        **
** xxSimulator, "turn-based game-style" evolution simulator               **
** Copyright (C) 2022  hansico                                            **
**                                                                        **
** This program is free software: you can redistribute it and/or modify   **
** it under the terms of the GNU General Public License as published by   **
** the Free Software Foundation, either version 3 of the License, or      **
** (at your option) any later version.                                    **
**                                                                        **
** This program is distributed in the hope that it will be useful,        **
** but WITHOUT ANY WARRANTY; without even the implied warranty of         **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          **
** GNU General Public License for more details.                           **
**                                                                        **
** You should have received a copy of the GNU General Public License      **
** along with this program.  If not, see <https://www.gnu.org/licenses/>. **
****************************************************************************
** Author: hansico                                                        **
** Contact: hansico@outlook.com                                           **
** Date: 27.01.2022                                                       **
** Version: 0.01                                                          **
****************************************************************************/
#ifndef IMAGERENDERER_H
#define IMAGERENDERER_H

#include <QWidget>
#include <QPen>
#include <QImage>
#include <QGraphicsView>
#include <QLabel>
#include "simumap.h"
#include "creatureBase.h"

class ImageRenderer: public QObject
{
  Q_OBJECT
public:
  ImageRenderer(int sizex=0, int sizey=0, QSize labelsize={});
  ~ImageRenderer();
  void fillHexagon(int index, int color=2);
  void drawBearing(int index, int direction);  
  void saveImageToFile();

public slots:
  void drawCreature(int location, int facingDirection,
                    const std::vector<int> &sightIndices={},
                    const std::vector<int> &hearingIndices={});
  void drawPlant(int location, int color=2);
  void clean();
  void show();

signals:
  void imageUpdate(QPixmap &pixmap);

private:
  QImage img;
  QPixmap labelimg;
  QSize labelSize;
  int size_x;
  int size_y;
  void draw_hexamap();
  std::pair<int,int> calcoffset(int index);

};

#endif // IMAGERENDERER_H
