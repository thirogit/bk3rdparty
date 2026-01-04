#ifndef JSONRPC_CPP_MESSAGEPROPERTIES_H_
#define JSONRPC_CPP_MESSAGEPROPERTIES_H_

#include <map>
#include <casajson/details/basic_types.h>

namespace jsonrpc
{
	typedef std::map<utility::string_t,utility::string_t> property_map; 
	typedef property_map::iterator property_iterator;
	typedef property_map::const_iterator const_property_iterator;
        	
 
	class RPCMessageProperties
	{
	public:
		utility::string_t get(const utility::string_t& name);
		void set(const utility::string_t& name,const utility::string_t& value); 
		
		property_iterator begin();
		property_iterator end();

		const_property_iterator cbegin() const;
		const_property_iterator cend() const;
		
	private:
		property_map properties;
	};

 
} /* namespace jsonrpc */
#endif /* JSONRPC_CPP_MESSAGEPROPERTIES_H_ */
