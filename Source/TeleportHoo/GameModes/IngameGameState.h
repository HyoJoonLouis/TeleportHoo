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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintCallable)
	void AddTeamScore(ETeam WinTeam);
	
	UFUNCTION()
	void OnRep_ScoreChanged();

	// Setters
	FORCEINLINE void SetTeamScore(ETeam Team, int Value) { Team == ETeam::RED ? RedTeamScore = Value : BlueTeamScore = Value; }
	// Getters
	FORCEINLINE int GetTeamScore(ETeam Team) { return  Team == ETeam::RED? RedTeamScore : BlueTeamScore; }

protected:
	UPROPERTY(ReplicatedUsing = OnRep_ScoreChanged, VisibleAnywhere, BlueprintReadWrite, Category = "Game | Score")
	int RedTeamScore;
	UPROPERTY(ReplicatedUsing = OnRep_ScoreChanged, VisibleAnywhere, BlueprintReadWrite, Category = "Game | Score")
	int BlueTeamScore;	
};
