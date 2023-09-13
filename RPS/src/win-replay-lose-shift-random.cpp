RPSChoice makeRPSChoice(){
  /*
    Returns choice made by an Arduino for a round.

    TODO: Complete this function.
    
    Available global variables:
      arduinoChoices : The set of RPS Choices made by the Arduino in previous rounds.
      opponentChoices : The set of RPS Choices made by the opponent in previous rounds.
      roundResults : The set of round results for previous rounds.
      roundsPlayed : The number of rounds played by the Arduino.
    
    Available functions:
      randRPS
      winningChoiceAgainst
      losingChoiceAgainst
      RPSChoiceToStr
      RoundResToWinner
  */

  // fixed choice implementation
  RPSChoice choice;
  if (roundsPlayed == 0){
    choice = RPS_Paper;
  } if (roundsPlayed > 0){
    int random = rand();
    if (random%2 == 0){
      if (roundResults[roundsPlayed - 1] == Round_Won){
              choice = arduinoChoices[roundsPlayed - 1];
      } if (roundResults[roundsPlayed - 1] == Round_Lost){
              choice = opponentChoices[roundsPlayed - 1];
      } else if (roundResults[roundsPlayed - 1] == Round_Draw){
              choice = arduinoChoices[roundsPlayed - 1];
      }
    } else {
      choice = randRPS();
      Serial.println("Random");
    }
  }
  return choice;
}