#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TELEPORTHOO_API ICharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnTargeted(const AActor* CauseActor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUntargeted();
};
