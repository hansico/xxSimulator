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
#include "imagerenderer.h"
#include <QPainter>
#include <iostream>
#include <QException>

ImageRenderer::ImageRenderer(int sizex, int sizey, QSize labelsize)
{
  size_x = sizex;
  size_y = sizey;
  labelSize = labelsize;

  labelimg = QPixmap(labelsize);

  int imgsize_y = 10+(sizey-1)*7;
  int imgsize_x = 1+sizex*8;

  img = QImage(imgsize_x,imgsize_y,QImage::Format_Indexed8);

  QRgb value;

  value = qRgb(0,0,0);
  img.setColor(1,value);

  value = qRgb(255,255,255);
  img.setColor(0,value);
  value = qRgb(0,25,255);
  img.setColor(2,value);
  value = qRgb(255,0,0);
  img.setColor(3,value);
  value = qRgb(95,205,209);
  img.setColor(4,value);
  value = qRgb(115,220,115);
  img.setColor(5,value);
  value = qRgb(255,165,0);
  img.setColor(6,value);
}

ImageRenderer::~ImageRenderer()
{

}

void ImageRenderer::fillHexagon(int index, int color)
{  
  int fpx,fpy;
  std::pair<int,int> offsets = calcoffset(index);
  fpx = offsets.first;
  fpy = offsets.second;
  for(int i=0;i<5;++i){
    for(int p=0;p<6;++p)
      img.setPixel(fpx+i,fpy+p,color);
  }
  for(int i=0;i<4;++i){
    img.setPixel(fpx-1,fpy+1+i,color);
    img.setPixel(fpx+5,fpy+1+i,color);
  }
  img.setPixel(fpx+2,fpy-1,color);
  img.setPixel(fpx+2,fpy+6,color);
  //show();
}

void ImageRenderer::drawBearing(int index, int direction)
{
  int offx, offy = 0;
  //std::pair<int,int> offsets {offx,offy};
  std::pair<int,int> offsets = calcoffset(index);

  offx = offsets.first;
  offy = offsets.second;

  switch(direction){

  case SimuMap::bearings::EAST:
    for(int i=0;i<2;++i){
      img.setPixel(offx+2+i,offy+1,1);
      img.setPixel(offx+2+i,offy+4,1);
      img.setPixel(offx+4,offy+2+i,1);
    }
    break;

  case SimuMap::bearings::N_EAST:
    for(int i=0;i<2;++i){
      img.setPixel(offx+1+i,offy+1,1);
      img.setPixel(offx+3,offy+2+i,1);
    }
    img.setPixel(offx+3,offy+1,1);
    break;

  case SimuMap::bearings::N_WEST:
    for(int i=0;i<2;++i){
      img.setPixel(offx+2+i,offy+1,1);
      img.setPixel(offx+1,offy+2+i,1);
    }
    img.setPixel(offx+1,offy+1,1);
    break;

  case SimuMap::bearings::WEST:
    for(int i=0;i<2;++i){
      img.setPixel(offx+1+i,offy+1,1);
      img.setPixel(offx+1+i,offy+4,1);
      img.setPixel(offx+0,offy+2+i,1);
    }
    break;

  case SimuMap::bearings::S_WEST:
    for(int i=0;i<2;++i){
      img.setPixel(offx+2+i,offy+4,1);
      img.setPixel(offx+1,offy+2+i,1);
    }
    img.setPixel(offx+1,offy+4,1);
    break;

  case SimuMap::bearings::S_EAST:
    for(int i=0;i<2;++i){
      img.setPixel(offx+1+i,offy+4,1);
      img.setPixel(offx+3,offy+2+i,1);
    }
    img.setPixel(offx+3,offy+4,1);
    break;

  default:
    // TODO
    std::cout << "Unrecognized bearing: " << direction << '\n';
  }
}

void ImageRenderer::drawCreature(int location, int facingDirection, const std::vector<int> &sightIndices, const std::vector<int> &hearingIndices)
{
  fillHexagon(location);
  drawBearing(location, facingDirection);

  for(const auto &s: sightIndices)
    fillHexagon(s, 4);

  for(const auto &h: hearingIndices)
    fillHexagon(h, 5);
}

void ImageRenderer::drawPlant(int location, int color)
{
  fillHexagon(location, color);
}

void ImageRenderer::draw_hexamap()
{
  QSize s = img.size();
  int w = s.width();
  int h = s.height();

  for(int y=1;y<h;y+=7){
    for(int x=1;x<w;x+=4){
      img.setPixel(x,y,1);
      img.setPixel(x+1,y,1);
      img.setPixel(x+2,y,1);
    }
  }
  for(int y=0;y<h;y+=14){
    for(int x=3;x<w;x+=8){
      img.setPixel(x,y,1);
      img.setPixel(x+1,y,1);
      img.setPixel(x+2,y,1);
    }
  }
  for(int y=9;y<h;y+=14){
    for(int x=3;x<w;x+=8){
      img.setPixel(x,y,1);
      img.setPixel(x+1,y,1);
      img.setPixel(x+2,y,1);
    }
  }
  for(int y=2;y<h;y+=14){
    for(int x = 0;x<w;x+=8){
      img.setPixel(x-1,y,1);
      img.setPixel(x,y,1);
      img.setPixel(x+1,y,1);
    }
  }
  for(int y=7;y<h;y+=14){
    for(int x = 0;x<w;x+=8){
      img.setPixel(x-1,y,1);
      img.setPixel(x,y,1);
      img.setPixel(x+1,y,1);
    }
  }
  for(int y=3;y<h;y+=14){
    for(int x=0;x<w;x+=8){
      img.setPixel(x,y,1);
      img.setPixel(x,y+1,1);
      img.setPixel(x,y+2,1);
      img.setPixel(x,y+3,1);
    }
  }
  for(int y=10;y<h;y+=14){
    for(int x=4;x<w;x+=8){
      img.setPixel(x,y,1);
      img.setPixel(x,y+1,1);
      img.setPixel(x,y+2,1);
      img.setPixel(x,y+3,1);
    }
  }
}

std::pair<int,int> ImageRenderer::calcoffset(int index)
{
  int offx, offy = 0;
  int r = index%(2*size_x-1);

  // short row
  if(r>=size_x)
  {
    offx  = 6+(r-size_x)*8;
    offy = 9+int(index/(2*size_x-1))*14;
  }else{ // long row
    offx  = 2+r*8;
    offy = 2+int(index/(2*size_x-1))*14;
  }
  return {offx,offy};
}

void ImageRenderer::clean()
{
  img.fill(0);
  draw_hexamap();
  int outcolor = 3;
  int pixels_x = img.size().width()-1;
  int pixels_y = img.size().height()-1;
  int s_rows = size_y/2;
  // OUTSKIRTS; horrible, should clean // TODO
  for(int i=0;i<3;++i){
    img.setPixel(0+i,0,outcolor);
    img.setPixel(pixels_x-i,0,outcolor);
  }
  for(int i=0;i<size_x;++i)
  {
    for(int u=0;u<5;++u)
      img.setPixel(6+u+i*8,0,outcolor);
    img.setPixel(0+i*8,1,outcolor);
  }
  img.setPixel(pixels_x,1,outcolor);
  for(int i=0;i<s_rows;++i){
    for(int p=0;p<8;++p){
      img.setPixel(0,8+14*i+p,outcolor);
      img.setPixel(pixels_x,8+14*i+p,outcolor);
    }
    for(int p=0;p<6;++p){
      img.setPixel(1,9+14*i+p,outcolor);
      img.setPixel(pixels_x-1,9+14*i+p,outcolor);
      img.setPixel(2,9+14*i+p,outcolor);
      img.setPixel(pixels_x-2,9+14*i+p,outcolor);
    }
    for(int p=0;p<4;++p){
      img.setPixel(3,10+14*i+p,outcolor);
      img.setPixel(pixels_x-3,10+14*i+p,outcolor);
    }
  }
  if(size_y%2==0){
    // ends on short row
    for(int i=0;i<7;++i){
      img.setPixel(i,pixels_y,outcolor);
      img.setPixel(pixels_x-i,pixels_y,outcolor);
    }
    for(int i=0;i<4;++i){
      img.setPixel(1+i,pixels_y-1,outcolor);
      img.setPixel(pixels_x-i-1,pixels_y-1,outcolor);
    }
    img.setPixel(3,pixels_y-2,outcolor);
    img.setPixel(pixels_x-3,pixels_y-2,outcolor);
    for(int i=0;i<(size_x-1);++i)
    {
      for(int u=0;u<5;++u)
        img.setPixel(10+u+i*8,pixels_y,outcolor);
      img.setPixel(12+i*8,pixels_y-1,outcolor);
    }
  }else{
    for(int i=0;i<3;++i){
      img.setPixel(0+i,pixels_y,outcolor);
      img.setPixel(pixels_x-i,pixels_y,outcolor);
    }
    for(int i=0;i<size_x;++i)
    {
      for(int u=0;u<5;++u)
        img.setPixel(6+u+i*8,pixels_y,outcolor);
      img.setPixel(0+i*8,pixels_y-1,outcolor);
    }
    img.setPixel(pixels_x,pixels_y-1,outcolor);
  }
}

void ImageRenderer::saveImageToFile()
{
  bool success = img.save("img01.png","PNG",75);
  if(success){std::cout << "IMAGE was saved.\n";}
  if(!success){std::cout << "IMAGE was not saved.\n";}
}

void ImageRenderer::show()
{
  labelimg = QPixmap::fromImage(img.scaled(labelSize));
  emit imageUpdate(labelimg);
}
