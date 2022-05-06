#include "../include/Error.hpp"


Error::Error()
{
	this->error_code = "200";
	this->error_msg = "OK";
	this->content_msg = "Content-Type: text/html; charset=utf-8\n\n";
	this->redir = 0;
}

Error::~Error()
{}

Error::Error(Error const & other)
{
	this->error_page = other.error_page;
	this->error_code = other.error_code;
	this->error_msg = other.error_msg;
	this->content_msg = other.content_msg;
	this->redir = other.redir;
}

Error &Error::operator=(Error const & other)
{
	if (this != &other)
	{
		this->error_code = other.error_code;
		this->error_page = other.error_page;
		this->error_msg = other.error_msg;
		this->content_msg = other.content_msg;
		this->redir = other.redir;
	}
	return (*this);
}

void Error::SetErrorPage(std::map<int, std::string> other)
{
	this->error_page = other;
}

void Error::SetErrorCode(std::string code)
{
	this->error_code = code;
}

void Error::SetErrorMsg(std::string msg)
{
	this->error_msg = msg;
}

void Error::SetContentMsg(std::string msg)
{
	this->content_msg = msg;
}

void Error::SetRedir(int i)
{
	this->redir = i;
}

std::string Error::GetErrorCode(void)
{
	return this->error_code;
}

std::string Error::GetErrorMsg(void)
{
	return this->error_msg;
}

std::string Error::GetContentMsg(void)
{
	return this->content_msg;
}

int Error::GetRedir(void)
{
	return this->redir;
}

std::string Error::error_301(std::string path)
{
	this->error_code = "301";
	this->error_msg = "Moved Permanently";
	this->content_msg = "Location: " + path + "\n";
	return "";
}

std::string Error::error_302(std::string path)
{
	this->error_code = "302";
	this->error_msg = "Found";
	this->content_msg = "Location: " + path + "\n";
	return "";
}

std::string Error::error_403(void)
{
	std::map<int, std::string>::iterator it = this->error_page.find(403);
	std::ifstream input;
	std::stringstream buff;
	std::string ret;

	this->error_code = "403";
	this->error_msg = "Forbidden";
	if (it != this->error_page.end())
	{
		input.open(it->second.c_str());
		buff << input.rdbuf();
		ret = buff.str();
		input.close();
		return ret;
	}
	ret = "<html>\n\t<head>\n\t\t<title>403 Forbidden</title>\n\t</head>\n\t<body bgcolor=";
	ret += '"';
	ret += "white";
	ret += '"';
	ret += ">\n\t\t<center>\n\t\t\t<hl>403 Forbidden</hl>\n\t\t</center>\n\t\t<hr>\n\t\t<center>webserveur fleger/pacorrei</center>\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t</body>\n</html>";
	return ret;
}

std::string Error::error_404(void)
{
	std::map<int, std::string>::iterator it = this->error_page.find(404);
	std::ifstream input;
	std::stringstream buff;
	std::string ret;

	this->error_code = "404";
	this->error_msg = "Not Found";
	if (it != this->error_page.end())
	{
		input.open(it->second.c_str());
		buff << input.rdbuf();
		ret = buff.str();
		input.close();
		return ret;
	}
	ret = "<html>\n\t<head>\n\t\t<title>404 Not Found</title>\n\t</head>\n\t<body bgcolor=";
	ret += '"';
	ret += "white";
	ret += '"';
	ret += ">\n\t\t<center>\n\t\t\t<hl>404 Not Found</hl>\n\t\t</center>\n\t\t<hr>\n\t\t<center>webserveur fleger/pacorrei</center>\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t</body>\n</html>";
	return ret;
}

std::string Error::error_405(void)
{
		std::map<int, std::string>::iterator it = this->error_page.find(405);
	std::ifstream input;
	std::stringstream buff;
	std::string ret;

	this->error_code = "405";
	this->error_msg = "Method Not Allowed";
	if (it != this->error_page.end())
	{
		input.open(it->second.c_str());
		buff << input.rdbuf();
		ret = buff.str();
		input.close();
		return ret;
	}
	ret = "<html>\n\t<head>\n\t\t<title>405 Method Not Allowed</title>\n\t</head>\n\t<body bgcolor=";
	ret += '"';
	ret += "white";
	ret += '"';
	ret += ">\n\t\t<center>\n\t\t\t<hl>405 Method Not Allowed</hl>\n\t\t</center>\n\t\t<hr>\n\t\t<center>webserveur fleger/pacorrei</center>\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t</body>\n</html>";
	return ret;
}

std::string Error::error_204(void)
{
	this->error_code = "204";
	this->error_msg = "No Content";
	return "";
}

std::string Error::error_431(void)
{
	std::map<int, std::string>::iterator it = this->error_page.find(405);
	std::ifstream input;
	std::stringstream buff;
	std::string ret;

	this->error_code = "431";
	this->error_msg = "Request Header Fields Too Large";
	if (it != this->error_page.end())
	{
		input.open(it->second.c_str());
		buff << input.rdbuf();
		ret = buff.str();
		input.close();
		return ret;
	}
	ret = "<html>\n\t<head>\n\t\t<title>431 Request Header Fields Too Large</title>\n\t</head>\n\t<body bgcolor=";
	ret += '"';
	ret += "white";
	ret += '"';
	ret += ">\n\t\t<center>\n\t\t\t<hl>431 Request Header Fields Too Large</hl>\n\t\t</center>\n\t\t<hr>\n\t\t<center>webserveur fleger/pacorrei</center>\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t</body>\n</html>";
	return ret;
}