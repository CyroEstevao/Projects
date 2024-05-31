const difficulties = ["Easy", "Medium", "Hard", "Fact", "Very Hard", "Master of Typing"];
const goalString = ["CS 3308",
  "This is not a test.",
  "42 is the answer to the Ultimate Question of Life, the Universe, and Everything.",
  "The Javascript Lab is kinda fun!",
  "It sI geittng cmOplic@tEd!",
  "ThaT Is hOW wE uSed To tYpe oN iNterNet, iN E@rLy 20@0s!"
];


var i = 0;
let newNextChallengeButton;
let start;
let intervalID; // Variable to store the interval ID
let originalGoalString = goalString[i]; // Store the original goal string
let allowTyping = true; // Add this flag to track whether the user can type or not


// Event listener to handle keypress events
window.addEventListener("keypress", function (event) {
  const textbox = document.getElementById("userinput"); // Get the textbox element
  const restartButton = document.querySelector("#restartButton");

  if (!allowTyping) {
    return; // If not, return and do not update the textbox content
  }
  const currentChar = event.key;
  const goalChar = originalGoalString[textbox.textContent.length];

  // Highlight typos if the current character does not match the goalString
  if (currentChar !== goalChar) {
    textbox.innerHTML = textbox.textContent + `<span style="color: red;">${currentChar}</span>`;
  } else {
    textbox.textContent = textbox.textContent + currentChar; // Append the keypress
  }

  // Check if the user input matches the goalString[i]
  if (textbox.textContent == goalString[i]) {
    const result = document.getElementById("result"); // If matches, update result element
    result.textContent = "You Win!"; // Display "You Win"
    textbox.textContent = ""; // Clear the textbox
  
    allowTyping = false; //disallow typing to prevent many next challenge btns
    
    //check index of the challenge
    if (i < goalString.length - 1) {

      //create a button for the next challenge
      const newNextChallengeButton = document.createElement('button');
      newNextChallengeButton.textContent = 'Next Challenge';
      newNextChallengeButton.style.display = 'block'; // Display the button

      //add the button into the section
      const sectionDiv = document.getElementById('section');
      sectionDiv.appendChild(newNextChallengeButton);

      newNextChallengeButton.addEventListener('click', function() {
        nextChallenge(); // Call the nextChallenge function
        sectionDiv.removeChild(newNextChallengeButton); // Remove the newNextChallengeButton after it's clicked
      });
    
    } 
    else {
        // If all challenges are completed
        result.textContent = "You Finish All the challenges!";

        //reset these elements so it doesn't display
        const lvl = document.getElementById("level");
        lvl.textContent = "";

        const goal = document.getElementById("goal");
        goal.textContent = "";

    }
  }

  // Event listener for the restart button
  restartButton.addEventListener('click', restart);
  //update the color
  highlightUserInput();
  
});

// Event listener to handle special keystrokes (BACKSPACE and ENTER)
window.addEventListener("keydown", function (event) {

  
  // Check if BACKSPACE key is pressed Remove the last character
  if (event.key === "Backspace") {
    // Prevent the default behavior
    event.preventDefault();
    const textbox = document.getElementById("userinput");
    textbox.textContent = textbox.textContent.slice(0, -1);
  }

  // Check if the ENTER key is pressed
  if (event.key === "Enter") {
    // Prevent the default behavior Enter
    event.preventDefault();

    // Reset the level
    const textbox = document.getElementById("userinput");
    textbox.textContent = "";
  }
  //update color
  highlightUserInput();

});

function nextChallenge() {
  i++;
  start = 0; // Reset the start timestamp
  totalTime = 0; // Reset the start timestamp for the new challenge
  originalGoalString = goalString[i]; // Update the original goal string for the new challenge
  allowTyping = true; //set type to work again

  // Reset the heading with the new challenge
  const result = document.getElementById("result");
  result.textContent = "Type the Following text:";
  const lvl = document.getElementById("level");
  lvl.textContent = difficulties[i];
  const goal = document.getElementById("goal");
  goal.textContent = goalString[i];

  const timingElement = document.getElementById("timing");
  timingElement.textContent = "";
  clearInterval(intervalID); // Clear the interval to stop updating the timer display
  intervalID = startTimer();

  const textbox = document.getElementById("userinput"); // Get the textbox element
  highlightUserInput(); // Update the color for the new challenge

}

function restart() {
  i = 0; //reset i
  start = Date.now(); // Reset the start timestamp
  originalGoalString = goalString[i]; // Update the original goal string for the new challenge
  allowTyping = true; //set type to work again

  // Reset the heading to default
  const result = document.getElementById("result");
  result.textContent = "Type the Following text:"; 
  const textbox = document.getElementById("userinput");
  textbox.textContent = "";
  const lvl = document.getElementById("level");
  lvl.textContent = difficulties[i];
  const goal = document.getElementById("goal");
  goal.textContent = goalString[i];

  const timingElement = document.getElementById("timing");
  timingElement.textContent = ""; 

  //update highlight
  highlightUserInput(); 
}

function startTimer() {
  start = Date.now(); // Record the start
  allowTyping = true;
  // Update the timer display every 100 milliseconds
  setInterval(updateTimerDisplay, 100);
}
function updateTimerDisplay() {
  const currentTime = Date.now(); // Get the current timestamp
  const diff = (currentTime - start) / 1000; // Calculate the elapsed time in seconds

  const timingElement = document.getElementById("timing");
  timingElement.textContent = `Elapsed time: ${diff.toFixed(2)} seconds`;
}

function highlightUserInput() {
  const textbox = document.getElementById("userinput");
  const userText = textbox.textContent;

  // Extract the portion of the goal string that matches the user's input
  const goalText = originalGoalString.substring(0, userText.length); 

  let highlightedText = ""; // var to store the highlight text
  for (let j = 0; j < userText.length; j++) {
    if (userText[j] === goalText[j]) {
      //if matches the character, no change
      highlightedText += userText[j];
    } 
    else {
      //if does not match update the color style to red
      highlightedText += `<span style="color: red;">${userText[j]}</span>`;
    }
  }
  //update the textbox with the highlighted string
  textbox.innerHTML = highlightedText; 
}

//update timer
intervalID = startTimer();


