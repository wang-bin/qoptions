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
{
	*mValue = defaultValue;
	setName(name);
}

QOption::QOption(const char *name, Type type, const QString &description)
:mType(type),mDescription(description),mDefaultValue(QVariant()),mValue(new QVariant)
{
	if (mType==QOption::NoToken)
		*mValue = false;
	setName(name);
}
/*
QOption::QOption(const char *name, QVariant *value, Type type, const QString &description)
:mType(type),mDescription(description),mDefaultValue(*value),mValue(value)
{qDebug("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
	setName(name);
}
*/
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

QString QOption::formatName() const
{
	if (!mShortName.isEmpty())
		return "-" + mShortName + " [ --" + mLongName + "]";
	else
		return "--" + mLongName;
}

QString QOption::description() const
{
	return mDescription;
}

QVariant QOption::value() const
{
	return *mValue;
}

void QOption::setValue(const QVariant &value)
{
	*mValue = value;
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

void QOption::print()
{
	QString message = formatName();
	if (type()==QOption::SingleToken)
		message.append(" arg");
	else if (type()==QOption::MultiToken)
		message.append(" arg1 ... argN");


	message = QString("%1").arg(message, -33);

	message.append(mDescription);
	qDebug("%s", qPrintable(message));
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

	while (it!=args.end()) {

		if ((*it).startsWith("--")) {
			int e = (*it).indexOf('=');
			for (it_list=mOptions.begin();it_list!=mOptions.end();++it_list) {
				if ((*it_list)->longName().startsWith((*it).mid(2,e-2))) {
					if ((*it_list)->type()==QOption::NoToken) {
						(*it_list)->setValue(true);
						//qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
						it = args.erase(it);
						break;
					}
					if (e>0) { //
						(*it_list)->setValue((*it).mid(e+1));
						//qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
					} else {
						it = args.erase(it);
						(*it_list)->setValue(*it);
						//qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
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
						//qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
						it = args.erase(it);
						break;
					}
					if ((*it).length()==(sname_len+1)) {//-o abco
						it = args.erase(it);
						(*it_list)->setValue(*it);
						//qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
					} else {
						(*it_list)->setValue((*it).mid(sname_len+1));
						//qDebug("%d %s", __LINE__, qPrintable((*it_list)->value().toString()));
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

QOptions& QOptions::add(const QString &group_description)
{
	mCurrentDescription = group_description;
	return *this;
}

QOptions& QOptions::addDescription(const QString &description)
{
	mDescription = description;
	return *this;
}

QOptions& QOptions::operator ()(const char* name, const QString& description)
{
	QOption *op = new QOption(name, QOption::NoToken, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	return *this;
}

QOptions& QOptions::operator ()(const char* name, QOption::Type type, const QString& description)
{
	QOption *op = new QOption(name, type, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	return *this;
}

QOptions& QOptions::operator ()(const char* name, const QVariant& defaultValue, const QString& description)
{
	QOption *op = new QOption(name, defaultValue, QOption::SingleToken, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	return *this;
}

QOptions& QOptions::operator ()(const char* name, const QVariant& defaultValue, QOption::Type type, const QString& description)
{
	QOption *op = new QOption(name, defaultValue, type, description);
	mOptionGroupMap.insert(op, mCurrentDescription);
	return *this;
}
/*
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
*/
const QVariant& QOptions::value(const QString& name)
{
	if (mOptions.isEmpty())
		mOptions = mOptionGroupMap.keys();

	QList<QOption*>::Iterator it_list;
	for (it_list=mOptions.begin();it_list!=mOptions.end();++it_list) {
		if ((*it_list)->shortName()==name || (*it_list)->longName()==name) {
			mCurrentValue = (*it_list)->value();
			break;
		}
	}
	return mCurrentValue;
}

const QVariant& QOptions::operator [](const QString& name)
{
	return value(name);
}

void QOptions::print()
{
	qDebug("%s", qPrintable(mDescription));
	QList<QString> groups = mOptionGroupMap.values();
	std::list<QString> std_groups = groups.toStdList();
	std_groups.unique();
	groups = QList<QString>::fromStdList(std_groups);
	QList<QString>::ConstIterator it;

	QList<QOption*>::ConstIterator it_op;
	for (it=groups.constBegin();it!=groups.constEnd();++it) {
		qDebug("%s", qPrintable(*it));
		QList<QOption*> options = mOptionGroupMap.keys(*it);
		for (it_op=options.constBegin();it_op!=options.constEnd();++it_op)
			(*it_op)->print();
	}
}
