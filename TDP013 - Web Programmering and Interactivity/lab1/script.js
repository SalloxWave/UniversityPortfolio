/* ===================================== */
/* ==   Adds a new post to the list   == */
/* ===================================== */
function post()
{
    // Referance to new-post input field
    var text = document.getElementById("text").value;

    // Check for valid post
    if (text.length == 0 || text.length > 140)
    {
        toggleError(true);
        return;
    }

    // Template for a post
    var htmlText = "\
        <span>" + text + "</span>\
        <button onclick='markAsRead(\"post" + postCount + "\")'>\
            NEW!\
        </button>";

    // Create new post (not in the DOM yet)
    var li = document.createElement("li");
    li.setAttribute("id", "post" + postCount);  
    li.innerHTML = htmlText;

    // Insert the new post on the top of list
    var postList = document.getElementById("postList");
    postList.insertBefore(li, postList.childNodes[0]);

    // Adds one to the counter so the next post will get a unique ID
    postCount++;

    // Reseting the textfield, error message and charcount
    document.getElementById("text").value = "";
    toggleError(false);
    countChars();
}


/* ===================================== */
/* ==      Marks a post as read       == */
/* ===================================== */
function markAsRead(postId)
{
    document.getElementById(postId).classList.add("read");
    document.getElementById(postId).getElementsByTagName("button")[0].style.display = "none";
}

/* ===================================== */
/* ==    Count Characters in input    == */
/* ===================================== */
function countChars()
{
    var charLength = document.getElementById("text").value.length;
    document.getElementById("charsCount").innerHTML = "<span>"+charLength+"/140</span>";
    if(charLength > 140){
        document.getElementById("charsCount").getElementsByTagName("span")[0].style.background = "red";
    }
}


/* ============================================ */
/* ==    Disable/Enable the Error Message    == */
/* ============================================ */
function toggleError(activate)
{
    if (activate)
    {
        document.getElementById("postError").style.display = "block";
    }
    else
    {
        document.getElementById("postError").style.display = "none";
    }
}


/* ===================================== */
/* ==    Fast post adder for devs     == */
/* ===================================== */
function addRandomPosts(amount){

    // Init Variables
    var possible = "ABCD LMNOPTUVW XYZa qrstuv wxyz01 23456 789";

    // Add "amount" of posts
    for (var i = 0; i < amount; i++){

        // Generate random string
        var message = "";
        for (var char = 0; char < 130; char++)
        {
            message += possible.charAt(Math.floor(Math.random() * possible.length));
        }

        // Add post
        document.getElementById("text").value = message;
        post();
    }
    
    // Log to console
    console.log(amount + " random posts added.");

}


/* ======================================== */
/* == Init variables and start listeners == */
/* ======================================== */

// Init variables
var developerMode = false;
var postCount = 0;

// Adding listener to the post-button's click event
document.getElementById("text").addEventListener("click", toggleError(false));
document.getElementById("text").addEventListener("input", function(){
    countChars();
    toggleError(false);
});
document.getElementById("postForm").addEventListener("submit", function(e){
    post();
    // Disables the default action of the html "form"-element
    e.preventDefault();
}, false);

// Add Posts if in dev mode (nice when styling the list)
if(developerMode)
{
    console.log("Developer Mode is ON");
    addRandomPosts(10);
}