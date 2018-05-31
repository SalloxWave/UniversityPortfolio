/* ===================================== */
/* ==   Starting the site with jfdn   == */
/* ===================================== */
$.getJSON("http://localhost:3826/getall", function(data){
    addPosts(data);
});

/* ===================================== */
/* ==          Post section           == */
/* ===================================== */
function addPosts(posts) {

    $.each(posts, function(index, post) {

        // Template for a post
        var htmlText = "\
            <span>" + post.message + "</span>\
            <button onclick='markAsRead(\"" + post._id + "\")'>\
                NEW!\
            </button>";

        // Create new post (not in the DOM yet)
        var li = document.createElement("li");
        li.setAttribute("id", post._id);
        li.innerHTML = htmlText;

        // Insert the new post on the top of list
        var postList = document.getElementById("postList");
        postList.insertBefore(li, postList.childNodes[0]);

        if (post.flag == true) {
            HTMLMarkAsRead(post._id);
        }

    });
}

function post() {
    // Get message to post from form
    var text = document.getElementById("text").value;

    // Check for valid post
    if (text.length == 0 || text.length > 140) {
        setError(true);
        return;
    }

    $.ajax({
        url: "http://localhost:3826/save?m="+text,
        success: function(data) {
            addPosts([data]);
            resetPostArea();
        },
        error: function() {
            alert("Failed to post :(. The server is probably down, we are working on it...");
        }
    });
}

function resetPostArea() {
    // Reseting the textfield, error message and charcount
    document.getElementById("text").value = "";
    setError(false);
    countChars();
}

/* ===================================== */
/* ==   Mark as read (flag) section   == */
/* ===================================== */
function markAsRead(postId) {
    $.ajax({
        url: "http://localhost:3826/flag?ID="+postId,
        success: function() {
            HTMLMarkAsRead(postId);            
        },
        error: function() {
            alert("Could not mark post as read :(. The server is proabably down, we are working on it...");
        }
    });
}

function HTMLMarkAsRead(postID) {
    document.getElementById(postID).classList.add("read");
    document.getElementById(postID).getElementsByTagName("button")[0].style.display = "none";
}

/* ===================================== */
/* ==  Other help functions section   == */
/* ===================================== */
function countChars() {
    var charLength = document.getElementById("text").value.length;
    document.getElementById("charsCount").innerHTML = "<span>"+charLength+"/140</span>";
    if (charLength > 140) {
        document.getElementById("charsCount").getElementsByTagName("span")[0].style.background = "red";
    }
}

function setError(activate) {
    activate ? document.getElementById("postError").style.display = "block":
               document.getElementById("postError").style.display = "none";
}

/* ======================================== */
/* == Init variables and start listeners == */
/* ======================================== */

//Set error to false when highlighting textbox
document.getElementById("text").addEventListener("click", setError(false));

//Update character count and disable error on change of input for the textbox
document.getElementById("text").addEventListener("input", function(){
    countChars();
    setError(false);
});

//Call post function when submitting form
document.getElementById("postForm").addEventListener("submit", function(e){
    post();
    // Disables the default action of the html "form"-element. 
    // This basically means not refreshing page after post
    e.preventDefault();
}, false);
