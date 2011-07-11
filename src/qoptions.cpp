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

#include "qoptions.h"
#include <qstringlist.h>

QOption::QOption(const char *name, const QVariant &defaultValue, Type type, const QString &description)
:mType(type),mDescription(description),mDefaultValue(defaultValue),mValue(new QVariant)
{qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
	*mValue = defaultValue;
	setName(name);
}

QOption::QOption(const char *name, Type type, const QString &description)
{qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
	QOption(name, QVariant(), type, description);

	if (mType==QOption::NoToken)
		*mValue = false;
}

QOption::QOption(const char *name, QVariant *value, Type type, const QString &description)
:mType(type),mDescription(description),mDefaultValue(*value),mValue(value)
{qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
	setName(name);
}

QOption::~QOption()
{
	delete mValue;
}

QString QOption::shortName() const
{
	return mShortName;
}

QString QOption::longName() const
{
	return mLongName;
}

QString QOption::description() const
{
	return mDescription;
}

QVariant QOption::value() const
{
	qDebug("%d %s", __LINE__, qPrintable(mValue->toString()));
	return *mValue;
}

void QOption::setValue(const QVariant &value)
{
	*mValue = value;
	qDebug("%d %s", __LINE__, qPrintable(mValue->toString()));
}

void QOption::setType(QOption::Type type)
{
	mType = type;
	if (mType==NoToken)
		*mValue = false;
}

QOption::Type QOption::type() const
{
	return mType;
}

void QOption::setName(const QString &name)
{
	int comma = name.indexOf(',');
	if (comma>0) {
		mLongName = name.left(comma);
		mShortName = name.mid(comma+1);
	} else {
		mLongName = name;
	}
	qDebug("name:%s, short:%s, long:%s", qPrintable(name), qPrintable(shortName()), qPrintable(longName()));
}



QOptions::QOptions()
{
}

QOptions::~QOptions()
{
	mOptionGroupMap.clear();
	mOptions.clear();
}

bool QOptions::parse(int argc, const char *const*argv)
{
	if (mOptionGroupMap.isEmpty())
		return false;

	if (argc==1)
		return true;

	bool result = true;
	QStringList args;
	for (int i=1;i<argc;++i) {
		args.append(argv[i]);
	}

	QStringList::Iterator it = args.begin();
	QList<QOption*>::Iterator it_list;
	mOptions = mOptionGroupMap.keys();

	print();
	while (it!=args.end()) {

		if ((*it).startsWith("--")) {
			int e = (*it).indexOf('=');
			for (it_list=mOptions.begin();it_list!=mOptions.end();++it_list) {
				if ((*it_list)->longName().startsWith((*it).mid(2,e-2))) {
					if ((*it_list)->type()==QOption::NoToken) {
						(*it_list)->setValue(true);
						qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
						//Do not search the existence. Fast up the search.
						it = args.erase(it);
						break;
					}
					if (e>0) { //
						(*it_list)->setValue((*it).mid(e+1));
						qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
					} else {
						it = args.erase(it);
						(*it_list)->setValue(*it);
						qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
					}
					it = args.erase(it);
					break;
				}
			}
			if (it_list==mOptions.end())
				++it;
			//handle unknow option
		} else if ((*it).startsWith('-')) {
			for (it_list=mOptions.begin();it_list!=mOptions.end();++it_list) {
				QString sname = (*it_list)->shortName();
				int sname_len = sname.length(); //usally is 1
				//Not endsWith, -oabco
				if ((*it).indexOf(sname)==1) {
					if ((*it_list)->type()==QOption::NoToken) {
						(*it_list)->setValue(true);
						qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
						it = args.erase(it);
						break;
					}
					if ((*it).length()==(sname_len+1)) {//-o abco
						it = args.erase(it);
						(*it_list)->setValue(*it);
						qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
					} else {
						(*it_list)->setValue((*it).mid(sname_len+1));
						qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
					}
					it = args.erase(it);
					break;
				}
			}
			if (it_list==mOptions.end())
				++it;
			//handle unknow option
		}
	}
	return result;
}

QOptions& QOptions::add(const QString &description)
{
	mCurrentDescription = description;
	return *this;
}

QOptions& QOptions::operator ()(const char* name, const QString& description)
{
	QOption *op = new QOption(name, QOption::SingleToken, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	qDebug("%d--------%s", __LINE__, qPrintable(op->longName()));
	return *this;
}

QOptions& QOptions::operator ()(const char* name, QOption::Type type, const QString& description)
{
	QOption *op = new QOption(name, type, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	qDebug("%d--------%s", __LINE__, qPrintable(op->shortName()));
	return *this;
}

QOptions& QOptions::operator ()(const char* name, const QVariant& defaultValue, const QString& description)
{
	QOption *op = new QOption(name, defaultValue, QOption::SingleToken, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	qDebug("%d--------%s", __LINE__, qPrintable(op->longName()));
	return *this;
}

QOptions& QOptions::operator ()(const char* name, const QVariant& defaultValue, QOption::Type type, const QString& description)
{
	QOption *op = new QOption(name, defaultValue, type, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	qDebug("%d--------%s", __LINE__, qPrintable(op->longName()));
	return *this;
}

QOptions& QOptions::operator ()(const char* name, QVariant* value, const QString& description)
{qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
	QOption *op = new QOption(name, value, QOption::SingleToken, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	return *this;
}

QOptions& QOptions::operator ()(const char* name, QVariant* value, QOption::Type type, const QString& description)
{qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
	QOption *op = new QOption(name, value, type, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	return *this;
}

QVariant QOptions::value(const QString& name)// const
{
	if (mOptions.isEmpty())
		mOptions = mOptionGroupMap.keys();

	QList<QOption*>::Iterator it_list;
	for (it_list=mOptions.begin();it_list!=mOptions.end();++it_list) {
		qDebug("%s=", qPrintable((*it_list)->longName()));//, qPrintable((*it_list)->value().toString()));
		if ((*it_list)->shortName()==name || (*it_list)->longName()==name)
			return (*it_list)->value();
	}
	return QVariant();
}


void QOptions::print()
{
	QList<QOption*>::Iterator it_list;
	for (it_list=mOptions.begin();it_list!=mOptions.end();++it_list) {
		qDebug("%s=", qPrintable((*it_list)->longName()));//, qPrintable((*it_list)->value().toString()));
	}
}
