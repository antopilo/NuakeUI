#pragma once

#include "DataBindObject.h"

#include <string>

namespace NuakeUI
{
	enum class OperationType
	{
		If
	};

	enum class ComparaisonType
	{
		Equal,
		NotEqual,
		Greater,
		GreaterOrEqual,
		Less,
		LessOrEqual
	};

	class DataModelOperation
	{
	public:
		std::string Name;
		OperationType Type;
		ComparaisonType CompType;
		
		static std::shared_ptr<DataModelOperation> New(const std::string& name, OperationType type, ComparaisonType compType);
		
		int RightI;
		bool RightB;
		std::string RightS;
		float RightF;
		
		DataModelOperation(const std::string& name, OperationType type, ComparaisonType compType);
		~DataModelOperation() = default;

		bool Compare(std::shared_ptr<DataModel> object);
	};
}