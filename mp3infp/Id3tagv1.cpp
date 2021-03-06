// Id3tagv1.cpp: CId3tagv1 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalCommand.h"
#include "Id3tagv1.h"
#include <io.h>

static const unsigned char SCMPX_GENRE_NULL = 247;
static const unsigned char WINAMP_GENRE_NULL = 255;
static const LPCTSTR szId3gnr[256]={
	_T("Blues"),_T("Classic Rock"),_T("Country"),_T("Dance"),_T("Disco"),_T("Funk"),_T("Grunge"),
	_T("Hip-Hop"),_T("Jazz"),_T("Metal"),_T("New Age"),_T("Oldies"),_T("Other"),_T("Pop"),_T("R&B"),_T("Rap"),
	_T("Reggae"),_T("Rock"),_T("Techno"),_T("Industrial"),_T("Alternative"),_T("Ska"),_T("Death Metal"),
	_T("Pranks"),_T("Soundtrack"),_T("Euro-Techno"),_T("Ambient"),_T("Trip-Hop"),_T("Vocal"),
	_T("Jazz+Funk"),_T("Fusion"),_T("Trance"),_T("Classical"),_T("Instrumental"),_T("Acid"),_T("House"),
	_T("Game"),_T("Sound Clip"),_T("Gospel"),_T("Noise"),_T("Alt. Rock"),_T("Bass"),_T("Soul"),_T("Punk"),
	_T("Space"),_T("Meditative"),_T("Instrumental Pop"),_T("Instrumental Rock"),_T("Ethnic"),
	_T("Gothic"),_T("Darkwave"),_T("Techno-Industrial"),_T("Electronic"),_T("Pop-Folk"),
	_T("Eurodance"),_T("Dream"),_T("Southern Rock"),_T("Comedy"),_T("Cult"),_T("Gangsta Rap"),
	_T("Top 40"),_T("Christian Rap"),_T("Pop/Funk"),_T("Jungle"),_T("Native American"),_T("Cabaret"),
	_T("New Wave"),_T("Psychedelic"),_T("Rave"),_T("Showtunes"),_T("Trailer"),_T("Lo-Fi"),_T("Tribal"),
	_T("Acid Punk"),_T("Acid Jazz"),_T("Polka"),_T("Retro"),_T("Musical"),_T("Rock & Roll"),
	_T("Hard Rock"),_T("Folk"),_T("Folk/Rock"),_T("National Folk"),_T("Swing"),_T("Fast-Fusion"),
	_T("Bebob"),_T("Latin"),_T("Revival"),_T("Celtic"),_T("Bluegrass"),_T("Avantgarde"),_T("Gothic Rock"),
	_T("Progressive Rock"),_T("Psychedelic Rock"),_T("Symphonic Rock"),_T("Slow Rock"),
	_T("Big Band"),_T("Chorus"),_T("Easy Listening"),_T("Acoustic"),_T("Humour"),_T("Speech"),
	_T("Chanson"),_T("Opera"),_T("Chamber Music"),_T("Sonata"),_T("Symphony"),_T("Booty Bass"),
	_T("Primus"),_T("Porn Groove"),_T("Satire"),_T("Slow Jam"),_T("Club"),_T("Tango"),_T("Samba"),
	_T("Folklore"),_T("Ballad"),_T("Power Ballad"),_T("Rhythmic Soul"),_T("Freestyle"),_T("Duet"),
	_T("Punk Rock"),_T("Drum Solo"),_T("A Cappella"),_T("Euro-House"),_T("Dance Hall"),_T("Goa"),
	_T("Drum & Bass"),_T("Club-House"),_T("Hardcore"),_T("Terror"),_T("Indie"),_T("BritPop"),
	_T("Negerpunk"),_T("Polsk Punk"),_T("Beat"),_T("Christian Gangsta Rap"),_T("Heavy Metal"),
	_T("Black Metal"),_T("Crossover"),_T("Contemporary Christian"),_T("Christian Rock"),
	_T("Merengue"),_T("Salsa"),_T("Thrash Metal"),_T("Anime"),_T("JPop"),_T("Synthpop"),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),
	_T("Heavy Rock(J)"),_T("Doom Rock(J)"),_T("J-POP(J)"),_T("Seiyu(J)"),_T("Tecno Ambient(J)"),_T("Moemoe(J)"),_T("Tokusatsu(J)"),_T("Anime(J)")
	};

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CId3tagv1::CId3tagv1(BOOL bScmpxGenre)
{
	m_bScmpxGenre = bScmpxGenre;
	Release();
}

CId3tagv1::~CId3tagv1()
{

}

void CId3tagv1::Release()
{
	m_bEnable = FALSE;
	m_szTitle[0] = '\0';
	m_szArtist[0] = '\0';
	m_szAlbum[0] = '\0';
	m_szYear[0] = '\0';
	if(m_bScmpxGenre)
		m_cGenre = SCMPX_GENRE_NULL;
	else
		m_cGenre = WINAMP_GENRE_NULL;
	m_szComment[0] = '\0';
	m_cTrackNo = 0;
}

void CId3tagv1::SetString(LPSTR dest, LPCTSTR src, long len)
{
#ifdef UNICODE
	const char *buf = TstrToBytesAlloc(src, -1, NULL, BTC_CODE_ANSI);
#else
	const char *buf = src;
#endif
	m_bEnable = TRUE;
	long length = check2ByteLength(buf,len);
	strncpy(dest,buf,length);
	dest[length] = '\0';
#ifdef UNICODE
	free((void *)buf);
#endif
}

unsigned char CId3tagv1::GetGenreNum()
{
	if(!m_bScmpxGenre && (m_cGenre >= SCMPX_GENRE_NULL))
		return WINAMP_GENRE_NULL;
	return m_cGenre;
}

CString CId3tagv1::GetTrackNo()
{
	CString str;
	if(m_cTrackNo)
		str.Format(_T("%d"),m_cTrackNo);
	return str;
}

CString CId3tagv1::GenreNum2String(unsigned char cGenre,BOOL bScmpxGenre)
{
	if(!bScmpxGenre && (cGenre >= SCMPX_GENRE_NULL))
		return _T("");
	return szId3gnr[cGenre];
}

long CId3tagv1::GenreString2Num(LPCTSTR szGenre)
{
	long cGenre = -1;
	if(szGenre[0] == '\0')
	{
		if(m_bScmpxGenre)
			return SCMPX_GENRE_NULL;
		else
			return WINAMP_GENRE_NULL;
	}
	for(long i=0; i<256; i++)
	{
		if(lstrcmp(szId3gnr[i],szGenre) == 0)
		{
			cGenre = i;
			break;
		}
	}
	return cGenre;
}

void CId3tagv1::ReadTagString(LPSTR dest, LPCSTR src, long len)
{
	mbsncpy2((unsigned char *)dest,(const unsigned char *)src,len);
	dest[len] = '\0';
	for(int i=len-1; i>=0; i--)
	{
		if(dest[i] == ' ')
			dest[i] = '\0';
		else
			break;
	}
}

DWORD CId3tagv1::Load(LPCTSTR szFileName)
{
	DWORD	dwWin32errorCode = ERROR_SUCCESS;
	Release();
	FILE *fp;
	if((fp=_tfopen(szFileName,_T("rb"))) == NULL)
	{
		dwWin32errorCode = GetLastError();
		return dwWin32errorCode;
	}

	//ID3タグがあるはずの位置までSEEK
	if(fseek(fp,-128,SEEK_END) != 0)
	{
		dwWin32errorCode = GetLastError();
		fclose(fp);
		return dwWin32errorCode;
	}

	//ID3タグかどうかをチェック
	ID3_TAG tag;
	fread(&tag,1,sizeof(tag),fp);
	fclose(fp);
	if(!IsTagValid(&tag))
	{
		return -1;
	}
	
	m_bEnable = TRUE;
	//情報の採取
	ReadTagString(m_szTitle, tag.Title, 30);
	ReadTagString(m_szArtist, tag.Artist, 30);
	ReadTagString(m_szAlbum, tag.Album, 30);
	ReadTagString(m_szYear, tag.Year, 4);
	if((tag.Comment[28] == '\0') && tag.Track)
	{
		ReadTagString(m_szComment, tag.Comment, 28);
		m_cTrackNo = tag.Track;
	}
	else
	{
		ReadTagString(m_szComment, tag.Comment, 30);
		m_cTrackNo = 0;
	}
	m_cGenre = tag.Genre;
	return dwWin32errorCode;
}

#if 0
DWORD CId3tagv1::LoadMulti(LPCTSTR szFileName)
{
	DWORD dwError;
	CId3tagv1 tag;
	tag.SetScmpxGenre(m_bScmpxGenre);
	dwError = tag.Load(szFileName);
	if(dwError != ERROR_SUCCESS)
	{
		return dwError;
	}

	if(tag.IsEnable())
	{
		if(!this->GetTitle().Compare(tag.GetTitle()))
		{
			this->SetTitle(_T(""));
		}
		if(!this->GetArtist().Compare(tag.GetArtist()))
		{
			this->SetArtist(_T(""));
		}
		if(!this->GetAlbum().Compare(tag.GetAlbum()))
		{
			this->SetAlbum(_T(""));
		}
		if(!this->GetYear().Compare(tag.GetYear()))
		{
			this->SetYear(_T(""));
		}
		if(!(this->GetGenreNum() == tag.GetGenreNum()))
		{
			if(m_bScmpxGenre)
				this->SetGenre(SCMPX_GENRE_NULL);
			else
				this->SetGenre(WINAMP_GENRE_NULL);
		}
		if(!this->GetComment().Compare(tag.GetComment()))
		{
			this->SetComment(_T(""));
		}
		if(!this->GetTrackNo().Compare(tag.GetTrackNo()))
		{
			this->SetTrackNo(_T(""));
		}
	}
	else
	{
		this->SetTitle(_T(""));
		this->SetArtist(_T(""));
		this->SetAlbum(_T(""));
		this->SetYear(_T(""));
		if(m_bScmpxGenre)
			this->SetGenre(SCMPX_GENRE_NULL);
		else
			this->SetGenre(WINAMP_GENRE_NULL);
		this->SetComment(_T(""));
		this->SetTrackNo(_T(""));
	}

	return ERROR_SUCCESS;
}
#endif

DWORD CId3tagv1::Save(LPCTSTR szFileName)
{
	DWORD	dwWin32errorCode = ERROR_SUCCESS;
	FILE	*fp;
	ID3_TAG	tag;
	char	szTagTmp[4];

	//情報の保存
	memset(&tag,0x00,sizeof(tag));
	strncpy(tag.Title,m_szTitle,strlen(m_szTitle));
	strncpy(tag.Artist,m_szArtist,strlen(m_szArtist));
	strncpy(tag.Album,m_szAlbum,strlen(m_szAlbum));
	strncpy(tag.Year,m_szYear,strlen(m_szYear));
	strncpy(tag.Comment,m_szComment,strlen(m_szComment));
	if(m_cTrackNo)
	{
		tag.Comment[28] = '\0';
		tag.Track = m_cTrackNo;
	}
	tag.Genre = m_cGenre;

	if((fp = _tfopen(szFileName,_T("r+b"))) == NULL)
	{
		dwWin32errorCode = GetLastError();
		return dwWin32errorCode;
	}
	//ID3タグがあるはずの位置までSEEK
	if(fseek(fp,-128,SEEK_END) != 0)
	{
		dwWin32errorCode = GetLastError();
		fclose(fp);
		return dwWin32errorCode;
	}
	//ID3タグかどうかをチェック
	if(fread(szTagTmp,1,3,fp) < 3)
	{
//		dwWin32errorCode = GetLastError();//?
		fclose(fp);
		return -1;
	}
	if(!IsTagValid((ID3_TAG *)szTagTmp))
	{
		//ID3TAGが見つからない
		if(fseek(fp,0,SEEK_END))
		{
			dwWin32errorCode = GetLastError();
			fclose(fp);
			return dwWin32errorCode;
		}
		fwrite("TAG",1,3,fp);
	}
	if(fseek(fp,0,SEEK_CUR))
	{
		dwWin32errorCode = GetLastError();
		fclose(fp);
		return dwWin32errorCode;
	}
	if(fwrite(tag.Title,1,sizeof(tag)-3,fp) < sizeof(tag)-3)
	{
		dwWin32errorCode = GetLastError();
		fclose(fp);
		return dwWin32errorCode;
	}
	if(fclose(fp) == EOF)	//ライトプロテクトはここで検出
	{
		dwWin32errorCode = GetLastError();
		return dwWin32errorCode;
	}

	return dwWin32errorCode;
}

DWORD CId3tagv1::DelTag(LPCTSTR szFileName)
{
	DWORD	dwWin32errorCode = ERROR_SUCCESS;
	FILE	*fp;
	char	szTag[4];

	//開く
	if((fp=_tfopen(szFileName,_T("r+b"))) == NULL)
	{
		dwWin32errorCode = GetLastError();
		return dwWin32errorCode;
	}
	//ID3タグがあるはずの位置までSEEK
	if(fseek(fp,-128,SEEK_END) != 0)
	{
		dwWin32errorCode = GetLastError();
		fclose(fp);
		return dwWin32errorCode;
	}
	//ID3タグかどうかをチェック
	if(fread(szTag,1,3,fp) < 3)
	{
//		dwWin32errorCode = GetLastError();
		fclose(fp);
		return -1;
	}
	if(!IsTagValid((ID3_TAG *)szTag))
	{
		fclose(fp);
		//ID3タグが無いならそれもOK
		return -1;
	}
	if(fseek(fp,-3,SEEK_CUR))
	{
		dwWin32errorCode = GetLastError();
		fclose(fp);
		return dwWin32errorCode;
	}
	//切りつめ
	if(_chsize(fileno(fp),ftell(fp)) == -1)
	{
		dwWin32errorCode = GetLastError();
		fclose(fp);
		return dwWin32errorCode;
	}

	if(fclose(fp) == EOF)	//ライトプロテクトはここで検出
	{
		dwWin32errorCode = GetLastError();
		return dwWin32errorCode;
	}

	Release();

	return dwWin32errorCode;
}

void CId3tagv1::GetId3tag(ID3_TAG *tag,BOOL bScmpxGenre)
{
	memset(tag,0x00,sizeof(ID3_TAG));
	strncpy(tag->TagHeader,"TAG",3);
	if(bScmpxGenre)
		tag->Genre = SCMPX_GENRE_NULL;
	else
		tag->Genre = WINAMP_GENRE_NULL;
}

DWORD CId3tagv1::MakeTag(LPCTSTR szFileName)
{
	//ID3タグを作成
	Release();
	SetTitle(getFileName(szFileName));

	return Save(szFileName);
}

