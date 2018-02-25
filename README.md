# UE4StealthGame
Unreal Engine 4 C++ FPS Template modded to become a Stealth Game,
following the Udemy course "Unreal Engine 4 Mastery: Create Multiplayer Games" by Tom Looman.

## Content

- a game [Objective Actor](Source/FPSGame/Public/FPSObjective.h) that the player has to pickup
- an [Extraction Zone](Source/FPSGame/Public/FPSExtractionZone.h) where to carry this objective
- a [Game Mode](Source/FPSGame/Public/FPSGameMode.h) to trigger mission completion when the objective has been carried to the extraction zone

## Challenges

- a [Black Hole](Source/FPSGame/Public/BlackHole.h) that attracts and destroys every physic-enabled Actors in it's sphere of influence
- a [Launch Pad](Source/FPSGame/Public/LaunchPad.h) to launch the player in the air, as well as any other physic-enabled Actors