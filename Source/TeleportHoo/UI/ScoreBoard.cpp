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
	AIngameGameState* GameState = GetWorld()->GetGameState<AIngameGameState>();
	Round->SetText(FText::FromString(FString::Printf(TEXT("ROUND %i"), GameState->GetRound())));
	Timer->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), (180 - GameState->GetGameTime()) / 60, (180 - GameState->GetGameTime()) % 60)));
	RedScore->SetText(FText::FromString(FString::FromInt(GameState->GetTeamScore(ETeam::RED))));
	BlueScore->SetText(FText::FromString(FString::FromInt(GameState->GetTeamScore(ETeam::BLUE))));
}
