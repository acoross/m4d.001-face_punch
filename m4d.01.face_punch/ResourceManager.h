#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include "Utilities.h"

template <class Derived, class T>
class ResourceManager
{
public:
	ResourceManager(const std::string& pathsFile)
	{
		LoadPaths(pathsFile);
	}

	virtual ~ResourceManager()
	{
		PurgeResources();
	}

	T* GetResource(const std::string& id)
	{
		auto res = Find(id);
		return (res ? res->first : nullptr);
	}

	std::string GetPath(const std::string& id)
	{
		auto path = paths_.find(id);
		return (path != paths_.end() ? path->second : "");
	}

	bool RequireResource(const std::string& id)
	{
		auto res = Find(id);
		if (res)
		{
			++res->second;
			return true;
		}

		auto path = paths_.find(id);
		if (path == paths_.end())
		{
			return false;
		}

		T* resource = Load(path->second);
		if (!resource)
		{
			return false;
		}

		resources_.emplace(id, std::make_pair(resource, 1));
		return true;
	}

	bool ReleaseResource(const std::string& id)
	{
		auto res = Find(id);
		if (!res)
		{
			return false;
		}

		-- res->second;
		if (!res->second)
		{
			Unload(id);
		}

		return true;
	}

	void PurgeResources()
	{
		while (resources_.begin() != resources_.end())
		{
			delete resources_.begin()->second.first;
			resources_.erase(resources_.begin());
		}
	}

	T* Load(const std::string& path)
	{
		return static_cast<Derived*>(this)->Load(path);
	}

	std::pair<T*, unsigned int>* Find(const std::string& l_id) 
	{
		auto itr = resources_.find(l_id);
		return (itr != resources_.end() ? &itr->second : nullptr);
	}

	bool Unload(const std::string& id)
	{
		auto itr = resources_.find(id);
		if (itr == resources_.end())
		{
			return false;
		}

		delete itr->second.first;
		resources_.erase(itr);
		return true;
	}

	void LoadPaths(const std::string& pathFile)
	{
		std::ifstream paths;
		paths.open(Utils::GetWorkingDirectory() + pathFile);

		if (paths.is_open())
		{
			std::string line;
			while (std::getline(paths, line))
			{
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;
				keystream >> pathName;
				keystream >> path;
				paths_.emplace(pathName, path);
			}

			paths.close();
			return;
		}
		else
		{
			std::cerr << "! Failed loading the path file: " << pathFile << std::endl;
		}
	}

private:
	std::unordered_map<std::string, std::pair<T*, unsigned int>> resources_;
	std::unordered_map<std::string, std::string> paths_;
};

