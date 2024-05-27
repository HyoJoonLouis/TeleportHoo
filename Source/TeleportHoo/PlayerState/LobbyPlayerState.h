#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "../Structs/LobbyStructs.h"
#include "LobbyPlayerState.generated.h"

UCLASS()
class TELEPORTHOO_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// VARIABLES
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	FPlayerInfo PlayerInfo;

	// FUNCTION
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
