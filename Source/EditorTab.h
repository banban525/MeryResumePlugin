#pragma once


#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include <shlwapi.h>

#include "rapidjson\document.h"
#include "rapidjson\prettywriter.h"
#include "rapidjson\stringbuffer.h"

typedef rapidjson::GenericDocument< rapidjson::UTF16<> > Document_UTF16;
typedef rapidjson::GenericValue< rapidjson::UTF16<> > Value_UTF16;
typedef rapidjson::GenericStringStream< rapidjson::UTF16<> > StringStream_UTF16;
typedef rapidjson::GenericStringBuffer< rapidjson::UTF16<> > StringBuffer_UTF16;

class EditorTab
{
	const std::wstring filePath_;
	const std::wstring id_;
public:
	EditorTab(const std::wstring& filePath, const std::wstring& id)
		:filePath_(filePath),
		id_(id)
	{
	}
	const std::wstring& GetFilePath() const
	{
		return filePath_;
	}
	const std::wstring& GetId() const
	{
		return id_;
	}
	std::wstring LoadContent(const std::wstring& workDir) const
	{
		wchar_t dest[1024];
		::PathCombine(dest, workDir.c_str(), id_.c_str());

		std::wifstream ifs(dest);
		if (ifs.fail())
		{
			std::cerr << "Ž¸”s" << std::endl;
			return L"";
		}

		std::istreambuf_iterator<wchar_t> it(ifs);
		std::istreambuf_iterator<wchar_t> last;
		std::wstring str(it, last);

		return str;
	}

};

class EditorStatus
{
	std::vector<EditorTab> list;
public:
	EditorStatus() {}
	void AddTab(const EditorTab& tab)
	{
		list.push_back(tab);
	}

	const std::vector<EditorTab>& Tabs()
	{
		return list;
	}

	static EditorStatus DeserializeFromJson(const std::wstring& json)
	{
		EditorStatus result;

		Document_UTF16 doc;
		doc.Parse(json.c_str());

		auto& documents = doc[L"documents"];
		for (unsigned int i = 0; i < documents.Size(); i++)
		{
			auto& val = documents[i];
			auto filePath = val[L"FilePath"].GetString();
			auto id = val[L"Id"].GetString();

			result.AddTab(EditorTab(std::wstring(filePath), std::wstring(id)));
		}
		return result;
	}
	static std::wstring SerializeToJson(const EditorStatus& status)
	{
		Document_UTF16 doc(rapidjson::kObjectType);
		Value_UTF16 value(rapidjson::kArrayType);

		for (auto it = status.list.begin(); it != status.list.end(); it++)
		{
			auto filePath = it->GetFilePath();
			auto id = it->GetId();

			Value_UTF16 val;
			val.SetObject();
			val.AddMember(L"Id", Value_UTF16(id.c_str(), doc.GetAllocator()), doc.GetAllocator());
			val.AddMember(L"FilePath", Value_UTF16(filePath.c_str(), doc.GetAllocator()), doc.GetAllocator());

			value.PushBack(val, doc.GetAllocator());
		}

		doc.AddMember(L"documents", value, doc.GetAllocator());

		StringBuffer_UTF16 s;
		rapidjson::PrettyWriter<StringBuffer_UTF16, rapidjson::UTF16<>, rapidjson::UTF16<> > writer(s);

		doc.Accept(writer);

		const wchar_t* wstr = s.GetString();
		return wstr;
	}
};