/******************************************************************************
	Name: description
	Copyright (C) 2011 Wang Bin <wbsecg1@gmail.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/

#include "../src/qoptions.h"

int main(int argc, char *argv[])
{
	bool male;
	QOptions ops;qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
	ops.add("All options")
	("name,n", "WangBin", QOption::SingleToken, "Your name")
	("help,h", "help message")
	("age,a", 24, "Your age");
	//("gender,g", &male, "Your gender");

	ops.parse(argc, argv);
	ops.print();
	qDebug("name: %s, Age: %d, ", qPrintable(ops.value("name").toString()), ops.value("age").toInt());

	return 0;
}
