#include "LogicObject.h"
#include "../Service/Exception/Exception.h"

using namespace qReal;

using namespace client;

LogicObject::LogicObject( const IdType &id, const IdType &parent ) : mId(id)
{
	addParent(parent);
}

void LogicObject::addParent( const IdType &parent )
{
	if (!mParents.contains(parent)) {
		mParents.append(parent);
	} else {
		throw Exception("Object " + mId + ": adding existing parent " + parent);
	}
}

void LogicObject::removeParent( const IdType &parent )
{
	if (mParents.contains(parent)) {
		if (mParents.size() != 1) {
			mParents.removeAll(parent);
		} else {
			throw Exception("Object " + mId + ": removing the only parent " + parent);
		}
	} else {
		throw Exception("Object " + mId + ": removing nonexistent parent " + parent);
	}
}

void LogicObject::addChild( const IdType &child )
{
	if (!mChildren.contains(child)) {
		mChildren.append(child);
	} else {
		throw Exception("Object " + mId + ": adding existing child " + child);
	}
}

void LogicObject::removeChild( const IdType &child )
{
	if (mChildren.contains(child)) {
		mChildren.removeAll(child);
	} else {
		throw Exception("Object " + mId + ": removing nonexistent child " + child);
	}
}

IdTypeList LogicObject::children() const
{
	return mChildren;
}

IdTypeList LogicObject::parents() const
{
	return mParents;
}

void LogicObject::setProperty( const PropertyName &name, const QVariant &value )
{
	mProperties.insert(name,value);
}

QVariant LogicObject::property( const PropertyName &name ) const
{
	if (mProperties.contains(name)) {
		return mProperties[name];
	} else {
		throw Exception("Object " + mId + ": requesting nonexistent property " + name);
	}
}

void LogicObject::removeProperty( const PropertyName &name )
{
	if (mProperties.contains(name)) {
		mProperties.remove(name);
	} else {
		throw Exception("Object " + mId + ": removing nonexistent property " + name);
	}
}