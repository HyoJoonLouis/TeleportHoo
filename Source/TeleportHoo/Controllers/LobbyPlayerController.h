#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TeleportHoo/Structs/LobbyStructs.h"
#include "LobbyPlayerController.generated.h"

UCLASS()
class TELEPORTHOO_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController();

	// FUNCTION
public:
	
	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerInfo(int32 PlayerIndex, const FPlayerInfo& PlayerInfo);
	
	UFUNCTION()
	FString GetPlayerName();

	UFUNCTION()
	UTexture2D* GetPlayerAvatar();

private:
	void UpdatePlayerInfoUI(int32 PlayerIndex, const FPlayerInfo& PlayerInfo);

	// VARIABLES
protected:
	// 플레이어 정보를 표시할 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	UPROPERTY()
	class ULobbyWidget* LobbyWidget;
	
private:

};