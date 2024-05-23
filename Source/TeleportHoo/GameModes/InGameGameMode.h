// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "InGameGameMode.generated.h"

UCLASS()
class TELEPORTHOO_API AInGameGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AInGameGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	UFUNCTION()
	void OnPlayerDiedDelegate(class AIngamePlayerController* DeadCharacter);


protected:
	UPROPERTY(VisibleAnywhere)
	TArray<class AIngamePlayerController*> ConnectedPlayers;
	UPROPERTY()
	TArray<class AActor*> PlayerStarts;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseCharacter> SpawnActor;
	UPROPERTY(EditAnywhere)
	uint8 MaxPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game | Rule")
	int32 MaxRound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game | Rule")
	int32 CurrentRound;
};
