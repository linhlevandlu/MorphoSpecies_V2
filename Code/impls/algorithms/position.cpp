/*
 Image processing for morphometrics (IPM)
 Copyright (C) 2014  Nguyen Hoang Thao (hoangthao.ng@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "position.h"

namespace algorithms {

Position::Position(int r,int c)
{
	row=r;
	col=c;
}

int Position::operator==(const Position &rhs) const
{
   if( this->row != rhs.row) return 0;
   if( this->col != rhs.col) return 0;
   return 1;
}

// This function is required for built-in STL list functions like sort
int Position::operator<(const Position &rhs) const
{
   if( this->row == rhs.row && this->col < rhs.col) return 1;
   if( this->row < rhs.row ) return 1;
   return 0;
}

} /* namespace algorithms */
