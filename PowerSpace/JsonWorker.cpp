#include <stdafx.h>
#include "JsonWorker.h"
#include <sstream>

JSON &CJsonWorker::AddElementToDescription( JSON& description, const JSON& element, int depth, std::wstring sep )
{
	std::wstringstream sstream( element );
	std::wstring row;
	while( std::getline( sstream, row ) ) {
		description += std::wstring( depth, '\t' ) + row + L"\n";
	}
	description.pop_back();
	description += sep;
	return description;
}

JSON& CJsonWorker::AddRowToDescription( JSON& description, const JSON& row, int depth )
{
	return description += std::wstring( depth, '\t' ) + row;
}

int CJsonWorker::OpenTag( JSON& description, const std::wstring& tag, int depth )
{
	description += L"\n" + std::wstring( depth++, L'\t' ) + tag + L"\n";
	return depth;
}

int CJsonWorker::CloseTag( JSON& description, const std::wstring& tag, int depth )
{
	description += L"\n" + std::wstring( --depth, L'\t' ) + tag;
	return depth;
}