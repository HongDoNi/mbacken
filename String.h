#ifndef String_H
#define String_H

#include "utf8.h"
#include <string>
#include <cstring>
#include <sstream>
#include <errno.h>
#include <iconv.h>
using namespace std;

namespace b2be
{
	namespace utils
	{
		#define TEXTENC_BUFFER 10000 //buffer size for iconv text encoding conversion - currently limited to 10KB which should be plenty for individual string or line processing

		/**
		 * A collection of useful static string utility methods
		 */
		class String
		{
			public:
				enum PADMETHOD {PADRIGHT, PADLEFT};

				static string FixField(const string &source, int start, int length);
				static string TakeField(const string &source, int fieldNumber, char delimiter);
				static string TakeField(const string &source, int fieldNumber, string delimiter);
				static string TakeFieldCsv(string source, int fieldNumber);
				static int CountField(const string &source, char delimiter);
				static int CountField(const string &source, string delimiter);
				static bool IsBeginWith(const string &source, string prefix);
				static bool IsEndWith(const string &source, string suffix);
				static bool IsNumeric(const string &number, string ignoreChars = "");
				static string RemoveChar(const string &source, char remove);
				static string RemoveChars(const string &source, const string &remove);
				static string ReplaceChar(const string &source, char find, char replace);
				static string Replace(const string &source, const string &find, const string &replace);
				static string Replace(const string &source, const string &find, const string &replace, const string &escape);
				static string SwapChar(const string &source, char swapChar1, char swapChar2);
				static string Trim (const string &source);
				static string Trim (const string &source, const string &charset);
				static string ToUpper(string source);
				static string ToLower(string source);
				static string PadString(const string in, const char chr, const unsigned int len, PADMETHOD padMethod = PADLEFT);
				static string Base64Encode(const string &source);
				static string Base64Decode(string source);
				static bool ConvertTextEncoding(string fromEncoding, string toEncoding, const string &inputStr, string &outputStr, string &errMsg);
				static bool IsMatchingRecordID(const string &record, string recordID, string recordIDFieldType, unsigned int recordIDFieldPosition, string recordIDFieldDelimiter, unsigned int recordIDFieldLength);
				static string GetDataFromRecord(const string &record, string dataFieldType, unsigned int dataFieldPosition, unsigned int dataSubFieldPosition, string dataFieldDelimiter, unsigned int dataFieldLength);
				static string BasicXmlEncode(const string &source);
				static string QuoteEscapeCommandLineArgument(const string &inputStr);
				static string Unescape(const string &input, string &errorMessage);
				static string Unescape(const string &input, const char escapeChar, const char charMap[][2], const unsigned int charMapLength, string &errorMessage);
				static string FilenameSanitizer(const string &filename, const char replace = '\0');

				//template function which takes any basic type and converts it to a string using the stringstream which has already overloaded the << operator for basic types
				template <class T>
				static string ToString(T a){stringstream s; s << a; return s.str();}

			private:
				static string _Base64Chars;

				static string ParseFieldCsv(string &data, int &pos);
				static int FindNextQuote(string &data, int startFrom);
		};
	}
}

#endif
