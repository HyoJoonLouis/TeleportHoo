#include "DirectionWidget.h"
#include "../Structs/CharacterStructs.h"
#include "Components/Image.h"


void UDirectionWidget::ChangeDirection(EDamageDirection NewDirection, ECharacterStates CharacterState)
{
	if (NewDirection == EDamageDirection::LEFT)
	{
		LeftImage->SetBrushFromMaterial(DirectionWidgetImages[CharacterState].Image[EDamageDirection::LEFT]);
		if (CharacterState == ECharacterStates::PARRIABLE || CharacterState == ECharacterStates::SKILL)
			PlayAnimationForward(LeftScaleUpAnimation);
		else
			LeftImage->SetRenderScale(FVector2D(1.0f, 1.0f));
		RightImage->SetBrushFromMaterial(DirectionWidgetImages[ECharacterStates::STUN].Image[EDamageDirection::RIGHT]);
		//LeftImage->SetBrushFromTexture(DirectionWidgetImages[CharacterState].Image[EDamageDirection::LEFT], true);
		//RightImage->SetBrushFromTexture(DirectionWidgetImages[ECharacterStates::STUN].Image[EDamageDirection::RIGHT], true);
	}
	else if (NewDirection == EDamageDirection::RIGHT)
	{
		LeftImage->SetBrushFromMaterial(DirectionWidgetImages[ECharacterStates::STUN].Image[EDamageDirection::LEFT]);
		RightImage->SetBrushFromMaterial(DirectionWidgetImages[CharacterState].Image[EDamageDirection::RIGHT]);
		if (CharacterState == ECharacterStates::PARRIABLE || CharacterState == ECharacterStates::SKILL)
			PlayAnimationForward(RightScaleUpAnimation);
		else
			RightImage->SetRenderScale(FVector2D(1.0f, 1.0f));
		//LeftImage->SetBrushFromTexture(DirectionWidgetImages[ECharacterStates::STUN].Image[EDamageDirection::LEFT], true);
		//RightImage->SetBrushFromTexture(DirectionWidgetImages[CharacterState].Image[EDamageDirection::RIGHT], true);
	}
}
