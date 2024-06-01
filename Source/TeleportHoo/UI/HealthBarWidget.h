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

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MomentumBar;

	UPROPERTY(EditAnywhere)
	FProgressBarStyle RedBar;
	
	UPROPERTY(EditAnywhere)
	FProgressBarStyle GreenBar;


public:
	void IsLocal(bool isLocal);
	void SetHealth(float Value);
	void SetMomentum(float Value);
};
