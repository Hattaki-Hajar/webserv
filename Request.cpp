#include "Request.hpp"
# include <cstdio>

template <typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

std::string generate_request_file() {
    // Seed for the random number generator
    std::srand(static_cast<unsigned>(time(0)));

    const char hex_chars[] = "0123456789abcdef";

    std::string uuid;

    // Generate 32 random hexadecimal characters
    for (int i = 0; i < 32; ++i) {
        uuid += hex_chars[std::rand() % 16];
    }

    // Insert hyphens at appropriate positions
    uuid.insert(8, 1, '-');
    uuid.insert(13, 1, '-');
    uuid.insert(18, 1, '-');
    uuid.insert(23, 1, '-');

    return uuid;
}

/*  constructors destructors  */
Request::Request() {
	_size_read = 0;
	_chunk_read = 0;
	_chunks_size = 0;
	_status_code = 200;
	_request_headers = "";
	_remaining = NULL;
	_remaining_size = 0;
	_is_file_open = false;
	_headers_read = false;
	_end_of_request = false;
}

Request::~Request() {
	_file.close();
}

/*  setters  */
/*	getters	*/
std::map<std::string, std::string>	Request::get_headers() const {
	return (_headers);
}
const request_line	&Request::get_request_line() const {
	return (_request_line);
}
long	Request::get_size_read() const {
	return (_size_read);
}
std::fstream	&Request::get_file() {
	return (_file);
}
bool	Request::get_end_of_request() const {
	return (_end_of_request);
}
unsigned int	Request::get_status_code() const {
	return (_status_code);
}
/*	additional functions	*/
void	Request::is_req_well_formed(void) {
	// Check if Transfer-Encoding header exist and is different to “chunked”.
	if (_headers.find("Transfer-Encoding") == _headers.end() && _headers.find("Content-Length") == _headers.end() && _request_line.method == "POST")
		_status_code = 400;
	// if (_headers.find("Content-Type") == _headers.end() && _request_line.method == "POST")
	// 	_status_code = 400;
	if (_headers.find("Transfer-Encoding") != _headers.end() && _headers["Transfer-Encoding"] != "chunked")
		_status_code = 501;
	// Check if Transfer-Encoding not exist, Content-Length not exist and the method is Post
	if (_headers.find("Transfer-Encoding") != _headers.end() && _headers.find("Content-Length") != _headers.end())
		_status_code = 400;
	// Check the request uri contain a character not allowed.
	if (_request_line.uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
		_status_code = 400;
	// Check if the request uri contain more the 2048 chars.
	if (_request_line.uri.length() > 2048)
		_status_code = 414;
	// Check if the content type is not supported.
	if (_headers.find("Content-Type") != _headers.end()) {
		if (_headers["Content-Type"].find("multipart") != std::string::npos)
			_status_code = 501;
	}
	// Check if the request method is not supported.
	if (_request_line.method != "GET" && _request_line.method != "DELETE" && _request_line.method != "POST")
		_status_code = 501;
}

std::string	Request::generate_extension() {
	std::string extension;
	if (_headers.find("Content-Type") != _headers.end()) {
		if (_headers["Content-Type"] == "text/plain")
			extension = ".txt";
		else if (_headers["Content-Type"] == "text/csv")
			extension = ".csv";
		else if (_headers["Content-Type"] == "text/html")
			extension = ".html";
		else if (_headers["Content-Type"] == "text/css")
			extension = ".css";
		else if (_headers["Content-Type"] == "text/javascript")
			extension = ".js";
		else if (_headers["Content-Type"] == "audio/mpeg")
			extension = ".mp3";
		else if (_headers["Content-Type"] == "audio/wav")
			extension = ".wav";
		else if (_headers["Content-Type"] == "audio/webm")
			extension = ".weba";
		else if (_headers["Content-Type"] == "image/avif")
			extension = ".avif";
		else if (_headers["Content-Type"] == "image/webp")
			extension = ".webp";
		else if (_headers["Content-Type"] == "image/png")
			extension = ".png";
		else if (_headers["Content-Type"] == "image/jpeg")
			extension = ".jpeg";
		else if (_headers["Content-Type"] == "image/gif")
			extension = ".gif";
		else if (_headers["Content-Type"] == "video/webm")
			extension = ".webm";
		else if (_headers["Content-Type"] == "video/mp4")
			extension = ".mp4";
		else if (_headers["Content-Type"] == "application/vnd.openxmlformats-officedocument.wordprocessingml.document")
			extension = ".docx";
		else if (_headers["Content-Type"] == "application/octet-stream")
			extension = "";
		else if (_headers["Content-Type"] == "application/pdf")
			extension = ".pdf";
		else if (_headers["Content-Type"] == "application/json")
			extension = ".json";
		else if (_headers["Content-Type"] == "application/xml")
			extension = ".xml";
		else if (_headers["Content-Type"] == "application/zip")
			extension = ".zip";
	}
	else
		extension = ".txt";
	
	return (extension);
}

void	Request::split_request(char *buffer, ssize_t bytesread) {
	ssize_t	i = 0;
	while (i < bytesread && !_headers_read)
	{
		if (buffer[i] == '\r')
			if (buffer[i + 1] == '\n')
				if (buffer[i + 2] == '\r')
					if (buffer[i + 3] == '\n')
					{
						_headers_read = true;
						this->parse_request();
						// parse for error codes
						is_req_well_formed();
						i += 4;
						break ;
					}
		_request_headers += buffer[i];
		i++;
	}
	if (_request_line.method == "POST" || _request_line.method == "DELETE" || _request_line.method == "GET") {
		if (_request_line.method == "POST" && _status_code == 200) {
			if (!_is_file_open) {
				std::string	extension = generate_extension();
				_file_path = "/nfs/homes/";
				_file_path += USER;
				_file_path += "/.cache/" + generate_request_file() + extension;
				_file.open(_file_path.c_str(), std::ios::out | std::ios::app);
				if (!_file.good()) {
					return ;
				}
				_is_file_open = true;
			}
		}
		// Check if the request is chunked.
		if (_headers.find("Transfer-Encoding") != _headers.end() && _headers.find("Transfer-Encoding")->second == "chunked") {
			if (!_chunks_size) {
				// Check if there is a remaining from the previous buffer.
				if (_remaining)	{
					// Create a temporaty tmp to store the buffer.
					char *tmp = new char[bytesread - i];
					// Copy the buffer into tmp.
					ssize_t	j = 0;
					ssize_t k = 0;
					while (j < bytesread - i) {
						tmp[j] = buffer[i + j];
						j++;
					}
					// Create a new buffer with the size of the remaining + the size of the tmp.
					buffer = new char[_remaining_size + bytesread - i];
					// Copy the remaining into the new buffer.
					j = 0;
					while (j < _remaining_size) {
						buffer[j] = _remaining[j];
						j++;
					}
					// Copy the tmp into the new buffer.
					while (k < bytesread - i) {
						buffer[j] = tmp[k];
						j++;
						k++;
					}
					// Update the bytesread.
					bytesread += _remaining_size;
					// Update the index.
					i = 0;
					// Skip the \r\n Before the chunck size.
					if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
						i += 2;
					}
					// Delete the allocated buffers.
					delete [] tmp;
					delete [] _remaining;
					_remaining = NULL;
				}
				// Allocate for line aka the buffer holder of the size of the next chunk.
				ssize_t k = i;
				while (k < bytesread) {
					if (buffer[k] == '\r' && buffer[k + 1] == '\n') {
						break;
					}
					k++;
				}
				ssize_t line_size = k - i;
				char *line = new char[line_size + 1];
				bzero(line, line_size + 1);
				k = 0;
				// Copy the next chunck size into line.
				while (k < line_size + 1) {
					if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
						i += 2;
						break;
					}
					line[k] = buffer[i];
					i++;
					k++;
				}
				// Convert the size of the next chunck from hex to decimal.
				std::stringstream hex;
				hex << std::hex << line;
				hex >> _chunks_size;
				std::cout << "chunk size: " << _chunks_size << std::endl;
				if (_chunks_size == 0) {
					std::cout << "end of request in calculating chunk" << std::endl;
					_end_of_request = true;
					return ;
				}
			}
			// Put the chunk into the file.
			while (_chunk_read < _chunks_size && i < bytesread) {
					if (_request_line.method == "POST" && _status_code == 200) {
						_file.put(buffer[i]);
						_file.flush();
					}
					_chunk_read++;
					_size_read++;
					i++;
			}
			// Check if the chunk is complete and be ready to the next one.
			if (_chunk_read  == _chunks_size && i < bytesread) {
				// reset the variables for the next chunk.
				_chunk_read = 0;
				_chunks_size = 0;
				// Allocate for a buffer which will hold the remaining of the buffer.
				_remaining = new char[bytesread - i];
				_remaining_size = bytesread - i;
				int j = 0;
				// Copy the remaining into the buffer _remaining.
				while(i < bytesread) {
					_remaining[j] = buffer[i];
					j++;
					i++;
				}
				// Check if the remaining is the end of the request.
				if (_remaining_size >= 5) {
					if (_remaining[0] == '\r' && _remaining[1] == '\n' && _remaining[2] == '0' && _remaining[3] == '\r' && _remaining[4] == '\n') {
						delete [] _remaining;
						_remaining = NULL;
						_end_of_request = true;
						std::cout << "end of request in remaining" << std::endl;
						return ;
					}
				}
			}
		}
		// If the request is not chunked.
		else if (_headers.find("Content-Length") != _headers.end() && _headers.find("Content-Length")->second != "0") {
			if (bytesread - i)	{
				if ( _request_line.method == "POST" && _status_code == 200) {
					_file.write(buffer + i, bytesread - i);
					_file.flush();
				}
				_size_read += bytesread - i;
			}
			// Check if the request is complete.
			if (get_size_read() == atol(_headers.find("Content-Length")->second.c_str())) {
				_end_of_request = true;
				return ;
			}
		}
		// else if (_request_line.method == "GET" || _request_line.method == "DELETE" || _request_line.method == "POST") {
		// 	_end_of_request = true;
		// 	return ;
		// }
	}
}

void	Request::parse_request() {
	if (_request_headers.empty())
		return ;
	std::istringstream ss(_request_headers);
	std::string line;
	getline(ss, line);
	line = line.substr(0, line.length() - 1);

	_request_line.method = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	_request_line.uri = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	_request_line.version = line.substr(0, line.find(' '));
	while (getline(ss, line))
	{
		if (line.find("\r") == std::string::npos) {
			line = line.substr(0, line.length());
		} else {
			line = line.substr(0, line.length() - 1);
		}
		_headers[line.substr(0, line.find(':'))] = line.substr(line.find(':') + 2);
	}
	_request_headers.clear();
}
