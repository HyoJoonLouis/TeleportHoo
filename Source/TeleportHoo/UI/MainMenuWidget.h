#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class TELEPORTHOO_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
};