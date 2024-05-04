#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPC.generated.h"

UCLASS()
class TELEPORTHOO_API AMainMenuPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainMenuPC();
	virtual void BeginPlay() override;

protected:	
	// VARIABLES
	UPROPERTY()
	class UUserWidget* MainMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class TSubclassOf<UUserWidget> MainMenuWidgetClass;
};