#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TeleportHoo/Structs/LobbyStructs.h"
#include "LobbyPlayerController.generated.h"

class ULobbyWidget;

UCLASS()
class TELEPORTHOO_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController();

	// FUNCTION
public:
	virtual  void BeginPlay() override;
	virtual  void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Client
	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerInfo(int32 PlayerIndex, const FPlayerInfo& PlayerInfo);
	UFUNCTION(Client, Reliable)
	void Client_SetStartButtonEnabled(bool bEnabled);
	UFUNCTION(Client, Reliable)
	void Client_SetStartButtonVisibility(bool bIsVisible);
	
	// Server
	UFUNCTION(Server, Reliable)
	void Server_ToggleReady(bool bIsReady);

	UFUNCTION()
	FString GetPlayerName();
	UFUNCTION()
	UTexture2D* GetPlayerAvatar();
	UFUNCTION(BlueprintCallable)
	ULobbyWidget* GetLobbyWidgetRef();

	UFUNCTION(BlueprintCallable)
	void ToggleReady();

private:
	void InitializeLobbyWidget();
	void UpdatePlayerInfoUI(int32 PlayerIndex, const FPlayerInfo& PlayerInfo);

	// VARIABLES
protected:
	UPROPERTY()
	class ULobbyWidget* LobbyWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> LobbyWidgetClass;
	
private:

};