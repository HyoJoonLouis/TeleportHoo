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
	UPROPERTY(ReplicatedUsing=OnRep_PlayerInfo, VisibleAnywhere, BlueprintReadOnly)
	FPlayerInfo PlayerInfo;

	// FUNCTION
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_PlayerInfo();
};
