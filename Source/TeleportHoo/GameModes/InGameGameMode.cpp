// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGameMode.h"

AInGameGameMode::AInGameGameMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("AInGameGameMode Constructor"));

	MaxPlayer = 2;
}

void AInGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//UE_LOG(LogTemp, Warning, TEXT("=========================="));
	//UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	//UE_LOG(LogTemp, Warning, TEXT("현재 접속한 인원 : %d"), GetNumPlayers());
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetMatchState().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("=========================="));
	//
	//if (MaxPlayer <= GetNumPlayers())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("게임 시작"));
	//	//bDelayedStart = false;
	//	HandleMatchHasStarted();
	//}
}

void AInGameGameMode::RestartPlayer(AController* NewPlayer)
{
	//UE_LOG(LogTemp, Warning, TEXT("RestartPlayer"));
	Super::RestartPlayer(NewPlayer);
}

void AInGameGameMode::HandleMatchHasStarted()
{
	//UE_LOG(LogTemp, Warning, TEXT("HandleMatchHasStarted - %s"), *GetMatchState().ToString());
	Super::HandleMatchHasStarted();
}