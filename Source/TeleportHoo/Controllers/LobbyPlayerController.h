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

	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerInfo(int32 PlayerIndex, const FPlayerInfo& PlayerInfo);
	
	UFUNCTION()
	FString GetPlayerName();

	UFUNCTION()
	UTexture2D* GetPlayerAvatar();

	UFUNCTION(BlueprintCallable)
	ULobbyWidget* GetLobbyWidgetRef();

private:
	void UpdatePlayerInfoUI(int32 PlayerIndex, const FPlayerInfo& PlayerInfo);

	// VARIABLES
protected:
	UPROPERTY()
	class ULobbyWidget* LobbyWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	
private:

};