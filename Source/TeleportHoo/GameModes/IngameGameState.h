#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "IngameGameState.generated.h"

UCLASS()
class TELEPORTHOO_API AIngameGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AIngameGameState();
	UFUNCTION(BlueprintCallable)
	void AddTeamScore(ETeam WinTeam);
	

	//Getters
	FORCEINLINE int GetTeamScore(ETeam Team) { return TeamScore[Team]; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game | Rule")
	TMap<ETeam, int> TeamScore;
};
