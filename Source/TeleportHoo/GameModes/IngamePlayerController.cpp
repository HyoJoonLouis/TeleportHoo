#include "IngamePlayerController.h"
#include "../UI/IngameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"


void AIngamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		HUD = Cast<UIngameHUD>(CreateWidget(this, HUDClass));
		HUD->AddToViewport();
	}
}

void AIngamePlayerController::Server_SendChat_Implementation(const FText& TextToSend)
{
	TArray<TObjectPtr<APlayerState>> PlayerStates = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;
	for (const auto& PlayerStateForChat : PlayerStates)
	{
		Cast<AIngamePlayerController>(PlayerStateForChat->GetPlayerController())->Client_SendChat(FText::FromString(PlayerStateForChat->GetName()), TextToSend);
	}
}

void AIngamePlayerController::Client_SendChat_Implementation(const FText& Name, const FText& TextToSend)
{
	if (IsValid(GetIngameHUD()))
	{
		GetIngameHUD()->SendChat(Name, TextToSend);
	}
}
