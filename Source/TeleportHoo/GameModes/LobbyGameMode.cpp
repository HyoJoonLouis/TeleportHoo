// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TeleportHoo/GameState/LobbyGameState.h"

ALobbyGameMode::ALobbyGameMode()
{
}

void ALobbyGameMode::StartPlay()
{
	Super::StartPlay();
	// 게임 시작 시 초기화 작업
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	// 게임 시작 시 첫 번째로 호출되는 초기화 작업
}

void ALobbyGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	// 게임 시작 처리
}

void ALobbyGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
	// 게임 대기 상태 처리
}

bool ALobbyGameMode::IsMatchInProgress() const
{
	return Super::IsMatchInProgress();
	// 게임 진행 중인지 확인
}

void ALobbyGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	// 게임 종료 처리
}

void ALobbyGameMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();
	// 맵 떠날 때 처리
}

void ALobbyGameMode::HandleMatchAborted()
{
	Super::HandleMatchAborted();
	// 게임 중단 처리
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	// 플레이어 로그인 처리

	FString PlayerName = "PlayerDefaultName";
	UTexture2D* AvatarImage = nullptr;

	ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();
	if(LobbyGameState)
	{
		FPlayerLobbyInfo PlayerLobbyInfo;
		PlayerLobbyInfo.PlayerName = PlayerName;
		PlayerLobbyInfo.AvatarImage = AvatarImage;
		PlayerLobbyInfo.bIsReady = false;

		LobbyGameState->AddPlayerInfo(PlayerLobbyInfo);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	// 플레이어 로그아웃 처리
}