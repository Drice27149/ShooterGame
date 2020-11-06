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
    
    void SetNameText(FString Name);
    
    void SetScoreText(FString Score);
    
    void SetMessageText(FString Message);
    
    void SetRemainTimeText(FString RemainTime);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "InfoWidget")
    void OnGetMessage(const FString& Message);
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ShooterName = NULL;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ShooterScore = NULL;
    
    /** message recieved from server **/
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MatchMessage = NULL;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MatchRemainTime = NULL;
};
