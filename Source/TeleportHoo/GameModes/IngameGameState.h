#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TeleportHoo/Structs/CharacterStructs.h"
#include "IngameGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameTimeFinished);

UCLASS()
class TELEPORTHOO_API AIngameGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AIngameGameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	void StartGameTimer();
	UFUNCTION(BlueprintCallable)
	void ResetGameTimer();
	UFUNCTION(BlueprintCallable)
	void StopGameTimer();
	UFUNCTION(BlueprintCallable)
	void AddRound();
	UFUNCTION(BlueprintCallable)
	void AddTeamScore(ETeam WinTeam);
	
	UFUNCTION()
	void OnRep_ScoreChanged();

	FOnGameTimeFinished OnGameTimeFinished;

	// Setters
	FORCEINLINE void SetTeamScore(ETeam Team, int Value) { Team == ETeam::RED ? RedTeamScore = Value : BlueTeamScore = Value; }
	// Getters
	FORCEINLINE int GetRound() { return CurrentRound; }
	FORCEINLINE int GetGameTime() { return SecondsAfterGameStart; }
	FORCEINLINE int GetTeamScore(ETeam Team) { return  Team == ETeam::RED? RedTeamScore : BlueTeamScore; }

protected:
	UPROPERTY(ReplicatedUsing = OnRep_ScoreChanged, VisibleAnywhere, BlueprintReadWrite)
	int CurrentRound;
	UPROPERTY(ReplicatedUsing = OnRep_ScoreChanged, VIsibleAnywhere, BlueprintReadWrite)
	int SecondsAfterGameStart;
	FTimerHandle GameTimerHandle;
	UPROPERTY(ReplicatedUsing = OnRep_ScoreChanged, VisibleAnywhere, BlueprintReadWrite, Category = "Game | Score")
	int RedTeamScore;
	UPROPERTY(ReplicatedUsing = OnRep_ScoreChanged, VisibleAnywhere, BlueprintReadWrite, Category = "Game | Score")
	int BlueTeamScore;	
};