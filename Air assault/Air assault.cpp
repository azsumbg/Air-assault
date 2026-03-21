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
D2D1_RECT_F b2Rect{ scr_width / 3.0f + 50.0f, 5.0f, scr_width * 1.5f - 50.0f, sky - 5.0f };
D2D1_RECT_F b3Rect{ scr_width * 1.5f + 50.0f, 5.0f, scr_width - 50.0f, sky - 5.0f };

D2D1_RECT_F b1TxtRect{ 80.0f, 10.0f, scr_width / 3.0f - 50.0f, sky - 5.0f };
D2D1_RECT_F b2TxtRect{ scr_width / 3.0f + 80.0f, 10.0f, scr_width * 1.5f - 50.0f, sky - 5.0f };
D2D1_RECT_F b3TxtRect{ scr_width * 1.5f + 70.0f, 10.0f, scr_width - 50.0f, sky - 5.0f };

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
int mins = 0;
int secs = 0;

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

dll::HERO* Hero{ nullptr };

std::vector<dll::GROUND*>vFields;
std::vector<dll::GROUND*>vTiles;

std::vector<dll::CLOUDS*>vClouds;

dirs assets_move_dir{ dirs::stop };

bool need_field_up = false;
bool need_field_down = false;





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
	mins = 0;
	secs = 300;

	level_skipped = false;
	assets_move_dir = dirs::stop;

	need_field_up = false;
	need_field_down = false;


	if (!vFields.empty())for (int i = 0; i < vFields.size(); ++i)if (!FreeMem(&vFields[i]))LogErr(L"Error releasing vFields !");
	vFields.clear();

	if (!vTiles.empty())for (int i = 0; i < vTiles.size(); ++i)if (!FreeMem(&vTiles[i]))LogErr(L"Error releasing vTiles !");
	vTiles.clear();

	if (!vClouds.empty())for (int i = 0; i < vClouds.size(); ++i)if (!FreeMem(&vClouds[i]))LogErr(L"Error releasing vClouds !");
	vClouds.clear();

	if (Hero)Hero->Release();
	Hero = dll::HERO::create(scr_width / 2.0f - 50.0f, ground - 100.0f);

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

	return (INT_PTR)(FALSE);
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
	switch (ReceivedMsg)
	{
	case WM_CREATE:
		if (bIns)
		{
			SetTimer(hwnd, bTimer, 1000, NULL);

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
		if (pause)break;
		--secs;
		mins = secs / 60;
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





	default: return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);
	}

	return (LRESULT)(FALSE);
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












	ClearResources();
	std::remove(tmp_file);
    return (int) bMsg.wParam;
}