#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::SetHealth(float Value)
{
	HealthBar->SetPercent(Value);
}

void UHealthBarWidget::SetMomentum(float Value)
{
	MomentumBar->SetPercent(Value);
}
