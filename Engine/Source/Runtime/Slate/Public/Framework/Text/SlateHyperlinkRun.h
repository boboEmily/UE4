// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#if WITH_FANCY_TEXT

class SLATE_API FSlateHyperlinkRun : public ISlateRun, public TSharedFromThis< FSlateHyperlinkRun >
{
public:

	typedef TMap< FString, FString > FMetadata;
	DECLARE_DELEGATE_OneParam( FOnClick, const FMetadata& /*Metadata*/ )

public:

	class FWidgetViewModel
	{
	public:

		bool IsPressed() const { return bIsPressed; }
		bool IsHovered() const { return bIsHovered; }

		void SetIsPressed( bool Value ) { bIsPressed = Value; }
		void SetIsHovered( bool Value ) { bIsHovered = Value; }

	private:

		bool bIsPressed;
		bool bIsHovered;
	};

public:

	static TSharedRef< FSlateHyperlinkRun > Create( const TSharedRef< const FString >& InText, const FHyperlinkStyle& InStyle, const FMetadata& Metadata, FOnClick NavigateDelegate );
																																	 
	static TSharedRef< FSlateHyperlinkRun > Create( const TSharedRef< const FString >& InText, const FHyperlinkStyle& InStyle, const FMetadata& Metadata, FOnClick NavigateDelegate, const FTextRange& InRange );

public:

	virtual ~FSlateHyperlinkRun() {}

	virtual FTextRange GetTextRange() const override;

	virtual void SetTextRange( const FTextRange& Value ) override;

	virtual int16 GetBaseLine( float Scale ) const override;

	virtual int16 GetMaxHeight( float Scale ) const override;

	virtual FVector2D Measure( int32 StartIndex, int32 EndIndex, float Scale ) const override;

	virtual int8 GetKerning(int32 CurrentIndex, float Scale) const override;

	virtual TSharedRef< ILayoutBlock > CreateBlock( int32 StartIndex, int32 EndIndex, FVector2D Size, const TSharedPtr< IRunRenderer >& Renderer ) override;

	virtual int32 OnPaint( const FPaintArgs& Args, const FTextLayout::FLineView& Line, const TSharedRef< ILayoutBlock >& Block, const FTextBlockStyle& DefaultStyle, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;

	virtual FChildren* GetChildren() override;

	virtual void ArrangeChildren( const TSharedRef< ILayoutBlock >& Block, const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren ) const override;

	virtual int32 GetTextIndexAt( const TSharedRef< ILayoutBlock >& Block, const FVector2D& Location, float Scale, ETextHitPoint* const OutHitPoint = nullptr ) const override;

	virtual FVector2D GetLocationAt( const TSharedRef< ILayoutBlock >& Block, int32 Offset, float Scale ) const override;

	virtual void BeginLayout() override { Children.Empty(); }
	virtual void EndLayout() override {}

	virtual void Move(const TSharedRef<FString>& NewText, const FTextRange& NewRange) override;
	virtual TSharedRef<IRun> Clone() const override;

	virtual void AppendText(FString& AppendToText) const override;
	virtual void AppendText(FString& AppendToText, const FTextRange& PartialRange) const override;

private:

	FSlateHyperlinkRun( const TSharedRef< const FString >& InText, const FHyperlinkStyle& InStyle, const FMetadata& InMetadata, FOnClick InNavigateDelegate );
																										 
	FSlateHyperlinkRun( const TSharedRef< const FString >& InText, const FHyperlinkStyle& InStyle, const FMetadata& InMetadata, FOnClick InNavigateDelegate, const FTextRange& InRange );

	FSlateHyperlinkRun( const FSlateHyperlinkRun& Run );

private:

	void OnNavigate();

private:

	TSharedRef< const FString > Text;
	FTextRange Range;
	FHyperlinkStyle Style;
	TMap<FString,FString> Metadata;
	FOnClick NavigateDelegate;

	TSharedRef< FWidgetViewModel > ViewModel;
	TSlotlessChildren< SWidget > Children;
};

#endif //WITH_FANCY_TEXT