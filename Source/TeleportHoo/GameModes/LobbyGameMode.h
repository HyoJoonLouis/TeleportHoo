#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class TELEPORTHOO_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	//FUNCTION
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	UFUNCTION(NetMulticast, Reliable)
	void OnPlayerInfoUpdated();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void CheckAllPlayersReady();
};