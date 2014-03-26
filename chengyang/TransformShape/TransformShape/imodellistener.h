#pragma once
class IModelListener
{
public:
	IModelListener(void);
	virtual ~IModelListener(void);

	virtual void notify() = 0;
};

