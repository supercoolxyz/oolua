#include "oolua_push_pull.h"


//if called by Lua
//	luaL_error - as it throw an exception if Lua compiled as C++
//				- returns an error code to the caller if compiled as C

//if called from cpp
//	using exceptions
//	throw

//	storing last error
//		store error
//		return false


namespace OOLUA
{
	namespace INTERNAL
	{
		
		void handle_cpp_pull_fail(lua_State* l,char const * lookingFor)
		{
#	if OOLUA_USE_EXCEPTIONS == 1
			std::string message( std::string("Stack type is not ") + lookingFor );
			std::string stackType = lua_typename(l, lua_type(l,-1) );
			message += std::string(", yet ") + stackType;
			throw OOLUA::Type_error(message);
#	elif OOLUA_STORE_LAST_ERROR == 1
			lua_pushfstring(l, "Stack type is not a %s, yet &s"
							,lookingFor
							,lua_typename(l, lua_type(l,-1) ) );
			
			OOLUA::INTERNAL::set_error_from_top_of_stack(l);
			return ;
#	else
			(void)l;
			(void)lookingFor;
			return ;
#	endif			
		}
		
		
		bool cpp_runtime_type_check_of_top(lua_State* l, int looking_for_lua_type, char const * type)
		{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
			if ( lua_type(l,-1) != looking_for_lua_type )
			{
				handle_cpp_pull_fail(l,type);
				return false;
			}
#else
			(void)l;
			(void)looking_for_lua_type;
			(void)type;	
#endif
			return true;
		}
		
		
		bool cpp_runtime_type_check_of_top(lua_State* l, is_type compareFunc, char const * type)
		{
#if OOLUA_RUNTIME_CHECKS_ENABLED  == 1
			if ( ! compareFunc(l,-1) )
			{
				handle_cpp_pull_fail(l,type);
				return false;
			}
#else
			(void)l;
			(void)type;
			(void)type;	
#endif
			return true;
		}
		
		
	}
	
	
	
	
	void push2lua(lua_State* const s, bool const& value)
	{
		assert(s);
		lua_pushboolean(s, (value? 1 : 0) );
	}
	void push2lua(lua_State* const s, std::string const& value)
	{
		assert(s);
		lua_pushstring (s, value.c_str());
	}
	void push2lua(lua_State* const s, char const * const& value)
	{
		assert(s && value);
		lua_pushstring (s, value);
	}
	void push2lua(lua_State* const s, char * const& value)
	{
		assert(s && value);
		lua_pushstring (s, value);
	}
	void push2lua(lua_State* const s, double const& value)
	{
		assert(s);
		lua_pushnumber(s, value);
	}
	void push2lua(lua_State* const s, float const&  value)
	{
		assert(s);
		lua_pushnumber(s, value);
	}
	void push2lua(lua_State* const s, lua_CFunction const &  value)
	{
		assert(s );
		lua_pushcclosure(s,value,0);
	}
	void push2lua(lua_State* const s, Lua_table const &  value)
	{
		assert(s /*&& value.valid() */);
		//value.get_table();
		value.push_on_stack(s);
	}
	
	void push2lua(lua_State* const s, Lua_func_ref const &  value)
	{
		assert(s /*value.valid()*/ );
		value.push(s);
	}
	

	
	void pull2cpp(lua_State* const s, bool& value)
	{
		//assert(lua_isboolean(s,-1));
		if(! INTERNAL::cpp_runtime_type_check_of_top(s,LUA_TBOOLEAN,"bool") ) return ;//false;
		value =  lua_toboolean( s, -1) ? true : false;
		lua_pop( s, 1);
	}
	void pull2cpp(lua_State* const s, std::string& value)
	{
		if(! INTERNAL::cpp_runtime_type_check_of_top(s,LUA_TSTRING,"string") ) return ;//false;
		value = lua_tolstring(s,-1,0);
		lua_pop( s, 1);
	}
	
	void pull2cpp(lua_State* const s, double& value)
	{
		if(! INTERNAL::cpp_runtime_type_check_of_top(s,LUA_TNUMBER,"double") ) return ;//false;
		value = static_cast<double>( lua_tonumber( s, -1) );
		lua_pop( s, 1);
	}
	void pull2cpp(lua_State* const s, float& value)
	{
		if(! INTERNAL::cpp_runtime_type_check_of_top(s,LUA_TNUMBER,"float") ) return ;//false;
		value = static_cast<float>( lua_tonumber( s, -1) );
		lua_pop( s, 1);
	}
	void pull2cpp(lua_State* const s, lua_CFunction& value)
	{
		if (! INTERNAL::cpp_runtime_type_check_of_top(s,lua_iscfunction,"lua_CFunction") ) return;
		value = lua_tocfunction( s, -1);
		lua_pop( s, 1);
	}
	void pull2cpp(lua_State* const s, Lua_func_ref& value)
	{
		//TODO: implement
		value.pull(s);
	}
	
	void pull2cpp(lua_State* const s, Lua_table&  value)
	{
		//TODO: implement
		value.pull_from_stack(s);
	}
	
	void pull2cpp(lua_State* const s, Lua_table_ref& value)
	{
		//TODO: implement
		value.pull(s);
	}

	
	
	
	namespace INTERNAL 
	{

		namespace LUA_CALLED
		{
			void pull_class_type_error(lua_State* const s,char const* type)
			{
				luaL_error(s,"%s %s %s","tried to pull type"
						   ,type
						   ,"which is not the type or a base of the type on the stack");
			}
			
			void pull_error(lua_State* l, char const* when_pulling_this_type)
			{
				luaL_error(l,"trying to pull %s when %s is on stack"
						   ,when_pulling_this_type
						   , lua_typename(l, lua_type(l,-1)) );
			}
			
			void pull2cpp(lua_State* const s, bool& value)
			{

#ifdef OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_isboolean(s,-1) )pull_error(s,"bool");
#endif	
				value =  lua_toboolean( s, -1) ? true : false;
				lua_pop( s, 1);
			}
			void pull2cpp(lua_State* const s, std::string& value)
			{
#ifdef OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_isstring(s,-1) )pull_error(s,"std::string");
#endif
				value = lua_tolstring(s,-1,0);
				lua_pop( s, 1);
			}
		
			void pull2cpp(lua_State* const s, double& value)
			{
#ifdef OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_isnumber(s,-1) )pull_error(s,"double");
#endif
				value = static_cast<double>( lua_tonumber( s, -1) );
				lua_pop( s, 1);
			}
			void pull2cpp(lua_State* const s, float& value)
			{
#ifdef OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_isnumber(s,-1) )pull_error(s,"float");
#endif
				value = static_cast<float>( lua_tonumber( s, -1) );
				lua_pop( s, 1);
			}
			void pull2cpp(lua_State* const s, lua_CFunction& value)
			{
#ifdef OOLUA_RUNTIME_CHECKS_ENABLED  == 1
				if(! lua_iscfunction(s,-1) )pull_error(s,"lua_CFunction");
#endif
				value = lua_tocfunction( s, -1);
				lua_pop( s, 1);
			}
			void pull2cpp(lua_State* const s, Lua_func_ref& value)
			{
				//TODO: implement
				//value.lua_pull(s);
				value.pull(s);
			}
		
			void pull2cpp(lua_State* const s, Lua_table&  value)
			{
				//TODO: implement
				//value.lua_pull_from_stack(s);
				value.pull_from_stack(s);
			}
		
			void pull2cpp(lua_State* const s, Lua_table_ref& value)
			{
				//TODO: implement
				//value.lua_pull(s);
				value.pull(s);
			}
		
		}
	}
}
