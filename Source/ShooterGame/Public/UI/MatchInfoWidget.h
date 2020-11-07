// Drice

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchInfoWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UMatchInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:    
    virtual void NativeConstruct() override;
    
    /** notify name changed to update name display**/
    UFUNCTION(BlueprintImplementableEvent, Category = "MatchInfo")
    void OnNameChange();
    
    /** notify score changed to update score display**/
    UFUNCTION(BlueprintImplementableEvent, Category = "MatchInfo")
    void OnScoreChange();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "MatchInfo")
    void OnTimerChange(const FString& NewTime);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "MatchInfo")
    void OnRecieveServerMessage(const FString& Message);
};
