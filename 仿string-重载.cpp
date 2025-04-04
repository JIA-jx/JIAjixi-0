#include <iostream>
#include <bitset>
#include <iomanip>
#include <cctype>
#include <conio.h>
#include <cmath>
#include <array>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>

#define ll long long
#define db double
#define usn unsigned

const int N = 100000 + 10;
const db eps = 1e-6;

class Role
{
public:
	int hp;
	int mp;
};

class hstring
{
	usn short usmlen; //�ַ����ڴ泤
	usn short uslen;  //�ַ�����
	char*     cstr;   //�ַ�������
	usn short GetLength(const char* str) const;
	void CopyStrs(char* dest, const char* source);
public:
	inline static char nochar = -1;
	char* c_str() { return cstr; }
	hstring();
	hstring(const char* str);//����c�ַ�������hstring
	hstring(const hstring& str);//����hstring����hstring
	hstring& operator=(const hstring& str);
	hstring& operator+(const hstring& str);
	hstring& operator+(const hstring&& str);
	hstring& operator<<(const hstring& str);
	~hstring();
	hstring(char clen);
	char& operator[](const usn short index) const;
	const char& operator()();

	//��ȡ�ַ���
	hstring operator()(const usn short start, const usn length) const;
	
	hstring& operator+(int val);
	explicit operator int();//const
	operator bool();
	bool operator==(std::nullptr_t nptr);
};

hstring::operator bool()
{
	return cstr != nullptr;
}

bool hstring::operator==(std::nullptr_t nptr)
{
	return cstr == nptr;
}

std::ostream& operator<<(std::ostream& _cout, hstring& _str)
{
	_cout << _str.c_str();
	return _cout;
}

std::istream& operator>>(std::istream& _cin, hstring& _str)
{
	char _sRead[0x1FF]{};
	_cin >> _sRead;
	_str = _sRead;

	return _cin;
}

hstring& hstring::operator+(int val)
{
	char	str[0x20]{};//����һ���ڴ�
	char	len{ 0x1F };//����
	bool	bzs = val >= 0;//�Ƿ�����
	val = val * (bzs * 2 - 1);//ȡ����ֵ
	do str[--len] = val % 10 + 48; while (val /= 10);
	str[len = len - 1 * (1 - bzs)] = '-' * (bzs + 1) * (1 - bzs) + str[len] * bzs;
	
	usn short slen = uslen + 0x20 - len - 1;

	if (slen > usmlen)
	{
		char* lstr = cstr;
		cstr = new char[slen] {};//���·����ڴ�
		usmlen = slen;//�����ڴ泤��
		memcpy(cstr, lstr, uslen);
		delete[] cstr;//ɾ���ڴ�
	}

	memcpy(cstr + uslen - 1, str + len, 0x20 - len);
	uslen = slen;//�ַ�����������

	return *this;
}
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	



}
//////////////////////////////////////////////////////////////////////////////
usn short hstring::GetLength(const char* str) const
{
	usn short len{ 0 };
	for (; str[len++];);
	return len;
}

void hstring::CopyStrs(char* dest, const char* source)
{
	usn short len = GetLength(source);
	if (len > usmlen)
	{
		//�ڴ�й¶����
		delete[] cstr;	
		cstr = new char[len];//���·����ڴ�
		usmlen = len;//�����ڴ泤��
	}
	memcpy(cstr, source, len);
	uslen = len;//�ַ�����������
}

hstring::hstring()
{
	usmlen = 0x32;
	uslen = 0;
	cstr = new char[usmlen];
}

hstring::hstring(const char* str) :hstring()
{
	CopyStrs(cstr, str);
}

hstring::hstring(const hstring& str) :hstring()
{
	CopyStrs(cstr, str.cstr);
}

hstring& hstring::operator=(const hstring& str)
{
	CopyStrs(cstr, str.cstr);
	return *this;
}

hstring& hstring::operator+(const hstring& str)
{
	return *this << str;
}

hstring& hstring::operator+(const hstring&& str)
{
	return *this << str;
}

hstring& hstring::operator<<(const hstring& str)
{
	usn short slen{ GetLength(str.cstr) };
	slen = uslen + slen - 1;
	if (slen > usmlen)
	{
		char* lstr = cstr;
		cstr = new char[slen];//���·����ڴ�
		usmlen = slen;//�����ڴ泤��
		memcpy(cstr, lstr, uslen);
		delete[] cstr;//ɾ���ڴ�
	}

	memcpy(cstr + uslen - 1, str.cstr, slen - uslen + 1);
	uslen = slen;//�ַ�����������
	
	return *this;
}

hstring::~hstring()
{
	if (cstr != nullptr) delete[] cstr;
}

char& hstring::operator[](const usn short index) const
{
	if (index > uslen) return cstr[index];
}

const char& hstring::operator()()
{
	return cstr[0];
}

hstring::hstring(char clen)
{
	usmlen = clen;
	uslen = 0;
	cstr = 0;
	if (clen > 0)
	{
		usmlen = clen;
		uslen = 1;
		cstr = new char[usmlen] {};
	}
}

hstring hstring::operator()(const usn short start, const usn length) const
{
	if (start > uslen)
	{
		return hstring("");
	}

	usn slen = start + length > uslen - 1 ? uslen - start - 1 : length;

	char* newstr = new char[slen + 1];
	memcpy(newstr, cstr, slen + 1);
	newstr[slen] = 0;

	return hstring(newstr);
}

hstring::operator int()
{
	int val{};
	int i = cstr[0] == '-';//���
	while (cstr[i] >= '0' && cstr[i] <= '9')
	{
		val = val * 10 + cstr[i++] - 48;
	}
	val = val * ((cstr[0] != '-') * 2 - 1);

	return val;
}
