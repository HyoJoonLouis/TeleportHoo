// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"


AMainMenuGameMode::AMainMenuGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("AMainMenuGameMode Constructor"));
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AMainMenuGameMode BeginPlay"));
}