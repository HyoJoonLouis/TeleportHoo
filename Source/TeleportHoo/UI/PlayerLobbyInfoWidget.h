#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structs/LobbyStructs.h"
#include "PlayerLobbyInfoWidget.generated.h"

UCLASS()
class TELEPORTHOO_API UPlayerLobbyInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdatePlayerInfo(const FPlayerInfo& PlayerInfo);

	// VARIABLES
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UImage* AvatarImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ReadyStatus;
};