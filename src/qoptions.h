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

#ifndef QOPTIONS_H
#define QOPTIONS_H

#include <qvariant.h>
#include <qmap.h>
#include <qlist.h>

class QOption {
public:
	enum Type {
		NoToken, SingleToken, MultiToken
	};
	explicit QOption(const char* name, const QVariant& defaultValue, Type type, const QString& description);
	explicit QOption(const char* name, Type type, const QString& description);
	explicit QOption(const char* name, QVariant* value, Type type, const QString& description);
	~QOption();

	QString shortName() const;
	QString longName() const;
	QString description() const;
	QVariant value() const;
	void setValue(const QVariant& value);

	void setType(QOption::Type type);
	QOption::Type type() const;

private:
	void setName(const QString& name);

	QOption::Type mType;
	QString mShortName, mLongName, mDescription;
	QVariant mDefaultValue;
	QVariant *mValue;
};


class QOptions {
public:
	QOptions();
	~QOptions();

	bool parse(int argc, const char*const* argv);
	QOptions& add(const QString& description);

	QOptions& operator ()(const char* name, const QString& description = "");
	QOptions& operator ()(const char* name, QOption::Type type, const QString& description = "");
	QOptions& operator ()(const char* name, const QVariant& defaultValue, const QString& description);
	QOptions& operator ()(const char* name, const QVariant& defaultValue, QOption::Type type, const QString& description = "");
	QOptions& operator ()(const char* name, QVariant* value, const QString& description = "");
	QOptions& operator ()(const char* name, QVariant* value, QOption::Type type, const QString& description = "");

	QVariant value(const QString& name);// const;

	void print();
private:
	QString mCurrentDescription;
	QList<QOption*> mOptions;
	QMap<QOption*, QString/*group*/> mOptionGroupMap;

};

#endif // QOPTIONS_H
