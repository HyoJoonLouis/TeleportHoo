#include "MainMenuWidget.h"
#include "TeleportHoo/Controllers/MainMenuPC.h"
#include "Components/Button.h"

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

	Cast<AMainMenuPC>(GetWorld()->GetFirstPlayerController())->HostSession();
}

 void UMainMenuWidget::JoinButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("JoinButtonClicked"));
	
	Cast<AMainMenuPC>(GetWorld()->GetFirstPlayerController())->JoinSession();
}