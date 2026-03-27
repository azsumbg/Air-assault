#include "framework.h"
#include "Air assault.h"
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include "FCheck.h"
#include "ErrH.h"
#include "D2BMPLOADER.h"
#include "gifresizer.h"
#include "airgame.h"
#include <chrono>
#include <clocale>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "fcheck.lib")
#pragma comment(lib, "errh.lib")
#pragma comment(lib, "d2bmploader.lib")
#pragma comment(lib, "gifresizer.lib")
#pragma comment(lib, "airgame.lib")

constexpr wchar_t bWinClassName[]{ L"Assault2" };
constexpr char tmp_file[]{ ".\\res\\data\\temp.dat" };
constexpr wchar_t Ltmp_file[]{ L".\\res\\data\\temp.dat" };
constexpr wchar_t record_file[]{ L".\\res\\data\\record.dat" };
constexpr wchar_t save_file[]{ L".\\res\\data\\save.dat" };
constexpr wchar_t help_file[]{ L".\\res\\data\\help.dat" };
constexpr wchar_t sound_file[]{ L".\\res\\snd\\main.wav" };

constexpr int mNew{ 1001 };
constexpr int mLvl{ 1002 };
constexpr int mExit{ 1003 };
constexpr int mSave{ 1004 };
constexpr int mLoad{ 1005 };
constexpr int mHoF{ 1006 };

constexpr int no_record{ 2001 };
constexpr int first_record{ 2002 };
constexpr int record{ 2003 };

WNDCLASS bWinClass{};
HINSTANCE bIns{ nullptr };
HWND bHwnd{ nullptr };
HMENU bBar{ nullptr };
HMENU bMain{ nullptr };
HMENU bStore{ nullptr };
HICON mainIcon{ nullptr };
HCURSOR mainCur{ nullptr };
HCURSOR outCur{ nullptr };
HDC PaintDC{ nullptr };
PAINTSTRUCT bPaint{};
MSG bMsg{};
BOOL bRet{ 0 };
POINT cur_pos{};
UINT bTimer{ 0 };

D2D1_RECT_F b1Rect{ 50.0f, 5.0f, scr_width / 3.0f - 50.0f, sky - 5.0f };
D2D1_RECT_F b2Rect{ scr_width / 3.0f + 50.0f, 5.0f, scr_width * 2.0f / 3.0f  - 50.0f, sky - 5.0f };
D2D1_RECT_F b3Rect{ scr_width * 2.0f / 3.0f + 50.0f, 5.0f, scr_width - 50.0f, sky - 5.0f };

D2D1_RECT_F b1TxtRect{ 90.0f, 15.0f, scr_width / 3.0f - 50.0f, sky - 5.0f };
D2D1_RECT_F b2TxtRect{ scr_width / 3.0f + 90.0f, 15.0f, scr_width * 2.0f / 3.0f - 50.0f, sky - 5.0f };
D2D1_RECT_F b3TxtRect{ scr_width * 2.0f / 3.0f + 80.0f, 15.0f, scr_width - 50.0f, sky - 5.0f };

bool pause{ false };
bool in_client{ true };
bool show_help{ false };
bool sound{ true };
bool b1Hglt{ false };
bool b2Hglt{ false };
bool b3Hglt{ false };
bool name_set{ false };

bool level_skipped{ false };

wchar_t current_player[16]{ L"TARLYO" };

float x_scale{ 0 };
float y_scale{ 0 };

int level = 1;
int score = 0;
int distance = 0;

///////////////////////////////////////////////////

ID2D1Factory* iFactory{ nullptr };
ID2D1HwndRenderTarget* Draw{ nullptr };

ID2D1RadialGradientBrush* b1Bckg{ nullptr };
ID2D1RadialGradientBrush* b2Bckg{ nullptr };
ID2D1RadialGradientBrush* b3Bckg{ nullptr };

ID2D1SolidColorBrush* statBrush{ nullptr };
ID2D1SolidColorBrush* txtBrush{ nullptr };
ID2D1SolidColorBrush* hgltBrush{ nullptr };
ID2D1SolidColorBrush* inactBrush{ nullptr };

IDWriteFactory* iWriteFactory{ nullptr };
IDWriteTextFormat* nrmText{ nullptr };
IDWriteTextFormat* midText{ nullptr };
IDWriteTextFormat* bigText{ nullptr };

ID2D1Bitmap* bmpField{ nullptr };
ID2D1Bitmap* bmpLogo{ nullptr };
ID2D1Bitmap* bmpCloud1{ nullptr };
ID2D1Bitmap* bmpCloud2{ nullptr };
ID2D1Bitmap* bmpCloud3{ nullptr };
ID2D1Bitmap* bmpCloud4{ nullptr };
ID2D1Bitmap* bmpCloud5{ nullptr };
ID2D1Bitmap* bmpSeaTile{ nullptr };
ID2D1Bitmap* bmpTree1Tile{ nullptr };
ID2D1Bitmap* bmpTree2Tile{ nullptr };
ID2D1Bitmap* bmpTree3Tile{ nullptr };

ID2D1Bitmap* bmpBigGun{ nullptr };
ID2D1Bitmap* bmpRepair{ nullptr };
ID2D1Bitmap* bmpRocket{ nullptr };
ID2D1Bitmap* bmpShield{ nullptr };

ID2D1Bitmap* bmpShot{ nullptr };
ID2D1Bitmap* bmpBigShot{ nullptr };
ID2D1Bitmap* bmpRocketU{ nullptr };
ID2D1Bitmap* bmpRocketD{ nullptr };

ID2D1Bitmap* bmpBoss1[4]{ nullptr };
ID2D1Bitmap* bmpBoss2[2]{ nullptr };
ID2D1Bitmap* bmpBoss3[10]{ nullptr };

ID2D1Bitmap* bmpEvil1L[6]{ nullptr };
ID2D1Bitmap* bmpEvil1R[6]{ nullptr };
ID2D1Bitmap* bmpEvil1S[12]{ nullptr };

ID2D1Bitmap* bmpEvil2L[5]{ nullptr };
ID2D1Bitmap* bmpEvil2R[7]{ nullptr };
ID2D1Bitmap* bmpEvil2S[12]{ nullptr };

ID2D1Bitmap* bmpEvil3L[6]{ nullptr };
ID2D1Bitmap* bmpEvil3R[6]{ nullptr };
ID2D1Bitmap* bmpEvil3S[13]{ nullptr };

ID2D1Bitmap* bmpEvil4L[5]{ nullptr };
ID2D1Bitmap* bmpEvil4R[6]{ nullptr };
ID2D1Bitmap* bmpEvil4S[12]{ nullptr };

ID2D1Bitmap* bmpEvil5L[5]{ nullptr };
ID2D1Bitmap* bmpEvil5R[5]{ nullptr };
ID2D1Bitmap* bmpEvil5S[14]{ nullptr };

ID2D1Bitmap* bmpHeroL[7]{ nullptr };
ID2D1Bitmap* bmpHeroR[7]{ nullptr };
ID2D1Bitmap* bmpHeroS[10]{ nullptr };

ID2D1Bitmap* bmpExplosion[24]{ nullptr };
ID2D1Bitmap* bmpIntro[72]{ nullptr };

////////////////////////////////////////////////////////

dll::RANDIT RandIt{};

dll::HERO* Hero{ nullptr };
dll::EVILS* Boss{ nullptr };

std::vector<dll::GROUND*>vFields;
std::vector<dll::GROUND*>vTiles;

std::vector<dll::CLOUDS*>vClouds;

dirs assets_move_dir{ dirs::stop };

bool need_field_up = false;
bool need_field_down = false;

bool boss_active = false;

std::vector<dll::EVILS*>vEvils;

std::vector<dll::SHOTS*>vHeroShots;

std::vector<dll::SHOTS*>vEvilShots;

std::vector<dll::POWERUPS*>vPowerUps;

struct EXPLOSION
{
	float sx = 0;
	float sy = 0;
	
	int frame = 0;
	int frame_delay = 3;
};

std::vector<EXPLOSION> vExplosions;

/////////////////////////////////////////////////////

template<typename T>concept HasRelease = requires(T check)
{
	check.Release();
};
template<HasRelease T>bool FreeMem(T** var)
{
	if (*var)
	{
		(*var)->Release();
		(*var) = nullptr;
		return true;
	}
	return false;
};
void LogErr(const wchar_t* what)
{
	std::wofstream err(L".\\res\\data\\error.log", std::ios::app);
	err << what << L" time stamp: " << std::chrono::system_clock::now() << std::endl;
	err.close();
}
void ClearResources()
{
	if (!FreeMem(&iFactory))LogErr(L"Error releasing D2D1 iFactory !");
	if (!FreeMem(&Draw))LogErr(L"Error releasing D2D1 hwndRenderTarget !");
	if (!FreeMem(&b1Bckg))LogErr(L"Error releasing D2D1 b1Bckg !");
	if (!FreeMem(&b2Bckg))LogErr(L"Error releasing D2D1 b2Bckg !");
	if (!FreeMem(&b3Bckg))LogErr(L"Error releasing D2D1 b3Bckg !");
	if (!FreeMem(&statBrush))LogErr(L"Error releasing D2D1 statBrush !");
	if (!FreeMem(&txtBrush))LogErr(L"Error releasing D2D1 txtBrush !");
	if (!FreeMem(&hgltBrush))LogErr(L"Error releasing D2D1 hgltBrush !");
	if (!FreeMem(&inactBrush))LogErr(L"Error releasing D2D1 inactBrush !");

	if (!FreeMem(&iWriteFactory))LogErr(L"Error releasing D2D1 iWiteFactory !");
	if (!FreeMem(&nrmText))LogErr(L"Error releasing D2D1 nrmText !");
	if (!FreeMem(&midText))LogErr(L"Error releasing D2D1 midText !");
	if (!FreeMem(&bigText))LogErr(L"Error releasing D2D1 bigText !");

	if (!FreeMem(&bmpField))LogErr(L"Error releasing D2D1 bmpField !");
	if (!FreeMem(&bmpLogo))LogErr(L"Error releasing D2D1 bmpLogo !");
	if (!FreeMem(&bmpCloud1))LogErr(L"Error releasing D2D1 bmpCloud1 !");
	if (!FreeMem(&bmpCloud2))LogErr(L"Error releasing D2D1 bmpCloud2 !");
	if (!FreeMem(&bmpCloud3))LogErr(L"Error releasing D2D1 bmpCloud3 !");
	if (!FreeMem(&bmpCloud4))LogErr(L"Error releasing D2D1 bmpCloud4 !");
	if (!FreeMem(&bmpCloud5))LogErr(L"Error releasing D2D1 bmpCloud5 !");
	if (!FreeMem(&bmpSeaTile))LogErr(L"Error releasing D2D1 bmpSeaTile !");
	if (!FreeMem(&bmpTree1Tile))LogErr(L"Error releasing D2D1 bmpTree1Tile !");
	if (!FreeMem(&bmpTree2Tile))LogErr(L"Error releasing D2D1 bmpTree2Tile !");
	if (!FreeMem(&bmpTree3Tile))LogErr(L"Error releasing D2D1 bmpTree3Tile !");

	if (!FreeMem(&bmpBigGun))LogErr(L"Error releasing D2D1 bmpBigGun !");
	if (!FreeMem(&bmpRepair))LogErr(L"Error releasing D2D1 bmpRepair !");
	if (!FreeMem(&bmpRocket))LogErr(L"Error releasing D2D1 bmpRocket !");
	if (!FreeMem(&bmpShield))LogErr(L"Error releasing D2D1 bmpShield !");

	if (!FreeMem(&bmpShot))LogErr(L"Error releasing D2D1 bmpShot !");
	if (!FreeMem(&bmpBigShot))LogErr(L"Error releasing D2D1 bmpBigShot !");
	if (!FreeMem(&bmpRocketU))LogErr(L"Error releasing D2D1 bmpRocketU !");
	if (!FreeMem(&bmpRocketD))LogErr(L"Error releasing D2D1 bmpRocketD !");

	for (int i = 0; i < 4; ++i)if (!FreeMem(&bmpBoss1[i]))LogErr(L"Error releasing D2D1 bmpBoss1 !");
	for (int i = 0; i < 2; ++i)if (!FreeMem(&bmpBoss2[i]))LogErr(L"Error releasing D2D1 bmpBoss2 !");
	for (int i = 0; i < 10; ++i)if (!FreeMem(&bmpBoss3[i]))LogErr(L"Error releasing D2D1 bmpBoss3 !");

	for (int i = 0; i < 6; ++i)if (!FreeMem(&bmpEvil1L[i]))LogErr(L"Error releasing D2D1 bmpEvil1L !");
	for (int i = 0; i < 6; ++i)if (!FreeMem(&bmpEvil1R[i]))LogErr(L"Error releasing D2D1 bmpEvil1R !");
	for (int i = 0; i < 12; ++i)if (!FreeMem(&bmpEvil1S[i]))LogErr(L"Error releasing D2D1 bmpEvil1S !");

	for (int i = 0; i < 5; ++i)if (!FreeMem(&bmpEvil2L[i]))LogErr(L"Error releasing D2D1 bmpEvil2L !");
	for (int i = 0; i < 7; ++i)if (!FreeMem(&bmpEvil2R[i]))LogErr(L"Error releasing D2D1 bmpEvil2R !");
	for (int i = 0; i < 12; ++i)if (!FreeMem(&bmpEvil2S[i]))LogErr(L"Error releasing D2D1 bmpEvil2S !");

	for (int i = 0; i < 6; ++i)if (!FreeMem(&bmpEvil3L[i]))LogErr(L"Error releasing D2D1 bmpEvil3L !");
	for (int i = 0; i < 6; ++i)if (!FreeMem(&bmpEvil3R[i]))LogErr(L"Error releasing D2D1 bmpEvil3R !");
	for (int i = 0; i < 13; ++i)if (!FreeMem(&bmpEvil3S[i]))LogErr(L"Error releasing D2D1 bmpEvil3S !");

	for (int i = 0; i < 5; ++i)if (!FreeMem(&bmpEvil4L[i]))LogErr(L"Error releasing D2D1 bmpEvil4L !");
	for (int i = 0; i < 6; ++i)if (!FreeMem(&bmpEvil4R[i]))LogErr(L"Error releasing D2D1 bmpEvil4R !");
	for (int i = 0; i < 12; ++i)if (!FreeMem(&bmpEvil4S[i]))LogErr(L"Error releasing D2D1 bmpEvil4S !");

	for (int i = 0; i < 5; ++i)if (!FreeMem(&bmpEvil5L[i]))LogErr(L"Error releasing D2D1 bmpEvil5L !");
	for (int i = 0; i < 5; ++i)if (!FreeMem(&bmpEvil5R[i]))LogErr(L"Error releasing D2D1 bmpEvil5R !");
	for (int i = 0; i < 14; ++i)if (!FreeMem(&bmpEvil5S[i]))LogErr(L"Error releasing D2D1 bmpEvil5S !");

	for (int i = 0; i < 7; ++i)if (!FreeMem(&bmpHeroL[i]))LogErr(L"Error releasing D2D1 bmpHeroL !");
	for (int i = 0; i < 7; ++i)if (!FreeMem(&bmpHeroR[i]))LogErr(L"Error releasing D2D1 bmpHeroR !");
	for (int i = 0; i < 10; ++i)if (!FreeMem(&bmpHeroS[i]))LogErr(L"Error releasing D2D1 bmpHeroS !");

	for (int i = 0; i < 24; ++i)if (!FreeMem(&bmpExplosion[i]))LogErr(L"Error releasing D2D1 bmpExplosion !");

	for (int i = 0; i < 72; ++i)if (!FreeMem(&bmpIntro[i]))LogErr(L"Error releasing D2D1 bmpIntro !");
}
void ErrExit(int what)
{
	MessageBeep(MB_ICONERROR);
	MessageBox(NULL, ErrHandle(what), L"Критична грешка !", MB_OK | MB_APPLMODAL | MB_ICONERROR);

	std::remove(tmp_file);
	ClearResources();
	exit(1);
}
int IntroFrame()
{
	static int frame{ -1 };
	++frame;
	if (frame > 71)frame = 0;
	return frame;
}
BOOL CheckRecord()
{
	if (score < 1)return no_record;

	int result = 0;
	CheckFile(record_file, &result);

	if (result == FILE_NOT_EXIST)
	{
		std::wofstream rec(record_file);
		rec << score << std::endl;
		for (int i = 0; i < 16; ++i)rec << static_cast<int>(current_player[i]) << std::endl;
		rec.close();
		return first_record;
	}
	else
	{
		std::wifstream check(record_file);
		check >> result;
		check.close();
	}

	if (score > result)
	{
		std::wofstream rec(record_file);
		rec << score << std::endl;
		for (int i = 0; i < 16; ++i)rec << static_cast<int>(current_player[i]) << std::endl;
		rec.close();
		return record;
	}

	return no_record;
}
void GameOver()
{
	PlaySound(NULL, NULL, NULL);
	KillTimer(bHwnd, bTimer);





	bMsg.message = WM_QUIT;
	bMsg.wParam = 0;
}
void InitGame()
{
	wcscpy_s(current_player, L"TARLYO");
	name_set = false;
	distance = 400;

	level_skipped = false;
	boss_active = false;
	assets_move_dir = dirs::stop;

	need_field_up = false;
	need_field_down = false;

	if (!vFields.empty())for (int i = 0; i < vFields.size(); ++i)if (!FreeMem(&vFields[i]))LogErr(L"Error releasing vFields !");
	vFields.clear();

	if (!vTiles.empty())for (int i = 0; i < vTiles.size(); ++i)if (!FreeMem(&vTiles[i]))LogErr(L"Error releasing vTiles !");
	vTiles.clear();

	if (!vClouds.empty())for (int i = 0; i < vClouds.size(); ++i)if (!FreeMem(&vClouds[i]))LogErr(L"Error releasing vClouds !");
	vClouds.clear();

	if (!vHeroShots.empty())for (int i = 0; i < vHeroShots.size(); ++i)if (!FreeMem(&vHeroShots[i]))LogErr(L"Error releasing vHeroShots !");
	vHeroShots.clear();

	if (!vEvilShots.empty())for (int i = 0; i < vEvilShots.size(); ++i)if (!FreeMem(&vEvilShots[i]))LogErr(L"Error releasing vEvilShots !");
	vEvilShots.clear();

	if (!vEvils.empty())for (int i = 0; i < vEvils.size(); ++i)if (!FreeMem(&vEvils[i]))LogErr(L"Error releasing vEvils !");
	vEvils.clear();

	if (!vPowerUps.empty())for (int i = 0; i < vPowerUps.size(); ++i)if (!FreeMem(&vPowerUps[i]))LogErr(L"Error releasing vPowerUps !");
	vPowerUps.clear();

	if (Hero)Hero->Release();
	Hero = dll::HERO::create(scr_width / 2.0f - 50.0f, ground - 100.0f);

	if (Boss)Boss->Release();

	vExplosions.clear();

	for (float i = -scr_height; i < 2 * scr_height; i += scr_height)vFields.push_back(dll::GROUND::create(tiles::field, 0, i));
}
void LevelUp()
{
	if (!level_skipped)
	{
		if (bigText && hgltBrush)
		{
			if (sound)mciSendString(L"play .\\res\\snd\\bonus.wav", NULL, NULL, NULL);
			Draw->BeginDraw();
			Draw->DrawBitmap(bmpIntro[IntroFrame()], D2D1::RectF(0, 0, scr_width, scr_height));
			Draw->DrawTextW(L"БОНУС ПОСТИГНАТ !", 18, bigText, D2D1::RectF(100.0f, sky + 300.0f, scr_width, scr_height), hgltBrush);
			Draw->EndDraw();
			Sleep(3500);
		}

		score += 50 + level * 10;
	}

	if (bigText && hgltBrush)
	{
		if (sound)mciSendString(L"play .\\res\\snd\\levelup.wav", NULL, NULL, NULL);
		Draw->BeginDraw();
		Draw->DrawBitmap(bmpIntro[IntroFrame()], D2D1::RectF(0, 0, scr_width, scr_height));
		Draw->DrawTextW(L"СЛЕДВАЩО НИВО !", 16, bigText, D2D1::RectF(100.0f, sky + 300.0f, scr_width, scr_height), hgltBrush);
		Draw->EndDraw();
		Sleep(3000);
	}

	level_skipped = false;
	boss_active = false;
	assets_move_dir = dirs::stop;

	++level;
	distance = 400 + level * 10;

	need_field_up = false;
	need_field_down = false;

	if (!vFields.empty())for (int i = 0; i < vFields.size(); ++i)if (!FreeMem(&vFields[i]))LogErr(L"Error releasing vFields !");
	vFields.clear();

	if (!vTiles.empty())for (int i = 0; i < vTiles.size(); ++i)if (!FreeMem(&vTiles[i]))LogErr(L"Error releasing vTiles !");
	vTiles.clear();

	if (!vClouds.empty())for (int i = 0; i < vClouds.size(); ++i)if (!FreeMem(&vClouds[i]))LogErr(L"Error releasing vClouds !");
	vClouds.clear();

	if (!vHeroShots.empty())for (int i = 0; i < vHeroShots.size(); ++i)if (!FreeMem(&vHeroShots[i]))LogErr(L"Error releasing vHeroShots !");
	vHeroShots.clear();

	if (!vEvilShots.empty())for (int i = 0; i < vEvilShots.size(); ++i)if (!FreeMem(&vEvilShots[i]))LogErr(L"Error releasing vEvilShots !");
	vEvilShots.clear();

	if (!vEvils.empty())for (int i = 0; i < vEvils.size(); ++i)if (!FreeMem(&vEvils[i]))LogErr(L"Error releasing vEvils !");
	vEvils.clear();

	if (!vPowerUps.empty())for (int i = 0; i < vPowerUps.size(); ++i)if (!FreeMem(&vPowerUps[i]))LogErr(L"Error releasing vPowerUps !");
	vPowerUps.clear();

	if (Hero)Hero->Release();
	Hero = dll::HERO::create(scr_width / 2.0f - 50.0f, ground - 100.0f);

	if (Boss)Boss->Release();

	vExplosions.clear();

	for (float i = -scr_height; i < 2 * scr_height; i += scr_height)vFields.push_back(dll::GROUND::create(tiles::field, 0, i));
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
	switch (ReceivedMsg)
	{
	case WM_INITDIALOG:
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)(mainIcon));
		return true;

	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;

		case IDOK:
			if (GetDlgItemText(hwnd, IDC_NAME, current_player, 16) < 1)
			{
				if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
				wcscpy_s(current_player, L"TARLYO");
				MessageBox(bHwnd, L"Ха, ха, ха ! Забрави си името !", L"Забраватор !", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
				EndDialog(hwnd, IDCANCEL);
				break;
			}
			EndDialog(hwnd, IDOK);
			break;
		}
		break;
	}

	return (INT_PTR)(FALSE);
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
	switch (ReceivedMsg)
	{
	case WM_CREATE:
		if (bIns)
		{
			SetTimer(hwnd, bTimer, 500, NULL);

			bBar = CreateMenu();
			bMain = CreateMenu();
			bStore = CreateMenu();

			AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bMain), L"Основно меню");
			AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bStore), L"Меню за данни");

			AppendMenu(bMain, MF_STRING, mNew, L"Нова игра");
			AppendMenu(bMain, MF_STRING, mLvl, L"Следващо ниво");
			AppendMenu(bMain, MF_SEPARATOR, NULL, NULL);
			AppendMenu(bMain, MF_STRING, mExit, L"Изход");

			AppendMenu(bStore, MF_STRING, mSave, L"Запази игра");
			AppendMenu(bStore, MF_STRING, mLoad, L"Зареди игра");
			AppendMenu(bStore, MF_SEPARATOR, NULL, NULL);
			AppendMenu(bStore, MF_STRING, mHoF, L"Зала на славата");
			SetMenu(hwnd, bBar);

			InitGame();
		}
		break;

	case WM_CLOSE:
		pause = true;
		if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
		if (MessageBox(hwnd, L"Ако излезеш ще изгубиш играта !\n\nНаистина ли излизаш ?", L"Изход !",
			MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
		{
			pause = false;
			break;
		}
		GameOver();
		break;

	case WM_PAINT:
		PaintDC = BeginPaint(hwnd, &bPaint);
		FillRect(PaintDC, &bPaint.rcPaint, CreateSolidBrush(RGB(100, 100, 100)));
		EndPaint(hwnd, &bPaint);
		break;

	case WM_SETCURSOR:
		GetCursorPos(&cur_pos);
		ScreenToClient(hwnd, &cur_pos);
		if (LOWORD(lParam) == HTCLIENT)
		{
			if (!in_client)
			{
				in_client = true;
				pause = false;
			}

			if (cur_pos.y * y_scale <= 50)
			{
				if (cur_pos.x * x_scale >= b1Rect.left && cur_pos.x * x_scale <= b1Rect.right)
				{
					if (!b1Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = true;
						b2Hglt = false;
						b3Hglt = false;
					}
				}
				else if (cur_pos.x * x_scale >= b2Rect.left && cur_pos.x * x_scale <= b2Rect.right)
				{
					if (!b2Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = false;
						b2Hglt = true;
						b3Hglt = false;
					}
				}
				else if (cur_pos.x * x_scale >= b3Rect.left && cur_pos.x * x_scale <= b3Rect.right)
				{
					if (!b3Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = false;
						b2Hglt = false;
						b3Hglt = true;
					}
				}
				else if (b1Hglt || b2Hglt || b3Hglt)
				{
					if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
					b1Hglt = false;
					b2Hglt = false;
					b3Hglt = false;
				}

				SetCursor(outCur);
				return true;
			}
			else if (b1Hglt || b2Hglt || b3Hglt)
			{
				if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
				b1Hglt = false;
				b2Hglt = false;
				b3Hglt = false;
			}

			SetCursor(mainCur);
			return true;
		}
		else
		{
			if (in_client)
			{
				in_client = false;
				pause = true;
			}

			if (b1Hglt || b2Hglt || b3Hglt)
			{
				if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
				b1Hglt = false;
				b2Hglt = false;
				b3Hglt = false;
			}

			SetCursor(LoadCursor(NULL, IDC_ARROW));

			return true;
		}
		break;

	case WM_TIMER:
		if (pause || boss_active)break;
		if (assets_move_dir == dirs::down)distance--;
		else distance++;
		if (distance <= 0)
		{
			boss_active = true;
			Boss = dll::EVILS::create(static_cast<creatures>(RandIt(5, 7)), RandIt(0.0f, scr_width - 200.0f), sky - 100.0f);
			if (sound)mciSendString(L"play .\\res\\snd\\champion.wav", NULL, NULL, NULL);
		}
		break;

	case WM_LBUTTONDOWN:
		if (HIWORD(lParam) * y_scale <= 50)
		{
			if (LOWORD(lParam) * x_scale >= b1Rect.left && LOWORD(lParam) * x_scale <= b1Rect.right)
			{
				if (name_set)
				{
					if (sound)mciSendString(L"play .\\res\\snd\\negative.wav", NULL, NULL, NULL);
					break;
				}

				if (sound)mciSendString(L"play .\\res\\snd\\select.wav", NULL, NULL, NULL);
				if (DialogBox(bIns, MAKEINTRESOURCE(IDD_PLAYER), hwnd, &DlgProc) == IDOK)name_set = true;
			}
			if (LOWORD(lParam) * x_scale >= b2Rect.left && LOWORD(lParam) * x_scale <= b2Rect.right)
			{
				mciSendString(L"play .\\res\\snd\\select.wav", NULL, NULL, NULL);
				if (sound)
				{
					sound = false;
					PlaySound(NULL, NULL, NULL);
					break;
				}
				else
				{
					sound = true;
					PlaySound(sound_file, NULL, SND_ASYNC | SND_LOOP);
					break;
				}
			}
		}
		else
		{
			if (sound)mciSendString(L"play .\\res\\snd\\select.wav", NULL, NULL, NULL);

			if (!pause)
			{
				pause = true;
				break;
			}
			else
			{
				pause = false;
				break;
			}
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case mNew:
			pause = true;
			if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
			if (MessageBox(hwnd, L"Ако рестартираш ще изгубиш играта !\n\nНаистина ли рестартираш ?", L"Рестарт !",
				MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
			{
				pause = false;
				break;
			}
			InitGame();
			break;

		case mLvl:
			pause = true;
			if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
			if (MessageBox(hwnd, L"Ако продължиш ще изгубиш бонусите за нивото !\n\nНаистина ли прескачаш ниво ?", L"Следващо ниво !",
				MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
			{
				pause = false;
				break;
			}
			//LevelUp();
			break;

		case mExit:
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			break;



		}
		break;

	case WM_KEYDOWN:
		if (Hero)
		{
			switch (wParam)
			{
			case VK_LEFT:
				Hero->dir = dirs::left;
				Hero->move((float)(level));
				break;

			case VK_RIGHT:
				Hero->dir = dirs::right;
				Hero->move((float)(level));
				break;

			case VK_UP:
				Hero->dir = dirs::up;
				Hero->move((float)(level));
				break;

			case VK_DOWN:
				Hero->dir = dirs::down;
				Hero->move((float)(level));
				break;

			case VK_SPACE:
				if (Hero->get_current_ammo() == BULLET)
				{
					if (Hero->get_move_dir() == move_dirs::straight)
					{
						if (Hero->dir == dirs::up || Hero->dir == dirs::stop)
							vHeroShots.push_back(dll::SHOTS::create(shots::bullet, Hero->center.x, Hero->center.y,
								Hero->center.x, sky));
						else if (Hero->dir == dirs::down)
							vHeroShots.push_back(dll::SHOTS::create(shots::bullet, Hero->center.x, Hero->center.y,
								Hero->center.x, ground));
					}
					else if (Hero->get_move_dir() == move_dirs::left)
					{
						if (assets_move_dir == dirs::up || assets_move_dir == dirs::stop)
							vHeroShots.push_back(dll::SHOTS::create(shots::bullet, Hero->center.x, Hero->center.y,
								0, ground));
						else 
							vHeroShots.push_back(dll::SHOTS::create(shots::bullet, Hero->center.x, Hero->center.y,
								0, sky));
					}
					else if (Hero->get_move_dir() == move_dirs::right)
					{
						
						if (assets_move_dir == dirs::up || assets_move_dir == dirs::stop)
							vHeroShots.push_back(dll::SHOTS::create(shots::bullet, Hero->center.x, Hero->center.y,
								scr_width, ground));
						else 
							vHeroShots.push_back(dll::SHOTS::create(shots::bullet, Hero->center.x, Hero->center.y,
								scr_width, sky));
					}

					if (sound)mciSendStringW(L"play .\\res\\snd\\shoot.wav", NULL, NULL, NULL);
				}
				else if (Hero->get_current_ammo() == BIG_GUN)
				{
					if (Hero->get_move_dir() == move_dirs::straight)
					{
						if (Hero->dir == dirs::up || Hero->dir == dirs::stop)
							vHeroShots.push_back(dll::SHOTS::create(shots::blast, Hero->center.x, Hero->center.y,
								Hero->center.x, sky));
						else if (Hero->dir == dirs::down)
							vHeroShots.push_back(dll::SHOTS::create(shots::blast, Hero->center.x, Hero->center.y,
								Hero->center.x, ground));
					}
					else if (Hero->get_move_dir() == move_dirs::left)
					{
						if (assets_move_dir == dirs::up || assets_move_dir == dirs::stop)
							vHeroShots.push_back(dll::SHOTS::create(shots::blast, Hero->center.x, Hero->center.y,
								0, ground));
						else
							vHeroShots.push_back(dll::SHOTS::create(shots::blast, Hero->center.x, Hero->center.y,
								0, sky));
					}
					else if (Hero->get_move_dir() == move_dirs::right)
					{

						if (assets_move_dir == dirs::up || assets_move_dir == dirs::stop)
							vHeroShots.push_back(dll::SHOTS::create(shots::blast, Hero->center.x, Hero->center.y,
								scr_width, ground));
						else
							vHeroShots.push_back(dll::SHOTS::create(shots::blast, Hero->center.x, Hero->center.y,
								scr_width, sky));
					}

					if (sound)mciSendStringW(L"play .\\res\\snd\\biggun.wav", NULL, NULL, NULL);
				}
				else if (Hero->get_current_ammo() == ROCKET)
				{
					if (Hero->rockets_available > 0)
					{
						if (Hero->get_move_dir() == move_dirs::straight)
						{
							if (Hero->dir == dirs::up || Hero->dir == dirs::stop)
								vHeroShots.push_back(dll::SHOTS::create(shots::rocket, Hero->center.x, Hero->center.y,
									Hero->center.x, sky));
							else if (Hero->dir == dirs::down)
								vHeroShots.push_back(dll::SHOTS::create(shots::rocket, Hero->center.x, Hero->center.y,
									Hero->center.x, ground));
						}
						else if (Hero->get_move_dir() == move_dirs::left)
						{
							if (assets_move_dir == dirs::up || assets_move_dir == dirs::stop)
								vHeroShots.push_back(dll::SHOTS::create(shots::rocket, Hero->center.x, Hero->center.y,
									0, ground));
							else
								vHeroShots.push_back(dll::SHOTS::create(shots::rocket, Hero->center.x, Hero->center.y,
									0, sky));
						}
						else if (Hero->get_move_dir() == move_dirs::right)
						{

							if (assets_move_dir == dirs::up || assets_move_dir == dirs::stop)
								vHeroShots.push_back(dll::SHOTS::create(shots::rocket, Hero->center.x, Hero->center.y,
									scr_width, ground));
							else
								vHeroShots.push_back(dll::SHOTS::create(shots::rocket, Hero->center.x, Hero->center.y,
									scr_width, sky));
						}

						Hero->rockets_available--;

						if (sound)mciSendStringW(L"play .\\res\\snd\\rocket.wav", NULL, NULL, NULL);
					
						if (Hero->rockets_available <= 0)
						{
							if (Hero->big_gun_found)Hero->set_current_ammo(BIG_GUN);
							else Hero->set_current_ammo(BULLET);
						}
					}
					else
					{
						if (Hero->big_gun_found)Hero->set_current_ammo(BIG_GUN);
						else Hero->set_current_ammo(BULLET);
					}
				}
				break;
			}
		}
		break;

	case WM_CHAR:
		if (Hero)
		{
			switch (wParam)
			{
			case '1':
				if (sound)mciSendString(L"play .\\res\\snd\\loadgun.wav", NULL, NULL, NULL);
				Hero->set_current_ammo(BULLET);
				break;

			case '2':
				if (!Hero->set_current_ammo(BIG_GUN))
				{
					if (sound)mciSendString(L"play .\\res\\snd\\negative.wav", NULL, NULL, NULL);
				}
				else if(sound)mciSendString(L"play .\\res\\snd\\loadgun.wav", NULL, NULL, NULL);
				break;

			case '3':
				if (!Hero->set_current_ammo(ROCKET))
				{
					if (sound)mciSendString(L"play .\\res\\snd\\negative.wav", NULL, NULL, NULL);
				}
				else if (sound)mciSendString(L"play .\\res\\snd\\loadgun.wav", NULL, NULL, NULL);
				break;
			}
		}
		break;

	default: return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);
	}

	return (LRESULT)(FALSE);
}

void CreateResources()
{
	int result = 0;
	CheckFile(Ltmp_file, &result);
	if (result == FILE_EXIST)ErrExit(eStarted);
	else
	{
		std::wofstream start{ Ltmp_file };
		start << L"Game started at: " << std::chrono::system_clock::now();
		start.close();
	}

	int win_x = (int)(GetSystemMetrics(SM_CXSCREEN) / 2 - scr_width / 2.0f);
	int win_y = 50;

	if (GetSystemMetrics(SM_CXSCREEN) < (int)(scr_width) + win_x
		|| GetSystemMetrics(SM_CYSCREEN) < (int)(scr_height) + win_y)ErrExit(eScreen);
	
	mainIcon = (HICON)(LoadImage(NULL, L".\\res\\main.ico", IMAGE_ICON, 255, 255, LR_LOADFROMFILE));
	if (!mainIcon)ErrExit(eIcon);
	mainCur = LoadCursorFromFileW(L".\\res\\main.ani");
	outCur = LoadCursorFromFileW(L".\\res\\out.ani");
	if (!mainCur || !outCur)ErrExit(eCursor);

	bWinClass.lpszClassName = bWinClassName;
	bWinClass.hInstance = bIns;
	bWinClass.lpfnWndProc = &WinProc;
	bWinClass.hbrBackground = CreateSolidBrush(RGB(100, 100, 100));
	bWinClass.hIcon = mainIcon;
	bWinClass.hCursor = mainCur;
	bWinClass.style = CS_DROPSHADOW;

	if (!RegisterClass(&bWinClass))ErrExit(eClass);

	bHwnd = CreateWindow(bWinClassName, L"AIR ASSAULT !", WS_CAPTION | WS_SYSMENU, win_x, win_y, (int)(scr_width),
		(int)(scr_height), NULL, NULL, bIns, NULL);
	if (!bHwnd)ErrExit(eWindow);
	else
	{
		ShowWindow(bHwnd, SW_SHOWDEFAULT);

		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &iFactory);
		if (hr != S_OK)
		{
			LogErr(L"Error creating D2D1 iFactory");
			ErrExit(eD2D);
		}

		if (iFactory)
		{
			hr = iFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(bHwnd,
				D2D1::SizeU((UINT32)(scr_width), (UINT32)(scr_height))), &Draw);
			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 HwndRenderTarget");
				ErrExit(eD2D);
			}
		}

		if (Draw)
		{
			RECT DPI_Rect{};
			GetClientRect(bHwnd, &DPI_Rect);

			D2D1_SIZE_F DIP_Rect{ Draw->GetSize() };

			x_scale = DIP_Rect.width / (DPI_Rect.right - DPI_Rect.left);
			y_scale = DIP_Rect.height / (DPI_Rect.bottom - DPI_Rect.top);

			D2D1_GRADIENT_STOP gSt[2]{};
			ID2D1GradientStopCollection* gColl{ nullptr };

			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkBlue), &statBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &txtBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &hgltBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkOrange), &inactBrush);

			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 SolidColor brushes");
				ErrExit(eD2D);
			}

			gSt[0].position = 0;
			gSt[0].color = D2D1::ColorF(D2D1::ColorF::MediumTurquoise);
			gSt[1].position = 1.0f;
			gSt[1].color = D2D1::ColorF(D2D1::ColorF::MediumSlateBlue);

			hr = Draw->CreateGradientStopCollection(gSt, 2, &gColl);
			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 GradientStopCollection");
				ErrExit(eD2D);
			}

			if (gColl)
			{
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b1Rect.left +
					(b1Rect.right - b1Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), 
					(b1Rect.right - b1Rect.left) / 2.0f, 25.0f), gColl, &b1Bckg);
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b2Rect.left +
					(b2Rect.right - b2Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0),
					(b2Rect.right - b2Rect.left) / 2.0f, 25.0f), gColl, &b2Bckg);
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b3Rect.left +
					(b3Rect.right - b3Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0),
					(b3Rect.right - b3Rect.left) / 2.0f, 25.0f), gColl, &b3Bckg);
			
				if (hr != S_OK)
				{
					LogErr(L"Error creating D2D1 RadialGradientColor brushes");
					ErrExit(eD2D);
				}

				if (!FreeMem(&gColl))LogErr(L"Error releasing D2D1 GradientStopCollection !");
			}

			bmpField = Load(L".\\res\\img\\field\\field.png", Draw);
			if (!bmpField)
			{
				LogErr(L"Error loading bmpField !");
				ErrExit(eD2D);
			}
			bmpLogo = Load(L".\\res\\img\\field\\logo.png", Draw);
			if (!bmpLogo)
			{
				LogErr(L"Error loading bmpLogo !");
				ErrExit(eD2D);
			}
			bmpCloud1 = Load(L".\\res\\img\\field\\Cloud1.png", Draw);
			if (!bmpCloud1)
			{
				LogErr(L"Error loading bmpCloud1 !");
				ErrExit(eD2D);
			}
			bmpCloud2 = Load(L".\\res\\img\\field\\Cloud2.png", Draw);
			if (!bmpCloud2)
			{
				LogErr(L"Error loading bmpCloud2 !");
				ErrExit(eD2D);
			}
			bmpCloud3 = Load(L".\\res\\img\\field\\Cloud3.png", Draw);
			if (!bmpCloud3)
			{
				LogErr(L"Error loading bmpCloud3 !");
				ErrExit(eD2D);
			}
			bmpCloud4 = Load(L".\\res\\img\\field\\Cloud4.png", Draw);
			if (!bmpCloud4)
			{
				LogErr(L"Error loading bmpCloud4 !");
				ErrExit(eD2D);
			}
			bmpCloud5 = Load(L".\\res\\img\\field\\Cloud5.png", Draw);
			if (!bmpCloud5)
			{
				LogErr(L"Error loading bmpCloud5");
				ErrExit(eD2D);
			}
			bmpSeaTile = Load(L".\\res\\img\\field\\sea.png", Draw);
			if (!bmpSeaTile)
			{
				LogErr(L"Error loading bmpSeaTile !");
				ErrExit(eD2D);
			}
			bmpTree1Tile = Load(L".\\res\\img\\field\\tree1.png", Draw);
			if (!bmpTree1Tile)
			{
				LogErr(L"Error loading bmpTree1Tile !");
				ErrExit(eD2D);
			}
			bmpTree2Tile = Load(L".\\res\\img\\field\\tree2.png", Draw);
			if (!bmpTree2Tile)
			{
				LogErr(L"Error loading bmpTree2Tile !");
				ErrExit(eD2D);
			}
			bmpTree3Tile = Load(L".\\res\\img\\field\\tree3.png", Draw);
			if (!bmpTree3Tile)
			{
				LogErr(L"Error loading bmpTree3Tile !");
				ErrExit(eD2D);
			}

			bmpBigGun = Load(L".\\res\\img\\powerups\\big_gun.png", Draw);
			if (!bmpBigGun)
			{
				LogErr(L"Error loading bmpBigGun !");
				ErrExit(eD2D);
			}
			bmpRepair = Load(L".\\res\\img\\powerups\\Repair.png", Draw);
			if (!bmpRepair)
			{
				LogErr(L"Error loading bmpRepair !");
				ErrExit(eD2D);
			}
			bmpRocket = Load(L".\\res\\img\\powerups\\Rocket.png", Draw);
			if (!bmpRocket)
			{
				LogErr(L"Error loading bmpRocket !");
				ErrExit(eD2D);
			}
			bmpShield = Load(L".\\res\\img\\powerups\\Shield.png", Draw);
			if (!bmpShield)
			{
				LogErr(L"Error loading bmpShield !");
				ErrExit(eD2D);
			}

			bmpShot = Load(L".\\res\\img\\shots\\bullet.png", Draw);
			if (!bmpShot)
			{
				LogErr(L"Error loading bmpShot !");
				ErrExit(eD2D);
			}
			bmpBigShot = Load(L".\\res\\img\\shots\\big_shot.png", Draw);
			if (!bmpBigShot)
			{
				LogErr(L"Error loading bmpBigShot !");
				ErrExit(eD2D);
			}
			bmpRocketD = Load(L".\\res\\img\\shots\\rocket_down.png", Draw);
			if (!bmpRocketD)
			{
				LogErr(L"Error loading bmpRocketD !");
				ErrExit(eD2D);
			}
			bmpRocketU = Load(L".\\res\\img\\shots\\rocket_up.png", Draw);
			if (!bmpRocketU)
			{
				LogErr(L"Error loading bmpRocketU !");
				ErrExit(eD2D);
			}

			for (int i = 0; i < 4; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\boss1\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpBoss1[i] = Load(name, Draw);
				if (!bmpBoss1[i])
				{
					LogErr(L"Error loading bmpBoss1 !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 2; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\boss2\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpBoss2[i] = Load(name, Draw);
				if (!bmpBoss2[i])
				{
					LogErr(L"Error loading bmpBoss2 !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 10; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\boss3\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpBoss3[i] = Load(name, Draw);
				if (!bmpBoss3[i])
				{
					LogErr(L"Error loading bmpBoss3 !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 6; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil1\\left\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil1L[i] = Load(name, Draw);
				if (!bmpEvil1L[i])
				{
					LogErr(L"Error loading bmpEvil1L !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 6; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil1\\right\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil1R[i] = Load(name, Draw);
				if (!bmpEvil1R[i])
				{
					LogErr(L"Error loading bmpEvil1R !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 12; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil1\\straight\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil1S[i] = Load(name, Draw);
				if (!bmpEvil1S[i])
				{
					LogErr(L"Error loading bmpEvil1S !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 5; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil2\\left\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil2L[i] = Load(name, Draw);
				if (!bmpEvil2L[i])
				{
					LogErr(L"Error loading bmpEvil2L !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 7; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil2\\right\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil2R[i] = Load(name, Draw);
				if (!bmpEvil2R[i])
				{
					LogErr(L"Error loading bmpEvil2R !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 12; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil2\\straight\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil2S[i] = Load(name, Draw);
				if (!bmpEvil2S[i])
				{
					LogErr(L"Error loading bmpEvil2S !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 6; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil3\\left\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil3L[i] = Load(name, Draw);
				if (!bmpEvil3L[i])
				{
					LogErr(L"Error loading bmpEvil3L !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 6; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil3\\right\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil3R[i] = Load(name, Draw);
				if (!bmpEvil3R[i])
				{
					LogErr(L"Error loading bmpEvil3R !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 13; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil3\\straight\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil3S[i] = Load(name, Draw);
				if (!bmpEvil3S[i])
				{
					LogErr(L"Error loading bmpEvil3S !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 5; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil4\\left\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil4L[i] = Load(name, Draw);
				if (!bmpEvil4L[i])
				{
					LogErr(L"Error loading bmpEvil4L !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 6; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil4\\right\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil4R[i] = Load(name, Draw);
				if (!bmpEvil4R[i])
				{
					LogErr(L"Error loading bmpEvil4R !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 12; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil4\\straight\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil4S[i] = Load(name, Draw);
				if (!bmpEvil4S[i])
				{
					LogErr(L"Error loading bmpEvil4S !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 5; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil5\\left\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil5L[i] = Load(name, Draw);
				if (!bmpEvil5L[i])
				{
					LogErr(L"Error loading bmpEvil5L !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 5; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil5\\right\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil5R[i] = Load(name, Draw);
				if (!bmpEvil5R[i])
				{
					LogErr(L"Error loading bmpEvil5R !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 14; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\evil5\\straight\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpEvil5S[i] = Load(name, Draw);
				if (!bmpEvil5S[i])
				{
					LogErr(L"Error loading bmpEvil5S !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 7; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\hero\\left\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroL[i] = Load(name, Draw);
				if (!bmpHeroL[i])
				{
					LogErr(L"Error loading bmpHeroL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 7; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\hero\\right\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroR[i] = Load(name, Draw);
				if (!bmpHeroR[i])
				{
					LogErr(L"Error loading bmpHeroR !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 10; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\hero\\straight\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroS[i] = Load(name, Draw);
				if (!bmpHeroS[i])
				{
					LogErr(L"Error loading bmpHeroS !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 24; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\explosion\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpExplosion[i] = Load(name, Draw);
				if (!bmpExplosion[i])
				{
					LogErr(L"Error loading bmpExplosion !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 72; ++i)
			{
				wchar_t name[75]{ L".\\res\\img\\intro\\" };
				wchar_t add[5]{ L"\0" };
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpIntro[i] = Load(name, Draw);
				if (!bmpIntro[i])
				{
					LogErr(L"Error loading bmpIntro !");
					ErrExit(eD2D);
				}
			}
		}

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&iWriteFactory));
		if (hr != S_OK)
		{
			LogErr(L"Error creating D2D1 iWriteFactory");
			ErrExit(eD2D);
		}

		if (iWriteFactory)
		{
			hr = iWriteFactory->CreateTextFormat(L"CASCADIA CODE", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_OBLIQUE,
				DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"", &nrmText);
			hr = iWriteFactory->CreateTextFormat(L"CASCADIA CODE", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_OBLIQUE,
				DWRITE_FONT_STRETCH_NORMAL, 24.0f, L"", &midText);
			hr = iWriteFactory->CreateTextFormat(L"CASCADIA CODE", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_OBLIQUE,
				DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"", &bigText);
			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 iWriteFactory Text formats");
				ErrExit(eD2D);
			}
		}
	}

	mciSendString(L"play .\\res\\snd\\intro.wav", NULL, NULL, NULL);
	for (int i = 0; i <= 200; ++i)
	{
		Draw->BeginDraw();
		Draw->DrawBitmap(bmpIntro[IntroFrame()], D2D1::RectF(0, 0, scr_width, scr_height));
		Draw->DrawBitmap(bmpLogo, D2D1::RectF(0, 0, scr_width, scr_height));
		Draw->EndDraw();
	}
	Draw->BeginDraw();
	Draw->DrawBitmap(bmpIntro[IntroFrame()], D2D1::RectF(0, 0, scr_width, scr_height));
	Draw->DrawBitmap(bmpLogo, D2D1::RectF(0, 0, scr_width, scr_height));
	Draw->EndDraw();

	PlaySound(L".\\res\\snd\\boom.wav", NULL, SND_SYNC);
	Sleep(1000);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_wsetlocale(LC_ALL, L"");

	bIns = hInstance;
	if (bIns == nullptr)
	{
		LogErr(L"Error in Windows hInstance !");
		ErrExit(eWindow);
	}

	CreateResources();

	while (bMsg.message != WM_QUIT)
	{
		if ((bRet = PeekMessage(&bMsg, NULL, NULL, NULL, PM_REMOVE)) != 0)
		{
			if (bRet == -1)ErrExit(eMsg);

			TranslateMessage(&bMsg);
			DispatchMessage(&bMsg);
		}

		if (pause)
		{
			if (show_help)continue;
			Draw->BeginDraw();
			Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkKhaki));
			if (bigText && hgltBrush)Draw->DrawTextW(L"ПАУЗА", 6, bigText,
				D2D1::RectF(scr_width / 2.0f - 100.0f, scr_height / 2.0f - 50.0f, scr_width, scr_height), hgltBrush);
			Draw->EndDraw();
			continue;
		}

		//////////////////////////////////////////////////////

		if (!vFields.empty())
		{
			for (std::vector<dll::GROUND*>::iterator field = vFields.begin(); field < vFields.end(); ++field)
			{
				(*field)->dir = assets_move_dir;

				if (!(*field)->move((float)(level)))
				{
					if ((*field)->type == tiles::field)
					{
						if ((*field)->dir == dirs::down)need_field_up = true;
						else need_field_down = true;
					}
					(*field)->Release();
					vFields.erase(field);
					break;
				}
			}
		}

		if (need_field_up)
		{
			need_field_up = false;
			vFields.insert(vFields.begin(), dll::GROUND::create(tiles::field, 0,
				vFields.front()->start.y - vFields.front()->get_height()));
		}
		if (need_field_down)
		{
			need_field_down = false;
			vFields.push_back(dll::GROUND::create(tiles::field, 0,
				vFields.back()->end.y));
		}

		if (vTiles.size() < 40 + level && RandIt(0, 100) == 66)
		{
			int type = RandIt(0, 3);

			if (type == 0)
			{
				float tx = RandIt(0.0f, scr_width - 320.0f);
				float ty = RandIt(-scr_height / 3.0f, sky - 200.0f);

				for (int row = 0; row < 5; ++row)
				{
					for (int col = 0; col < 10; ++col)
					{
						dll::GROUND* sea_tile{ dll::GROUND::create(tiles::sea, tx, ty) };
						bool is_ok = true;
						if (!vTiles.empty())
						{
							for (int i = 0; i < vTiles.size(); ++i)
							{
								FRECT dummy{ sea_tile->start.x + 1.0f, sea_tile->start.y + 1.0f, 
									sea_tile->end.x + 1.0f, sea_tile->end.y + 1.0f };

								if (dll::Intersect(FRECT{ vTiles[i]->start.x,vTiles[i]->start.y, 
									vTiles[i]->end.x, vTiles[i]->end.y }, dummy))
								{
									is_ok = false;
									break;
								}
							}
						}

						if (is_ok) vTiles.push_back(sea_tile);

						tx += 32.0f;
					}

					tx -= 320.0f;
					ty += 32.0f;
				}
			}
			else if (type == 1)
			{
				float tx = RandIt(0.0f, scr_width - 200.0f);
				float ty = RandIt(-scr_height / 3.0f, sky - 200.0f);

				dll::GROUND* tree_tile{ dll::GROUND::create(tiles::tree1, tx, ty) };

				bool is_ok = true;

				if (!vTiles.empty())
				{
					for (int i = 0; i < vTiles.size(); ++i)
					{
						if (dll::Intersect(FRECT{ vTiles[i]->start.x,vTiles[i]->start.y, vTiles[i]->end.x,vTiles[i]->end.y },
							FRECT{ tree_tile->start.x, tree_tile->start.y, tree_tile->end.x, tree_tile->end.y }))
						{
							is_ok = false;
							break;
						}
					}
				}

				if (is_ok)vTiles.push_back(tree_tile);
			}
			else if (type == 2)
			{
				float tx = RandIt(0.0f, scr_width - 200.0f);
				float ty = RandIt(-scr_height / 3.0f, sky - 200.0f);

				dll::GROUND* tree_tile{ dll::GROUND::create(tiles::tree2, tx, ty) };

				bool is_ok = true;

				if (!vTiles.empty())
				{
					for (int i = 0; i < vTiles.size(); ++i)
					{
						if (dll::Intersect(FRECT{ vTiles[i]->start.x,vTiles[i]->start.y, vTiles[i]->end.x,vTiles[i]->end.y },
							FRECT{ tree_tile->start.x, tree_tile->start.y, tree_tile->end.x, tree_tile->end.y }))
						{
							is_ok = false;
							break;
						}
					}
				}

				if (is_ok)vTiles.push_back(tree_tile);
			}
			else if (type == 3)
			{
				float tx = RandIt(0.0f, scr_width - 200.0f);
				float ty = RandIt(-scr_height / 3.0f, sky - 200.0f);

				dll::GROUND* tree_tile{ dll::GROUND::create(tiles::tree3, tx, ty) };

				bool is_ok = true;

				if (!vTiles.empty())
				{
					for (int i = 0; i < vTiles.size(); ++i)
					{
						if (dll::Intersect(FRECT{ vTiles[i]->start.x,vTiles[i]->start.y, vTiles[i]->end.x,vTiles[i]->end.y },
							FRECT{ tree_tile->start.x, tree_tile->start.y, tree_tile->end.x, tree_tile->end.y }))
						{
							is_ok = false;
							break;
						}
					}
				}

				if (is_ok)vTiles.push_back(tree_tile);
			}
		}
		if (!vTiles.empty())
		{
			for (std::vector<dll::GROUND*>::iterator tile = vTiles.begin(); tile < vTiles.end(); ++tile)
			{
				(*tile)->dir = assets_move_dir;

				if (!(*tile)->move((float)(level)))
				{
					(*tile)->Release();
					vTiles.erase(tile);
					break;
				}
			}
		}

		if (vClouds.size() < 4 + level && RandIt(0, 200) == 33)
		{
			if (RandIt(0, 2) == 1)vClouds.push_back(dll::CLOUDS::create(static_cast<clouds>(RandIt(0, 4)), RandIt(-200.0f, -190.0f + 
				RandIt(0.0f, 100.0f)), RandIt(sky, ground - 250.0f)));
			else
			{
				vClouds.push_back(dll::CLOUDS::create(static_cast<clouds>(RandIt(0, 4)), scr_width + 200.0f, 
					RandIt(sky, ground - 250.0f)));
				vClouds.back()->dir = dirs::left;
			}
		}
		if (!vClouds.empty())
		{
			for (std::vector<dll::CLOUDS*>::iterator cloud = vClouds.begin(); cloud < vClouds.end(); ++cloud)
			{
				float ty = 0;

				if (assets_move_dir == dirs::down)ty = ground;
				else ty = sky;

				if ((*cloud)->dir == dirs::right)
				{
					if (!(*cloud)->move(scr_width + scr_width / 3.0f, ty, (float)(level)))
					{
						(*cloud)->Release();
						vClouds.erase(cloud);
						break;
					}
					if ((*cloud)->start.x >= scr_width + scr_width / 3.0f)(*cloud)->dir = dirs::left;
				}
				else if ((*cloud)->dir == dirs::left)
				{
					if (!(*cloud)->move(- scr_width / 3.0f, ty, (float)(level)))
					{
						(*cloud)->Release();
						vClouds.erase(cloud);
						break;
					}
					if ((*cloud)->start.x <= - scr_width / 3.0f)(*cloud)->dir = dirs::right;
				}
			}
		}

		////////////////////////////////////////////

		if (Hero)
		{
			if (Hero->dir == dirs::left || Hero->dir == dirs::right)Hero->move((float)(level));
		}

		if (!vHeroShots.empty())
		{
			for (std::vector<dll::SHOTS*>::iterator shot = vHeroShots.begin(); shot < vHeroShots.end(); ++shot)
			{
				if (!(*shot)->move((float)(level)))
				{
					(*shot)->Release();
					vHeroShots.erase(shot);
					break;
				}
			}
		}

		////////////////////////////////////////////

		if (vEvils.size() < 6 + level && RandIt(0, 100) == 66)
		{
			creatures temp_type(static_cast<creatures>(RandIt(0, 4)));

			float tx = RandIt(0.0f, scr_width - 50.0f);

			vEvils.push_back(dll::EVILS::create(temp_type, tx, 0));
		}

		if (!vEvils.empty() && Hero)
		{
			for (std::vector<dll::EVILS*>::iterator evil = vEvils.begin(); evil < vEvils.end(); ++evil)
			{
				dll::BAG<FPOINT> Others(vEvils.size());

				dll::BAG<FPOINT> Bullets(vHeroShots.size());

				if (!vEvils.empty())
					for (size_t count = 0; count < Others.size(); ++count)
						if ((*evil)->center.x != vEvils[count]->center.x
							&& (*evil)->center.y != vEvils[count]->center.y)Others.push_back(vEvils[count]->center);

				if (!vHeroShots.empty())
					for (size_t count = 0; count < Bullets.size(); ++count)Bullets.push_back(vHeroShots[count]->center);

				actions next_action = dll::AINextMove(*(*evil), Hero->center, Bullets, Others);

				FPOINT current_target{ (*evil)->get_target_point() };
				float target_x = current_target.x;
				float target_y = current_target.y;
				int shoot_damage = 0;

				bool killed = false;

				switch (next_action)
				{
				case actions::dir_changed:
					if (!(*evil)->move(target_x, target_y, (float)(level)))
					{
						(*evil)->Release();
						vEvils.erase(evil);
						killed = true;
						break;
					}
					break;

				case actions::move:
					if (!(*evil)->move(target_x, target_y, (float)(level)))
					{
						(*evil)->Release();
						vEvils.erase(evil);
						killed = true;
						break;
					}
					break;

				case actions::shoot:
					shoot_damage = (*evil)->attack();
					if (shoot_damage > 0) 
					{
						if (sound)mciSendStringW(L"play .\\res\\snd\\shoot.wav", NULL, NULL, NULL);
						vEvilShots.push_back(dll::SHOTS::create(shots::bullet, (*evil)->center.x,
							(*evil)->center.y, Hero->center.x, Hero->center.y));
						vEvilShots.back()->damage = shoot_damage;
					}
					break;
				}

				if (killed)break;
			}
		}

		if (!vEvilShots.empty())
		{
			for (std::vector<dll::SHOTS*>::iterator shot = vEvilShots.begin(); shot < vEvilShots.end(); ++shot)
			{
				if (!(*shot)->move((float)(level)))
				{
					(*shot)->Release();
					vEvilShots.erase(shot);
					break;
				}
			}
		}

		////////////////////////////////////////////

		if (!vHeroShots.empty() && !vEvils.empty())
		{
			bool killed = false;

			for (std::vector<dll::EVILS*>::iterator evil = vEvils.begin(); evil < vEvils.end(); ++evil)
			{
				for (std::vector<dll::SHOTS*>::iterator shot = vHeroShots.begin(); shot < vHeroShots.end(); ++shot)
				{
					if (dll::Intersect(FPOINT((*shot)->center.x, (*shot)->center.y),
						FPOINT((*evil)->center.x, (*evil)->center.y),
						(*shot)->x_rad, (*evil)->x_rad, (*shot)->y_rad, (*evil)->y_rad))
					{
						(*evil)->lifes -= (*shot)->damage;
						(*shot)->Release();
						vHeroShots.erase(shot);
						if ((*evil)->lifes <= 0)
						{
							if (sound)mciSendString(L"play .\\res\\snd\\boom.wav", NULL, NULL, NULL);
							killed = true;
							score += 5 + level;
							vExplosions.push_back(EXPLOSION{ (*evil)->start.x,(*evil)->start.y });
							
							if (RandIt(0, 3) == 2)
							{
								powerups ttype = static_cast<powerups>(RandIt(0, 3));
								vPowerUps.push_back(dll::POWERUPS::create(ttype, (*evil)->center.x, (*evil)->center.y));
								if (sound)mciSendString(L"play .\\res\\snd\\powerup.wav", NULL, NULL, NULL);
							}
							
							(*evil)->Release();
							vEvils.erase(evil);
						}
						break;
					}
				}

				if (killed)break;
			}
		}
		
		if (Hero && !vEvilShots.empty())
		{
			for (std::vector<dll::SHOTS*>::iterator shot = vEvilShots.begin(); shot < vEvilShots.end(); ++shot)
			{
				if (dll::Intersect(FPOINT(Hero->start.x, Hero->start.y), FPOINT((*shot)->start.x, (*shot)->start.y),
					Hero->x_rad, (*shot)->x_rad, Hero->y_rad, (*shot)->y_rad))
				{
					Hero->lifes -= ((*shot)->damage - Hero->armour);
					(*shot)->Release();
					vEvilShots.erase(shot);

					if (Hero->lifes <= 0)
					{
						vExplosions.push_back(EXPLOSION{ Hero->center.x,Hero->center.y });
						Hero->Release();
						Hero = nullptr;
					}

					break;
				}
			}
		}

		if (Hero && !vEvils.empty())
		{
			for (std::vector<dll::EVILS*>::iterator evil = vEvils.begin(); evil < vEvils.end(); ++evil)
			{
				if (dll::Intersect(FPOINT(Hero->center.x, Hero->center.y), FPOINT((*evil)->center.x, (*evil)->center.y),
					Hero->x_rad, (*evil)->x_rad, Hero->y_rad, (*evil)->y_rad))
				{
					if (sound)mciSendString(L"play .\\res\\snd\\boom.wav", NULL, NULL, NULL);

					vExplosions.push_back(EXPLOSION((*evil)->center.x, (*evil)->center.y));
					vExplosions.push_back(EXPLOSION(Hero->center.x, Hero->center.y));
					
					(*evil)->Release();
					vEvils.erase(evil);
					Hero->Release();
					Hero = nullptr;
					break;
				}
			}
		}

		if (Boss && Hero)
		{
			dll::BAG<FPOINT>others;
			dll::BAG<FPOINT>shots;

			if (!vEvils.empty())
				for (int i = 0; i < vEvils.size(); ++i)others.push_back(vEvils[i]->center);

			if (!vHeroShots.empty())
				for (int i = 0; i < vHeroShots.size(); ++i)shots.push_back(vHeroShots[i]->center);

			actions next_action = dll::AINextMove(*Boss, Hero->center, shots, others);

			if (Boss->end.y >= scr_height - 100.0f)
			{
				if (Boss->end.x <= scr_width / 2.0f)Boss->set_path(scr_width, sky);
				else Boss->set_path(0, sky);
			}

			FPOINT BossTarget{ Boss->get_target_point() };

			float boss_ex = BossTarget.x;
			float boss_ey = BossTarget.y;
			int damage = 0;

			switch (next_action)
			{
			case actions::move:
				Boss->move(boss_ex, boss_ey, (float)(level));
				break;

			case actions::dir_changed:
				Boss->move(boss_ex, boss_ey, (float)(level));
				break;

			case actions::shoot:
				damage = Boss->attack();
				if (damage > 0)
				{
					switch (RandIt(0, 7))
					{
					case 7:
						vEvilShots.push_back(dll::SHOTS::create(shots::rocket, Boss->center.x, Boss->center.y,
							Hero->center.x, Hero->center.y));
						break;

					case 6:
						vEvilShots.push_back(dll::SHOTS::create(shots::blast, Boss->center.x, Boss->center.y,
							Hero->center.x, Hero->center.y));
						vEvilShots.back()->damage -= 10;
						break;

					case 4:
						vEvilShots.push_back(dll::SHOTS::create(shots::blast, Boss->center.x, Boss->center.y,
							Hero->center.x, Hero->center.y));
						vEvilShots.back()->damage -= 10;
						break;

					default:
						vEvilShots.push_back(dll::SHOTS::create(shots::bullet, Boss->center.x, Boss->center.y,
							Hero->center.x, Hero->center.y));
						vEvilShots.back()->damage -= 15;
						break;
					}
				}
				break;
			}
		}

		if (Boss && Hero)
		{
			if (dll::Intersect(FPOINT(Boss->center.x, Boss->center.y), FPOINT(Hero->center.x, Hero->center.y),
				Boss->x_rad, Hero->x_rad, Boss->y_rad, Hero->y_rad))
			{
				if (sound)mciSendString(L"play .\\res\\snd\\boom.wav", NULL, NULL, NULL);

				vExplosions.push_back(EXPLOSION(Boss->center.x, Boss->center.y));
				vExplosions.push_back(EXPLOSION(Hero->center.x, Hero->center.y));

				Boss->Release();
				Boss = nullptr;
				Hero->Release();
				Hero = nullptr;
			}
		}

		if (Boss && !vHeroShots.empty())
		{
			for (std::vector<dll::SHOTS*>::iterator shot = vHeroShots.begin(); shot < vHeroShots.end(); ++shot)
			{
				if (dll::Intersect(FPOINT(Boss->center.x, Boss->center.y), FPOINT((*shot)->center.x, (*shot)->center.y),
					Boss->x_rad, (*shot)->x_rad, Boss->y_rad, (*shot)->y_rad))
				{
					Boss->lifes -= (*shot)->damage;
					(*shot)->Release();
					vHeroShots.erase(shot);

					if (Boss->lifes <= 0)
					{
						Boss->Release();
						Boss = nullptr;
					}
					break;
				}
			}
		}

		if (Hero && !vPowerUps.empty())
		{
			for (std::vector<dll::POWERUPS*>::iterator power = vPowerUps.begin(); power < vPowerUps.end(); ++power)
			{
				if (dll::Intersect(FRECT{ Hero->start.x, Hero->start.y, Hero->end.x, Hero->end.y },
					FRECT{ (*power)->start.x, (*power)->start.y, (*power)->end.x, (*power)->end.y }))
				{
					switch ((*power)->type)
					{
					case powerups::big_gun:
						Hero->big_gun_found = true;
						if (sound)mciSendString(L"play .\\res\\snd\\load.wav", NULL, NULL, NULL);
						break;

					case powerups::rocket:
						Hero->rockets_available++;
						if (sound)mciSendString(L"play .\\res\\snd\\rocket.wav", NULL, NULL, NULL);
						break;

					case powerups::repair:
						if (sound)mciSendString(L"play .\\res\\snd\\heal.wav", NULL, NULL, NULL);
						if (Hero->lifes + 50 <= 150)Hero->lifes += 50;
						else Hero->lifes = 150;
						break;

					case powerups::shield:
						if (sound)mciSendString(L"play .\\res\\snd\\heal.wav", NULL, NULL, NULL);
						Hero->armour++;
						break;
					}

					(*power)->Release();
					vPowerUps.erase(power);
					break;
				}
						
			}
		}

		// DRAW THINGS ************************************************

		Draw->BeginDraw();

		if (!vFields.empty())
			for (int i = 0; i < vFields.size(); ++i)
				if ((vFields[i]->start.y >= 0 && vFields[i]->start.y <= scr_height)
					|| (vFields[i]->end.y >= 0 && vFields[i]->end.y <= scr_height))
					Draw->DrawBitmap(bmpField, D2D1::RectF(vFields[i]->start.x, vFields[i]->start.y,
						vFields[i]->end.x, vFields[i]->end.y));

		if (!vTiles.empty())
			for (int i = 0; i < vTiles.size(); ++i)
				if ((vTiles[i]->start.y >= sky && vTiles[i]->start.y <= scr_height)
					|| (vTiles[i]->end.y >= sky && vTiles[i]->start.y <= scr_height))

				{
					switch (vTiles[i]->type)
					{
					case tiles::sea:
						Draw->DrawBitmap(bmpSeaTile, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
							vTiles[i]->end.x, vTiles[i]->end.y));
						break;

					case tiles::tree1:
						Draw->DrawBitmap(bmpTree1Tile, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
							vTiles[i]->end.x, vTiles[i]->end.y));
						break;

					case tiles::tree2:
						Draw->DrawBitmap(bmpTree2Tile, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
							vTiles[i]->end.x, vTiles[i]->end.y));
						break;

					case tiles::tree3:
						Draw->DrawBitmap(bmpTree3Tile, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
							vTiles[i]->end.x, vTiles[i]->end.y));
						break;
					}
				}

		if (!vEvils.empty())
		{
			for (int i = 0; i < vEvils.size(); ++i)
			{
				if (vEvils[i]->end.y >= sky && vEvils[i]->start.y <= scr_height)

				{
					int frame = vEvils[i]->get_frame();

					switch (vEvils[i]->type)
					{
					case creatures::evil1:
						if (vEvils[i]->get_move_dir() == move_dirs::left)
							Draw->DrawBitmap(bmpEvil1L[frame], Resizer(bmpEvil1L[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						else if (vEvils[i]->get_move_dir() == move_dirs::right)
							Draw->DrawBitmap(bmpEvil1R[frame], Resizer(bmpEvil1R[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						else
							Draw->DrawBitmap(bmpEvil1S[frame], Resizer(bmpEvil1S[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						break;

					case creatures::evil2:
						if (vEvils[i]->get_move_dir() == move_dirs::left)
							Draw->DrawBitmap(bmpEvil2L[frame], Resizer(bmpEvil2L[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						if (vEvils[i]->get_move_dir() == move_dirs::right)
							Draw->DrawBitmap(bmpEvil2R[frame], Resizer(bmpEvil2R[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						else
							Draw->DrawBitmap(bmpEvil2S[frame], Resizer(bmpEvil2S[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						break;

					case creatures::evil3:
						if (vEvils[i]->get_move_dir() == move_dirs::left)
							Draw->DrawBitmap(bmpEvil3L[frame], Resizer(bmpEvil3L[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						else if (vEvils[i]->get_move_dir() == move_dirs::right)
							Draw->DrawBitmap(bmpEvil3R[frame], Resizer(bmpEvil3R[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						else
							Draw->DrawBitmap(bmpEvil3S[frame], Resizer(bmpEvil3S[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						break;

					case creatures::evil4:
						if (vEvils[i]->get_move_dir() == move_dirs::left)
							Draw->DrawBitmap(bmpEvil4L[frame], Resizer(bmpEvil4L[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						else if (vEvils[i]->get_move_dir() == move_dirs::right)
							Draw->DrawBitmap(bmpEvil4R[frame], Resizer(bmpEvil4R[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						else
							Draw->DrawBitmap(bmpEvil4S[frame], Resizer(bmpEvil4S[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						break;

					case creatures::evil5:
						if (vEvils[i]->get_move_dir() == move_dirs::left)
							Draw->DrawBitmap(bmpEvil5L[frame], Resizer(bmpEvil5L[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						else if (vEvils[i]->get_move_dir() == move_dirs::right)
							Draw->DrawBitmap(bmpEvil5R[frame], Resizer(bmpEvil5R[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						else
							Draw->DrawBitmap(bmpEvil5S[frame], Resizer(bmpEvil5S[frame], vEvils[i]->start.x, vEvils[i]->start.y));
						break;
					}

					Draw->DrawLine(D2D1::Point2F(vEvils[i]->start.x, vEvils[i]->end.y + 2.0f),
						D2D1::Point2F(vEvils[i]->start.x + vEvils[i]->lifes / 1.5f, vEvils[i]->end.y + 2.0f), hgltBrush, 5.0f);
				}
			}
		}
		
		if (Boss)
		{
			if (Boss->end.y >= sky && Boss->start.y <= scr_height)
			{
				int frame = Boss->get_frame();
				switch (Boss->type)
				{

				case creatures::boss1:
					Draw->DrawBitmap(bmpBoss1[frame], Resizer(bmpBoss1[frame], Boss->start.x, Boss->start.y));
					break;

				case creatures::boss2:
					Draw->DrawBitmap(bmpBoss2[frame], Resizer(bmpBoss2[frame], Boss->start.x, Boss->start.y));
					break;

				case creatures::boss3:
					Draw->DrawBitmap(bmpBoss3[frame], Resizer(bmpBoss3[frame], Boss->start.x, Boss->start.y));
					break;
				}

				Draw->DrawLine(D2D1::Point2F(Boss->start.x, Boss->end.y + 5.0f), D2D1::Point2F(Boss->start.x + Boss->lifes / 2.0f, 
					Boss->end.y + 5.0f), hgltBrush, 5.0f);
			}
		}

		if (nrmText && statBrush && txtBrush && hgltBrush && inactBrush && b1Bckg && b2Bckg && b3Bckg)
		{
			Draw->FillRectangle(D2D1::RectF(0, 0, scr_width, 50.0f), statBrush);
			Draw->FillRoundedRectangle(D2D1::RoundedRect(b1Rect, 10.0f, 15.0f), b1Bckg);
			Draw->FillRoundedRectangle(D2D1::RoundedRect(b2Rect, 10.0f, 15.0f), b2Bckg);
			Draw->FillRoundedRectangle(D2D1::RoundedRect(b3Rect, 10.0f, 15.0f), b3Bckg);
			if (name_set)Draw->DrawTextW(L"ИМЕ НА ПИЛОТ", 13, nrmText, b1TxtRect, inactBrush);
			else
			{
				if(!b1Hglt)Draw->DrawTextW(L"ИМЕ НА ПИЛОТ", 13, nrmText, b1TxtRect, txtBrush);
				else Draw->DrawTextW(L"ИМЕ НА ПИЛОТ", 13, nrmText, b1TxtRect, hgltBrush);
			}
			if (!b2Hglt)Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmText, b2TxtRect, txtBrush);
			else Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmText, b2TxtRect, hgltBrush);
			if (!b3Hglt)Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmText, b3TxtRect, txtBrush);
			else Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmText, b3TxtRect, hgltBrush);
		}
		
		///////////////////////////////////////////////////////////////

		if (Hero)
		{
			int current_frame = Hero->get_frame();

			switch (Hero->get_move_dir())
			{
			case move_dirs::straight:
				Draw->DrawBitmap(bmpHeroS[current_frame], Resizer(bmpHeroS[current_frame], Hero->start.x, Hero->start.y));
				break;

			case move_dirs::left:
				Draw->DrawBitmap(bmpHeroL[current_frame], Resizer(bmpHeroS[current_frame], Hero->start.x, Hero->start.y));
				break;

			case move_dirs::right:
				Draw->DrawBitmap(bmpHeroR[current_frame], Resizer(bmpHeroS[current_frame], Hero->start.x, Hero->start.y));
				break;
			}

			if (Hero->dir == dirs::down)assets_move_dir = dirs::up;
			else assets_move_dir = dirs::down;

			Draw->DrawLine(D2D1::Point2F(Hero->start.x - 10.0f, Hero->end.y + 2.0f),
				D2D1::Point2F(Hero->start.x + Hero->lifes / 2.0f, Hero->end.y + 2.0f), txtBrush, 5.0f);
		}

		if (!vHeroShots.empty())
		{
			for (int i = 0; i < vHeroShots.size(); ++i)
			{
				switch (vHeroShots[i]->type)
				{
				case shots::bullet:
					Draw->DrawBitmap(bmpShot, D2D1::RectF(vHeroShots[i]->start.x, vHeroShots[i]->start.y,
						vHeroShots[i]->end.x, vHeroShots[i]->end.y));
					break;

				case shots::blast:
					Draw->DrawBitmap(bmpBigGun, D2D1::RectF(vHeroShots[i]->start.x, vHeroShots[i]->start.y,
						vHeroShots[i]->end.x, vHeroShots[i]->end.y));
					break;

				case shots::rocket:
					if (vHeroShots[i]->dir == dirs::up)
						Draw->DrawBitmap(bmpRocketU, D2D1::RectF(vHeroShots[i]->start.x, vHeroShots[i]->start.y,
							vHeroShots[i]->end.x, vHeroShots[i]->end.y));
					else Draw->DrawBitmap(bmpRocketD, D2D1::RectF(vHeroShots[i]->start.x, vHeroShots[i]->start.y,
						vHeroShots[i]->end.x, vHeroShots[i]->end.y));
					break;
				}
			}
		}
		
		if (!vEvilShots.empty())
		{
			for (int i = 0; i < vEvilShots.size(); ++i)
			{
				switch (vEvilShots[i]->type)
				{
				case shots::bullet:
					Draw->DrawBitmap(bmpShot, D2D1::RectF(vEvilShots[i]->start.x, vEvilShots[i]->start.y,
						vEvilShots[i]->end.x, vEvilShots[i]->end.y));
					break;

				case shots::blast:
					Draw->DrawBitmap(bmpBigGun, D2D1::RectF(vEvilShots[i]->start.x, vEvilShots[i]->start.y,
						vEvilShots[i]->end.x, vEvilShots[i]->end.y));
					break;

				case shots::rocket:
					if (vEvilShots[i]->dir == dirs::up)
						Draw->DrawBitmap(bmpRocketU, D2D1::RectF(vEvilShots[i]->start.x, vEvilShots[i]->start.y,
							vEvilShots[i]->end.x, vEvilShots[i]->end.y));
					else 
						Draw->DrawBitmap(bmpRocketD, D2D1::RectF(vEvilShots[i]->start.x, vEvilShots[i]->start.y,
							vEvilShots[i]->end.x, vEvilShots[i]->end.y));
					break;
				}
			}
		}

		if (!vExplosions.empty())
		{
			for (int i = 0; i < vExplosions.size(); ++i)
			{
				vExplosions[i].frame_delay--;
				if (vExplosions[i].frame_delay <= 0)
				{
					vExplosions[i].frame_delay = 3;
					++vExplosions[i].frame;
					if (vExplosions[i].frame >= 23)
					{
						vExplosions.erase(vExplosions.begin() + i);
						break;
					}
				}
				Draw->DrawBitmap(bmpExplosion[vExplosions[i].frame], Resizer(bmpExplosion[vExplosions[i].frame],
					vExplosions[i].sx, vExplosions[i].sy));
			}
		}

		if (!vClouds.empty())
		{
			for (int i = 0; i < vClouds.size(); ++i)
			{
				if (vClouds[i]->end.x >= 0 && vClouds[i]->start.x <= scr_width &&
					vClouds[i]->end.y >= sky && vClouds[i]->start.y <= scr_height)
				{
					switch (vClouds[i]->type)
					{
					case clouds::cloud1:
						Draw->DrawBitmap(bmpCloud1, D2D1::RectF(vClouds[i]->start.x, vClouds[i]->start.y,
							vClouds[i]->end.x, vClouds[i]->end.y));
						break;

					case clouds::cloud2:
						Draw->DrawBitmap(bmpCloud2, D2D1::RectF(vClouds[i]->start.x, vClouds[i]->start.y,
							vClouds[i]->end.x, vClouds[i]->end.y));
						break;

					case clouds::cloud3:
						Draw->DrawBitmap(bmpCloud3, D2D1::RectF(vClouds[i]->start.x, vClouds[i]->start.y,
							vClouds[i]->end.x, vClouds[i]->end.y));
						break;

					case clouds::cloud4:
						Draw->DrawBitmap(bmpCloud4, D2D1::RectF(vClouds[i]->start.x, vClouds[i]->start.y,
							vClouds[i]->end.x, vClouds[i]->end.y));
						break;

					case clouds::cloud5:
						Draw->DrawBitmap(bmpCloud5, D2D1::RectF(vClouds[i]->start.x, vClouds[i]->start.y,
							vClouds[i]->end.x, vClouds[i]->end.y));
						break;
					}
				}
			}
		}

		if (!vPowerUps.empty())
		{
			for (int i = 0; i < vPowerUps.size(); ++i)
			{
				switch (vPowerUps[i]->type)
				{
				case powerups::big_gun:
					Draw->DrawBitmap(bmpBigGun, D2D1::RectF(vPowerUps[i]->start.x, vPowerUps[i]->start.y,
						vPowerUps[i]->end.x, vPowerUps[i]->end.y));
					break;

				case powerups::rocket:
					Draw->DrawBitmap(bmpRocket, D2D1::RectF(vPowerUps[i]->start.x, vPowerUps[i]->start.y,
						vPowerUps[i]->end.x, vPowerUps[i]->end.y));
					break;

				case powerups::repair:
					Draw->DrawBitmap(bmpRepair, D2D1::RectF(vPowerUps[i]->start.x, vPowerUps[i]->start.y,
						vPowerUps[i]->end.x, vPowerUps[i]->end.y));
					break;

				case powerups::shield:
					Draw->DrawBitmap(bmpShield, D2D1::RectF(vPowerUps[i]->start.x, vPowerUps[i]->start.y,
						vPowerUps[i]->end.x, vPowerUps[i]->end.y));
					break;
				}
			}
		}

		/////////////////////////////////////////////////////////////

		//STATUS TEXT **********************************************

		if (Hero)
		{
			wchar_t stat_txt[200]{ L"\0" };
			wchar_t add[5]{ L"\0" };
			int stat_size = 0;

			swprintf_s(stat_txt, L"остават: %.3f км.", distance / 10.0f);

			for (int i = 0; i < 200; ++i)
			{
				if (stat_txt[i] != '\0')++stat_size;
				else break;
			}

			if (statBrush && midText)
				Draw->DrawTextW(stat_txt, stat_size, midText, D2D1::RectF(scr_width - 350.0f, 60.0f, scr_width, scr_height), statBrush);

			stat_size = 0;

			wcscpy_s(stat_txt, current_player);

			for (int i = 0; i < 200; ++i)
			{
				if (stat_txt[i] != '\0')++stat_size;
				else break;
			}

			if (statBrush && midText)
				Draw->DrawTextW(stat_txt, stat_size, midText, D2D1::RectF(50.0f, 60.0f, scr_width, scr_height), statBrush);

			wcscpy_s(stat_txt, L"ниво: ");
			wsprintf(add, L"%d", level);
			wcscat_s(stat_txt, add);

			wcscat_s(stat_txt, L", резултат: ");
			wsprintf(add, L"%d", score);
			wcscat_s(stat_txt, add);

			wcscat_s(stat_txt, L", ракети: ");
			wsprintf(add, L"%d", Hero->rockets_available);
			wcscat_s(stat_txt, add);

			wcscat_s(stat_txt, L", броня: ");
			wsprintf(add, L"%d", Hero->armour);
			wcscat_s(stat_txt, add);

			wcscat_s(stat_txt, L", оръдие: ");
			if(Hero->big_gun_found)wcscat_s(stat_txt, L" да !");
			else wcscat_s(stat_txt, L" не !");

			stat_size = 0;
			for (int i = 0; i < 200; ++i)
			{
				if (stat_txt[i] != '\0')++stat_size;
				else break;
			}

			if (statBrush && midText)
				Draw->DrawTextW(stat_txt, stat_size, midText, D2D1::RectF(50.0f, ground, scr_width, scr_height), statBrush);
		}

		///////////////////////////////////////////////////////////////
		Draw->EndDraw();

		if (!Hero && vExplosions.empty())GameOver();
		else if (boss_active && !Boss && vExplosions.empty())LevelUp();
	}

	ClearResources();
	std::remove(tmp_file);
    return (int) bMsg.wParam;
}