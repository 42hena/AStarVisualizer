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

    for (auto brush : _pStatic_instance->_brushes)
    {
        DeleteObject(brush.second);
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
    // 이름 검색
    auto it = _brushes.find(name);
    
    // 검색 실패 경우
    if (it != _brushes.end()) {
        return;
    }

    // 추가
    _brushes[name] = brush;
}

HBRUSH GDIManager::GetBrush(std::string name) const
{
    // 이름 검색
    auto it = _brushes.find(name);
    
    // 검색 실패 경우
    if (it == _brushes.end()) {
        return nullptr;
    }

    return it->second;
}

void AddDefaultBrushes()
{
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));
    HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH magentaBrush = CreateSolidBrush(RGB(255, 0, 255));
    HBRUSH turquoiseBrush = CreateSolidBrush(RGB(0, 255, 255));
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));

    GDIManager::GetInstance()->AddBrush("red", redBrush);
    GDIManager::GetInstance()->AddBrush("green", greenBrush);
    GDIManager::GetInstance()->AddBrush("blue", blueBrush);
    GDIManager::GetInstance()->AddBrush("black", blackBrush);
    GDIManager::GetInstance()->AddBrush("yellow", yellowBrush);
    GDIManager::GetInstance()->AddBrush("Magenta", magentaBrush);
    GDIManager::GetInstance()->AddBrush("turquoise", turquoiseBrush);
    GDIManager::GetInstance()->AddBrush("white", whiteBrush);
}
