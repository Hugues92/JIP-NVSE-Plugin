#pragma once

DEFINE_COMMAND_PLUGIN(RefToString, "None", 0, 1, kParams_JIP_OptionalForm);
DEFINE_COMMAND_PLUGIN(StringToRef, "None", 0, 2, kParams_JIP_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetMinOf, "None", 0, 5, kParams_JIP_TwoFloats_ThreeOptionalFloats);
DEFINE_COMMAND_PLUGIN(GetMaxOf, "None", 0, 5, kParams_JIP_TwoFloats_ThreeOptionalFloats);
DEFINE_COMMAND_PLUGIN(ReadArrayFromFile, "None", 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(WriteArrayToFile, "None", 0, 3, kParams_JIP_OneString_TwoInts);
DEFINE_COMMAND_PLUGIN(ReadStringFromFile, "None", 0, 3, kParams_JIP_OneString_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(WriteStringToFile, "None", 0, 23, kParams_JIP_OneString_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetLoadOrderChanged, "None", 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ValidateModIndex, "None", 0, 1, kParams_OneInt);

std::unordered_map<UInt32, const char*> g_refStrings;

const char *RefToString(TESForm *form)
{
	if (!form) return "";
	UInt32 id = form->refID;
	auto findID = g_refStrings.find(id);
	if (findID != g_refStrings.end()) return findID->second;
	char *refStr = new char[267];
	if (id < 0xFF000000) strcpy_s(refStr, 260, DataHandler::Get()->GetNthModName(id >> 24));
	else refStr[0] = 0;
	strcat_s(refStr, 260, ":");
	char idStr[7];
	_itoa_s(id & 0xFFFFFF, idStr, 16);
	strcat_s(refStr, 261, idStr);
	g_refStrings[id] = refStr;
	return refStr;
}

UInt32 StringToRef(char *refStr, bool dcm = false)
{
	UInt16 pos = 0;
	UInt32 id = 0, modIdx = 0;

	while ((pos < 267) && refStr[pos])
	{
		if (refStr[pos] == ':')
		{
			id = strtoul(refStr + pos + 1, NULL, dcm ? 10 : 16);
			if (pos)
			{
				refStr[pos] = 0;
				modIdx = DataHandler::Get()->GetModIndex(refStr);
				if (modIdx < 0xFF) return (modIdx << 24) + id;
			}
			else return 0xFF000000 + id;
		}
		pos++;
	}
	if (pos == 8)
	{
		char *end = NULL;
		id = strtoul(refStr, &end, 16);
		if (end && !*end && g_serialization->ResolveRefID(id, &id)) return id;
	}
	return 0;
}

bool Cmd_RefToString_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;

	if (thisObj) form = thisObj;
	else if (ExtractArgs(EXTRACT_ARGS, &form) && form) form = form->TryGetREFRParent();

	g_string->Assign(PASS_COMMAND_ARGS, RefToString(form));

	return true;
}

bool Cmd_StringToRef_Execute(COMMAND_ARGS)
{
	UInt32 *refResult = (UInt32*)result, dcm = 0;
	*refResult = 0;
	char refStr[267] = {0};

	if (ExtractArgs(EXTRACT_ARGS, &refStr, &dcm)) *refResult = StringToRef(refStr, dcm != 0);

	return true;
}

bool Cmd_GetMinOf_Execute(COMMAND_ARGS)
{
	float val1, val2, val3 = NULL, val4 = NULL, val5 = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &val1, &val2, &val3, &val4, &val5))
	{
		*result = (val1 < val2) ? val1 : val2;
		if (val3 != NULL)
		{
			if (val3 < *result) *result = val3;
			if (val4 != NULL)
			{
				if (val4 < *result) *result = val4;
				if ((val5 != NULL) && (val5 < *result)) *result = val5;
			}
		}
	}
	return true;
}

bool Cmd_GetMaxOf_Execute(COMMAND_ARGS)
{
	float val1, val2, val3 = NULL, val4 = NULL, val5 = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &val1, &val2, &val3, &val4, &val5))
	{
		*result = (val1 > val2) ? val1 : val2;
		if (val3 != NULL)
		{
			if (val3 > *result) *result = val3;
			if (val4 != NULL)
			{
				if (val4 > *result) *result = val4;
				if ((val5 != NULL) && (val5 > *result)) *result = val5;
			}
		}
	}
	return true;
}

bool GetNextData(std::string &line, std::string &buffer)
{
	buffer.clear();
	if (line.empty()) return false;
	UInt16 idx = 0;
	
	do
	{
		if (line[idx] == '\t') break;
		buffer += line[idx];
	}
	while (++idx < line.size());

	line.erase(0, line.find_first_not_of('\t', idx + 1));
	return !buffer.empty();
}

NVSEArrayVarInterface::Element *CreateForType(std::string &dataStr)
{
	if (dataStr.empty())
		return new NVSEArrayVarInterface::Element();

	if (dataStr[0] == '$')
		return new NVSEArrayVarInterface::Element(dataStr.data() + 1);

	if (dataStr[0] == '@')
		return new NVSEArrayVarInterface::Element(LookupFormByID(StringToRef((char*)dataStr.data() + 1)));

	return new NVSEArrayVarInterface::Element(atof(dataStr.data()));
}

bool Cmd_ReadArrayFromFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	char filePath[512] = {0};

	NVSEArrayVarInterface::Array *mainArray = g_array->CreateArray(NULL, 0, scriptObj);
	if (ExtractArgs(EXTRACT_ARGS, &filePath))
	{
		std::ifstream sourceFile(g_gamePath + filePath);
		if (sourceFile.good())
		{
			std::string line, buffer;
			while (getline(sourceFile, line))
			{
				if (GetNextData(line, buffer)) break;
			}

			if (!buffer.empty())
			{
				UInt16 numColumns = 0, idx;
				std::vector<std::string> firstLine;
				do
				{
					firstLine.push_back(buffer);
					numColumns++;
				}
				while (GetNextData(line, buffer));

				NVSEArrayVarInterface::Array *column;
				NVSEArrayVarInterface::Element *elem;
				std::vector<NVSEArrayVarInterface::Array*> columnList;
				bool createArrays = false;

				while (getline(sourceFile, line))
				{
					if (line.empty()) continue;
					if (!createArrays)
					{
						createArrays = true;
						for (idx = 0; idx < numColumns; idx++)
						{
							column = g_array->CreateArray(NULL, 0, scriptObj);
							columnList.push_back(column);
							elem = new NVSEArrayVarInterface::Element(column);
							g_array->AppendElement(mainArray, *elem);
							elem = CreateForType(firstLine[idx]);
							g_array->AppendElement(column, *elem);
						}
					}
					for (idx = 0; idx < numColumns; idx++)
					{
						GetNextData(line, buffer);
						elem = CreateForType(buffer);
						g_array->AppendElement(columnList[idx], *elem);
					}
				}
				if (!createArrays)
				{
					for (idx = 0; idx < numColumns; idx++)
					{
						elem = CreateForType(firstLine[idx]);
						g_array->AppendElement(mainArray, *elem);
					}
				}
			}
		}
	}
	g_array->AssignCommandResult(mainArray, result);
	return true;
}

void ElementToFile(NVSEArrayVarInterface::Element *elem, std::ofstream &outputFile)
{
	UInt8 elemType = elem->GetType();
	if (elemType == 1)
	{
		char buffer[20] = {0};
		sprintf_s(buffer, "%.6f", elem->Number());
		outputFile << buffer;
	}
	else if (elemType == 2) outputFile << '@' << RefToString(elem->Form());
	else if (elemType == 3) outputFile << '$' << elem->String();
	else outputFile << '0';
}

struct ColumnElements
{
	SInt32							size;
	NVSEArrayVarInterface::Element	*elements;

	ColumnElements(SInt32 pSize, NVSEArrayVarInterface::Element *pElements) : size(pSize), elements(pElements) {}
};

bool Cmd_WriteArrayToFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	char filePath[512] = { 0 };
	UInt32 apnd = 0, arrID = 0;

	if (ExtractArgs(EXTRACT_ARGS, &filePath, &apnd, &arrID) && arrID)
	{
		NVSEArrayVarInterface::Array *mainArray = g_array->LookupArrayByID(arrID), *column;
		if (mainArray)
		{
			SInt32 numColumns = 0, numLines = 1, cnt = 0, idx;
			NVSEArrayVarInterface::Element *topLine = GetArrayData(mainArray, numColumns), *columnData;
			if (numColumns > 0)
			{
				std::vector<ColumnElements> columnBuffer;

				for (idx = 0; idx < numColumns; idx++)
				{
					if (column = topLine[idx].Array())
					{
						columnData = GetArrayData(column, cnt);
						columnBuffer.push_back(ColumnElements(cnt, columnData));
						if (numLines < cnt) numLines = cnt;
					}
					else columnBuffer.push_back(ColumnElements(1, &topLine[idx]));
				}

				std::string path = g_gamePath + filePath;
				bool exists = FileExists(path);
				if (!exists) IFileStream::MakeAllDirs(path.data());

				std::ofstream outputFile;
				if (apnd) outputFile.open(path, std::ofstream::ate | std::ofstream::app);
				else outputFile.open(path, std::ofstream::ate | std::ofstream::trunc);

				if (outputFile.good())
				{
					if (exists && apnd) outputFile << '\n';

					for (idx = 0; idx < numLines; idx++)
					{
						for (cnt = 0; cnt < numColumns; cnt++)
						{
							if (columnBuffer[cnt].size > idx) ElementToFile(&columnBuffer[cnt].elements[idx], outputFile);
							else outputFile << '0';
							if (cnt < (numColumns - 1)) outputFile << '\t';
						}
						if (idx < (numLines - 1)) outputFile << '\n';
					}
					*result = 1;
				}

				for (idx = 0; idx < numColumns; idx++)
				{
					delete (topLine + idx);
					for (cnt = 0; cnt < columnBuffer[idx].size; cnt++) delete (columnBuffer[idx].elements + cnt);
				}
			}
		}
	}
	return true;
}

bool Cmd_ReadStringFromFile_Execute(COMMAND_ARGS)
{
	char filePath[512] = {0};
	UInt32 startAt = 1, lineCount = 0;
	
	if (ExtractArgs(EXTRACT_ARGS, &filePath, &startAt, &lineCount))
	{
		std::ifstream sourceFile(g_gamePath + filePath);
		if (sourceFile.good())
		{
			std::string line, buffer;
			UInt32 lineNum = 0, counter = 0;
			if (startAt < 1) startAt = 1;

			while (getline(sourceFile, line))
			{
				lineNum++;
				if (lineNum >= startAt)
				{
					buffer += line + '\n';
					if (lineCount)
					{
						counter++;
						if (counter == lineCount) break;
					}
				}
			}

			if (!buffer.empty())
			{
				buffer.back() = 0;
				g_string->Assign(PASS_COMMAND_ARGS, buffer.data());
				return true;
			}
		}
	}
	g_string->Assign(PASS_COMMAND_ARGS, "");
	return true;
}

bool Cmd_WriteStringToFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	char filePath[512] = {0};
	UInt32 apnd = 0;
	char outString[kMaxMessageLength] = {0};

	if (ExtractFormatStringArgs(2, outString, paramInfo, scriptData, opcodeOffsetPtr, scriptObj, eventList, kCommandInfo_WriteStringToFile.numParams, &filePath, &apnd))
	{
		std::string path = g_gamePath + filePath;
		bool exists = FileExists(path);
		if (!exists) IFileStream::MakeAllDirs(path.data());

		std::ofstream outputFile;
		if (apnd) outputFile.open(path, std::ofstream::ate | std::ofstream::app);
		else outputFile.open(path, std::ofstream::ate | std::ofstream::trunc);

		if (outputFile.good())
		{
			if (exists && apnd) outputFile << '\n';
			outputFile << outString;
			*result = 1;
		}
	}
	return true;
}

bool Cmd_GetLoadOrderChanged_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 modIdx = 0, resolved = 0;
	for (UInt8 idx = 1; idx < 255; idx++)
	{
		modIdx = idx << 24;
		if (!g_serialization->ResolveRefID(modIdx, &resolved) || (modIdx != resolved))
		{
			*result = 1;
			return true;
		}
	}
	return true;
}

bool Cmd_ValidateModIndex_Execute(COMMAND_ARGS)
{
	*result = -1;
	UInt32 modIdx = 0;
	ExtractArgs(EXTRACT_ARGS, &modIdx);
	if ((modIdx == 0) || (modIdx == 255)) *result = modIdx;
	else if ((modIdx > 0) && (modIdx < 255) && g_serialization->ResolveRefID(modIdx << 24, &modIdx)) *result = modIdx >> 24;
	return true;
}
