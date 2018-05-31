/* Status codes
200: OK
400: Bad request, invalid or missing parameter
401: Unauthorized request
404: Missing Page
405: Method not allowed
409: Conflict (for example username already taken)
500: Internal server error
*/

/* ================================== */
/* ==      Constant variables      == */
/* ================================== */
HTML_PATH = "html/";
PORT = 3826;
MONGO_PORT = 27017;
MONGO_URL = "localhost";
DB_NAME = "website3826";
SALT_ROUNDS = 10;
BASEURL = "http://localhost:3826/";

/* =============================== */
/* ==      Library imports      == */
/* =============================== */
var express = require("express");  //https://www.npmjs.com/package/express
var bcrypt = require("bcrypt");  //https://www.npmjs.com/package/bcrypt
var session = require('express-session');  //https://www.npmjs.com/package/express-session
var cookieParser = require('cookie-parser'); //https://www.npmjs.com/package/cookie-parser
var bodyParser = require("body-parser");  //https://www.npmjs.com/package/body-parser
var mongo = require('mongodb');  //https://www.npmjs.com/package/mongodb
var superRequest = require("superagent");  //https://www.npmjs.com/package/superagent

/* ================================ */
/* ==      Configure server      == */
/* =================================*/
var app = express();
app.set('view engine', 'pug');  //https://pugjs.org/api/getting-started.html
app.set('views', HTML_PATH);

/* =========================== */
/* ==      Middlewares      == */
/* =========================== */
app.use(express.static("public"));
app.use(cookieParser());
app.use(bodyParser.json());
app.use(function (err, request, response, next) {
    var templateData = getBaseTemplateData(request.cookies);
    templateData.title = "Something went wrong (500)";    
    response.status(500).render("500.pug", templateData);
});

/* ========================================= */
/* ==      Setup database connection      == */
/* ========================================= */
var mongoServer = new mongo.Server(MONGO_URL, MONGO_PORT);
var db = new mongo.Db(DB_NAME, mongoServer);
db.open(function (err, db) {
    if (!err) {
        console.log("Connected to db " + DB_NAME + " on port " + MONGO_PORT);
        console.log("Server located at URL " + MONGO_URL);
    }
    else {
        console.log("Error occured when trying to connect to database " + DB_NAME + " :" + err.message);        
    }
});

/* ================================ */
/* ==      Server functions      == */
/* ================================ */
app.get("/", function(request, response) {
    if (request.cookies.loggedIn == "true") {
        response.redirect("/" + request.cookies.userName);
        return;
    }

    var templateData = getBaseTemplateData(request.cookies);
    templateData.title = "Home";
    response.render("index.pug", templateData);
});

app.get("/register", function(request, response) {
    var templateData = getBaseTemplateData(request.cookies);
    templateData.title = "Register";
    response.render("register.pug", templateData);
});

app.get("/login", function(request, response) {
    var templateData = getBaseTemplateData(request.cookies);
    templateData.title = "Login";
    response.render("login.pug", templateData);
});

app.get("/search", function(request, response) {
    var templateData = getBaseTemplateData(request.cookies);
    templateData.title = "Search";
    templateData.results = [];
    
    var searchQuery = request.query.q;

    if (searchQuery == null) {
        response.render("search", templateData);
        return;
    }

    //Default limit to no limit (0) if there is no parameter given
    var limit = request.query.l ? parseInt(request.query.l) : 0;
        
    db.collection("users", function (err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        var query = { $or: [{ userName: {$regex: new RegExp(searchQuery,"i")} }, { email: {$regex: searchQuery} }] };
        var returnValues = { userName: 1, profileImg: 1 };

        //Search for matching username or email and return values neccessary
        collection.find(query, returnValues)
        .limit(limit)  //Limit amount of search items
        .sort({userName: 1})  //Sort by username in ascending order
        .toArray(function(err, result) {            
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

            console.log("Searched for: " + searchQuery + " (" + result.length + ") with limit " + limit);
            templateData.results = result;
            response.render("search", templateData);
        });               
    });
});

app.get("/registered", function(request, response) {
    var templateData = getBaseTemplateData(request.cookies);
    templateData.title = "Registered";    
    response.render("registered.pug", templateData);
});

app.get("/:userName", function(request, response) {    
    var userName = request.params.userName;
    var loggedIn = request.cookies.loggedIn == "true";

    //Must be logged in to add friend and can't add yourself as friend    
    var showRequestFriend = request.cookies.userName != userName && loggedIn;

    //Must be logged in to post and can't post on your own profile
    var showPost = request.cookies.userName != userName && loggedIn;

    var templateData = getBaseTemplateData(request.cookies);
    templateData.title = userName;
    templateData.ownProfile = request.cookies.userName == userName;
    templateData.showRequestFriend = showRequestFriend;
    templateData.showRequestPending = false;
    templateData.showPost = showPost;
    templateData.posts = [];
    templateData.user = null;
    
    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        collection.findOne({userName: userName}, function(err, user) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
            
            if (user == null) {
                response.status(404).render("404.pug", get404TemplateData(request.cookies));
                return;
            }
            
            //Don't show friend request if already friends
            if (user.friends.indexOf(request.cookies.userName) >= 0) {
                templateData.showRequestFriend = false;
            }

            //Show request is pending when already sent friend request            
            if (user.friendRequests.indexOf(request.cookies.userName) >= 0) {
                templateData.showRequestPending = true;
            }

            superRequest.get(BASEURL + "api/" + userName + "/posts", function(err, result) {                
                if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

                //Render profile page with found user and user's posts
                templateData.posts = result.body;                
                templateData.user = user;                           
                response.render("profile.pug", templateData);
            });       
        });
    });
});

app.post("/api/register", function(request, response) {    
    if (request.cookies.loggedIn == "true") {
        console.log("Can't register, already logged in");
        response.status(400).end();
        return;
    }
    
    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        //Get required user data from the request
        var user = request.body;        

        //Look for already taken username or email
        collection.findOne({ $or: [{userName: user.userName}, {email: user.email}] }, function(err, result) {
            if (result != null) {                
                response.status(409).end();
                return;
            }

            //Hash user's password
            bcrypt.hash(user.password, SALT_ROUNDS, function(err, hashedPw) {
                if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

                //Set hashed password and default value
                user.password = hashedPw;
                user.profileImg = "";
                user.friends = [];
                user.friendRequests = [];
                
                collection.insert(user, function(err) {
                    if (!err) {
                        console.log("Successfully added user '" + user.userName + "' to database");
                        response.status(200).end();
                    }
                    else { 
                        console.log("Error occured on registering: " + err);
                        response.status(500).end();
                    }
                });
            });            
        });
    });
});

app.get("/api/:userName", function(request, response) {
    var userName = request.params.userName;

    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        collection.findOne({userName: userName}, function(err, user) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

            if (user != null) {
                //Send user data if user was found
                response.send(user);
            }
            else {
                //Username was not found
                response.status(400).end();
            }
        });
    });
});

app.post("/api/takenname", function(request, response) {
    var userName = request.body.userName;    

    db.collection("users", function (err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        collection.findOne({ userName: userName }, function(err, user) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

            //Send if user with specified username exists or not
            response.send((user != null).toString());
        });
    });
});

app.post("/api/takenemail", function(request, response) {
    var email = request.body.email;    

    db.collection("users", function (err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        collection.findOne({ email: email }, function(err, user) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

            //Send if user with specified email exists or not
            response.send((user != null).toString());
        });
    });
});

app.post("/api/login", function(request, response) {        
    if (request.cookies.loggedIn == "true") {
        console.log("Can't login, already logged in");
        response.status(400).end();
        return;
    }

    var loginName = request.body.loginName;
    var password = request.body.password;
    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        //Try to find the user in database either by username or email
        collection.findOne({ $or: [{userName: loginName}, {email: loginName}] }, function(err, user) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

            //User exists
            if (user != null) {
                //Compare specified password with actual hashed password
                bcrypt.compare(password, user.password, function(err, correctPw) {
                    if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

                    if (correctPw) {
                        //Send username to allow client to store username as cookie
                        response.send(user.userName);
                        response.status(200).end();
                    }
                    else {                        
                        response.status(401).end();
                    }
                });                
            }
            else {                                 
                response.status(401).end();
            }
        });
    });
});

app.post("/api/friendrequest", function(request, response) {
    if (request.cookies.loggedIn != "true") {
        console.log("Must be logged in to send friend request");
        response.status(400).end();
        return;
    }

    var userName = request.body.userName;
    var friendName = request.body.friendName;
    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        //Check if you already are friends
        collection.findOne({userName: userName, friends: friendName}, function(err, result) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }
            
            if (result != null) {
                console.log(userName + " is already friend with " + friendName);
                response.status(400).end();
                return;
            }

            var qFriendExists = {userName: friendName};
            var qNotYourself = {userName: {$ne: userName}};
    
            //Find specified friend if it's not yourself
            collection.findOne({$and: [qFriendExists, qNotYourself]}, function(err, friend) {
                if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
    
                if (friend == null) {
                    console.log(friendName + " not found or is yourself. Could not send friend request");
                    response.status(400).end();
                    return;
                }
                
                //Friend request has already been sent
                if (friend.friendRequests.indexOf(userName) >= 0) {
                    console.log(userName + " already sent friend request to " + friendName);
                    response.status(400).end();
                    return;
                }   
                
                var updateQuery = {$push: {friendRequests: userName}};
    
                //Add friend request by inserting username of user sending the request
                collection.update({userName: friendName}, updateQuery, function(err, result) {
                    if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
                    
                    console.log(userName + " successfully sent friend request to " + friendName);
                    response.status(200).end();                            
                });            
            });
        });
    });
});

app.post("/api/addfriend", function(request, response) {
    if (request.cookies.loggedIn != "true") {
        console.log("Must be logged in to add friend!");
        response.status(400).end();
        return;
    }

    var userName = request.body.userName;
    var friendName = request.body.friendName;
    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        var qFriendExists = {userName: friendName};
        var qNotYourself = {userName: {$ne: userName}};

        //Find specified friend if it's not yourself
        collection.findOne({$and: [qFriendExists, qNotYourself]}, function(err, friend) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

            if (friend == null) {
                console.log(friendName + " not found or is yourself. Could not add friend");
                response.status(400).end();
                return;
            }

            //Check if not already friends
            var qNotAlreadyFriend = {userName: userName, friends: {$ne: friendName}};
            collection.findOne(qNotAlreadyFriend, function(err, user) {
                if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

                if (user == null) {
                    console.log(userName + " already has " + friendName + " as friend");
                    response.status(400).end();
                    return;
                }
                
                //Insert specified friend to user's friend list
                var updateQuery = {$push: {friends: friendName}, $pull: {friendRequests: friendName}};
                collection.update({userName: userName}, updateQuery, function(err) {
                    if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
                    
                    console.log(userName + " successfully added " + friendName + " as friend");

                    //Insert specified user to friend's friend list, also make sure to remove friend request
                    updateQuery = {$push: {friends: userName}, $pull: {friendRequests: userName}};
                    collection.update({userName: friendName}, updateQuery, function(err) {
                        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
                        
                        console.log(friendName + " sucessfully added " + userName + " as friend");
                        response.status(200).end();  
                    });
                });
            });                
        })
    });
});

app.post("/api/removefriend", function(request, response) {
    if (request.cookies.loggedIn != "true") {
        console.log("Must be logged in to remove friend!");
        response.status(400).end();
        return;
    }

    var friendName = request.body.friendName;
    var userName = request.body.userName;   
    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

        collection.findOne({userName: userName, friends: friendName}, function(err, user) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

            if (user == null) {
                console.log("Could not remove friend. " + userName + " is not friend with " + friendName);
                response.status(400).end();
                return;
            }

            //Remove specified friend from user
            var updateQuery = {$pull: {friends: friendName}};
            collection.update({userName: userName}, updateQuery, function(err) {
                if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

                console.log("Successfully removed " + friendName + " as a friend of " + userName);

                //Remove user from specified friend
                updateQuery = {$pull: {friends: userName}};
                collection.update({userName: friendName}, updateQuery, function(err) {
                    if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

                    console.log("Successfully removed " + userName + " as a friend of " + friendName);
                    response.status(200).end();
                });
            });
        });
    });
});

app.post("/api/post", function(request, response) {    
    if (request.cookies.loggedIn != "true") {
        console.log("Must be logged in to post!");
        response.status(400).end();
        return;
    }

    var postData = request.body;    
    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        //Check if user trying to post is a friend of the post receiver
        var query = { userName: postData.ownerName, friends: postData.friendName};
        collection.findOne(query, function(err, user) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

            if (user == null) {
                console.log(postData.ownerName + " is not a friend of " + postData.friendName);
                response.status(400).end();
                return;
            }            

            db.collection("posts").insert(postData, function(err, result) {
                if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
                
                console.log(postData.ownerName + " successfully posted on " + postData.friendName + "'s profile");
                response.status(200).end();                
            });
        });
    });
});

//Remove all posts posted on specified profile
app.post("/api/clearposts", function(request, response) {
    if (request.cookies.loggedIn != "true") {
        console.log("Must be logged in to clear posts!");
        response.status(400).end();
        return;
    }

    var userName = request.body.userName;
    db.collection("posts", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

        collection.remove({friendName: userName}, function(err) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

            console.log("Successfully removed all posts posted on profile " + userName);
            response.status(200).end();
        });
    });
});

//Remove all posts posted by specified user
app.post("/api/clearposted", function(request, response) {
    if (request.cookies.loggedIn != "true") {
        console.log("Must be logged in to clear posts!");
        response.status(400).end();
        return;
    }

    var userName = request.body.userName;
    db.collection("posts", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

        collection.remove({ownerName: userName}, function(err) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

            console.log("Successfully removed all posts posted by user " + userName);
            response.status(200).end();
        });
    });
});

app.get("/api/:userName/posts", function(request, response) {
    var userName = request.params.userName;

    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }

        collection.findOne({userName: userName}, function(err, user) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
            
            if (user == null) {            
                response.status(400).end();
                return;
            }
    
            db.collection("posts", function(err, collection) {
                if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
    
                //Get posts posted on specified profile, sorted by post owner in ascending order
                var query = {friendName: userName};
                //Sort by id makes it sort by date?                
                collection.find(query).sort({_id: 1}).toArray(function(err, posts) {
                    if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
                    
                    //Return posts as array
                    response.send(posts);
                });
            });
        });
    });
});

app.get("/api/:userName/posted", function(request, response) {    
    var userName = request.params.userName;
    
    db.collection("users", function(err, collection) {
        if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            

        collection.findOne({userName: userName}, function(err, user) {
            if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
            
            if (user == null) {            
                response.status(404).render("404.pug", get404TemplateData(request.cookies));
                return;
            }
    
            db.collection("posts", function(err, collection) {
                if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
    
                //Get posted posts of specified user, sorted by post owner in ascending order
                var query = {ownerName: userName};
                collection.find(query).sort({friendName: 1}).toArray(function(err, posts) {
                    if (err) { response.status(500).render("500.pug", get500TemplateData(request.cookies)); return; }            
                    
                    //Return posts as array
                    response.send(posts);
                });
            });
        });
    });
});

//Page was not found
app.get("*", function(request, response) {
    response.status(404).render("404.pug", get404TemplateData(request.cookies));
});

//Incorrect method
app.all("*", function(request, response) {        
    response.status(405).end();
});

app.listen(PORT);

function getBaseTemplateData(cookies) {
    return {
        loggedIn: cookies.loggedIn == "true",
        userName: cookies.userName
    };
}

function get404TemplateData(cookies) {
    return {
        title: "Page not found",
        loggedIn: cookies.loggedIn == "true",
        userName: cookies.userName
    };
}

function get500TemplateData(cookies) {
    return {
        title: "Something went wrong",
        loggedIn: cookies.loggedIn == "true",
        userName: cookies.userName
    };
}