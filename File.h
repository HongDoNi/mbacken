#ifndef File2_H
#define File2_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cstring>

#include "DateTime.h"
#include "String.h"

using namespace std;

#define WRITE_BUFFER_SIZE 32768
#define GZIP_COMPRESS_COMMAND "gzip -9q"
#define GZIP_DECOMPRESS_COMMAND "gzip -dq"

#define ZIP_COMPRESS_COMMAND "zip -qj -9"
#define ZIP_DECOMPRESS_COMMAND "unzip -oj"

namespace b2be
{
	namespace utils
	{		
		/**
		 * A simple File class providing some basic static utility functions and for wrapping the reading and writing of files
		 */
		class File
		{
			public:
				enum Mode {MODE_READ, MODE_WRITE, MODE_APPEND, MODE_CREATEUNIQUE};
				
				File(const string &filename, File::Mode mode, bool preserveEol = true, char eol = '\n');
				~File();
				
				static bool Move(const string &srcFilename, const string &destFilename);
				static bool Copy(const string &srcFilename, const string &destFilename);
				static bool Delete(const string &filename);
				static unsigned long GetSize(const string &filename);
				static time_t GetLastModifiedTimestamp(const string &filename);
				static bool Exists(const string &filename);
				static bool MakeDirectory(const string &path);
				
				static string GzipCompress(const string &srcFilename, const string &destFilename);
				static string GzipDecompress(const string &path);
				
				static bool ZipCompress(const string &srcFilename, const string &destFilename);
				static bool ZipCompress(const vector<string> &srcFilenames, const string &destFilename);
				static bool ZipDecompress(const string &srcFilename, const string &destPath, vector<string> &outputFilenames);
				
				static string ExtractDirectory(const string &path);
				static string ExtractFilename(const string &path);
				
				bool IsError() const;
				string GetErrorMessage() const;
				void ResetError();
				
				string GetFilename() const;
								
				void Close();
				
				//reading
				unsigned long GetNumLines();
				string GetLineAt(unsigned long idx);
				
				//writing
				void Write(const string &data);								
				void Flush();		
				
			private:
				void Open();
				void CreateUnique();
				void ReadFileContents();
				void Write();		
				static string FilenameEscapeChars(const string &filename);
				
				static string CopyToTemp(const string &srcFilename);
				
				string _ErrorMessage;
				bool _IsError;
				
				//ofstream _Outfile;
				int _Outfd;
				
				bool _PreserveEol;
				char _Eol;
				
				File::Mode _Mode;
				string _Filename;		
				string _FileContent;
				vector<size_t> _Lines;
				unsigned long _NumLines;
				string _WriteBuffer;

		};
	}
}

#endif
