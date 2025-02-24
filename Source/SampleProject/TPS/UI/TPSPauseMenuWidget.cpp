#include "UI/TPSPauseMenuWidget.h"

#include "CommonUIExtensions.h"
#include "TPSCommonButtonBase.h"
#include "TPSHelper.h"
#include "TPSSystemManager.h"
#include "Character/TPSPlayerController.h"
#include "Components/Button.h"

void UTPSPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (APlayerController* PC = TPSHelper::GetPlayerController(GetWorld()))
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);	
	}
}

void UTPSPauseMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (APlayerController* PC = TPSHelper::GetPlayerController(GetWorld()))
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}

bool UTPSPauseMenuWidget::Initialize()
{
	bool bResult = Super::Initialize();

	SetGamePause(true);
	
	if (OptionButton != nullptr)
	{
		OptionButton->SetButtonText(FText::FromString(TEXT("Option")));
		OptionButton->OnClicked().AddUObject(this, &UTPSPauseMenuWidget::OnClickedOption);
	}

	if (QuitButton != nullptr)
	{
		QuitButton->SetButtonText(FText::FromString(TEXT("Quit")));
		QuitButton->OnClicked().AddUObject(this, &UTPSPauseMenuWidget::OnClickedQuit);
	}

	if (ContinueButton != nullptr)
	{
		ContinueButton->SetButtonText(FText::FromString(TEXT("Continue")));
		ContinueButton->OnClicked().AddUObject(this, &UTPSPauseMenuWidget::OnClickedContinue);
	}
	
	return bResult;
}

void UTPSPauseMenuWidget::BeginDestroy()
{
	Super::BeginDestroy();
}

void UTPSPauseMenuWidget::OnClickedOption()
{
	UE_LOG(LogTemp, Log, TEXT("OnClickedOption"));
}

void UTPSPauseMenuWidget::OnClickedQuit()
{
	UE_LOG(LogTemp, Log, TEXT("OnClickedQuit"));
	UTPSSystemManager::Get()->QuitGame();
}

void UTPSPauseMenuWidget::OnClickedContinue()
{
	UE_LOG(LogTemp, Log, TEXT("OnClickedContinue"));

	UCommonUIExtensions::PopContentFromLayer(this);
	SetGamePause(false);
}

void UTPSPauseMenuWidget::SetGamePause(const bool bPauseGame)
{
	APlayerController* PC = TPSHelper::GetPlayerController(GetWorld());

	if (ATPSPlayerController* TPSPC = Cast<ATPSPlayerController>(PC))
	{
		TPSPC->SetGamePause(bPauseGame);
	}
}
