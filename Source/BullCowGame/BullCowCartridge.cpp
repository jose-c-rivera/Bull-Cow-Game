// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    FBullCowCount Count;


    ValidWordList = GetValidWords(Words);
    

    SetupGame();

    PrintLine(TEXT("The number of possible words is %i"), ValidWordList.Num());
    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    /*
        If game is over then do ClearScreen() and SetupGame() the game
        else Checking PlayerGuess
    */
   if (bGameOver)
   {
       ClearScreen();
       SetupGame();
   }
   else // Checking PlayerGuess
   {
       ProcessGuess(PlayerInput);
   }
}

void UBullCowCartridge::SetupGame()
{
    // Welcoming the players
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = ValidWordList[FMath::RandRange(0, ValidWordList.Num()-1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("The word is: [ %s ]"), *HiddenWord);
    PrintLine(TEXT("You have %i Lives"), Lives);
    PrintLine(TEXT("Please enter a word and press ENTER."));
    
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPlease press ENTER to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Congrats! You guessed the right word."));
        EndGame();
        return;
    }

    // Check if isogram
    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }

    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("Sorry, try guessing again.\nYou have %i Lives remaining"), Lives);
        PrintLine(TEXT("The hidden word is %i characters long"), Lives);
        return;
    }

    // Remove a life
    --Lives;
    PrintLine(TEXT("You have lost a life."));

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player the Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Try guessing again.\nYou have %i Lives remaining"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& CheckString) const
{

    for(int32 Index = 0; Index < CheckString.Len(); Index++){

        for (int32 Comparison = Index + 1; Comparison < CheckString.Len(); Comparison++){

            if (CheckString[Index] == CheckString[Comparison])
            {
                return false;
            }
        }
    }

    return true;
}


TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >=4 && Word.Len() <=8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for(int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls ++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows ++;
                break;
            }
        }
    }

    return Count;
}