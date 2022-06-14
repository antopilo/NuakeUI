#include "DataModelOperations.h"
#include <iostream>

namespace NuakeUI
{
	DataModelOperation::DataModelOperation(const std::string& name, OperationType type, ComparaisonType compType)
	{
		Name = name;
		Type = type;
		CompType = compType;
	}

	std::shared_ptr<DataModelOperation> DataModelOperation::New(const std::string& name, OperationType type, ComparaisonType compType)
	{
		return std::make_shared<DataModelOperation>(name, type, compType);
	}

	bool DataModelOperation::Compare(std::shared_ptr<DataModel> object)
	{
		if (!object->HasData(Name))
		{
			std::cout << "DataModel has no data named " << Name << std::endl;
			return false;
		}
		
		DataBindType dataType = object->GetDataType(Name);
		switch (dataType)
		{
			case DataBindType::Bool:
				return object->GetDatab(Name);
			case DataBindType::Int:
			{
				int data = object->GetDatai(Name);
				switch (CompType)
				{
				case ComparaisonType::Equal:
					return data == RightI;
				case ComparaisonType::NotEqual:
					return data != RightI;
				case ComparaisonType::Greater:
					return data > RightI;
				case ComparaisonType::GreaterOrEqual:
					return data >= RightI;
				case ComparaisonType::Less:
					return data < RightI;
				case ComparaisonType::LessOrEqual:
					return data <= RightI;
				}
			}
			break;
		}
		
		return false;
	}
}