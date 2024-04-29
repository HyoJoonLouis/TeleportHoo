#include "DirectionWidget.h"
#include "../Structs/CharacterStructs.h"
#include "Components/Image.h"


void UDirectionWidget::ChangeDirection(EDamageDirection NewDirection)
{
	if (NewDirection == EDamageDirection::LEFT)
	{
		LeftImage->SetColorAndOpacity(FLinearColor::Red);
		RightImage->SetColorAndOpacity(FLinearColor::Black);
	}
	else if (NewDirection == EDamageDirection::RIGHT)
	{
		LeftImage->SetColorAndOpacity(FLinearColor::Black);
		RightImage->SetColorAndOpacity(FLinearColor::Red);
	}
}
