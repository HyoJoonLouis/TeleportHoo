#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class TELEPORTHOO_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMainMenuGameMode();

protected:
	virtual void BeginPlay() override;
};