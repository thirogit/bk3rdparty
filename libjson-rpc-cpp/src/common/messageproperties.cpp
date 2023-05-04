#include "messageproperties.h"


namespace jsonrpc
{

utility::string_t RPCMessageProperties::get(const utility::string_t& name)
{
	property_iterator it = properties.find(name);
	if(it != properties.end())
	{
		return it->second;
	}
	
	return utility::string_t();
}

void RPCMessageProperties::set(const utility::string_t& name,const utility::string_t& value)
{
	properties[name] = value;
}

		
property_iterator RPCMessageProperties::begin()
{
	return properties.begin();
}

property_iterator RPCMessageProperties::end()
{
	return properties.end();
}

const_property_iterator RPCMessageProperties::cbegin() const
{
	return properties.cbegin();
}

const_property_iterator RPCMessageProperties::cend() const
{
	return properties.cend();
}


}