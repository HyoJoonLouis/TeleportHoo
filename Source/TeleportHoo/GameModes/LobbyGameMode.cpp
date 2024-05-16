// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
	: IsPlatformsSetUpDone(false)
{
}

void ALobbyGameMode::SetUpPlatforms()
{
	// TArray<AActor*> FoundPlatforms;
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALobbyPlayerPlatform::StaticClass(), FoundPlatforms);
	//
	// for (AActor* Actor : FoundPlatforms)
	// {
	// 	ALobbyPlayerPlatform* Platform = Cast<ALobbyPlayerPlatform>(Actor);
	// 	if (Platform)
	// 	{
	// 		// 플랫폼에 필요한 설정을 수행
	// 		// 예: 플레이어 할당, 리소스 로드, 초기화 등
	// 	}
	// }
	//
	// // 모든 플랫폼 설정이 완료되면 플래그를 true로 설정
	// IsPlatformsSetUpDone = true;
}
