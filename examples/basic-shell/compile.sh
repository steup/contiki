#!/bin/bash

#
#  Copyright (c) 2015, Otto-von-Guericke-Universität Magdeburg (OVGU)
#
#  Authors:	André Keuns		<andre.keuns@st.ovgu.de>
#			Marcus Viererbe	<marcus.viererbe@st.ovgu.de>
#
#  All rights reserved
#
#  This script free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  The GNU General Public License can be found at
#  http://www.gnu.org/copyleft/gpl.html.
#
#  This script is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  This copyright notice MUST APPEAR in all copies of the script!
#  Thanks for all inspirations.
#

export PATH=/opt/arm-none-eabi/build/bin:$PATH

if [ "$1" == "1" ]; then

	make clean TARGET=deRFsam3-23M10-2

fi

make TARGET=deRFsam3-23M10-2

