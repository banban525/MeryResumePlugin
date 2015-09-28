// RapidJsonTrial.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include<vector>
#include<string>
#include "rapidjson\document.h"
#include "rapidjson\prettywriter.h"
#include "rapidjson\stringbuffer.h"

typedef rapidjson::GenericDocument< rapidjson::UTF16<> > Document_UTF16;
typedef rapidjson::GenericValue< rapidjson::UTF16<> > Value_UTF16;
typedef rapidjson::GenericStringStream< rapidjson::UTF16<> > StringStream_UTF16;
typedef rapidjson::GenericStringBuffer< rapidjson::UTF16<> > StringBuffer_UTF16;

class EditorTab
{
	std::wstring filePath_;
	std::wstring contentFileName_;
public:
	EditorTab(const std::wstring& filePath, const std::wstring& contentFileName)
		:filePath_(filePath),
		contentFileName_(contentFileName)
	{
	}
	std::wstring GetFilePath() const
	{
		return filePath_;
	}
	std::wstring GetContentFileName() const
	{
		return contentFileName_;
	}
};

class EditorStatus
{
	std::vector<EditorTab> list;
public:
	EditorStatus(){}
	void AddTab(const EditorTab& tab)
	{
		list.push_back(tab);
	}
	static std::wstring SerializeToJson(const EditorStatus& status)
	{
		const wchar_t* json = LR"({"name":"value"})";
		Document_UTF16 doc;
		Value_UTF16 value;
		value.SetArray();

		doc.AddMember(L"documents", value, doc.GetAllocator());
		

		StringBuffer_UTF16 s;
		rapidjson::PrettyWriter<StringBuffer_UTF16, rapidjson::UTF16<>> writer(s);

		doc.Accept(writer);

		const wchar_t* wstr = s.GetString();
		return wstr;
	}
};

int main()
{
	
    return 0;
}

