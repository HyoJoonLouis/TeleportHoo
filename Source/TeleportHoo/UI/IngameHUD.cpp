#include "IngameHUD.h"
#include "ChatBox.h"
#include "Components/Image.h"
#include "CinemaWidget.h"

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

void UIngameHUD::OnHitEffect()
{
	PlayAnimationForward(BloodAnimation);
}
