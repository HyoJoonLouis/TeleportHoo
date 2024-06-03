#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "../GameModes/IngamePlayerController.h"

void UHealthBarWidget::IsLocal(bool isLocal)
{
	if (isLocal)
		HealthBar->SetWidgetStyle(RedBar);
	else
		HealthBar->SetWidgetStyle(GreenBar);
}

void UHealthBarWidget::SetHealth(float Value)
{
	HealthBar->SetPercent(Value);
}

void UHealthBarWidget::SetMomentum(float Value)
{
	MomentumBar->SetPercent(Value);
}
