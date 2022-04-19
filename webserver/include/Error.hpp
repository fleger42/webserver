#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <map>



class Error
{
	public :

		Error();
		Error(Error const & other);
		~Error();

		Error &operator=(Error const & other);
		void SetErrorPage(std::map<int, std::string> const other);
		void SetErrorCode(std::string code);
		std::string GetErrorCode(void);
		std::string error_404(void);
		std::string error_204(void);
	
	private:

		std::map<int, std::string> error_page;
		std::string error_code;


};

#endif