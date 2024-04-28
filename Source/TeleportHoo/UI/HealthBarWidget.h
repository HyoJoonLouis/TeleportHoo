#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

UCLASS()
class TELEPORTHOO_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

public:
	void SetHealth(float Value);
};
