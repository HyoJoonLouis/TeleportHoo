#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "TeleportHoo/GameModes/HooGameInstance.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(LogTemp, Warning, TEXT("UMainMenuWidget Constructor"));
	HostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostButtonClicked);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinButtonClicked);
}

void UMainMenuWidget::HostButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("HostButtonClicked"));
	Cast<UHooGameInstance>(GetWorld()->GetGameInstance())->CreateServer();
}

 void UMainMenuWidget::JoinButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("JoinButtonClicked"));
	Cast<UHooGameInstance>(GetWorld()->GetGameInstance())->JoinServer();
}