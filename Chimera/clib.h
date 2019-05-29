#include <QString>
#include <windows.h>

long OpenSharedMemory(const char *filename, int size)
{
	unsigned int filesize = 4096;
	//todo:: filesize
	WCHAR wfilename[256];
	//sharedmemory name length cannot be more than 256 chars.
	memset(wfilename, 0, sizeof(wfilename));
	MultiByteToWideChar(CP_ACP, 0, filename, strlen(filename) + 1, wfilename, sizeof(wfilename) / sizeof(wfilename[0]));
	HANDLE hmap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, false, wfilename);
	if (hmap == NULL)
	{
		hmap = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, filesize, wfilename);
		HANDLE pview = MapViewOfFile((HANDLE)hmap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		memset(pview, 0, sizeof(pview));
		strcpy_s((char*)pview, filesize, "UNINITIALIZATION");
		UnmapViewOfFile(pview);
	}

	return HandleToLong(hmap);
}

void WriteSharedMemory(long int hmap, const char * message)
{
	if (LongToHandle(hmap) == NULL) {
		return;
	}
	else
	{
		HANDLE pview = MapViewOfFile(LongToHandle(hmap), FILE_MAP_ALL_ACCESS, 0, 0, 0);
		strcpy_s((char*)pview, strlen(message) + 1, message);
		UnmapViewOfFile(pview);
	}
}

QString ReadSharedMemory(long int hmap)
{
	char message[4096];
	if (LongToHandle(hmap) == NULL)
	{
		strcpy_s(message, 5, "NULL");
	}
	else
	{
		HANDLE pview = MapViewOfFile(LongToHandle(hmap), FILE_MAP_ALL_ACCESS, 0, 0, 0);
		strcpy_s(message, 4096, (char*)pview);
		UnmapViewOfFile(pview);
	}
	return QString(message);
}

void CloseSharedMemory(long int hmap)
{
	if (LongToHandle(hmap) == NULL)
	{
		return;
	}
	else
	{
		CloseHandle(LongToHandle(hmap));
	}
}

long int InquiredSharedMemory(const char * filename)
{
	WCHAR wfilename[256];
	memset(wfilename, 0, sizeof(wfilename));
	MultiByteToWideChar(CP_ACP, 0, filename, strlen(filename) + 1, wfilename, sizeof(wfilename) / sizeof(wfilename[0]));
	HANDLE hmap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, false, wfilename);
	if (hmap == NULL)
	{
		return 0;
	}
	return HandleToLong(hmap);
}