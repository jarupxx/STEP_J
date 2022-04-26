// STEP_wma.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "STEP_wma.h"
#include "STEPlugin.h"

#include "FileWMA.h"
#include "DlgSetup.h"

#include "..\SuperTagEditor\INI\ini.h"
//�ݒ�̓ǂݏ���
//WritePrivateProfileString �̓t�@�C�������݂��Ȃ��ꍇ��A
//���t�@�C���� ANSI ���� ANSI �ŕ��������������
//�g���h���̂� STEP �{�̂� INI �ǂݏ����N���X���g����
//UTF8/UTF16/ANSI �Ή�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	����!
//
//		���� DLL �� MFC DLL �ɑ΂��ē��I�Ƀ����N�����ꍇ�A
//		MFC ���ŌĂяo����邱�� DLL ����G�N�X�|�[�g���ꂽ
//		�ǂ̊֐����֐��̍ŏ��ɒǉ������ AFX_MANAGE_STATE 
//		�}�N�����܂�ł��Ȃ���΂Ȃ�܂���B
//
//		��:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �ʏ�֐��̖{�̂͂��̈ʒu�ɂ���܂�
//		}
//
//		���̃}�N�����e�֐��Ɋ܂܂�Ă��邱�ƁAMFC ����
//		�ǂ̌Ăяo�����D�悷�邱�Ƃ͔��ɏd�v�ł��B
//		����͊֐����̍ŏ��̃X�e�[�g�����g�łȂ���΂�
//		��Ȃ����Ƃ��Ӗ����܂��A�R���X�g���N�^�� MFC 
//		DLL ���ւ̌Ăяo�����s���\��������̂ŁA�I�u
//		�W�F�N�g�ϐ��̐錾�����O�łȂ���΂Ȃ�܂���B
//
//		�ڍׂɂ��Ă� MFC �e�N�j�J�� �m�[�g 33 �����
//		58 ���Q�Ƃ��Ă��������B
//

/////////////////////////////////////////////////////////////////////////////
// CSTEP_wmaApp

BEGIN_MESSAGE_MAP(CSTEP_wmaApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_wmaApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_wmaApp �̍\�z

CSTEP_wmaApp::CSTEP_wmaApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTEP_wmaApp �I�u�W�F�N�g

CSTEP_wmaApp theApp;

UINT nPluginID;
UINT nFileTypeWMA;
UINT nFileTypeWMV;
UINT nFileTypeASF;

CString strINI;
bool bOptGenreListSelect;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return _T("Version 1.01 Copyright (C) 2003-2006 haseta\r\n")
           _T("Version 1.03M Copyright (C) 2008-2010 Mimura\r\n")
           _T("Version 1.05 Copyright (C) 2016 Kobarin\r\n")
           _T("WMA/WMV/ASF�`�����T�|�[�g���Ă��܂�");
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INI�t�@�C���̓ǂݍ���
	strINI = szPluginFolder;
	strINI += _T("STEP_wma.ini");
    CIniFile iniFile(strINI);
	bOptGenreListSelect = iniFile.ReadInt(_T("WMA"), _T("GenreListSelect"), 0) != 0;

	HBITMAP hWMABitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_WMA));
	HBITMAP hWMVBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_WMA));
	HBITMAP hASFBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_WMA));
	nFileTypeWMA = STEPRegisterExt(nPluginID, _T("wma"), hWMABitmap);
	nFileTypeWMV = STEPRegisterExt(nPluginID, _T("wmv"), hWMVBitmap);
	nFileTypeASF = STEPRegisterExt(nPluginID, _T("asf"), hASFBitmap);
	DeleteObject(hWMABitmap);
	DeleteObject(hWMVBitmap);
	DeleteObject(hASFBitmap);

	return true;
}

STEP_API void WINAPI STEPFinalize() {
	Finalize();
}

STEP_API UINT WINAPI STEPGetAPIVersion(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return STEP_API_VERSION;
}

STEP_API LPCTSTR WINAPI STEPGetPluginName(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return _T("STEP_wma");
}

STEP_API bool WINAPI STEPSupportSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}

STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}

STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!isEditSIF)	return _NULL;
	switch (nColumn) {
	case COLUMN_TRACK_NAME:
	case COLUMN_ARTIST_NAME:
	case COLUMN_ALBUM_NAME:
	case COLUMN_TRACK_NUMBER:
	case COLUMN_DISC_NUMBER:
	case COLUMN_YEAR:
	case COLUMN_URL:
	case COLUMN_COPYRIGHT:
	case COLUMN_OTHER:
	case COLUMN_ORIG_ARTIST: /* 2005.08.19 add */
	case COLUMN_COMPOSER: /* 2005.08.19 add */
	case COLUMN_ENGINEER: /* 2005.09.07 add */
	case COLUMN_ALBM_ARTIST: /* Mimura add */
	case COLUMN_WRITER: /* Mimura add */
    case COLUMN_SOFTWARE:
		return _EDIT;
	case COLUMN_COMMENT:
		return _MEDIT;
	case COLUMN_GENRE:
		if (bOptGenreListSelect) {
			return _CBOX;
		} else {
			return _EDIT;
		}
	}
	return _NULL;
}

STEP_API UINT WINAPI STEPGetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
	case COLUMN_TRACK_NAME:		return 1024;
	case COLUMN_ARTIST_NAME:	return 1024;
	case COLUMN_ALBUM_NAME:		return 1024;
	case COLUMN_YEAR:			return 1024;
	case COLUMN_GENRE:			return 1024;
	case COLUMN_COMMENT:		return 1024;
	case COLUMN_URL:			return 1024;
	case COLUMN_COPYRIGHT:		return 1024;
	case COLUMN_OTHER:			return 1024;
	case COLUMN_ORIG_ARTIST:	return 1024; /* 2005.08.19 add */
	case COLUMN_COMPOSER:		return 1024; /* 2005.08.19 add */
	case COLUMN_ENGINEER:		return 1024; /* 2005.09.07 add */
	case COLUMN_ALBM_ARTIST:	return 1024; /* Mimura add */
	case COLUMN_WRITER:			return 1024; /* Mimura add */
    case COLUMN_SOFTWARE:		return 1024; 
	case COLUMN_TRACK_NUMBER:	return 32;
	case COLUMN_DISC_NUMBER:	return 32;
	}
	return 0;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (_tcsicmp(szExt, _T("wma")) == 0 || _tcsicmp(szExt, _T("wmv")) == 0 || _tcsicmp(szExt, _T("asf")) == 0) {
		if (LoadFileWMA(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(_T("%s �̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, _T("WMA�t�@�C���̓ǂݍ��ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			if (_tcsicmp(szExt, _T("wma")) == 0) {
				SetFormat(pFileMP3, nFileTypeWMA);
				SetFileTypeName(pFileMP3, _T("WMA"));
				return STEP_SUCCESS;
			}
			if (_tcsicmp(szExt, _T("wmv")) == 0) {
				SetFormat(pFileMP3, nFileTypeWMV);
				SetFileTypeName(pFileMP3, _T("WMV"));
				return STEP_SUCCESS;
			}
			if (_tcsicmp(szExt, _T("asf")) == 0) {
				SetFormat(pFileMP3, nFileTypeASF);
				SetFileTypeName(pFileMP3, _T("ASF"));
				return STEP_SUCCESS;
			}
		}
	}
	return STEP_UNKNOWN_FORMAT;
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UINT nFormat = GetFormat(pFileMP3);

	if (nFormat == nFileTypeWMA || nFormat == nFileTypeWMV || nFormat == nFileTypeASF) {
		if (WriteFileWMA(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(_T("%s �̏������݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, _T("WMA�t�@�C���̏������ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		}
		return STEP_SUCCESS;
	}
	return STEP_UNKNOWN_FORMAT;
}

STEP_API void WINAPI STEPShowOptionDialog(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDlgSetup dlg1;
	CPropertySheet page;
	dlg1.m_bGenreListSelect = bOptGenreListSelect;
	page.AddPage(&dlg1);
	page.SetTitle(CString(STEPGetPluginName()) + _T(" �I�v�V�����ݒ�"));
	if (page.DoModal() == IDOK) {
		bOptGenreListSelect = dlg1.m_bGenreListSelect ? true : false;
        CIniFile iniFile(strINI);
		iniFile.WriteInt(_T("WMA"), _T("GenreListSelect"), bOptGenreListSelect);
        iniFile.Flush();//�ۑ����s
	}
}



/*
STEP_API LPCTSTR WINAPI STEPGetToolTipText(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return NULL;
}
*/

/*
STEP_API LPCTSTR WINAPI STEPGetStatusMessage(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return NULL;
}
*/

/*
STEP_API bool WINAPI STEPOnUpdateCommand(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return false;
}
*/

/*
STEP_API bool WINAPI STEPOnCommand(UINT nID, HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return false;
}
*/

/*
STEP_API void WINAPI STEPOnLoadMenu(HMENU hMenu, UINT nType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}
*/


STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT nFormatType, COLUMNTYPE nColumn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
	case COLUMN_URL:			// URL
		return _T("URL(�֘A)");
	case COLUMN_OTHER:			// ���̑�
		return _T("URL(Album)");
		break;
	case COLUMN_ENGINEER:		// �G���W�j�A(�����) /* 2005.09.07 add */
		return _T("���[�x��");
		break;
	}
	return NULL;
}

STEP_API bool WINAPI STEPHasSpecificColumnName(UINT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}
