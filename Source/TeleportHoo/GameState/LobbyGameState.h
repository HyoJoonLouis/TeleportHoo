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
	
// VARIABLES
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TArray<FPlayerInfo> ConnectedPlayers;
};