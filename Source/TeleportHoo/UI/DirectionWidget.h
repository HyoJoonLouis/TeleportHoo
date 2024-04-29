#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DirectionWidget.generated.h"

UCLASS()
class TELEPORTHOO_API UDirectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UImage* LeftImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* RightImage;

public:
	UFUNCTION()
	void ChangeDirection(EDamageDirection NewDirection);
};
