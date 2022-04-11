function homePageSetup() {
    const vibeOptions = document.getElementById("vibeOptions");
    const buttons = vibeOptions.getElementsByTagName("button");
    for(const button of buttons) {
      button.addEventListener("click", () => {
        fetch(`/vibes/${button.id}`, {
          method: "POST",
        }).then( () => {
            window.location.assign(`/stats/${button.id}`);
        });
      });
    }
}


function makeLetMeInRequest() {

}
