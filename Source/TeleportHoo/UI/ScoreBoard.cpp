// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreBoard.h"
#include "Components/TextBlock.h"
#include "../GameModes/IngameGameState.h"
#include "../Structs/CharacterStructs.h"

void UScoreBoard::NativeConstruct()
{
	UpdateScore();
	//GetWorld()->GetGameState<AIngameGameState>()->GetTeamScore(ETeam::RED);
}

void UScoreBoard::UpdateScore()
{
	RedScore->SetText(FText::FromString(FString::FromInt(GetWorld()->GetGameState<AIngameGameState>()->GetTeamScore(ETeam::RED))));
	BlueScore->SetText(FText::FromString(FString::FromInt(GetWorld()->GetGameState<AIngameGameState>()->GetTeamScore(ETeam::BLUE))));
}
