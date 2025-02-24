#include "UI/TPSCommonButtonBase.h"

#include "CommonTextBlock.h"

UTPSCommonButtonBase::UTPSCommonButtonBase()
{
}

void UTPSCommonButtonBase::SetButtonText(const FText& NewText)
{
	ButtonTextBlock->SetText(NewText);
}
