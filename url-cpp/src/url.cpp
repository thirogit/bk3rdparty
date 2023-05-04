#include <algorithm>
#include <string>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <iterator>
#include <sstream>

#include "url.h"
#include "punycode.h"


std::vector<std::string> split_query(const std::string& q) {
	std::string str = q;
	std::vector<std::string> result;
	
	while (str.size()) {
		int index = str.find_first_of('&');
		if (index != std::string::npos) {
			result.push_back(str.substr(0, index));
			str = str.substr(index + 1);
			if (str.size() == 0) result.push_back(str);
		}
		else {
			result.push_back(str);
			str = "";
		}
	}
	return result;
}

namespace Url
{

    /* Character classes */
    const CharacterClass Url::GEN_DELIMS(":/?#[]@");
    const CharacterClass Url::SUB_DELIMS("!$&'()*+,;=");
    const CharacterClass Url::DIGIT("0123456789");
    const CharacterClass Url::ALPHA(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    const CharacterClass Url::UNRESERVED(
        Url::ALPHA.chars() + Url::DIGIT.chars() + "-._~");
    const CharacterClass Url::RESERVED(
        Url::GEN_DELIMS.chars() + Url::SUB_DELIMS.chars());
    const CharacterClass Url::PCHAR(
        Url::UNRESERVED.chars() + Url::SUB_DELIMS.chars() + ":@");
    const CharacterClass Url::PATH(
        Url::PCHAR.chars() + "/");
    const CharacterClass Url::QUERY(
        Url::PCHAR.chars() + "/?");
    const CharacterClass Url::FRAGMENT(
        Url::PCHAR.chars() + "/?");
    const CharacterClass Url::USERINFO(
        Url::UNRESERVED.chars() + Url::SUB_DELIMS.chars() + ":");
    const CharacterClass Url::HEX("0123456789ABCDEF");
    const CharacterClass Url::SCHEME(
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-.");
    const std::vector<signed char> Url::HEX_TO_DEC = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,

        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    const std::unordered_map<std::string, int> Url::PORTS = {
        {"http", 80},
        {"https", 443}
    };
    const std::unordered_set<std::string> Url::USES_RELATIVE = {
        "",
        "file",
        "ftp",
        "gopher",
        "http",
        "https",
        "imap",
        "mms",
        "nntp",
        "prospero",
        "rtsp",
        "rtspu",
        "sftp",
        "shttp",
        "svn",
        "svn+ssh",
        "wais"
    };
    const std::unordered_set<std::string> Url::USES_NETLOC = {
        "",
        "file",
        "ftp",
        "git",
        "git+ssh",
        "gopher",
        "http",
        "https",
        "imap",
        "mms",
        "nfs",
        "nntp",
        "prospero",
        "rsync",
        "rtsp",
        "rtspu",
        "sftp",
        "shttp",
        "snews",
        "svn",
        "svn+ssh",
        "telnet",
        "wais"
    };
    const std::unordered_set<std::string> Url::USES_PARAMS = {
        "",
        "ftp",
        "hdl",
        "http",
        "https",
        "imap",
        "mms",
        "prospero",
        "rtsp",
        "rtspu",
        "sftp",
        "shttp",
        "sip",
        "sips",
        "tel"
    };
    const std::unordered_set<std::string> Url::KNOWN_PROTOCOLS = {
        "",
        "file",
        "ftp",
        "git",
        "git+ssh",
        "gopher",
        "hdl",
        "http",
        "https",
        "imap",
        "mms",
        "nfs",
        "nntp",
        "prospero",
        "rsync",
        "rtsp",
        "rtspu",
        "sftp",
        "shttp",
        "sip",
        "sips",
        "sms",
        "snews",
        "svn",
        "svn+ssh",
        "tel",
        "telnet",
        "wais"
    };

    Url::Url(const std::string& url): port_(0), has_params_(false)
    {
        size_t position = 0;
        size_t index = url.find(':');
        if (index != std::string::npos)
        {
            // All the characters in our would-be scheme must be in SCHEME
            if (std::all_of(
                    url.begin(),
                    url.begin() + index,
                    [](char c) { return SCHEME(c); } ))
            {
                // If there is nothing after the : or there are any non-digits, this is
                // the scheme
                if ((index + 1) >= url.length()
                    || std::any_of(
                        url.begin() + index + 1,
                        url.end(),
                        [](char c) { return !DIGIT(c); }))
                {
                    scheme_.assign(url, 0, index);
                    std::transform(
                        scheme_.begin(), scheme_.end(), scheme_.begin(), ::tolower);
                    position = index + 1;
                }
                else
                {
                    scheme_.assign(url, 0, index);
                    std::transform(
                        scheme_.begin(), scheme_.end(), scheme_.begin(), ::tolower);
                    if (KNOWN_PROTOCOLS.find(scheme_) != KNOWN_PROTOCOLS.end())
                    {
                        position = index + 1;
                    }
                    else
                    {
                        scheme_.clear();
                    }
                }
            }
        }

        // Search for the netloc
        if ((url.length() - position) >= 1
            && url[position] == '/'
            && url[position + 1] == '/')
        {
            // Skip the '//'
            position += 2;
            index = url.find_first_of("/?#", position);
            host_.assign(url, position, index - position);
            position = index;

            // Extract any userinfo if there is any
            index = host_.find('@');
            if (index != std::string::npos)
            {
                userinfo_.assign(host_, 0, index);
                host_.assign(host_, index + 1, std::string::npos);
            }

            // Lowercase the hostname
            std::transform(host_.begin(), host_.end(), host_.begin(), ::tolower);

            // Try to find a port
            index = host_.find(':');
            if (index != std::string::npos)
            {
                std::string portText(host_, index + 1, std::string::npos);
                host_.resize(index);

                if (portText.empty())
                {
                    port_ = 0;
                }
                else
                {
                    try
                    {
                        port_ = std::stoi(portText, &index);

                        if (index != portText.length())
                        {
                            // Malformed port
                            throw UrlParseException("Port not a number: " + portText);
                        }

                        if (port_ > 65535)
                        {
                            throw UrlParseException("Port too high: " + portText);
                        }
                        else if (port_ < 0)
                        {
                            throw UrlParseException("Port negative: " + portText);
                        }
                    }
                    catch (const std::invalid_argument&)
                    {
                        // Malformed port
                        throw UrlParseException("Port not a number: " + portText);
                    }
                    catch (const std::out_of_range&)
                    {
                        throw UrlParseException("Port out of integer range: " + portText);
                    }
                }
            }
        }

        if (position != std::string::npos)
        {
            path_.assign(url, position, std::string::npos);

            index = path_.find('#');
            if (index != std::string::npos)
            {
                fragment_.assign(path_, index + 1, std::string::npos);
                path_.resize(index);
            }

            index = path_.find('?');
            if (index != std::string::npos)
            {
                std::string q;
				q.assign(path_, index + 1, std::string::npos);
				query_ = std::move(parse_query(q));                
                path_.resize(index);
            }

            if (USES_PARAMS.find(scheme_) != USES_PARAMS.end())
            {
                index = path_.find(';');
                if (index != std::string::npos)
                {
                    params_.assign(path_, index + 1, std::string::npos);
                    has_params_ = true;
                    path_.resize(index);
                }
            }
        }
    }

    Url& Url::assign(const Url& other)
    {
        return (*this) = other;
    }

    bool Url::operator==(const Url& other) const
    {
        return (
            (scheme_     == other.scheme_    ) &&
            (userinfo_   == other.userinfo_  ) &&
            (host_       == other.host_      ) &&
            (port_       == other.port_      ) &&
            (path_       == other.path_      ) &&
            (params_     == other.params_    ) &&
            (query_      == other.query_     ) &&
            (fragment_   == other.fragment_  ) &&
            (has_params_ == other.has_params_) 
        );
    }

    bool Url::operator!=(const Url& other) const
    {
        return !operator==(other);
    }

    
    std::string Url::fullpath() const
    {
        std::string result;
        if (path_.empty() || path_[0] != '/')
        {
            result.append(1, '/');
        }
        result.append(path_);

        if (has_params_)
        {
            result.append(";");
            result.append(params_);
        }

        if (!query_.empty())
        {
            result.append("?");
            result.append(print_query(query_));
        }

        if (!fragment_.empty())
        {
            result.append("#");
            result.append(fragment_);
        }
        return result;
    }

    std::string Url::str() const
    {
        std::string result;

        if (!scheme_.empty())
        {
            result.append(scheme_);
            if (USES_NETLOC.find(scheme_) == USES_NETLOC.end())
            {
                result.append(":");
            }
            else
            {
                result.append("://");
            }
        }
        else if (!host_.empty())
        {
            result.append("//");
        }

        if (!userinfo_.empty())
        {
            result.append(userinfo_);
            result.append("@");
        }

        if (!host_.empty())
        {
            result.append(host_);
        }

        if (port_)
        {
            result.append(":");
            result.append(std::to_string(port_));
        }

        if (path_.empty())
        {
            if (!result.empty())
            {
                result.append("/");
            }
        }
        else
        {
            if (!host_.empty() && path_[0] != '/')
            {
                result.append(1, '/');
            }
            result.append(path_);
        }

        if (has_params_)
        {
            result.append(";");
            result.append(params_);
        }

        if (!query_.empty())
        {
            result.append("?");
            result.append(print_query(query_));
        }

        if (!fragment_.empty())
        {
            result.append("#");
            result.append(fragment_);
        }

        return result;
    }

    

    Url& Url::abspath()
    {
        std::string copy;
        std::vector<size_t> segment_starts;

        if (path_.size() >= 1 && path_[0] == '/')
        {
            copy.append(1, '/');
            segment_starts.push_back(0);
        }

        bool directory = false;
        size_t previous = 0;
        size_t index = 0;
        for (index = path_.find('/')
            ; index != std::string::npos
            ; previous = index + 1, index = path_.find('/', index + 1))
        {
            // Skip empty segments
            if (index - previous == 0)
            {
                continue;
            }

            if ((index - previous == 2)
                && path_[previous] == '.'
                && path_[previous + 1] == '.')
            {
                if (!segment_starts.empty())
                {
                    copy.resize(segment_starts.back());
                    segment_starts.pop_back();
                }
                directory = true;
            }
            else if ((index - previous == 1) && path_[previous] == '.')
            {
                directory = true;
            }
            else
            {
                segment_starts.push_back(copy.length());
                copy.append(path_, previous, index - previous);
                copy.append(1, '/');
                directory = false;
            }
        }

        // Handle the last segment
        index = path_.length();
        if (previous == path_.length())
        {
            directory = true;
        }
        else if ((index - previous == 1) && path_[previous] == '.')
        {
            directory = true;
        }
        else if ((index - previous == 2)
                && path_[previous] == '.'
                && path_[previous + 1] == '.')
        {
            if (!segment_starts.empty())
            {
                copy.resize(segment_starts.back());
            }
            directory = true;
        }
        else
        {
            copy.append(path_, previous, index - previous);
            copy.append(1, '/');
            directory = false;
        }

        if (!directory && copy.size() >= 1)
        {
            copy.resize(copy.size() - 1);
        }
        else if (directory && copy.empty())
        {
            copy.append(1, '/');
        }
        path_.assign(copy);

        return *this;
    }

    Url& Url::relative_to(const Url& other)
    {
        // If this scheme does not use relative, return it unchanged
        if (USES_RELATIVE.find(scheme_) == USES_RELATIVE.end())
        {
            return *this;
        }

        // Support scheme-relative URLs
        if (scheme_.empty())
        {
            scheme_ = other.scheme_;
        }

        // If this is an absolute URL (or scheme-relative), return early
        if (!host_.empty()) {
            return *this;
        }

        // If it's not an absolute URL, we need to copy the other host and port
        host_ = other.host_;
        port_ = other.port_;
        userinfo_ = other.userinfo_;

        // If the path portion is absolute, then bail out early.
        if (!path_.empty() && path_.front() == '/')
        {
            return *this;
        }

        // Otherwise, this is a path that need to be evaluated relative to the other. If
        // there is no '/', then we just keep our current path if it's not empty.
        if (path_.empty())
        {
            if (params_.empty())
            {
                path_ = other.path_;
                params_ = other.params_;
                has_params_ = other.has_params_;
                if (query_.empty())
                {
                    query_ = other.query_;
                }
            }
            else
            {
                path_.assign(other.path_, 0, other.path_.rfind('/') + 1);
            }

            if (fragment_.empty())
            {
                fragment_ = other.fragment_;
            }
        }
        else
        {
            size_t index = other.path_.rfind('/');
            if (index != std::string::npos)
            {
                path_ = other.path_.substr(0, index + 1) + path_;
            }
            else if (!host_.empty())
            {
                path_ = "/" + path_;
            }
        }

        return *this;
    }

  //  Url& Url::escape(bool strict)
  //  {
		//path_ = ::Url::escape(path_, PATH, strict);
		//params_ = ::Url::escape(params_, QUERY, strict);
		//userinfo_ = ::Url::escape(userinfo_, USERINFO, strict);
  //      return *this;
  //  }

    std::string escape(const std::string& copy, const CharacterClass& safe, bool strict)
    {		
        std::string str(copy);
        size_t dest = 0;
        // Allocate space pessimistically -- if every entity is expanded, it will take 3x
        // the space.
        str.resize(copy.length() * 3);
        for (size_t src = 0; src < copy.length(); ++src)
        {
            if (copy[src] == '%' && (copy.length() - src) >= 2)
            {
                // Read ahead to see if there's a valid escape sequence. If not, treat
                // this like a normal character.
                if (Url::HEX_TO_DEC[copy[src+1]] != -1 && Url::HEX_TO_DEC[copy[src+2]] != -1)
                {
                    int value = (
                        Url::HEX_TO_DEC[copy[src+1]] * 16 + Url::HEX_TO_DEC[copy[src+2]]);

                    // In strict mode, we can only unescape parameters if they are both
                    // safe and not reserved
                    if (!strict || (strict && safe(value) && !Url::RESERVED(value)))
                    {
                        // Replace src + 2 with that byte, advance src to consume it and
                        // continue.
                        src += 2;
                        str[src] = value;
                    }
                    else
                    {
                        str[dest++] = copy[src++];
                        str[dest++] = ::toupper(copy[src++]);
                        str[dest++] = ::toupper(copy[src]);
                        continue;
                    }
                }
            }

            if (!safe(copy[src]))
            {
                // Not safe -- replace with %XX
                str[dest++] = '%';
                str[dest++] = Url::HEX.chars()[(copy[src] >> 4) & 0xF];
                str[dest++] = Url::HEX.chars()[copy[src] & 0xF];
            }
            else
            {
                str[dest++] = copy[src];
            }
        }
        str.resize(dest);
        return str;
    }

    std::string unescape(const std::string& copy)
    {
        std::string str(copy);
        size_t dest = 0;
        for (size_t src = 0; src < copy.length(); ++src, ++dest)
        {
            if (copy[src] == '%' && (copy.length() - src) >= 2)
            {
                // Read ahead to see if there's a valid escape sequence. If not, treat
                // this like a normal character.
                if (Url::HEX_TO_DEC[copy[src+1]] != -1 && Url::HEX_TO_DEC[copy[src+2]] != -1)
                {
                    int value = (
                        Url::HEX_TO_DEC[copy[src+1]] * 16 + Url::HEX_TO_DEC[copy[src+2]]);

                    // Replace src + 2 with that byte, advance src to consume it and
                    // continue.
                    src += 2;
                    str[dest] = value;
                    continue;
                }
            }

            // Either not a % or an incomplete entity
            str[dest] = copy[src];
        }
        str.resize(dest);
        return str;
    }

   
	const int Url::port() const 
	{
		if (port_ == 0 && !scheme_.empty())
        {
            auto it = PORTS.find(scheme_);
            if (it != PORTS.end())
            {
				return  it->second;
            }
        }
		return port_; 
	}


    Url& Url::remove_default_port()
    {
        if (port_ && !scheme_.empty())
        {
            auto it = PORTS.find(scheme_);
            if (it != PORTS.end() && port_ == it->second)
            {
                port_ = 0;
            }
        }
        return *this;
    }

    Url& Url::deuserinfo()
    {
        userinfo_.clear();
        return *this;
    }

    Url& Url::defrag()
    {
        fragment_.clear();
        return *this;
    }

    Url& Url::punycode()
    {
        check_hostname(host_);
        std::string encoded(Punycode::encodeHostname(host_));
        check_hostname(encoded);
        host_ = encoded;
        return *this;
    }

    Url& Url::unpunycode()
    {
        host_ = Punycode::decodeHostname(host_);
        return *this;
    }

    Url& Url::host_reversed()
    {
        std::reverse(host_.begin(), host_.end());
        for (size_t index = 0, position = 0; index < host_.size(); index = position + 1)
        {
            position = host_.find('.', index);
            if (position == std::string::npos)
            {
                std::reverse(host_.begin() + index, host_.end());
                break;
            }
            else
            {
                std::reverse(host_.begin() + index, host_.begin() + position);
            }
        }
        return *this;
    }

    void Url::check_hostname(std::string& host)
    {
        // Skip empty hostnames -- they are valid
        if (host.empty())
        {
            return;
        }

        size_t start = 0;
        size_t end = host.find('.');
        while (end != std::string::npos)
        {
            if ((end - start) > 63)
            {
                throw std::invalid_argument("Label too long.");
            }
            else if (end == start)
            {
                throw std::invalid_argument("Empty label.");
            }

            start = end + 1;
            end = host.find('.', start);
        }

        // For the final segment
        if ((host.size() - start) > 63)
        {
            throw std::invalid_argument("Label too long.");
        }
        else if (host.size() == start && start > 1)
        {
            // Remove a trailing empty segment
            host.resize(start - 1);
        }
    }
		
	Url& Url::operator&=(const std::tuple<std::string, std::string>& condition)
	{
		query_[std::get<0>(condition)] = std::get<1>(condition);
		return *this;
	}

	std::string Url::print_query(const std::map<std::string, std::string>& conditions) const
	{
		std::string result;
		std::map<std::string, std::string>::const_iterator it = conditions.cbegin();
		while (it != conditions.cend())
		{
			result.append(escape(it->first,QUERY,false) + "=" + escape(it->second,QUERY,false));
			it++;
			if (it != conditions.cend())
				result.append("&");
		}

		return result;
	}


	std::map<std::string, std::string> Url::parse_query(const std::string& q)
	{
		std::map<std::string, std::string> conditions_map;
		std::vector<std::string> conditions = split_query(q);
		std::vector<std::string>::iterator it = conditions.begin();

		while (it != conditions.end())
		{
			std::string condition = *it;

			int index = condition.find_first_of('=');
			if (index != std::string::npos)
			{
				std::string name = condition.substr(0, index - 1);
				std::string value = condition.substr(index + 1);
				conditions_map[unescape(name)] = unescape(value);
			}
			it++;
		}
		return conditions_map;
	}
	
	Url& Url::operator/=(const std::string& right)
	{
		if (right.empty())
			return *this;

		if (this->path_.length() > 0 && this->path_.at(this->path_.length() - 1) != '/')
		{
			this->path_ += "/";
		}

		std::string segment = right;
		while (segment.at(0) == '/') {
			segment = segment.substr(1);
		}
		
		this->path_ += segment;
		
		return *this;
	}

	Url operator/(const Url& left, const std::string& right)
	{
		Url result(left);

		result /= right;

		return result;
	}
		
};





