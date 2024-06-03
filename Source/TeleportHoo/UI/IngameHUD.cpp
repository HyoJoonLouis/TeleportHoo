#include "IngameHUD.h"
#include "ChatBox.h"
#include "Components/Image.h"
#include "CinemaWidget.h"
#include "ScoreBoard.h"
#include "RoundResultWidget.h"

void UIngameHUD::NativeConstruct()
{
	Super::NativeConstruct();
	Blood->SetVisibility(ESlateVisibility::Collapsed);
}

void UIngameHUD::SendChat(const FText& Name, const FText& TextToSend)
{
	WBP_ChatBox->AddChat(Name, TextToSend);
}

void UIngameHUD::PlayCinema(bool bStart)
{
	WBP_Cinema->PlayCinemaAnimation(bStart);
}

void UIngameHUD::UpdateScore()
{
	WBP_ScoreBoard->UpdateScore();
}

void UIngameHUD::ShowResult(int Round, bool isWin)
{
	WBP_RoundResult->SetVisibility(ESlateVisibility::Visible);
	WBP_RoundResult->ShowResult(Round, isWin);
	PlayAnimationForward(RoundEndAnimation, 1.0f, true);
}

void UIngameHUD::OnHitEffect()
{
	PlayAnimationForward(BloodAnimation);
}

void UIngameHUD::FadeInOut(bool FadeOut)
{
	FadeOut ? PlayAnimationForward(FadeOutAnimation) : PlayAnimationReverse(FadeOutAnimation);
}
