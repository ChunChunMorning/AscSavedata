# include "AscSavedata.hpp"
# include <shlobj.h>

using namespace asc;

FilePath Savedata::m_makeFilePath(const FilePath& path)
{
	wchar appdataPath[MAX_PATH];

	if (SHGetSpecialFolderPath(NULL, appdataPath, CSIDL_APPDATA, true))
	{
		return Format(appdataPath, L"/", path);
	}

	return NULL;
}
