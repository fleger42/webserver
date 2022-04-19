#include "../include/Error.hpp"


Error::Error()
{
	this->error_code = "200";
}

Error::~Error()
{}

Error::Error(Error const & other)
{
	this->error_page = other.error_page;
	this->error_code = other.error_code;
}

Error &Error::operator=(Error const & other)
{
	if (this != &other)
	{
		this->error_code = other.error_code;
		this->error_page = other.error_page;
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

std::string Error::GetErrorCode(void)
{
	return this->error_code;
}

std::string Error::error_404(void)
{
	std::map<int, std::string>::iterator it = this->error_page.find(404);
	std::ifstream input;
	std::stringstream buff;
	std::string ret;

	if (it != this->error_page.end())
	{
		std::cout << "WE OPEN IT!" << std::endl;
		input.open(it->second.c_str());
		buff << input.rdbuf();
		ret = buff.str();
		input.close();
		this->error_code = "404";
		return ret;
	}
	ret = "<html>\n\t<head>\n\t\t<title>404 Not Found</title>\n\t</head>\n\t<body bgcolor=";
	ret += '"';
	ret += "white";
	ret += '"';
	ret += ">\n\t\t<center>\n\t\t\t<hl>404 Not Found</hl>\n\t\t</center>\n\t\t<hr>\n\t\t<center>webserveur fleger/pacorrei</center>\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t\t<!-- a padding to disable MSIE and Chrome friendly error page -->\n\t</body>\n</html>";
	this->error_code = "404";
	std::cout << "NOT OPEN" << std::endl;
	return ret;
}

std::string Error::error_204(void)
{
	std::string ret;

	this->error_code = "204";
	return ret;
}