#pragma once

#include "CoreMinimal.h"
#include "BlueprintCopilotSettings.generated.h"

UCLASS(config = Game, defaultconfig)
class BLUEPRINTCOPILOT_API UBlueprintCopilotSettings : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FString APIKey;
};
