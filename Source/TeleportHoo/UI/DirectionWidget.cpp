#include "DirectionWidget.h"
#include "../Structs/CharacterStructs.h"
#include "Components/Image.h"


void UDirectionWidget::ChangeDirection(EDamageDirection NewDirection, ECharacterStates CharacterState)
{
	if (NewDirection == EDamageDirection::LEFT)
	{
		LeftImage->SetBrushFromTexture(DirectionWidgetImages[CharacterState].Image[EDamageDirection::LEFT], true);
		RightImage->SetBrushFromTexture(DirectionWidgetImages[ECharacterStates::STUN].Image[EDamageDirection::RIGHT], true);
	}
	else if (NewDirection == EDamageDirection::RIGHT)
	{
		LeftImage->SetBrushFromTexture(DirectionWidgetImages[ECharacterStates::STUN].Image[EDamageDirection::LEFT], true);
		RightImage->SetBrushFromTexture(DirectionWidgetImages[CharacterState].Image[EDamageDirection::RIGHT], true);
	}
}
