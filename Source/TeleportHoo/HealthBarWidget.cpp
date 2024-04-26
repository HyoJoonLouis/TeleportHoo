#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::SetHealth(float Value)
{
	HealthBar->SetPercent(Value);
}
