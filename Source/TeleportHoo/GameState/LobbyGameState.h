#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "../Structs/LobbyStructs.h"
#include "LobbyGameState.generated.h"

UCLASS()
class TELEPORTHOO_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()

public:
	ALobbyGameState();

// FUNCTION
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	bool AreAllPlayersReady() const;
	
// VARIABLES
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TArray<FPlayerInfo> ConnectedPlayers;
};