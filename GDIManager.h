#pragma once

#include <unordered_map>
#include <string>
#include <Windows.h>

class GDIManager
{
private:
	GDIManager() = default;
	~GDIManager();
public:
	GDIManager(const GDIManager&)				= delete;
	GDIManager(GDIManager&&)					= delete;
	GDIManager& operator=(const GDIManager&)	= delete;
	GDIManager& operator=(GDIManager&&)			= delete;

public:
	static GDIManager* GetInstance();
	static void Create();
	static void Destroy();

public:
	void AddBrush(std::string name, HBRUSH brush);
	HBRUSH GetBrush(std::string name) const;

private:
	static GDIManager* _pStatic_instance;
private:
	std::unordered_map<std::string, HBRUSH> _brushes;
};

void AddDefaultBrushes();