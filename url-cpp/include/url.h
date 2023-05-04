#ifndef URL_CPP_H
#define URL_CPP_H

#include <stdexcept>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

namespace Url
{

    struct UrlParseException : public std::logic_error
    {
        UrlParseException(const std::string& message) : std::logic_error(message) {}
    };

    struct CharacterClass
    {
        CharacterClass(const std::string& chars) : chars_(chars), map_(256, false)
        {
            for (auto it = chars_.begin(); it != chars_.end(); ++it)
            {
                map_[static_cast<size_t>(*it)] = true;
            }
        }

        bool operator()(char c) const
        {
            return map_[static_cast<unsigned char>(c)];
        }

        const std::string& chars() const
        {
            return chars_;
        }

    private:
        // Private, unimplemented to prevent use
        CharacterClass();
        CharacterClass(const CharacterClass& other);

        std::string chars_;
        std::vector<bool> map_;
    };
	
	/**
	 * Ensure all the provided characters are escaped if necessary
	 */
	std::string escape(const std::string& str, const CharacterClass& safe, bool strict);

	/**
	 * Unescape entities in the provided string
	 */
	std::string unescape(const std::string& str);


    struct Url
    {
        /* Character classes */
        const static CharacterClass GEN_DELIMS;
        const static CharacterClass SUB_DELIMS;
        const static CharacterClass ALPHA;
        const static CharacterClass DIGIT;
        const static CharacterClass UNRESERVED;
        const static CharacterClass RESERVED;
        const static CharacterClass PCHAR;
        const static CharacterClass PATH;
        const static CharacterClass QUERY;
        const static CharacterClass FRAGMENT;
        const static CharacterClass USERINFO;
        const static CharacterClass HEX;
        const static CharacterClass SCHEME;
        const static std::vector<signed char> HEX_TO_DEC;
        const static std::unordered_map<std::string, int> PORTS;
        const static std::unordered_set<std::string> USES_RELATIVE;
        const static std::unordered_set<std::string> USES_NETLOC;
        const static std::unordered_set<std::string> USES_PARAMS;
        const static std::unordered_set<std::string> KNOWN_PROTOCOLS;

        // The type of the predicate used for removing parameters
        typedef std::function<bool(std::string&, std::string&)> deparam_predicate;

        explicit Url(const std::string& url);

        Url(const Url& other)
            : scheme_(other.scheme_)
            , host_(other.host_)
            , port_(other.port_)
            , path_(other.path_)
            , params_(other.params_)
            , query_(other.query_)
            , fragment_(other.fragment_)
            , userinfo_(other.userinfo_)
            , has_params_(other.has_params_)
            { }

        /**
         * Take on the value of the other URL.
         */
        Url& assign(const Url& other);

        /**
         * To be considered equal, all fields must be equal.
         */
        bool operator==(const Url& other) const;
        bool operator!=(const Url& other) const;

        /**
         * Two URLs are considered equivalent if they have the same meaning.
         */
        bool equiv(const Url& other);

        /**************************************
         * Component-wise access and setting. *
         **************************************/
        const std::string& scheme() const { return scheme_; }
        Url& setScheme(const std::string& s)
        {
            scheme_ = s;
            return *this;
        }

        const std::string& host() const { return host_; }
        Url& setHost(const std::string& s)
        {
            host_ = s;
            return *this;
        }

		const int port() const;

        Url& setPort(int i)
        {
            port_ = i;
            return *this;
        }

        const std::string& path() const { return path_; }
        Url& setPath(const std::string& s)
        {
            path_ = s;
            return *this;
        }

        const std::string& params() const { return params_; }
        Url& setParams(const std::string& s)
        {
            params_ = s;
            has_params_ = !s.empty();
            return *this;
        }

        const std::map<std::string,std::string>& query() const { return query_; }

        Url& setQuery(const std::map<std::string, std::string>& q)
        {
            query_ = q;
            return *this;
        }

        const std::string& fragment() const { return fragment_; }
        Url& setFragment(const std::string& s)
        {
            fragment_ = s;
            return *this;
        }

        const std::string& userinfo() const { return userinfo_; }
        Url& setUserinfo(const std::string& s)
        {
            userinfo_ = s;
            return *this;
        }

        /**
         * Get a representation of all components of the path, params, query, fragment.
         *
         * Always includes a leading /.
         */
        std::string fullpath() const;

        /**
         * Get a new string representation of the URL.
         **/
        std::string str() const;

        /*********************
         * Chainable methods *
         *********************/
        
        /**
         * Make the path absolute.
         *
         * Evaluate '.', '..', and excessive slashes.
         */
        Url& abspath();

        /**
         * Evaluate this URL relative fo `other`, placing the result in this object.
         */
        Url& relative_to(const std::string& other)
        {
            return relative_to(Url(other));
        }

        /**
         * Evaluate this URL relative fo `other`, placing the result in this object.
         */
        Url& relative_to(const Url& other);

        
       
        /**
         * Remove the port if it's the default for the scheme.
         */
        Url& remove_default_port();

        /**
         * Remove the userinfo portion.
         */
        Url& deuserinfo();

        /**
         * Remove the fragment.
         */
        Url& defrag();

        /**
         * Punycode the hostname.
         */
        Url& punycode();

        /**
         * Unpunycode the hostname.
         */
        Url& unpunycode();

        /**
         * Reverse the hostname (a.b.c.d => d.c.b.a)
         */
        Url& host_reversed();
			

		Url& operator&=(const std::tuple<std::string, std::string>& condition);
		Url& operator/=(const std::string& right);

    private:
        // Private, unimplemented to prevent use.
        Url();

       
        /**
         * Check that the hostname is valid, removing an optional trailing '.'.
         */
        void check_hostname(std::string& host);

		std::map<std::string, std::string> parse_query(const std::string& q);
		std::string print_query(const std::map<std::string, std::string>& conditions) const;

        std::string scheme_;
        std::string host_;
        int port_;
        std::string path_;
        std::string params_;
		std::map<std::string, std::string> query_;
        std::string fragment_;
        std::string userinfo_;
        bool has_params_;

		friend Url operator/(const Url& left, const std::string& right);
    };

	Url operator/(const Url& left, const std::string& right);
	
}


#endif
