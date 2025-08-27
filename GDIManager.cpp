#include "GDIManager.h"

GDIManager* GDIManager::_pStatic_instance = nullptr;

/*
*		GDIManager의 특수 맴버 함수
*/

GDIManager::~GDIManager()
{
}

/*
*		static 함수
*/

GDIManager* GDIManager::GetInstance()
{
    return _pStatic_instance;
}

void GDIManager::Create()
{
    if (_pStatic_instance != nullptr) {
        return;
    }
    _pStatic_instance = new GDIManager();
}

void GDIManager::Destroy()
{
    if (_pStatic_instance == nullptr) {
        return;
    }

    // 삭제
    delete _pStatic_instance;
    _pStatic_instance = nullptr;
}

/*
*		요청 함수
*/

void GDIManager::AddBrush(std::string name, HBRUSH brush)
{
    auto it = _brushes.find(name);
    if (it != _brushes.end()) {
        return;
    }

    // 추가
    _brushes[name] = brush;
}

HBRUSH GDIManager::GetBrush(std::string name) const
{
    auto it = _brushes.find(name);
    if (it == _brushes.end()) {
        return nullptr;
    }

    return it->second;
}
