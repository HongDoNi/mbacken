#include "String.h"
using namespace b2be::utils;

string String::_Base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * Implements a string substr function that is aware of utf8
 * character encoding.
 *
 * @param source The source string to take the sub-string from
 * @param start The start position (0-n) to take the sub-string from
 * @param length The number of character to take from the start position
 * @return Sub-string or empty string if the range is outside the size of the string
 */
string String::FixField(const string &source, int start, int length)
{
	string result = "";

	if (utf8::unchecked::distance(source.begin(), source.end()) < start)
		return result;	

	int pos = 0;
	int len = 0;
	
	for (string::const_iterator it = source.begin(); it != source.end(); pos++)
	{		
		if (pos >= start)
		{			
			string r;
			utf8::unchecked::append(utf8::unchecked::next(it), back_inserter(r));
			result.append(r);

			len++;
			if (len == length)
				break;
		}
		else
			utf8::unchecked::next(it);
	}

	return result;
}

/**
 * Finds the field at the specified field number
 * and returns its data. The bounds of each field are governed
 * by the delimiter character.
 *
 * @param source The source string to take the field data from
 * @param fieldNumber The number (from 1-n) of the field you wish to take data from
 * @param delimiter The character which delimits the fields in the source
 * @return The data found in the field
 */
string String::TakeField(const string &source, int fieldNumber, char delimiter)
{
	int length = 0;
	int start = 0;
	int fieldNum = 1;

	for(unsigned int i = 0; i < source.length(); i++, length++)
	{
		if(source.at(i) == delimiter || (i == (source.length() - 1)) ) //Find delimiter
		{
			if(i == (source.length() - 1) && source.at(i) != delimiter)
				length++;

			if(fieldNum == fieldNumber) //Found field
				return source.substr(start, length);

			fieldNum++;
			start = i + 1;
			length = -1;
		}
	}

	return ""; //Couldn't find field
}

/**
 * Finds the field at the specified field number
 * and returns its data. The bounds of each field are governed
 * by the delimiter string.
 *
 * @param source The source string to take the field data from
 * @param fieldNumber The number (from 1-n) of the field you wish to take data from
 * @param delimiter The string (1-n characters) which delimits the fields in the source
 * @return The data found in the field
 */
string String::TakeField(const string &source, int fieldNumber, string delimiter)
{
	if(delimiter.length() == 1) //if the delimiter is a char use other TakeField - more efficient
		return TakeField(source, fieldNumber, delimiter[0]);
		
	int length = 0;
	int start = 0;
	int fieldNum = 1;
	unsigned int i = 0;
	string src_partial = "";

	while(i < source.length())
	{
		src_partial = source.substr(i, delimiter.length());
		if(strcmp(src_partial.c_str(), delimiter.c_str()) == 0) //Find delimiter
		{
			if(fieldNum == fieldNumber) //Found field
			{
				if(length == 0)
					return "";
				else
					return source.substr(start, length);
			}

			fieldNum++;
			start = i + delimiter.length();
			length = 0;
			i = i + delimiter.length();
		}
		else
		{
			i++;
			length++;
		}
	}

	if(start != 0 && fieldNumber == fieldNum) 
		return source.substr(start);
	else
		return ""; //Couldn't find field
}

/**
 * Finds the field at the specified field number
 * and returns its data. The bounds of each field are governed
 * by standard delimit rules of CSV.
 *
 * @param source The source string to take the field data from
 * @param fieldNumber The number (from 1-n) of the field you wish to take data from
 * @return The data found in the field
 */
string String::TakeFieldCsv(string source, int fieldNumber)
{
    int pos = -1;
    int currentFieldNumber=0;
    string field;

    while (pos < (int)(source.length()))
    {
        //Get a field
        field = ParseFieldCsv(source, pos);
        
        //update field counter
        currentFieldNumber++;

        //If found the right field
        if (currentFieldNumber == fieldNumber)
            return field;
    }
    
	return "";
}

/**
 * Returns a count of the number of fields found in
 * the source, based on the delimit character specified.
 *
 * @param source The source string to perform the count on
 * @param delimiter The character which delimits the fields in the source
 * @return Count of the number of fields in the source. Will return "1" even if no delimit chars are present.
 */
int String::CountField(const string &source, char delimiter)
{
	int count = 1; //always assume 1 field to start with
	
	for(unsigned int i = 0; i < source.length(); i++)
	{
		if(source.at(i) == delimiter)
			count++;
	}
	
	return count;
}

/**
 * Returns a count of the number of fields found in
 * the source, based on the delimit string specified.
 *
 * @param source The source string to perform the count on
 * @param delimiter The string (1-n characters) which delimits the fields in the source
 * @return Count of the number of fields in the source
 */
int String::CountField(const string &source, string delimiter)
{
	int count = 1;
	
	for(unsigned int i = 0; i < source.length(); i++)
	{
		if(source.substr(i, delimiter.length()) == delimiter)
		{
			count++;
			i += delimiter.length() - 1;
		}
	}

	return count;
}


/**
 * Checks if a given string begins with another string.
 *
 * @param source The input string to be checked.
 * @param prefix The string to look for at the start of the source string.
 * @return True if source starts with prefix, false otherwise.
 */
bool String::IsBeginWith(const string &source, string prefix)
{
	if(source.length() < prefix.length())
		return false;
		
	if(source.substr(0, prefix.length()) == prefix)
		return true;
	else
		return false;
}

/**
 * Checks if a given string ends with another string.
 *
 * @param source The input string to be checked.
 * @param suffix The string to look for at the end of the source string.
 * @return True if source ends with suffix, false otherwise.
 */
bool String::IsEndWith(const string &source, string suffix)
{
	if(source.length() < suffix.length())
		return false;
		
	if(source.substr((source.length() - suffix.length())) == suffix)
		return true;
	else
		return false;
}


/**
 * Checks if a string contains only numeric characters.
 * Allows for certin characters to be ignored by listing them in the second paramter.
 *
 * @param number String to check
 * @param ignoreChars String containing one or more characters to ignore when performing check
 */
bool String::IsNumeric(const string &number, string ignoreChars)
{
	unsigned int numericCharCount = 0;
	unsigned int len = number.length();
	
	if(len == 0)
		return false;

	for(unsigned int i = 0; i < len; i++)
	{
		if(number[i] < '0' || number[i] > '9')
		{
			bool is_ignore = false;
			for(unsigned int j = 0; j < ignoreChars.length() && !is_ignore; j++)
			{
				if(ignoreChars[j] == number[i])
				{
					is_ignore = true;
				}
			}
			if(!is_ignore)
				return false;
		}
		else
			numericCharCount++;
	}

	if(numericCharCount == 0)
		return false;

	return true;
}

/**
 * Removes all occurrences of the specified character from 
 * the string.
 *
 * @param source The source string to remove the character from
 * @param remove The character to remove from the source string
 * @return The string after the remove has been performed
 */
string String::RemoveChar(const string &source, char remove)
{
	string res = "";
	if (remove == 0)
	{
		res = source;
		return res;
	}

	int len = source.length();
	for(int i=0; i<len; i++)
	{
		if (source[i] != remove)
			res.push_back(source[i]);
	}

	return res;
}

/**
 * Removes all occurrences of the specified characters from 
 * the string.
 *
 * @param source The source string to remove the character from
 * @param remove The characters to remove from the source string
 * @return The string after the remove has been performed
 */
string String::RemoveChars(const string &source, const string &remove)
{
	string res = "";
	if (remove.length() == 0)
	{
		res = source;
		return res;
	}

	int slen = source.length();
	for (int i = 0; i < slen; i++)
	{
		bool isMatch = false;
		int rlen = remove.length();
		for (int j = 0; j < rlen; j++)
		{
			if (source[i] == remove[j])
			{
				isMatch = true;
				break;
			}
		}
		if (!isMatch) 
		{
			res.push_back(source[i]);
		}
	}

	return res;
}

/**
 * Replaces all occurrences of the 'find' character with the
 * 'replace' character.
 * 
 * @param source The source string to perform the replace on
 * @param find The character to find and replace
 * @param replace The character to replace the find character with
 * @return The string after the replace has been performed
 */
string String::ReplaceChar(const string &source, char find, char replace)
{
	string res = "";
	
	int len = source.length();
	
	for(int i=0; i<len; i++)
	{
		if (source[i] == find)
			res.push_back(replace);
		else
			res.push_back(source[i]);
	}
		
	return res;
}

/**
 * Replaces all occurrences of the 'find' string with the
 * 'replace' string.
 * 
 * @param source The source string to perform the replace on
 * @param find The string to find and replace
 * @param replace The string to replace the find character with
 * @return The string after the replace has been performed
 */
string String::Replace(const string &source, const string &find, const string &replace)
{
	return String::Replace(source, find, replace, "");
}

/**
 * Replaces all occurrences of the 'find' string with the
 * 'replace' string.
 * 
 * @param source The source string to perform the replace on
 * @param find The string to find and replace
 * @param replace The string to replace the find character with
 * @param escape A string which may precede the find string to prevent the replacement taking place
 * @return The string after the replace has been performed
 */
string String::Replace(const string &source, const string &find, const string &replace, const string &escape)
{
	string outputFileContents = "";

	if (find == "")
	{
		outputFileContents = source;
		return outputFileContents;
	}
	
	size_t findStringLength = find.length();
	size_t escapeStringLength = escape.length();
	
	//resets the replacements details so we start from the start of the content again
	size_t position = 0; 
	size_t prevPosition = 0; 	
	
	for( ;(position = source.find(find, prevPosition)) != std::string::npos; )
	{
		outputFileContents.append(source.substr(prevPosition,(position-prevPosition)));
		
		//check for escape string prior to the found string (if present, don't replace)
		bool isEscaped = false;
		int escapeStartPos = (int)position - (int)escapeStringLength;
		if(escapeStartPos >= 0 && escape.length() != 0)
		{
			string possibleEscapeString = source.substr(escapeStartPos, escapeStringLength);
			if(possibleEscapeString == escape)
				isEscaped = true;
		}
	
		if(!isEscaped)
			outputFileContents.append(replace);
		else
			outputFileContents.append(source.substr(position,findStringLength));

		position += findStringLength;			
		prevPosition = position;
	}
	outputFileContents.append(source.substr(prevPosition));
	
	return outputFileContents; 
}

/**
 * Swap all occurrences of one character for another, and vice-versa.
 *
 * @param source The source string to perform the swap on
 * @param swapChar1 The 1st character which should be swapped for the 2nd
 * @param swapChar2 The 2nd character which should be swapped for the 1st
 * @return The string after the swap has been performed
 */ 
string String::SwapChar(const string &source, char swapChar1, char swapChar2)
{
	string result = "";
	int len = source.length();

	if(len == 0)
		return "";

	for(int i = 0; i < len; i++)
	{
		if(source[i] == swapChar1)
			result.append(sizeof(char), swapChar2);
		else if(source[i] == swapChar2)
			result.append(sizeof(char), swapChar1);
		else
			result.append(sizeof(char), source[i]);
	}

	return result;
}

/**
 * Trims all white space from the start and end of a string.
 * White space character set {' ','\n','\r','\t'}.
 *
 * @param source The input string to be trimmed.
 * @return Output string containing the trimmed string.
 */
string String::Trim (const string &source)
{
    unsigned int src_length = source.length();
    if ( src_length == 0)
            return "";

    string res = "";
    unsigned int start_idx=0, end_idx=0;
    unsigned int i;

    //find start_idx from the beginning
    for(i = 0; i < src_length; i++)
    {
            if( source[i] != ' '  &&
            	source[i] != '\t' &&
            	source[i] != '\r' &&
            	source[i] != '\n')
            {
                    start_idx = i;
                    break;
            }
    }

    if(i == src_length)     //no non-space character found, return "".
    {
            return res;
    }

    //find end_idx from the ending, it must be greater or equal to start_idx.
    for(i=(src_length-1); i >= start_idx; i--)
    {
            if( source[i] != ' '  &&
            	source[i] != '\t' &&
            	source[i] != '\r' &&
            	source[i] != '\n')
            {
                    end_idx = i;
                    break;
            }
    }

    res = source.substr(start_idx, (end_idx-start_idx+1));

    return res;
}


/**
 * Trims all specified chars from the start and end of a string.
 * The specified char set is defined by a string charset.
 *
 * @param source The input string to be trimmed.
 * @param charset The definition string of character set that need be trimmed, could be combination of '\n','\t','\r',' ' and other chars.
 * @return Output The string containing the trimmed string.
 */
string String::Trim (const string &source, const string &charset)
{
    if (source.empty())
    {
        return "";
    }

    if (charset.empty())
    {   // no charset definition, just return source string, do not trim.
        return source;
    }

    // Search for the last char not defined in charset
    string::size_type i_end = source.find_last_not_of(charset);
    string::size_type i_start;

    if (i_end == string::npos)
    {	// Find nothing, the whole string are trimmed
    	return "";
    }
    else
    {	// Since find a char from end, so must be able to find one from beginning.
    	i_start = source.find_first_not_of(charset);
    }

    // Trim string by start and end index
    return source.substr(i_start, i_end - i_start + 1);
}


/**
 * Converts a string to upper case.
 *
 * @param source The source string
 * @return The upper case output string
 */
string String::ToUpper(string source)
{
	int len = source.length();

	for(int i = 0; i < len; i++)
		source[i] = toupper(source[i]);
	
	return source;
}

/**
 * Converts a string to lower case.
 *
 * @param source The source string
 * @return The lower case output string
 */
string String::ToLower(string source)
{
	int len = source.length();

	for(int i = 0; i < len; i++)
		source[i] = tolower(source[i]);

	return source;
}

/**
 * Right/Left pad data from the specified input string 
 * 
 * @param in The input string
 * @param chr Replacement character
 * @param len Target output string length
 * @param padMethod Padding right or left
 */
string String::PadString(const string in, const char chr, const unsigned int len, PADMETHOD padMethod)
{
	//	init
	string out = "";

	//	main
	if (len <= in.length())
	{
		out = in;
	}
	else if (padMethod == PADLEFT)
	{	
		out.append((len - in.length()), chr);
		out.append(in);
	}
	else if (padMethod == PADRIGHT)
	{
		out.append(in);
		out.append((len - in.length()), chr);
	}
	else 
	{
		out = in;
	}

	//	end
	return out;
}


/**
 * Encodes a string using the Base64 encoding scheme.
 *
 * @param source The input string
 * @return The encoded output string
 */
string String::Base64Encode(const string &source) 
{
	unsigned char* input = (unsigned char *)source.c_str();
	unsigned int len = source.length();
		
	string ret = "";
	int i = 0;
	int j = 0;
	unsigned char ca3[3];
	unsigned char ca4[4];
	
	unsigned int lineCnt = 0;

	while(len--) 
	{
		ca3[i++] = *(input++);
		
		if(i == 3) 
		{
			ca4[0] = (ca3[0] & 0xfc) >> 2;
			ca4[1] = ((ca3[0] & 0x03) << 4) + ((ca3[1] & 0xf0) >> 4);
			ca4[2] = ((ca3[1] & 0x0f) << 2) + ((ca3[2] & 0xc0) >> 6);
			ca4[3] = ca3[2] & 0x3f;

			for(i = 0; (i < 4) ; i++) 
			{
				ret.push_back(_Base64Chars[ca4[i]]);
				if (ret.length() == (76+(78*lineCnt)))
				{
					ret.append("\r\n");
					lineCnt++;
				}
			}			
			
			i = 0;
		}
	}

	if(i) 
	{
		for(j = i; j < 3; j++) 
			ca3[j] = '\0';
		
		ca4[0] = (ca3[0] & 0xfc) >> 2;
		ca4[1] = ((ca3[0] & 0x03) << 4) + ((ca3[1] & 0xf0) >> 4);
		ca4[2] = ((ca3[1] & 0x0f) << 2) + ((ca3[2] & 0xc0) >> 6);
		ca4[3] = ca3[2] & 0x3f;

		for(j = 0; (j < i + 1); j++) 
		{
			ret.push_back(_Base64Chars[ca4[j]]);
			if (ret.length() == (76+(78*lineCnt)))
			{
				ret.append("\r\n");
				lineCnt++;
			}
		}		
		
		while((i++ < 3)) 
		{
			ret.append("=");
			if (ret.length() == (76+(78*lineCnt)))
			{
				ret.append("\r\n");
				lineCnt++;
			}
		}
	}
	
	// support RFC 2045 by ending lines at 76 chars with \r\n
	/*if(ret.length() > 76)
	{
		for(unsigned int pos = 76; pos < ret.length(); pos += 78) 
			ret.insert(pos, "\r\n");
	}*/
	
	return ret;
}

/**
 * Decodes a string which has been encoded using
 * the Base64 encoding scheme.
 *
 * @param source The source base64 encoded string
 * @return The decoded output string
 */
string String::Base64Decode(string source) 
{
	string::iterator it;
	for(it=source.begin(); it < source.end(); ++it) {
		if(*it == '\n' || *it == '\r') {
			source.erase(it);
			it--;
		} 
	}

	int len = source.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char ca4[4];
	unsigned char ca3[3];
	string ret;
	
	// scan the start of the string until actual base64 encoded chars appear
	while(len-- && (source[in_] != '=')) {
		if(isalnum(source[in_]) || (source[in_] == '+') || (source[in_] == '/')) {
			break;
		} else {
			in_++;
		}
	}
	
	// decode string
	while(len-- && (source[in_] != '=') && 
		(isalnum(source[in_]) || (source[in_] == '+') || (source[in_] == '/'))) {
		ca4[i++] = source[in_]; 
		in_++;
		
		if(i != 4) {
			continue;
		}
		
		for(i = 0; i <4; i++) {
			ca4[i] = _Base64Chars.find(ca4[i]);
		}
		
		ca3[0] = (ca4[0] << 2) + ((ca4[1] & 0x30) >> 4);
		ca3[1] = ((ca4[1] & 0xf) << 4) + ((ca4[2] & 0x3c) >> 2);
		ca3[2] = ((ca4[2] & 0x3) << 6) + ca4[3];

		for(i = 0; (i < 3); i++) {
			ret.push_back(ca3[i]);
		}
		
		i = 0;
	}

	if(i) {
		for (j = i; j < 4; j++) {
			ca4[j] = 0;
		}
		
		for (j = 0; j <4; j++) {
			ca4[j] = _Base64Chars.find(ca4[j]);
		}
		
		ca3[0] = (ca4[0] << 2) + ((ca4[1] & 0x30) >> 4);
		ca3[1] = ((ca4[1] & 0xf) << 4) + ((ca4[2] & 0x3c) >> 2);
		ca3[2] = ((ca4[2] & 0x3) << 6) + ca4[3];

		for (j = 0; (j < i - 1); j++) {
			ret.push_back(ca3[j]);
		}
	}

	return ret;
}

/**
 * Uses the iconv library to perform conversion between text encodings.
 *
 * Example usage:
 * 	string inputStr = "text to convert, possibly with encoding specific characters";
 * 	string convertedStr = "";
 * 	string errMsg = "";
 *
 * 	if (!convertTextEncoding("ISO-8859-1","UTF-8", inputStr, convertedStr,errMsg))
 * 	{
 * 		cout << "ERROR: " << errMsg << endl;
 * 	}
 * 	else
 * 	{
 * 		cout << convertedStr << endl;
 * 	}
 *
 * @param fromEncoding The name of the character encoding you want to convert from
 * @param toEncoding The name of the character encoding you want to convert to
 * @param inputStr The string to convert
 * @param outputStr The string which will be populated with the converted output
 * @param errMsg The variable which will be populated with the error message data if the conversion fails
 * @return True if the conversion was successful, otherwise false
 */ 
bool String::ConvertTextEncoding(string fromEncoding, string toEncoding, const string &inputStr, string &outputStr, string &errMsg)
{
	outputStr = "";
		
	//setup the conversion descriptor
	errno = 0;
	iconv_t icDescriptor = iconv_open(toEncoding.c_str(), fromEncoding.c_str());
	if ((iconv_t)(-1) == icDescriptor)		//Ticket 247: fix logic bug here
	{
		errMsg = "iconv_open failed with errno=" + String::ToString(errno) + ", ";

		if (errno == EMFILE)
			errMsg.append("EMFILE: max file descriptors open in calling process");
		else if (errno == ENFILE)
			errMsg.append("ENFILE: too many files are currently open in the system");
		else if (errno == ENOMEM)
			errMsg.append("ENOMEM: insufficient memory available");
		else if (errno == EINVAL)
			errMsg.append("EINVAL: encoding specified for conversion is not supported");
		else
			errMsg.append("UNKNOWN ERROR: most likely invalid text encoding specified");
		
		return false;
	}

	size_t in_buffer_size = inputStr.size() + 4;
	size_t out_buffer_size = in_buffer_size*2 + 8; //allows for all input characters to be converted to 2-byte characters

	char * in = NULL;
	char * out = NULL;

	try
	{
		in = new char[in_buffer_size];
		out = new char[out_buffer_size];
	}
	catch (bad_alloc&)
	{
		errMsg = "Buffer memory allocation fails.";
		return false;
	}

	memset(in,'\0', in_buffer_size);
	memset(out,'\0',out_buffer_size);
	memcpy(in, inputStr.c_str(), inputStr.size());

	size_t inSize = inputStr.size();
	size_t outSize = out_buffer_size;

	char* inptr = in;
	char* outptr = out;
	errno = 0;
	if ((size_t)(-1) == iconv(icDescriptor, &inptr, &inSize, &outptr, &outSize))
	{
		errMsg = "iconv failed with errno=" + String::ToString(errno) + ", ";
					
		if (errno == E2BIG)
			errMsg.append("E2BIG: insufficent space in output buffer");
		else if (errno == EILSEQ)
			errMsg.append("EILSEQ: input byte does not belong to specified encoding");
		else if (errno == EINVAL)
			errMsg.append("EINVAL: incomplete character at end of input buffer");
		else
			errMsg.append("UNKNOWN ERROR: most likely invalid text encoding specified");

		// Free dynamically allocated char * buffers
		delete [] in;
		delete [] out;

		return false;
	}

	outputStr.append(out, out_buffer_size - outSize);

	iconv_close(icDescriptor);

	// Free dynamically allocated char * buffers
	delete [] in;
	delete [] out;

	errMsg = "";
	return true;
}

/**
 * Private method to assist the parsing of field in
 * CSV string.
 */
string String::ParseFieldCsv(string &data, int &pos)
{
    if (pos == (int)data.length() - 1)
    {
        pos++;
        // The last field is empty
        return "";
    }

    int fromPos = pos + 1;

    // Ignore leading spaces
    while(data[fromPos]==' '){fromPos++;}

    // Determine if this is a quoted field
    if (data[fromPos] == '"')
    {
        // Single quoted field at end of line
        if (fromPos == (int)data.length() - 1)
        {
            fromPos++;
            return "\"";
        }

        // Find the matching "end of field" Double quote
        int nextQuote = FindNextQuote(data,fromPos + 1);
        pos = (int)data.find(",",nextQuote); 
        if (pos == -1)
        {
            pos = (int)data.length();
        }
        
		string s = data.substr(fromPos + 1, nextQuote - fromPos - 1);

		//replace(s.begin(),s.end(),'\"', 'c');

		return s;
    }

    // The field ends in the next comma or EOL
    int nextComma = (int)data.find(",", fromPos);

    // Ignore trailing spaces
    while (data[fromPos] == ' ') { fromPos--; }

    // If "next comma" is not found = EOL
    if (nextComma == -1)
    {
        pos = (int)data.length();
        return data.substr(fromPos);
    }
    else
    {
        pos = nextComma;
        return data.substr(fromPos, nextComma - fromPos);
    }
}

/**
 * Private method to assist the parsing of field in
 * CSV string.
 */
int String::FindNextQuote(string &data, int startFrom)
{
    int i = startFrom - 1;
    while (++i < (int)data.length())
    {
        if (data[i] == '"')
        {
            // If there is 2 quotes, bypass the chars
            if (i < (int)data.length() - 1 && data[i + 1] == '"')
            {
				data.erase(i,1);					
                continue;
            }
            else
            {
                return i;
            }
        }
    }
	
    // If no quote found, return the end value of i (data.length())
    return i;
}

/**
 * Determines if the specified record matches the configured record id
 *
 * @param record The record to check against the configured record id
 * @param recordID Record ID
 * @param recordIDFieldType Record ID field type (Fixed, Delimited, CSV)
 * @param recordIDFieldPosition Record ID field position
 * @param recordIDFieldDelimiter Record ID field delimiter
 * @param recordIDFieldLength Recorrd ID field length
 * @return True if record matched, otherwise false
 */
bool String::IsMatchingRecordID(const string &record, string recordID, string recordIDFieldType, unsigned int recordIDFieldPosition, string recordIDFieldDelimiter, unsigned int recordIDFieldLength)
{
	if(recordIDFieldType == "Fixed")
	{
		string temp = String::FixField(record, (recordIDFieldPosition - 1), recordIDFieldLength);

		if (temp == recordID)
			return true;
	}
	else if(recordIDFieldType == "Delimited")
	{
		string temp = String::TakeField(record, recordIDFieldPosition, recordIDFieldDelimiter);

		if (temp == recordID)
			return true;	
	}
	else if(recordIDFieldType == "CSV")
	{
		string temp = String::TakeFieldCsv(record, recordIDFieldPosition);

		if (temp == recordID)
			return true;	
	}
	
	return false;
}

/**
 * Extracts data from the specified record using the configured data field options
 *
 * @param record The record to extract data from
 * @param dataFieldType Data field type (Fixed, Delimited, CSV, EDIFACT, Tradacom)
 * @param dataFieldPosition Data field position
 * @param dataSubFieldPosition Data sub-field position
 * @param dataFieldDelimiter Data field delimiter
 * @param dataFieldLength Data field length
 * @return String containing the extracted data, or blank if not found
 */
string String::GetDataFromRecord(const string &record, string dataFieldType, unsigned int dataFieldPosition, unsigned int dataSubFieldPosition, string dataFieldDelimiter, unsigned int dataFieldLength)
{
	if(dataFieldType == "Fixed")
	{
		string temp = String::FixField(record, (dataFieldPosition - 1), dataFieldLength);
		return temp;
	}
	else if(dataFieldType == "Delimited")
	{
		string temp = String::TakeField(record, dataFieldPosition, dataFieldDelimiter);
		return temp;	
	}
	else if(dataFieldType == "CSV")
	{
		string temp = String::TakeFieldCsv(record, dataFieldPosition);
		return temp;	
	}
	else if(dataFieldType == "EDIFACT")
	{
		string temp = String::TakeField(record, dataFieldPosition, '+');
		if(dataSubFieldPosition > 0)
			temp = String::TakeField(temp, dataSubFieldPosition, ':');
		return temp;
	}
	else if(dataFieldType == "Tradacom")
	{
		string temp = String::TakeField(record, dataFieldPosition, '+');
		if(dataFieldPosition == 1)
			temp = String::TakeField(temp, 2, '='); //if we are trying to take the first field in the record, then ignore the record id prior to the '=' before taking the subfield
				
		if(dataSubFieldPosition > 0)
			temp = String::TakeField(temp, dataSubFieldPosition, ':');
		
		return temp;	
	}
	
	return "";
}

/**
 * Convert all applicable characters to XML entities
 *
 * @param source The source string to perform convertion
 * @return String The string after perform convertion
 */
string String::BasicXmlEncode(const string &source)
{
	string output = "";
	
	for (unsigned int i=0; i<source.length(); i++)
	{
		if (source[i] == '&')
			output.append("&amp;");
		else if (source[i] == '<')
			output.append("&lt;");
		else if (source[i] == '>')
			output.append("&gt;");
		else
			output.push_back(source[i]);
	}
	
	return output;
}


/**
 * Convert any single quote ' in a string to the escape sequence '\'', and surround the string with single quotes.
 * Required for literal command-line arguments in bash
 *
 * Method of using '\'' given on page 43 of the Advanced Bash-Scripting Guide (version 6.4) by Mendel Cooper.
 * Adapted into our code below.
 *
 * @param inputStr input string for convertion
 * @return string converted string
 */
string String::QuoteEscapeCommandLineArgument(const string &inputStr)
{
	if(inputStr.empty())
	{
		return "''";
	}

	size_t pos = 0;
	string outputStr = inputStr;

	while ((pos < outputStr.length()) && (pos = outputStr.find('\'', pos)) != string::npos)
	{
		outputStr.replace(pos, 1, "'\\''");
		pos += 4;
	}

	return "'" + outputStr + "'";
}


/**
 * Convert escaped string to its matching chars with default char map.
 *
 * @param input The input string to perform conversion
 * @param errorMessage The error message generated during processing, if success, it's empty.
 * @return string the string after perform conversion, or empty if an invalid sequence is encountered
 */
string String::Unescape(const string &input, string &errorMessage)
{
	char charMap[][2] = {{'\\', '\\'}, {'t', '\t'}, {'v', '\v'}, {'f', '\f'}, {'r', '\r'}, {'0', '\0'}};

	return String::Unescape(input, '\\', charMap, sizeof(charMap)/2, errorMessage);
}


/**
 * Convert all escaped string to its matching char,
 * for example: "\\" -> '\', "\t" -> '\t' (TAB)
 *
 * @param input The input string to perform conversion
 * @param escapeChar The character after which escape sequences should be found
 * @param charMap The char map array, each item is a pair of char mapping, 1st char is the escaped char, 2nd char is the converted char.
 * @param charMapLength The number to total pairs in the char map.
 * @param errorMessage The error message generated during processing, if success, it's empty.
 * @return string the string after perform conversion, or empty if an invalid sequence is encountered
 */
string String::Unescape(const string &input, const char escapeChar, const char charMap[][2], const unsigned int charMapLength, string &errorMessage)
{
	string output = input;
	errorMessage = "";

	if (input.empty())
	{
		return "";
	}

	if ((charMapLength == 0) || (charMap == NULL))
	{
		return output;
	}

	size_t uPos = 0;
	unsigned int i = 0;
	unsigned int outputLength = output.length();

	while  ( (uPos = output.find(escapeChar, uPos)) != string::npos )
	{
		if ((uPos + 1) == outputLength)
		{
			// Opened an escape sequence at the end of the string, which is invalid (unclosed sequence)
			errorMessage = "Unclosed escape sequence at end of input";
			return "";
		}

		for (i = 0; i < charMapLength; i++)
		{
			if (output[uPos + 1] == charMap[i][0])
			{
				output.replace(uPos, 2, 1, charMap[i][1]);
				outputLength = output.length();

				break; // performed necessary replacement - break the for()
			}

			if (i == charMapLength-1)
			{
				// At the end of the expected escape sequences without a match - this is invalid
				errorMessage = "Unknown escape sequence in input" ;
				return "";
			}
		}

		uPos++; // move to next char (unchecked)
	}

	return output;
}

/**
 * Replace the illegal chars in filename with specified char or just remove them
 * The illegal char set includes all illegal chars of Windows & Linux
 * For Windows  \ / : * ? " < > |
 * For Linux    /
 * So combined illegal char set should be  \ / : * ? " < > |
 *
 * @param filename The file name string to be sanitized
 * @param replace  The char to be used to replace those illegal chars, default is '\0', means all illegal chars are removed without replacement.
 * @return
 */
string String::FilenameSanitizer(const string &filename, const char replace)
{
	if (filename.empty())
	{
		return "";
	}

	char illegalCharSet[] = {'/', '\\', ':', '*', '?', '"', '<', '>', '|'};
	string result = "";

	int slen = filename.length();
	int ilen = sizeof(illegalCharSet)/sizeof(illegalCharSet[0]);
	bool isReplace = (replace == '\0')?false:true;
	bool isMatch = false;
	for (int i = 0; i < slen; i++)
	{
		isMatch = false;
		for (int j = 0; j < ilen; j++)
		{
			if (filename[i] == illegalCharSet[j])
			{
				isMatch = true;
				break;
			}
		}

		if (isMatch)
		{
			if(isReplace)
			{
				result.push_back(replace);
			}
		}
		else
		{

			result.push_back(filename[i]);
		}
	}

	return result;
}
