#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DirectionWidget.generated.h"

USTRUCT(Blueprintable)
struct FDirectionWidgetImages
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EDamageDirection, class UTexture2D*> Image;
};

UCLASS()
class TELEPORTHOO_API UDirectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Image")
	TMap<ECharacterStates, FDirectionWidgetImages> DirectionWidgetImages;

	UPROPERTY(meta = (BindWidget))
	class UImage* LeftImage;
	UPROPERTY(meta = (BindWidget))
	class UImage* RightImage;

public:
	UFUNCTION()
	void ChangeDirection(EDamageDirection NewDirection, ECharacterStates CharacterState);
};
