// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"
#include "Kismet/GameplayStatics.h"

void ATestGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Platform, OutActors);

	for (auto& const OutActor : OutActors)
	{

	}
}
