#include "File.h"
using namespace b2be::utils;

/**
 * Constructor
 */
File::File(const string &filename, File::Mode mode, bool preserveEol, char eol)
{
	_Filename = filename;
	_Mode = mode;
	ResetError();
	
	_Outfd = -1;
	_PreserveEol = preserveEol;
	
	_Eol = eol;
	
	_NumLines = 0;
	
	_WriteBuffer = "";
	_FileContent = "";
	
	Open();
}

/**
 * Destructor
 */
File::~File()
{
	Close();
}

/**
 * Moves the source file to the destination file.
 * Uses a system call instead of "rename()" as rename will fail if moving between different filesystems/volumes.
 *
 * @param srcFilename The source filename.
 * @param destFilename The destination filename
 * @return Bool indicating if the file was moved successfully.
 */
bool File::Move(const string &srcFilename, const string &destFilename)
{
	//must use system call
	//using rename() function fails when trying to move files across partitions
	string cmd = "mv \"" + FilenameEscapeChars(srcFilename) + "\" \"" + FilenameEscapeChars(destFilename) + "\"";
	
	if(system(cmd.c_str()) == 0)
		return true;
	else
		return false;
}

/**
 * Copies the source file to the destination file.
 *
 * @param srcFilename The source filename.
 * @param destFilename The destination filename
 * @return Bool indicating if the file was copied successfully.
 */
bool File::Copy(const string &srcFilename, const string &destFilename)
{
	string cmd = "cp \"" + FilenameEscapeChars(srcFilename) + "\" \"" + FilenameEscapeChars(destFilename) + "\"";

	if(system(cmd.c_str()) == 0)
		return true;
	else
		return false;
}

/**
 * Escapes invalid filename characters for running system commands.
 * Only for use internally by the File class.
 *
 * @param filename The filename to process.
 * @return string with the invalid characters escaped.
 */
string File::FilenameEscapeChars(const string &filename)
{
	string fn = "";	
	size_t found = filename.find_first_of("$\"");
	
	if (found != string::npos)
	{
		int fnSize = filename.length();
		for (int i=0; i<fnSize; i++)
		{
			if (filename[i] == '$' || filename[i] == '\"')
				fn.push_back('\\');
			fn.push_back(filename[i]);
		}	
	}	
	else
		fn = filename;
	
	return fn;
}

/**
 * Deletes the specified file.
 *
 * @param filename The filename to be deleted.
 * @return Bool indicating if the file was deleted successfully.
 */
bool File::Delete(const string &filename)
{
	if(remove(filename.c_str()) == 0)
		return true;
	else
		return false;
}

/**
 * Gets the size of the specified file.
 *
 * @param filename The filename to be checked.
 * @return Long containing the size of the specified file.
 */
unsigned long File::GetSize(const string &filename)
{
	struct stat st;
	stat(filename.c_str(), &st);
	unsigned long size = st.st_size;
	return size;
}

/**
 * Gets the last modified timestamp of the specified file.
 *
 * @param filename The filename to be checked.
 * @return time_t containing the last modified timestamp of the specified file.
 */
time_t File::GetLastModifiedTimestamp(const string &filename)
{
	struct stat st;
	stat(filename.c_str(), &st);
	time_t ts = st.st_mtime;
	return ts;
}

/**
 * Returns whether the specified file exists or not.
 *
 * @param filename The filename to be checked.
 * @return Bool indicating if the file exists.
 */
bool File::Exists(const string &filename)
{
	ifstream testF (filename.c_str());
	if(testF.fail())
		return false;
	testF.close();
	return true;
}

/**
 * Creates the specified directory.
 *
 * @param path The directory to be created.
 * @return Bool indicating if the directory was created.
 */
bool File::MakeDirectory(const string &path)
{
	//if all directory already exist
   char* curdir = new char [500];
   getcwd(curdir,500); //current directory
   if(chdir(path.c_str())==0)//directory alreay exist 
   {
	   chdir(curdir); //change back to orignial directory 
	   delete[]curdir;
	   return true;
   } 
   delete[]curdir;

   char* temp = new char[500];
   char* dir = new char[path.length()+1];
   strcpy(dir,path.c_str());

   int length = strlen(dir);
   int i,j;

   //first make sure all the slashes are correct
   for(i=0 ; i<length ; i++) 
   {
      if(dir[i]=='\\') 
		  dir[i] = '/';
   }   

   bool isNewDir = false;
   
   for(i=1 ; i<length ; i++) 
   {
      if(dir[i]=='/') //if it is a starting of next directory
      {
         for(j=0; j<i; j++){temp[j]=dir[j];}
         temp[j]='\0';

         if( mkdir(temp,0777)>=0){isNewDir=true;}

		 //error if there is a new directory created previous and 
         //fail to create another new directory after the previous one
		 else{if(isNewDir){delete[]temp;delete[]dir;return false;}} 
      }
   } 
   delete[] temp;
   delete[] dir;
   return true;
}

/**
 * Returns the directory part of a complete path.
 *
 * @param path The full path of the file.
 * @return String with the directory part of the path.
 */
string File::ExtractDirectory(const string &path)
{
	size_t lastSlash = path.find_last_of( '/' );
	if (lastSlash == string::npos)
		return ""; //assume it was just the filename with no directory
		
	return path.substr( 0, lastSlash +1 );
}

/**
 * Returns the filename part of a complete path.
 *
 * @param path The full path of the file.
 * @return String with the filename part of the path.
 */
string File::ExtractFilename(const string &path)
{
	size_t lastSlash = path.find_last_of( '/' );
	if (lastSlash == string::npos)
		return path; //assume it was just the filename with no directory
		
	return path.substr(lastSlash +1 );
}

/**
 * Check if an error has occurred or not
 * 
 * @return Return if error has occurred, otherwise false
 */
bool File::IsError() const
{
	return _IsError;
}

/**
 * Get the message related to last error which occurred
 * 
 * @return Error message related to last error
 */
string File::GetErrorMessage() const
{
	return _ErrorMessage;
}

/**
 * Resets the IsError and ErrorMessage details
 */
void File::ResetError()
{
	_IsError = false;
	_ErrorMessage = "";
}

/**
 * Get the filename. 
 * Useful when using the CREATEUNIQUE option so you can find out what the unique filename is.
 * 
 * @return The current filename
 */
string File::GetFilename() const
{
	return _Filename;
}

/**
 * Opens the current file using the mode specified in the constructor. 
 */
void File::Open()
{
	errno = 0;
	
	if (_Mode == File::MODE_READ)
		ReadFileContents();
	else if (_Mode == File::MODE_WRITE)
	{
		_Outfd = open(_Filename.c_str(),O_WRONLY | O_CREAT, 00666);		
		if(_Outfd == -1)
		{
			_ErrorMessage = "Unable to open file for writing. ";
			_ErrorMessage.append(strerror(errno));
			_IsError = true;
			return;
		}
	}
	else if (_Mode == File::MODE_APPEND)
	{
		_Outfd = open(_Filename.c_str(),O_WRONLY | O_CREAT | O_APPEND, 00666);		
		if(_Outfd == -1)
		{
			_ErrorMessage = "Unable to open file for appending. ";
			_ErrorMessage.append(strerror(errno));
			_IsError = true;
			return;
		}
	}
	else if (_Mode == File::MODE_CREATEUNIQUE)
		CreateUnique();
}

/**
 * Closes the current file if it is open.
 */
void File::Close()
{
	if (_WriteBuffer.size() != 0)
		Write();
	if (_Outfd != -1)
	{
		close(_Outfd);
		_Outfd = -1;
	}	
}

/**
 * Reads the entire contents of the current file into an internal vector with one line per entry.
 */
void File::ReadFileContents()
{
	ifstream infile;
	infile.open(_Filename.c_str(), ios::in|ios::binary);
	if(infile.fail())
	{
		_ErrorMessage = "Unable to open file for reading";
		_IsError = true;
		return;
	}
	
	stringstream buffer;
	buffer << infile.rdbuf();
	infile.close();
	_FileContent = buffer.str();	
		
	unsigned long len = _FileContent.size();
	
	if (len > 0)
	{
		size_t position = 0;
		size_t prevPosition = 0; 
		//_Lines contains the position of the start of each line
		//if the length of the file is greater than 0, then we have at least 1 line of data before we find the first CRLF
		_Lines.push_back(position);
		
		while ((position = _FileContent.find(_Eol, prevPosition)) != std::string::npos)	
		{		
			position++; //increment position to be the start of the next line
			if (position < len) //if the LF wasn't the last character in the file, then store the start of the next line
				_Lines.push_back(position);
			prevPosition = position;
		}	
	}
	
	_NumLines = _Lines.size();
}

/**
 * Gets the number of lines from the file which has been read into memory.
 *
 * @return Long containing the number of lines read in.
 */
unsigned long File::GetNumLines()
{
	if (_Mode != File::MODE_READ)
	{
		_ErrorMessage = "File not opened for reading";
		_IsError = true;
		return 0;
	}
	
	return _NumLines;
}

/**
 * Gets the file line at the specified index.
 *
 * @param idx Index of the file line to be returned.
 * @return String containing the specified line from the file.
 */
string File::GetLineAt(unsigned long idx)
{
	if (_Mode != File::MODE_READ)
	{
		_ErrorMessage = "File not opened for reading";
		_IsError = true;
		return "";
	}
	
	if (idx >= _NumLines)
		return "";
		
	size_t start = _Lines[idx];
	size_t len = 0; 

	if (idx == _NumLines-1)
		len = _FileContent.size() - start;
	else
		len = _Lines[idx+1] - start;
			
	if (!_PreserveEol && len != 0)
	{
		//check the end of the line for LF and then CRLF and remove it
		char a = _FileContent[start+len-1];
		if (a == _Eol) //remove the defined EOL
		{
			len--;
			if (len != 0 && _Eol == '\n')
			{
				//tries to remove the CR which will precede the LF in a CRLF termintated line
				a = _FileContent[start+len-1];
				if (a == '\r') 
					len--;
			}
		}
	}
		
	return _FileContent.substr(start,len);
}

/**
 * Writes data to the file.
 *
 * @param data The string data to write to the file.
 */
void File::Write(const string &data)
{
	if (_Mode != File::MODE_WRITE && _Mode != File::MODE_APPEND && _Mode != File::MODE_CREATEUNIQUE)
	{
		_ErrorMessage = "File not opened for writing";
		_IsError = true;
		return;
	}
	
	_WriteBuffer.append(data);	
	
	if (_WriteBuffer.size() > WRITE_BUFFER_SIZE)
		Write();
		
	//write(_Outfd,data.c_str(),data.size());	
}

/**
 * Private function used for writing the internal buffer to file.
 * Buffering the writes provides a performance improvements, particularly when writing to the SAN
 *
 */ 
void File::Write()
{
	write(_Outfd,_WriteBuffer.c_str(),_WriteBuffer.size());
	_WriteBuffer = "";
}

/**
 * Simply forces the current write buffer to be written to disk
 *
 */ 
void File::Flush()
{
	Write();
}

/**
 * Creates a unique file by appending "_XXXXXX" to the provided filename.
 * The X's are then replaced with a unique string generated using mkstemp().
 */
void File::CreateUnique()
{
	char* tmpFilename = new char[500];
	strcpy(tmpFilename,"\0");
	strcat(tmpFilename, _Filename.c_str());
	strcat(tmpFilename, "_XXXXXX");
	
	errno = 0;
	_Outfd = mkstemp(tmpFilename); //returns a file descriptor
	if (_Outfd == -1)
	{
		delete [] tmpFilename;
		_ErrorMessage = "Unable to create unique file. ";
		_ErrorMessage.append(strerror(errno));
		_IsError = true;
		return;
	}	
	fchmod(_Outfd, 0666);
	
	_Filename = tmpFilename;
	delete [] tmpFilename;
}

/**
 * Handles the copying of a file to /tmp with a guaranteed unique filename.
 *
 * @param srcFilename The filename of the file to be copied to the /tmp folder.
 * @return The final unique filename which was created under /tmp.
 */
string File::CopyToTemp(const string &srcFilename)
{
	char* tmpFilename = new char[50];
	strcpy(tmpFilename,"/tmp/");
	strcat(tmpFilename, lvki::DateTime::NowString("%Y%m%d%H%M%S_XXXXXX", "GMT").c_str());
	
	int tmpfd = mkstemp(tmpFilename); //returns a file descriptor
	if (tmpfd == -1)
	{
		delete [] tmpFilename;
		return "";
	}	
	fchmod(tmpfd, 0666);
	close(tmpfd); //closes the empty file
	
	if (!File::Copy(srcFilename, tmpFilename))
	{
		delete [] tmpFilename; 
		return "";
	}	
	
	string result = tmpFilename;
	delete [] tmpFilename;
	return result;
}

/**
 * Handles the gz compression of a file by copying your source file to /tmp first so the original src is not removed.
 * Appends a unique string to the end of each filename in the form "_XXXXXX"
 *
 * @param srcFilename The source filename to be gz'd
 * @param destFilename The destination path for the file (excluding the unique filename placeholder 'XXXXXX' and excluding the .gz extension)  
 * @return The final unique filename which was created.
 */
string File::GzipCompress(const string &srcFilename, const string &destFilename)
{
	//move the gzip'd temp file to the destFilename	
	//automatically append an underscore followed by XXXXXX then generate the unique file pointer first
	char* tmpDestFilename = new char[500];
	strcpy(tmpDestFilename,destFilename.c_str());
	strcat(tmpDestFilename,"_XXXXXX");
	int tmpfd = mkstemp(tmpDestFilename); //returns a file descriptor
	if (tmpfd == -1)
	{
		delete [] tmpDestFilename;
		return "";
	}	
	close(tmpfd); //closes the empty file
	
	string result = tmpDestFilename;
	delete [] tmpDestFilename;
	
	//copy the source file to the temp destination
	if (!File::Copy(srcFilename, result))
	{
		return "";
	}
	
	//fix the permissions on the file
	chmod(result.c_str(), 0666);
	
	//gzip the destination file
	//gzip the temp file
	string cmd = GZIP_COMPRESS_COMMAND;
	cmd.append(" \"");
	cmd.append(result);
	cmd.append("\"");
	int status = system(cmd.c_str());
	if (status != 0)
	{
		return "";
	}
	
	result.append(".gz");
	
	//return the final filename for the backup file	
	return result;	
}

/**
 * Puts a gz file into a temporary location (/tmp) so your program can use it.
 * Handles the GZIP de-compression of files automatically.
 * Ensures the temporary file has a unique filename
 *
 * @param path The filename of the file you wish to decompress
 * @return The final unique filename which was created under /tmp.
 */
string File::GzipDecompress(const string &path)
{
	//copy the backup file to a temp location (using a unique filename generated by XXXXXX)
	string tmpFilename = CopyToTemp(path);
	
	//rename the temp file to include .gz extension
	if (!File::Move(tmpFilename, tmpFilename+".gz"))
	{
		return "";
	}
	
	//un-gzip the temp file
	string cmd = GZIP_DECOMPRESS_COMMAND;
	cmd.append(" ");
	cmd.append(tmpFilename);
	cmd.append(".gz");
	int status = system(cmd.c_str());
	if (status != 0)
	{
		return "";
	}
	
	chmod(tmpFilename.c_str(), 0666);
	//return the final filename for the un-gzip'd backup file in the temp location
	return tmpFilename;
}

/**
 * Handles the Zip compression of a file to a specified zip file
 *
 * @param srcFilename The source filename to be zipped
 * @param destFilename The destination zip filename
 * @return True if successful, false otherwise.
 */
bool File::ZipCompress(const string &srcFilename, const string &destFilename)
{
	if (!File::Exists(srcFilename))
		return false;
		
	string cmd = ZIP_COMPRESS_COMMAND;
	cmd.append(" \"");
	cmd.append(destFilename);
	cmd.append("\" \"");
	cmd.append(srcFilename);
	cmd.append("\"");
	
	int status = system(cmd.c_str());
	if (status != 0)
	{
		return false;
	}
	
	return true;
}

/**
 * Handles the Zip compression of list of files to a specified zip file
 *
 * @param srcFilenames The source filenames to be zipped
 * @param destFilename The destination zip filename
 * @return True if successful, false otherwise.
 */
bool File::ZipCompress(const vector<string> &srcFilenames, const string &destFilename)
{
	for (vector<string>::const_iterator it = srcFilenames.begin(); it != srcFilenames.end(); ++it)
	{
		if (!File::ZipCompress((*it),destFilename))
		{
			File::Delete(destFilename);
			return false;
		}	
	}		
	return true;
}

/**
 * Handles the Zip compression of list of files to a specified zip file
 *
 * @param srcFilename The source filename to be unzipped
 * @param destPath The destination path to unzip the files into
 * @param outputFilenames The filename of each file which was unzipped
 * @return True if successful, false otherwise.
 */
bool File::ZipDecompress(const string &srcFilename, const string &destPath, vector<string> &outputFilenames)
{
	if (!File::Exists(srcFilename))
		return false;
		
	string cmd = ZIP_DECOMPRESS_COMMAND;
	cmd.append(" \"");
	cmd.append(srcFilename);
	cmd.append("\"");
	if (destPath != "")
	{
		cmd.append(" -d \"");
		cmd.append(destPath);
		cmd.append("\"");
	}
	
	/*command output
	Archive:  test2.zip
	  inflating: input.txt
      inflating: input2.txt
	*/

	FILE* fCmd = popen(cmd.c_str(),"r");
		
	if (fCmd == NULL) 
		return false;
	else
	{
		int c;
		string line("");
		string filename("");
		size_t pos;
		do 
		{
			c = fgetc (fCmd);
			if (c == '\n') 
			{
				line = String::Trim(line);
				
				if (String::IsBeginWith(line,"inflating:") || String::IsBeginWith(line,"extracting:"))
				{
					pos = line.find(':');
					if(pos != string::npos)
					{
						filename = String::Trim(line.substr(pos + 1));
						outputFilenames.push_back(File::ExtractFilename(filename));
					}
				}
				line = "";
			}
			else
			{
				line.push_back((char)c);
			}
		} while (c != EOF);
		
		pclose(fCmd);		
	}
	
	if(outputFilenames.empty())
		return false;
	else
		return true;
}
