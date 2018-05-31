BASEURL = "http://localhost:3826/";

function getUserNameFromUrl() {
    return window.location.href.split(BASEURL)[1];
}

function requestFriend() {
    var userName = document.getCookie("userName");
    var friendName = getUserNameFromUrl();

    $.ajax({
        method: "POST",
        url: "api/friendrequest",
        contentType: "application/json",
        data: JSON.stringify({
            userName: userName,
            friendName: friendName            
        }),
        success: function() {
            var msg = "Successfully sent friend request to " + friendName;
            addHTMLMessage("friendAlert", msg, "green");
        },
        error: function(response, textStatus, err) {
            if (response.status == 400) {                
                var msg = "You already are friend or you have already sent friend request";
                addHTMLMessage("friendAlert", msg, "red");
            }
            else {
                var msg = "Error: " + response.status + ": " + textStatus;
                addHTMLMessage("friendAlert", msg, "red");                
            }
        }
    });
}

function addFriendHTML(friendName) {
    /* Create template for a friend:
        li.list-group-item
            a(href="/" + friend)
                img(src="https://static1.squarespace.com/static/526839d5e4b0a6ea6c312276/526ef8b1e4b0aa6f78f3f614/5271642ce4b03e61739879b6/1383162937731/tim+in+India+profile_circle.png") 
                span #{friend}
    */
    var imgSrc = "https://static1.squarespace.com/static/526839d5e4b0a6ea6c312276/526ef8b1e4b0aa6f78f3f614/5271642ce4b03e61739879b6/1383162937731/tim+in+India+profile_circle.png";
    var htmlText = "\
    <a href='/" + friendName + "'>\
    <img src='" + imgSrc + "' />\
    <span>" + friendName + "</span></a>";    

    // Add friend content into "li"-tag
    var li = document.createElement("li");
    li.setAttribute("class", "list-group-item");
    li.innerHTML = htmlText;

    // Insert the new post on the top of list
    var friendList = document.getElementById("friendList");
    friendList.insertBefore(li, friendList.childNodes[0]);
}

function addFriend(friendName) {    
    var userName = document.getCookie("userName");    

    $.ajax({
        method: "POST",
        url: "api/addFriend",
        contentType: "application/json",
        data: JSON.stringify({
            userName: userName,
            friendName: friendName            
        }),
        success: function() {
            //Update friend data
            addFriendHTML(friendName);
            var friendCount = document.getElementById("friendList").childElementCount
            document.getElementById("friendHeader").innerHTML = "Friends (" + friendCount + ")";

            //Remove friend request
            document.getElementById("friendRequest" + friendName).remove();
            
            //Count friend requests
            var reqCount = document.getElementById("friendRequestList").childElementCount;            

            //Remove friend requests if there are no friend requests
            if (reqCount == 0) {
                //Remove both header and friend request list
                document.getElementById("friendRequestHeader").remove();
                document.getElementById("friendRequestList").remove();
            }
            else {
                //Update header
                document.getElementById("friendRequestHeader").innerHTML = "Friend requests (" + reqCount + ")";
            }
        },
        error: function(response, textStatus, err) {
            if (response.status == 400) {
                var msg = "You are already friends";
                addHTMLMessage("friendRequestAlert", msg, "red");
            }
            else {
                var msg = "Error: " + response.status + ": " + textStatus;
                addHTMLMessage("friendRequestAlert", msg, "red");
            }
        }
    });
}

function removeFriend(friendName) {
    if (!friendName) {
        friendName = getUserNameFromUrl();
    }
    
    var userName = document.getCookie("userName");
    
    $.ajax({
        method: "POST",
        url: "api/removefriend",
        contentType: "application/json",
        data: JSON.stringify({    
            userName: userName,    
            friendName: friendName
        }),
        success: function() {            
            location.reload();
        },
        error: function(response, textStatus, err) {
            if (response.status == 400) {
                var msg = "Must be friends to remove friend";
                addHTMLMessage("friendAlert", msg, "red");                
            }
            else {
                var msg = "Error: " + response.status + ": " + textStatus;                
                addHTMLMessage("friendAlert", msg, "red");
            }
        }
    });
}

function addPost(ownerName, message) {
    /* Create template for a post:
        li.list-group-item
            h3 #{post.message}                                    
            a(href="/" + post.ownerName) Poster: #{post.ownerName}
    */
    var htmlText = "\
    <h3>" + message + "</h3>\
    <a href='/" + ownerName + "'>Poster: " + ownerName + "</a>";    

    // Add post content into "li"-tag
    var li = document.createElement("li");
    li.setAttribute("class", "list-group-item");  
    li.innerHTML = htmlText;

    // Insert the new post on the top of list
    var postList = document.getElementById("postList");
    postList.insertBefore(li, postList.childNodes[0]);
}

function post() {
    var ownerName = document.getCookie("userName");
    var friendName = getUserNameFromUrl();
    var message = document.getElementById("txtMessage").value;

    if (message.length == 0 || message.length > 140) {
        var msg = "Message can't be empty and cannot surpass 140 characters";
        addHTMLMessage("postAlert", msg, "red");
        return;
    }
    
    $.ajax({
        method: "POST",
        url: "api/post",
        contentType: "application/json",
        data: JSON.stringify({
            ownerName: ownerName,
            friendName: friendName,
            message: message
        }),
        success: function() {
            addPost(ownerName, message);
        },
        error: function(response, textStatus, err) {
            if (response.status == 400) {
                var msg = "Must be friends to post";
                addHTMLMessage("postAlert", msg, "red");
            }
            else {
                var msg = "Error: " + response.status + ": " + textStatus;
                addHTMLMessage("postAlert", msg, "red");
            }
        }
    });
}

document.getElementById("postForm").addEventListener("submit", function(e) {
    e.preventDefault();
    post();    
});