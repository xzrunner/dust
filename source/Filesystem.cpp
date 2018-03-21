#include "moon/Filesystem.h"

#include <boost/filesystem.hpp>

namespace moon
{

void Filesystem::GetDirectoryItems(const char* dir, std::vector<std::string>& items) const
{
	boost::filesystem::recursive_directory_iterator itr(dir), end;
	while (itr != end) 
	{
		std::string relative = itr->path().string();
		items.push_back(boost::filesystem::relative(relative, dir).string());
		++itr;
	}
}

bool Filesystem::IsDirectory(const char* dir) const
{
	return boost::filesystem::is_directory(dir);
}

}