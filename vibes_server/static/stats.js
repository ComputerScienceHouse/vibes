function getStats() {
   // http://localhost:8080
   return fetch('/vibes/all')
   .then(response => {
       if (!response.ok) {
           throw new Error("HTTP error " + response.status);
       }
       return response.json();
   })
   .then(json => {
       console.log(json);
       goodVibesTicker = document.getElementById("good_vibes_ticker");
       badVibesTicker = document.getElementById("bad_vibes_ticker");
       goodVibesTicker.innerHTML = json.good_vibes;
       badVibesTicker.innerHTML = json.bad_vibes;

   })
   .catch(function () {
       this.dataError = true;
   })
}
