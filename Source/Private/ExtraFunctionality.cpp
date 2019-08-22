#include "ExtraFunctionality.h"

DEFINE_LOG_CATEGORY(ExtraFunctionality);

#define LOCTEXT_NAMESPACE "ExtraFunctionality"

class FExtraFunctionalityModule : public IExtraFunctionalityModule
{
public:
	virtual void StartupModule() override
	{
		UE_LOG(ExtraFunctionality, Display, TEXT("Extra Functionality Module Started"));
	}

	virtual void ShutdownModule() override
	{
		UE_LOG(ExtraFunctionality, Display, TEXT("Extra Functionality Module Shutdown"));
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}

};

IMPLEMENT_MODULE(FExtraFunctionalityModule, ExtraFunctionality)
