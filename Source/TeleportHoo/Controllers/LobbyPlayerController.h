// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"

#include "LobbyPlayerController.generated.h"

UCLASS()
class TELEPORTHOO_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class TSubclassOf<UUserWidget> MainMenuWidgetClass;

public:
	// FUNCTION
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "PlayerInfo")
	void ServerSetPlayerInfo(const FString& PlayerName, UTexture2D* AvatarImage);

private:
	// VARIABLES
	
	
	




	
public:
};