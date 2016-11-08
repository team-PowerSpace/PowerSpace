#pragma once
#include "JsonObject.h"

class CJsonWorker
{
public:

	static JSON& AddRowToDescription(
		JSON& description,
		const JSON &row,
		int depth
		);

	static JSON& AddElementToDescription(
		JSON& description,
		const JSON &element,
		int depth,
		std::wstring sep = L""
		);

	static int OpenTag(
		JSON& description,
		const std::wstring& tag,
		int depth
		);

	static int CloseTag(
		JSON& description,
		const std::wstring& tag,
		int depth
		);

};