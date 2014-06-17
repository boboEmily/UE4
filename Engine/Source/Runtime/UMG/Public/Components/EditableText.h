// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EditableText.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditableTextChangedEvent, const FText&, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEditableTextCommittedEvent, const FText&, Text, ETextCommit::Type, CommitMethod);

/** Editable text box widget */
UCLASS(meta=( Category="Common" ), ClassGroup=UserInterface)
class UMG_API UEditableText : public UWidget
{
	GENERATED_UCLASS_BODY()

protected:

	/** The text content for this editable text box widget */
	UPROPERTY(EditDefaultsOnly, Category=Content)
	FText Text;

	/** Hint text that appears when there is no text in the text box */
	UPROPERTY(EditDefaultsOnly, Category=Content)
	FText HintText;

	/** Text style */
	UPROPERTY(EditDefaultsOnly, Category=Style, meta=( DisplayThumbnail = "true" ))
	USlateWidgetStyleAsset* Style;

	/** Background image for the selected text (overrides Style) */
	UPROPERTY(EditDefaultsOnly, Category="Style", meta=( DisplayThumbnail = "true" ), AdvancedDisplay)
	USlateBrushAsset* BackgroundImageSelected;

	/** Background image for the selection targeting effect (overrides Style) */
	UPROPERTY(EditDefaultsOnly, Category="Style", meta=( DisplayThumbnail = "true" ), AdvancedDisplay)
	USlateBrushAsset* BackgroundImageSelectionTarget;

	/** Background image for the composing text (overrides Style) */
	UPROPERTY(EditDefaultsOnly, Category="Style", meta=( DisplayThumbnail = "true" ), AdvancedDisplay)
	USlateBrushAsset* BackgroundImageComposing;

	/** Image brush used for the caret (overrides Style) */
	UPROPERTY(EditDefaultsOnly, Category="Style", meta=( DisplayThumbnail = "true" ), AdvancedDisplay)
	USlateBrushAsset* CaretImage;

	/** Font color and opacity (overrides Style) */
	UPROPERTY(EditDefaultsOnly, Category=Appearance)
	FSlateFontInfo Font;

	/** Text color and opacity (overrides Style) */
	UPROPERTY(EditDefaultsOnly, Category=Appearance)
	FSlateColor ColorAndOpacity;

	/** Sets whether this text box can actually be modified interactively by the user */
	UPROPERTY(EditDefaultsOnly, Category=Appearance)
	bool IsReadOnly;

	/** Sets whether this text box is for storing a password */
	UPROPERTY(EditDefaultsOnly, Category=Appearance)
	bool IsPassword;

	/** Minimum width that a text block should be */
	UPROPERTY(EditDefaultsOnly, Category=Appearance)
	float MinimumDesiredWidth;

	/** Workaround as we loose focus when the auto completion closes. */
	UPROPERTY(EditDefaultsOnly, Category=Behavior, AdvancedDisplay)
	bool IsCaretMovedWhenGainFocus;

	/** Whether to select all text when the user clicks to give focus on the widget */
	UPROPERTY(EditDefaultsOnly, Category=Behavior, AdvancedDisplay)
	bool SelectAllTextWhenFocused;

	/** Whether to allow the user to back out of changes when they press the escape key */
	UPROPERTY(EditDefaultsOnly, Category=Behavior, AdvancedDisplay)
	bool RevertTextOnEscape;

	/** Whether to clear keyboard focus when pressing enter to commit changes */
	UPROPERTY(EditDefaultsOnly, Category=Behavior, AdvancedDisplay)
	bool ClearKeyboardFocusOnCommit;

	/** Whether to select all text when pressing enter to commit changes */
	UPROPERTY(EditDefaultsOnly, Category=Behavior, AdvancedDisplay)
	bool SelectAllTextOnCommit;

	/** Called whenever the text is changed interactively by the user */
	UPROPERTY(BlueprintAssignable)
	FOnEditableTextChangedEvent OnTextChanged;

	/** Called whenever the text is committed.  This happens when the user presses enter or the text box loses focus. */
	UPROPERTY(BlueprintAssignable)
	FOnEditableTextCommittedEvent OnTextCommitted;

	/**  */
	UFUNCTION(BlueprintCallable, Category="Widget")
	FText GetText() const;

	/**  */
	UFUNCTION(BlueprintCallable, Category="Widget")
	void SetText(FText InText);
	
	// UWidget interface
	virtual void SyncronizeProperties() override;
	// End of UWidget interface

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget

	void HandleOnTextChanged(const FText& Text);
	void HandleOnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

protected:
	TSharedPtr<SEditableText> MyEditableText;
};
