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

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual bool ReadyToStartMatch_Implementation() override;
	virtual bool ReadyToEndMatch_Implementation() override;

	virtual void HandleMatchHasEnded() override;

	UFUNCTION()
	void RoundStart();

	UFUNCTION()
	void RoundEnd();

	UFUNCTION()
	void OnGameTimeFinished();

	UFUNCTION()
	void OnPlayerDiedDelegate(class AIngamePlayerController* DeadCharacter);
protected:
	UPROPERTY(VisibleAnywhere)
	TArray<class AIngamePlayerController*> ConnectedPlayers;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseCharacter> SpawnActor;

	UPROPERTY()
	bool isMatchEnd;
	UPROPERTY(EditAnywhere)
	uint8 MaxPlayer;

	FTimerHandle RoundStartTimerHandle;
	FTimerHandle RoundEndTimerHandle;
};
