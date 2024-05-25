#include "MainMenuPC.h"
#include "Blueprint/UserWidget.h"

AMainMenuPC::AMainMenuPC()
{
	UE_LOG(LogTemp, Warning, TEXT("AMainMenuPC Constructor"));
}

void AMainMenuPC::BeginPlay()
{
	Super::BeginPlay();

	// this->bShowMouseCursor = true;
	// MainMenuWidget = CreateWidget(this, MainMenuWidgetClass);
	// UE_LOG(LogTemp, Warning, TEXT("CreateWidget : MainMenuWidget"));
	// if (IsValid(MainMenuWidget))
	// {
	// 	MainMenuWidget->AddToViewport();
	// }
}