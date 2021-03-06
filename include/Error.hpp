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
		void SetErrorPage(std::map<int, std::string> other);
		void SetErrorCode(std::string code);
		void SetErrorMsg(std::string msg);
		void SetContentMsg(std::string msg);
		void SetRedir(int i);
		std::string GetErrorCode(void);
		std::string GetErrorMsg(void);
		std::string GetContentMsg(void);
		int GetRedir(void);
		std::string error_301(std::string path);
		std::string error_302(std::string path);
		std::string error_403(void);
		std::string error_404(void);
		std::string error_405(void);
		std::string error_204(void);
		std::string error_431(void);
	private:

		std::map<int, std::string> error_page;
		std::string error_code;
		std::string error_msg;
		std::string content_msg;
		int	redir;


};

#endif